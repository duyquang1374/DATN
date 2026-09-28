/*
 * ============================================================
 *  HỆ THỐNG NHÀ MÀNG THÔNG MINH - SMART GREENHOUSE v2.0
 * ============================================================
 *  Main file: Global objects, setup(), loop()
 *
 *  Phần cứng:
 *    ESP32 DevKit + SHT30 + BH1750 + LCD 20x4 I2C
 *    TB6612FNG (quạt DC PID) + Relay bơm 5V + Cảm biến đất
 *
 *  Libraries cần cài (Arduino IDE → Library Manager):
 *    1. Adafruit SHT31 Library         (by Adafruit)
 *    2. BH1750                         (by Christopher Laws)
 *    3. LiquidCrystal I2C              (by Frank de Brabander)
 *    4. PID                            (by Brett Beauregard)
 *    5. PubSubClient                   (by Nick O'Leary)
 *    6. ArduinoJson                    (by Benoit Blanchon)
 *    7. LittleFS → built-in ESP32
 * ============================================================
 */

// ─── Thư viện ────────────────────────────────────────────────
#include <Wire.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <time.h>
#include <Adafruit_SHT31.h>
#include <BH1750.h>
#include <LiquidCrystal_I2C.h>
#include <PID_v1.h>
#include "config.h"
#include "forecast.h"

// ─── Đối tượng phần cứng ────────────────────────────────────
TwoWire           I2C_Ext = TwoWire(1);
Adafruit_SHT31    sht30;
Adafruit_SHT31    sht30_ext(&I2C_Ext);
BH1750            lightMeter;
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// ─── UART cho LoRa ──────────────────────────────────────────
HardwareSerial    LoRaSerial(2);

// ─── PID Quạt ────────────────────────────────────────────────
// Chiến lược: Input = nhiệt độ, Setpoint = nhiệt độ mục tiêu
// REVERSE mode: khi nhiệt độ tăng → output tăng → quạt nhanh hơn
double pidInput    = 0.0;
double pidSetpoint = DEFAULT_FAN_SETPOINT;
double pidOutput   = 0.0;
PID fanPID(&pidInput, &pidOutput, &pidSetpoint,
           PID_KP, PID_KI, PID_KD, REVERSE);

// ─── Dữ liệu cảm biến ────────────────────────────────────────
float g_temperature  = NAN;
float g_humidity     = NAN;
float g_tempExt      = NAN;
float g_humidExt     = NAN;
float g_lightLux     = NAN;
float g_soilMoisture = NAN;   // 0–100 %
bool  g_sht30OK      = false;
bool  g_sht30ExtOK   = false;
bool  g_bh1750OK     = false;
int   g_sensorErrors = 0;

// ─── Cấu hình ngưỡng điều khiển ──────────────────────────────
float g_fanSetpoint   = DEFAULT_FAN_SETPOINT;
float g_pumpOnThresh  = DEFAULT_PUMP_ON_THRESH;
float g_pumpOffThresh = DEFAULT_PUMP_OFF_THRESH;

// ─── Trạng thái quạt ─────────────────────────────────────────
int  g_fanMode    = FAN_MODE_AUTO;  // FAN_MODE_OFF / AUTO / MANUAL
int  g_fanPWM     = 0;             // PWM hiện tại 0–255
int  g_fanSpeed   = 0;             // Phần trăm tốc độ 0–100
int  g_manualPWM  = 150;           // PWM thủ công (web/serial)
bool g_fanRunning = false;

// ─── Trạng thái bơm ──────────────────────────────────────────
bool          g_pumpRunning  = false;
bool          g_pumpManual   = false;  // true = đang điều khiển tay
unsigned long g_pumpStart    = 0;
unsigned long g_pumpStop     = 0;

// ─── Hệ thống ────────────────────────────────────────────────
unsigned long g_lastSensorTime   = 0;
unsigned long g_lastLoraTime     = 0;
unsigned long g_lastLCDTime      = 0;

// ═══════════════════════════════════════════════════════════════
//  SETUP
// ═══════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  delay(200);
  printBanner();

  // GPIO cơ bản
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // I2C bus
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.printf("[I2C] SDA=%d  SCL=%d\n", I2C_SDA, I2C_SCL);

#if I2C_SCAN_ON_BOOT
  scanI2C();  // In địa chỉ tất cả thiết bị I2C ra Serial
