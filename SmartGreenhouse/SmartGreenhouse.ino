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
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
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
Adafruit_SHT31    sht30;
BH1750            lightMeter;
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
WiFiClient        espClient;
PubSubClient      mqttClient(espClient);
WebServer         webServer(80);

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
float g_lightLux     = NAN;
float g_soilMoisture = NAN;   // 0–100 %
bool  g_sht30OK      = false;
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
bool          g_ntpSynced    = false;
bool          g_wifiOK       = false;
unsigned long g_totalRecords = 0;

unsigned long g_lastSensorTime   = 0;
unsigned long g_lastCSVTime      = 0;
unsigned long g_lastLCDTime      = 0;
unsigned long g_lastMqttReconn   = 0;
unsigned long g_lastWifiReconn   = 0;

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
  initDataLogger();
  loadConfig(); // Đọc cấu hình ngưỡng sau khi mount LittleFS

  // WiFi
  lcdShowBoot("Ket noi WiFi...", 2);
  connectWiFi();

  // NTP
  if (g_wifiOK) {
    lcdShowBoot("Dong bo NTP...", 3);
    syncNTPSetup();
  }

  // MQTT
  if (MQTT_ENABLED) {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setBufferSize(512);
  }

  // Web server
  lcdShowBoot("Khoi dong Web...", 3);
  setupWebServer();

  // Forecast ML
  initForecast();

  // Hoàn tất
  delay(1000);
  lcdShowBoot("San sang!", 3);
  delay(1500);

  printSystemInfo();
}

// ═══════════════════════════════════════════════════════════════
//  MAIN LOOP
// ═══════════════════════════════════════════════════════════════
void loop() {
  unsigned long now = millis();

  // Web server xử lý request
  webServer.handleClient();

  // ─ WiFi watchdog ─
  if (WiFi.status() != WL_CONNECTED) {
    g_wifiOK = false;
    digitalWrite(LED_PIN, LOW);
    if (now - g_lastWifiReconn >= WIFI_RECONNECT) {
      g_lastWifiReconn = now;
      WiFi.reconnect();
      Serial.println("[WiFi] Reconnecting...");
    }
  } else {
    g_wifiOK = true;
    digitalWrite(LED_PIN, HIGH);
  }

  // ─ MQTT watchdog ─
  if (MQTT_ENABLED && g_wifiOK) {
    if (!mqttClient.connected()) {
      if (now - g_lastMqttReconn >= MQTT_RECONNECT) {
        g_lastMqttReconn = now;
        reconnectMQTT();
      }
    } else {
      mqttClient.loop();
    }
  }

  // ─ NTP sync retry ─
  if (!g_ntpSynced && g_wifiOK) {
    tryNTPSync();
  }

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

  // ─ Lưu CSV + MQTT (mỗi CSV_INTERVAL) ─
  if (now - g_lastCSVTime >= CSV_INTERVAL) {
    g_lastCSVTime = now;
    if (g_sht30OK && g_bh1750OK) {
      saveToCSV();
      if (MQTT_ENABLED && mqttClient.connected()) {
        publishMQTT();
      }
    }
  }

  // ─ Dự báo ML (mỗi FORECAST_INTERVAL = 5 phút) ─
  if (now - g_lastForecastTime >= FORECAST_INTERVAL) {
    g_lastForecastTime = now;
    updateForecast();
    if (g_forecast_ready && MQTT_ENABLED && mqttClient.connected()) {
      publishForecastMQTT();
    }
  }
}

// ═══════════════════════════════════════════════════════════════
//  WIFI
// ═══════════════════════════════════════════════════════════════
void connectWiFi() {
  Serial.println("[WiFi] Kết nối: " + String(WIFI_SSID));
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    g_wifiOK = true;
    Serial.println("\n[WiFi] OK - IP: " + WiFi.localIP().toString());
    digitalWrite(LED_PIN, HIGH);
  } else {
    g_wifiOK = false;
    Serial.println("\n[WiFi] Thất bại → chạy offline mode");
  }
}

void reconnectMQTT() {
  if (mqttClient.connect(MQTT_CLIENT_ID)) {
    Serial.println("[MQTT] Kết nối thành công ✓");
  } else {
    Serial.println("[MQTT] Kết nối thất bại, rc=" + String(mqttClient.state()));
  }
}

