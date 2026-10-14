// ─── Thư viện ────────────────────────────────────────────────
#include <Wire.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <time.h>
#include <Adafruit_SHT31.h>
#include <BH1750.h>
#include <PID_v1.h>
#include <ESP32Servo.h>
#include "config.h"
#include "forecast.h"

// ─── I2C thứ 2 ──────────────────────────────────────────────
TwoWire I2C_Ext = TwoWire(1);

// ─── Đối tượng phần cứng ────────────────────────────────────
Adafruit_SHT31    sht30;
Adafruit_SHT31    sht30_ext(&I2C_Ext);
BH1750            lightMeter;
Servo             roofServo;

// ─── UART ──────────────────────────────────────────
HardwareSerial    LoRaSerial(2);
HardwareSerial    HMISerial(1);

// ─── PID Quạt & Sưởi ──────────────────────────────────────────
double pidInput    = 0.0;
double pidSetpoint = DEFAULT_FAN_SETPOINT;
double pidOutput   = 0.0;
PID fanPID(&pidInput, &pidOutput, &pidSetpoint,
           PID_KP, PID_KI, PID_KD, REVERSE);

double heaterPidOutput = 0.0;
double heaterSetpoint  = DEFAULT_FAN_SETPOINT - 2.0;
PID heaterPID(&pidInput, &heaterPidOutput, &heaterSetpoint,
              PID_KP, PID_KI, PID_KD, DIRECT);

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
float g_lightThresh   = DEFAULT_LIGHT_THRESH;

// ─── Trạng thái cơ cấu chấp hành ─────────────────────────────
int  g_fanMode    = FAN_MODE_AUTO;  
int  g_fanPWM     = 0;             
int  g_fanSpeed   = 0;             
int  g_manualPWM  = 150;           
bool g_fanRunning = false;

bool          g_pumpRunning  = false;
bool          g_pumpManual   = false;  
unsigned long g_pumpStart    = 0;
unsigned long g_pumpStop     = 0;

bool g_mistRunning = false;
bool g_mistManual  = false;

bool g_lightRunning = false;
bool g_lightManual  = false;

bool g_heaterRunning = false;
bool g_heaterManual = false;
int  g_heaterPWM = 0;

bool g_valve1Running = false;
bool g_valve2Running = false;
bool g_valveManual = false;

int g_roofAngle = 0;
bool g_roofManual = false;

// ─── Hệ thống ────────────────────────────────────────────────
unsigned long g_lastSensorTime   = 0;
unsigned long g_lastLoraTime     = 0;
unsigned long g_lastHMITime      = 0;

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("MASTER NODE STARTING");

  Wire.begin(I2C_SDA, I2C_SCL);

  // UART LoRa
  LoRaSerial.begin(LORA_BAUD, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);

  // UART HMI
  HMISerial.begin(HMI_BAUD, SERIAL_8N1, HMI_RX_PIN, HMI_TX_PIN);

  // Cảm biến
  initSensors();

  // Cơ cấu chấp hành
  initFan();
  initHeater();
  initPump();
  initMist();
  initLight();
  initValves();
  initRoof();

  // LittleFS
  if (LittleFS.begin(true)) {
    loadConfig();
  }

  // Forecast ML
  initForecast();

  Serial.println("Setup done.");
}

void loop() {
  unsigned long now = millis();

  checkLoRaCommand();
  checkHMICommand();

  if (now - g_lastSensorTime >= SENSOR_INTERVAL) {
    g_lastSensorTime = now;
    readSensors();
    updateFan();
    updateHeater();
    updatePump();
    updateMist();
    updateLight();
    updateValves();
    updateRoof();
  }
l
  if (now - g_lastHMITime >= HMI_INTERVAL) {
    g_lastHMITime = now;
    sendHMIData();
  }

  if (now - g_lastLoraTime >= LORA_INTERVAL) {
    g_lastLoraTime = now;
    sendDataToGateway();
  }

  if (now - g_lastForecastTime >= FORECAST_INTERVAL) {
    g_lastForecastTime = now;
    updateForecast();
    sendForecastToGateway();
  }
}

void sendHMIData() {
  StaticJsonDocument<256> doc;
  doc["t1"] = isnan(g_temperature) ? 0 : round(g_temperature*10)/10.0;
  doc["h1"] = isnan(g_humidity) ? 0 : round(g_humidity*10)/10.0;
  doc["t2"] = isnan(g_tempExt) ? 0 : round(g_tempExt*10)/10.0;
  doc["h2"] = isnan(g_humidExt) ? 0 : round(g_humidExt*10)/10.0;
  doc["lux"] = isnan(g_lightLux) ? 0 : (int)g_lightLux;
  
  JsonArray st = doc.createNestedArray("st");
  st.add(g_fanRunning ? 1 : 0);
  st.add(g_lightRunning ? 1 : 0);
  st.add(g_heaterRunning ? 1 : 0);
  st.add(g_mistRunning ? 1 : 0);
  st.add(g_pumpRunning ? 1 : 0);

  String out;
  serializeJson(doc, out);
  HMISerial.println(out);
}

void checkHMICommand() {
  if (HMISerial.available()) {
    String cmd = HMISerial.readStringUntil('\n');
    cmd.trim();
    if (cmd == "CMD:TOGGLE_FAN") {
      if (g_fanRunning) setFanMode(FAN_MODE_OFF);
      else { setFanMode(FAN_MODE_MANUAL); setFanManualSpeed(255); }
    } else if (cmd == "CMD:TOGGLE_LIGHT") {
      setLightManual(!g_lightRunning);
    } else if (cmd == "CMD:TOGGLE_HEATER") {
      setHeaterManual(!g_heaterRunning);
    } else if (cmd == "CMD:TOGGLE_MIST") {
      setMistManual(!g_mistRunning);
    } else if (cmd == "CMD:TOGGLE_PUMP") {
      setPumpManual(!g_pumpRunning);
    }
    sendHMIData(); // Phản hồi ngay
  }
}

void loadConfig() {
  if (LittleFS.exists("/config.json")) {
    File f = LittleFS.open("/config.json", "r");
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, f);
    if (!error) {
      if (doc.containsKey("fan_sp")) g_fanSetpoint = doc["fan_sp"];
      if (doc.containsKey("pump_on")) g_pumpOnThresh = doc["pump_on"];
      if (doc.containsKey("pump_off")) g_pumpOffThresh = doc["pump_off"];
      if (doc.containsKey("light_th")) g_lightThresh = doc["light_th"];
    }
    f.close();
  }
}

void saveConfig() {
  File f = LittleFS.open("/config.json", "w");
  if (f) {
    StaticJsonDocument<256> doc;
    doc["fan_sp"] = g_fanSetpoint;
    doc["pump_on"] = g_pumpOnThresh;
    doc["pump_off"] = g_pumpOffThresh;
    doc["light_th"] = g_lightThresh;
    serializeJson(doc, f);
    f.close();
  }
}