#endif

  // Khởi tạo UART cho LoRa
  LoRaSerial.begin(LORA_BAUD, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);
  Serial.println("[LoRa] Da khoi tao UART2 cho E32");

  // LCD – khởi tạo sớm để hiển thị tiến trình boot
  initLCD();
  lcdShowBoot("Khoi dong...", 0);

  // Cảm biến
  lcdShowBoot("Khoi tao sensor", 1);
  initSensors();

  // Cơ cấu chấp hành
  lcdShowBoot("Khoi tao co cau", 1);
  initFan();
  initPump();

  // Bộ nhớ flash
  lcdShowBoot("Kiem tra bo nho", 2);
  if (LittleFS.begin(true)) {
    loadConfig();
  } else {
    Serial.println("[FS] Mount failed");
  }

  // Forecast ML
  lcdShowBoot("Khoi tao ML", 2);
  initForecast();

  // Hoàn tất
  delay(1000);
  lcdShowBoot("San sang!", 3);
  delay(1500);
}

// ═══════════════════════════════════════════════════════════════
//  MAIN LOOP
// ═══════════════════════════════════════════════════════════════
void loop() {
  unsigned long now = millis();

  // ─ Xử lý lệnh nhận từ LoRa ─
  checkLoRaCommand();

  // ─ Đọc sensor + Điều khiển (mỗi SENSOR_INTERVAL) ─
  if (now - g_lastSensorTime >= SENSOR_INTERVAL) {
    g_lastSensorTime = now;
    readSensors();
    updateFan();
    updatePump();
  }

  // ─ Cập nhật LCD (mỗi LCD_INTERVAL) ─
  if (now - g_lastLCDTime >= LCD_INTERVAL) {
    g_lastLCDTime = now;
    updateLCD();
  }

  // ─ Gửi dữ liệu qua LoRa (mỗi 4 giây, giả sử CSV_INTERVAL = 4000) ─
  if (now - g_lastLoraTime >= 4000) {
    g_lastLoraTime = now;
    if (g_sht30OK && g_bh1750OK) {
      sendDataToGateway();
    }
  }

  // ─ Dự báo ML (mỗi FORECAST_INTERVAL = 5 phút) ─
  if (now - g_lastForecastTime >= FORECAST_INTERVAL) {
    g_lastForecastTime = now;
    updateForecast();
    // Gửi bản tin Forecast riêng qua LoRa
    sendForecastToGateway();
  }
}

// (Removed WiFi/NTP/MQTT sections)

// ═══════════════════════════════════════════════════════════════
//  UTILITY
// ═══════════════════════════════════════════════════════════════
void getTimestamp(char* buf, size_t len) {
  snprintf(buf, len, "uptime:%lus", millis() / 1000);
}

void printBanner() {
  Serial.println();
  Serial.println(F("╔═══════════════════════════════════════════╗"));
  Serial.println(F("║   NHÀ MÀNG THÔNG MINH - SMART GREENHOUSE  ║"));
  Serial.println(F("║   ESP32 v2.0 LORA NODE | SHT30+BH1750    ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));
}

// ═══════════════════════════════════════════════════════════════
//  QUẢN LÝ CẤU HÌNH (LITTLEFS)
// ═══════════════════════════════════════════════════════════════
void loadConfig() {
  if (LittleFS.exists("/config.json")) {
    File f = LittleFS.open("/config.json", "r");
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, f);
    if (!error) {
      if (doc.containsKey("fan_sp")) g_fanSetpoint = doc["fan_sp"];
      if (doc.containsKey("pump_on")) g_pumpOnThresh = doc["pump_on"];
      if (doc.containsKey("pump_off")) g_pumpOffThresh = doc["pump_off"];
      Serial.println("[CONFIG] Đã tải cấu hình từ Flash ✓");
    } else {
      Serial.println("[CONFIG] Lỗi đọc file config.json");
    }
    f.close();
  } else {
    Serial.println("[CONFIG] Dùng cấu hình mặc định");
  }
}

void saveConfig() {
  File f = LittleFS.open("/config.json", "w");
  if (f) {
    StaticJsonDocument<256> doc;
    doc["fan_sp"] = g_fanSetpoint;
    doc["pump_on"] = g_pumpOnThresh;
    doc["pump_off"] = g_pumpOffThresh;
    serializeJson(doc, f);
    f.close();
    Serial.println("[CONFIG] Đã lưu cấu hình ✓");
  }
}