// ═══════════════════════════════════════════════════════════════
//  NTP
// ═══════════════════════════════════════════════════════════════
void syncNTPSetup() {
  // 3 server du phong: neu pool.ntp.org bi block thi dung time.google.com
  configTime(GMT_OFFSET, DAYLIGHT_OFFSET, NTP_SERVER, NTP_SERVER2, NTP_SERVER3);
  Serial.print("[NTP] Đồng bộ");
  struct tm t;
  for (int i = 0; i < 15; i++) {
    if (getLocalTime(&t, 1000)) {
      g_ntpSynced = true;
      char buf[25];
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
      Serial.println("\n[NTP] ✓ " + String(buf));
      return;
    }
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n[NTP] Chưa sync, dùng millis()");
}

void tryNTPSync() {
  struct tm t;
  if (getLocalTime(&t, 500)) {
    g_ntpSynced = true;
    Serial.println("[NTP] Đã sync trong loop");
  }
}

// ═══════════════════════════════════════════════════════════════
//  MQTT PUBLISH
// ═══════════════════════════════════════════════════════════════
void publishMQTT() {
  char timeStr[25];
  getTimestamp(timeStr, sizeof(timeStr));

  // Topic: greenhouse/sensor
  StaticJsonDocument<512> doc;
  doc["ts"]          = timeStr;
  doc["temp"]        = isnan(g_temperature)  ? 0 : round(g_temperature  * 10) / 10.0;
  doc["humid"]       = isnan(g_humidity)     ? 0 : round(g_humidity     * 10) / 10.0;
  doc["lux"]         = isnan(g_lightLux)     ? 0 : (int)g_lightLux;
  doc["soil"]        = isnan(g_soilMoisture) ? 0 : round(g_soilMoisture * 10) / 10.0;
  doc["fan_mode"]    = g_fanMode;
  doc["fan_speed"]   = g_fanSpeed;
  doc["pump"]        = g_pumpRunning;
  doc["wifi_rssi"]   = WiFi.RSSI();
  doc["uptime"]      = millis() / 1000;

  char buf[512];
  serializeJson(doc, buf);
  mqttClient.publish(MQTT_TOPIC_SENSOR, buf);
  Serial.println("[MQTT] Published sensor ✓");
}

// ═══════════════════════════════════════════════════════════════
//  MQTT PUBLISH - DỰ BÁO
// ═══════════════════════════════════════════════════════════════
void publishForecastMQTT() {
  char timeStr[25];
  getTimestamp(timeStr, sizeof(timeStr));

  StaticJsonDocument<384> doc;
  doc["ts"] = timeStr;

  JsonArray arrT = doc.createNestedArray("temp");
  JsonArray arrH = doc.createNestedArray("humid");
  JsonArray arrL = doc.createNestedArray("lux");
  JsonArray arrM = doc.createNestedArray("min");
  const int labels[] = {30, 60, 90};
  for (int h = 0; h < FC_HORIZONS; h++) {
    arrT.add(isnan(g_fc_temp[h])  ? 0 : round(g_fc_temp[h]  * 10) / 10.0f);
    arrH.add(isnan(g_fc_humid[h]) ? 0 : round(g_fc_humid[h] * 10) / 10.0f);
    arrL.add(isnan(g_fc_lux[h])   ? 0 : (int)g_fc_lux[h]);
    arrM.add(labels[h]);
  }

  char buf[384];
  serializeJson(doc, buf);
  mqttClient.publish(MQTT_TOPIC_FORECAST, buf);
  Serial.println("[MQTT] Published forecast ✓");
}

// ═══════════════════════════════════════════════════════════════
//  UTILITY
// ═══════════════════════════════════════════════════════════════
void getTimestamp(char* buf, size_t len) {
  struct tm t;
  if (g_ntpSynced && getLocalTime(&t)) {
    strftime(buf, len, "%Y-%m-%d %H:%M:%S", &t);
  } else {
    snprintf(buf, len, "uptime:%lus", millis() / 1000);
  }
}

void printBanner() {
  Serial.println();
  Serial.println(F("╔═══════════════════════════════════════════╗"));
  Serial.println(F("║   NHÀ MÀNG THÔNG MINH - SMART GREENHOUSE  ║"));
  Serial.println(F("║   ESP32 v2.0 | SHT30+BH1750+TB6612+LCD   ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));
}

void printSystemInfo() {
  Serial.println(F("─────────────────────────────────────────────"));
  if (g_wifiOK) {
    Serial.println("[WEB] Dashboard: http://" + WiFi.localIP().toString());
    Serial.println("[WEB] Tải CSV  : http://" + WiFi.localIP().toString() + "/download");
  } else {
    Serial.println("[WEB] Offline mode (no WiFi)");
  }
  Serial.printf("[SHT30]  OK=%d\n", g_sht30OK);
  Serial.printf("[BH1750] OK=%d\n", g_bh1750OK);
  Serial.printf("[FAN]    Mode=AUTO  Setpoint=%.1f°C\n", g_fanSetpoint);
  Serial.printf("[PUMP]   ON<%.0f%%  OFF>%.0f%%\n", g_pumpOnThresh, g_pumpOffThresh);
  Serial.printf("[CSV]    Records=%lu\n", g_totalRecords);
  Serial.println(F("─────────────────────────────────────────────"));
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
