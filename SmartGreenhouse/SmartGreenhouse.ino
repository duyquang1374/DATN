// ═════════════════════════════════════════════════════════════
//  HỆ THỐNG NHÀ MÀNG THÔNG MINH - NODE 1: MASTER NODE (ESP32)
//  Bộ xử lý trung tâm, thuật toán TinyML AI & Điều khiển tự động
//  - Thu thập cảm biến: SHT30 Trong, SHT30 Ngoài, BH1750, 2 Cảm biến ẩm đất
//  - Điều khiển chấp hành: Quạt PID (MOSFET D4184), Bóng sưởi (MOSFET D4184),
//    Bơm phun sương, Đèn quang hợp, Mái che Servo, 2 Van tưới + Bơm liên động
//  - Chạy mô hình AI TinyML dự báo nhiệt độ, độ ẩm, ánh sáng 3 bước tương lai
//  - Giao tiếp không dây LoRa E32 với Gateway (định kỳ 10s)
//  - Giao tiếp UART1 có dây với Tủ điện HMI (định kỳ 1s)
//  - Lưu trữ cấu hình người dùng vào LittleFS (/config.json, /valves.json)
// ═════════════════════════════════════════════════════════════
// ─── Thư viện ────────────────────────────────────────────────
#include "config.h"
#include "forecast.h"
#include <Adafruit_SHT31.h>
#include <ArduinoJson.h>
#include <BH1750.h>
#include <ESP32Servo.h>
#include <LittleFS.h>
#include <PID_v1.h>
#include <Wire.h>
#include <time.h>

// ─── I2C thứ 2 ──────────────────────────────────────────────
TwoWire I2C_Ext = TwoWire(1);

// ─── Đối tượng phần cứng ────────────────────────────────────
Adafruit_SHT31 sht30;
Adafruit_SHT31 sht30_ext(&I2C_Ext);
BH1750 lightMeter;
Servo roofServo;

// ─── UART ──────────────────────────────────────────
HardwareSerial LoRaSerial(2);
HardwareSerial HMISerial(1);

// ─── PID Quạt & Sưởi ──────────────────────────────────────────
double pidInput = 0.0;
double pidSetpoint = DEFAULT_FAN_SETPOINT;
double pidOutput = 0.0;
PID fanPID(&pidInput, &pidOutput, &pidSetpoint, PID_KP, PID_KI, PID_KD,
           REVERSE);

double heaterPidOutput = 0.0;
double heaterSetpoint = DEFAULT_FAN_SETPOINT - 2.0;
PID heaterPID(&pidInput, &heaterPidOutput, &heaterSetpoint, PID_KP, PID_KI,
              PID_KD, DIRECT);

// ─── Dữ liệu cảm biến ────────────────────────────────────────
float g_temperature = NAN;
float g_humidity = NAN;
float g_tempExt = NAN;
float g_humidExt = NAN;
float g_lightLux = NAN;
float g_soilMoisture = NAN;  // Đất kênh 1 (0–100 %)
float g_soilMoisture2 = NAN; // Đất kênh 2 (0–100 %)
int g_soilRawADC = 0;
bool g_sht30OK = false;
bool g_sht30ExtOK = false;
bool g_bh1750OK = false;
int g_sensorErrors = 0;

// ─── Cấu hình ngưỡng điều khiển ──────────────────────────────
float g_fanSetpoint = DEFAULT_FAN_SETPOINT;
float g_pumpOnThresh = DEFAULT_PUMP_ON_THRESH;
float g_pumpOffThresh = DEFAULT_PUMP_OFF_THRESH;
float g_lightThresh = DEFAULT_LIGHT_THRESH;

// ─── CẤU HÌNH & TRẠNG THÁI HỆ THỐNG TƯỚI THÔNG MINH ──────────
ValveConfig g_valves[NUM_VALVES] = {
  {
    "Van 1 (Luống 1)",
    VALVE1_PIN,
    SOIL1_PIN,
    40.0f, // dryThresh: dưới 40% thì tưới
    65.0f, // wetThresh: trên 65% thì dừng
    DEFAULT_VALVE_TIMEOUT,   // maxDuration: 300s
    DEFAULT_VALVE_REST_TIME, // minRestTime: 60s
    SOIL_DRY_VALUE, // dryRaw: 1500
    SOIL_WET_VALUE, // wetRaw: 3000
    VALVE_MODE_AUTO, // mode: 1 (AUTO)
    false, // isRunning
    0, // runStartTime
    0, // lastStopTime
    NAN, // currentSoil
    0, // rawADC
    false // sensorError
  },
  {
    "Van 2 (Luống 2)",
    VALVE2_PIN,
    SOIL2_PIN,
    45.0f, // dryThresh: dưới 45% thì tưới
    70.0f, // wetThresh: trên 70% thì dừng
    DEFAULT_VALVE_TIMEOUT,   // maxDuration: 300s
    DEFAULT_VALVE_REST_TIME, // minRestTime: 60s
    SOIL_DRY_VALUE, // dryRaw: 1500
    SOIL_WET_VALUE, // wetRaw: 3000
    VALVE_MODE_AUTO, // mode: 1 (AUTO)
    false, // isRunning
    0, // runStartTime
    0, // lastStopTime
    NAN, // currentSoil
    0, // rawADC
    false // sensorError
  }
};

bool g_pumpRunning = false;
bool g_pumpEnabled = true;          // Khóa an toàn ("Cho phép bơm hoạt động")
bool g_pumpEmergencyStop = false;   // Cờ tắt khẩn cấp
bool g_sequentialIrrigation = true; // Chế độ tưới tuần tự (ưu tiên vùng khô nhất)
unsigned long g_pumpRunStartTime = 0; // Thời điểm bắt đầu chạy của bơm

// Biến tương thích ngược
bool g_pumpManual = false;
unsigned long g_pumpStart = 0;
unsigned long g_pumpStop = 0;
bool g_valve1Running = false;
bool g_valve2Running = false;
bool g_valveManual = false;

// ─── Các cơ cấu chấp hành khác ────────────────────────────────
int g_fanMode = FAN_MODE_AUTO;
int g_fanPWM = 0;
int g_fanSpeed = 0;
int g_manualPWM = 150;
bool g_fanRunning = false;

bool g_mistRunning = false;
bool g_mistManual = false;

bool g_lightRunning = false;
bool g_lightManual = false;

bool g_heaterRunning = false;
bool g_heaterManual = false;
int g_heaterPWM = 0;

int g_roofAngle = 0;
bool g_roofManual = false;

// ─── Hệ thống ────────────────────────────────────────────────
unsigned long g_lastSensorTime = 0;
unsigned long g_lastLoraTime = 0;
unsigned long g_lastHMITime = 0;

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("MASTER NODE STARTING");

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setTimeOut(50); // Chống treo I2C khi lỏng dây hoặc lỗi cảm biến

  // UART LoRa
  LoRaSerial.begin(LORA_BAUD, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);

  // UART HMI
  HMISerial.begin(HMI_BAUD, SERIAL_8N1, HMI_RX_PIN, HMI_TX_PIN);

  // Cảm biến
  initSensors();

  // Cơ cấu chấp hành
  initFan();
  initHeater();
  initIrrigation(); // Khởi tạo hệ thống tưới thông minh (Bơm & 2 Van)
  initMist();
  initLight();
  initRoof();

  // LittleFS
  if (LittleFS.begin(true)) {
    loadConfig();
    loadValvesConfig();
    sendValveConfigToGateway();
  }

  // Forecast ML
  initForecast();

  // Đồng bộ trạng thái khởi động sang Gateway & HMI
  sendStateEventToGateway();
  sendHMIData();

  Serial.println("Setup done.");
}

void loop() {
  unsigned long now = millis();

  checkLoRaCommand();
  checkHMICommand();

  // Máy trạng thái liên động an toàn và timeout tưới chạy thời gian thực mỗi chu kỳ loop
  updateIrrigation();

  if (now - g_lastSensorTime >= SENSOR_INTERVAL) {
    g_lastSensorTime = now;
    readSensors();
    updateFan();
    updateHeater();
    updateMist();
    updateLight();
    updateRoof();
  }

  if (now - g_lastHMITime >= HMI_INTERVAL) {
    g_lastHMITime = now;
    sendHMIData();
  }

  if (now - g_lastLoraTime >= LORA_INTERVAL) {
    if (LoRaSerial.available() > 0) {
      checkLoRaCommand();
      g_lastLoraTime = now - LORA_INTERVAL + 1500; // Hoãn lại 1.5s để xử lý xong lệnh điều khiển trước
    } else {
      g_lastLoraTime = now;
      sendDataToGateway();
    }
  }

  if (now - g_lastForecastTime >= FORECAST_INTERVAL) {
    g_lastForecastTime = now;
    updateForecast();
    sendForecastToGateway();
  }
}

void sendHMIData() {
  StaticJsonDocument<256> doc;
  doc["t1"] = isnan(g_temperature) ? -999 : round(g_temperature * 10) / 10.0;
  doc["h1"] = isnan(g_humidity) ? -999 : round(g_humidity * 10) / 10.0;
  doc["t2"] = isnan(g_tempExt) ? -999 : round(g_tempExt * 10) / 10.0;
  doc["h2"] = isnan(g_humidExt) ? -999 : round(g_humidExt * 10) / 10.0;
  doc["lux"] = isnan(g_lightLux) ? -999 : (int)g_lightLux;

  JsonArray st = doc.createNestedArray("st");
  st.add(g_fanRunning ? 1 : 0);
  st.add(g_lightRunning ? 1 : 0);
  st.add(g_heaterRunning ? 1 : 0);
  st.add(g_mistRunning ? 1 : 0);
  st.add(g_pumpRunning ? 1 : 0);
  st.add((g_roofAngle > 0)
             ? 1
             : 0);           // 5: Rèm cửa (1: Mở / >0 độ, 0: Đóng hoàn toàn)
  doc["roof"] = g_roofAngle; // Góc rèm thực tế gửi sang HMI hiển thị LCD

  doc["s_ok"] = g_sht30OK ? 1 : 0;
  doc["se_ok"] = g_sht30ExtOK ? 1 : 0;

  // Mức quạt (0, 1, 2, 3, 4)
  int fanLvl = 0;
  if (g_fanRunning && g_fanPWM > 0) {
    if (g_fanPWM <= 125) fanLvl = 1;
    else if (g_fanPWM <= 180) fanLvl = 2;
    else if (g_fanPWM <= 230) fanLvl = 3;
    else fanLvl = 4;
  }
  doc["fan_lvl"] = fanLvl;

  // Mức sưởi (0, 1, 2, 3, 4)
  int heatLvl = 0;
  if (g_heaterRunning && g_heaterPWM > 0) {
    if (g_heaterPWM <= 125) heatLvl = 1;
    else if (g_heaterPWM <= 180) heatLvl = 2;
    else if (g_heaterPWM <= 230) heatLvl = 3;
    else heatLvl = 4;
  }
  doc["heat_lvl"] = heatLvl;

  String out;
  serializeJson(doc, out);
  HMISerial.println(out);
}

void checkHMICommand() {
  static String hmiBuf = "";
  while (HMISerial.available()) {
    char c = HMISerial.read();
    if (c == '\n') {
      hmiBuf.trim();
      if (hmiBuf.length() > 0) {
        String cmd = hmiBuf;
        bool handled = false;
        if (cmd.indexOf("CMD:TOGGLE_FAN") != -1) {
          Serial.println("[HMI RX] Nhan lenh: " + cmd);
          cycleFanHMI();
          handled = true;
        } else if (cmd.indexOf("CMD:TOGGLE_LIGHT") != -1) {
          Serial.println("[HMI RX] Nhan lenh: " + cmd);
          setLightManual(!g_lightRunning);
          Serial.printf("[HMI RX] Den -> %s\n",
                        g_lightRunning ? "BAT (Xanh)" : "TAT (Do)");
          handled = true;
        } else if (cmd.indexOf("CMD:TOGGLE_HEATER") != -1) {
          Serial.println("[HMI RX] Nhan lenh: " + cmd);
          cycleHeaterHMI();
          handled = true;
        } else if (cmd.indexOf("CMD:TOGGLE_MIST") != -1) {
          Serial.println("[HMI RX] Nhan lenh: " + cmd);
          setMistManual(!g_mistRunning);
          handled = true;
        } else if (cmd.indexOf("CMD:TOGGLE_PUMP") != -1) {
          Serial.println("[HMI RX] Nhan lenh: " + cmd);
          bool isRunning = g_pumpRunning || (countRunningValves() > 0);
          setPumpManual(!isRunning);
          handled = true;
        } else if (cmd.indexOf("CMD:TOGGLE_ROOF") != -1) {
          Serial.println("[HMI RX] Nhan lenh: " + cmd);
          if (g_roofAngle >= 90)
            setRoofManual(0);
          else
            setRoofManual(180);
          Serial.printf("[HMI RX] Rem cua -> %s\n",
                        (g_roofAngle >= 90) ? "MO (Xanh)" : "DONG (Do)");
          handled = true;
        } else if (cmd.startsWith("CMD:SET_ROOF:")) {
          int angle = cmd.substring(13).toInt();
          angle = constrain(angle, 0, 180);

          static int s_lastAppliedAngle = -1;
          if (angle != s_lastAppliedAngle) {
            s_lastAppliedAngle = angle;
            setRoofManual(angle);
            handled = true;
          }
        }

        if (handled) {
          if (!cmd.startsWith("CMD:SET_ROOF:")) {
            sendHMIData();
            sendStateEventToGateway();
          }
        }
      }
      hmiBuf = "";
    } else if (c != '\r') {
      hmiBuf += c;
      if (hmiBuf.length() > 128) hmiBuf = "";
    }
  }
}

void loadConfig() {
  if (LittleFS.exists("/config.json")) {
    File f = LittleFS.open("/config.json", "r");
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, f);
    if (!error) {
      if (doc.containsKey("fan_sp"))
        g_fanSetpoint = doc["fan_sp"];
      if (doc.containsKey("pump_on"))
        g_pumpOnThresh = doc["pump_on"];
      if (doc.containsKey("pump_off"))
        g_pumpOffThresh = doc["pump_off"];
      if (doc.containsKey("light_th"))
        g_lightThresh = doc["light_th"];
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

void loadValvesConfig() {
  if (LittleFS.exists("/valves.json")) {
    File f = LittleFS.open("/valves.json", "r");
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, f);
    if (!error) {
      if (doc.containsKey("seq")) g_sequentialIrrigation = doc["seq"];
      g_pumpEnabled = true; // Luôn sẵn sàng hoạt động, quản lý dừng qua cờ khẩn cấp
      if (doc.containsKey("v")) {
        JsonArray arr = doc["v"];
        for (int i = 0; i < NUM_VALVES && i < (int)arr.size(); i++) {
          JsonObject v = arr[i];
          if (v.containsKey("name")) strncpy(g_valves[i].name, v["name"], sizeof(g_valves[i].name) - 1);
          if (v.containsKey("dry")) g_valves[i].dryThresh = v["dry"];
          if (v.containsKey("wet")) g_valves[i].wetThresh = v["wet"];
          if (v.containsKey("to")) g_valves[i].maxDuration = v["to"];
          if (v.containsKey("rest")) g_valves[i].minRestTime = v["rest"];
          if (v.containsKey("dry_raw")) g_valves[i].dryRaw = v["dry_raw"];
          if (v.containsKey("wet_raw")) g_valves[i].wetRaw = v["wet_raw"];
          if (v.containsKey("mode")) g_valves[i].mode = v["mode"];

          // Tự động sửa nếu cấu hình cũ từng lưu dry <= wet (ngược chiều cảm biến điện dung)
          if (g_valves[i].dryRaw <= g_valves[i].wetRaw) {
            g_valves[i].dryRaw = SOIL_DRY_VALUE;
            g_valves[i].wetRaw = SOIL_WET_VALUE;
          }
        }
      }
      Serial.println(F("[FS] Da load cau hinh van tuoi tu /valves.json"));
    }
    f.close();
  }
}

void saveValvesConfig() {
  File f = LittleFS.open("/valves.json", "w");
  if (f) {
    StaticJsonDocument<512> doc;
    doc["seq"] = g_sequentialIrrigation;
    doc["p_en"] = g_pumpEnabled;
    JsonArray arr = doc.createNestedArray("v");
    for (int i = 0; i < NUM_VALVES; i++) {
      JsonObject v = arr.createNestedObject();
      v["name"] = g_valves[i].name;
      v["dry"] = g_valves[i].dryThresh;
      v["wet"] = g_valves[i].wetThresh;
      v["to"] = g_valves[i].maxDuration;
      v["rest"] = g_valves[i].minRestTime;
      v["dry_raw"] = g_valves[i].dryRaw;
      v["wet_raw"] = g_valves[i].wetRaw;
      v["mode"] = g_valves[i].mode;
    }
    serializeJson(doc, f);
    f.close();
    Serial.println(F("[FS] Da luu cau hinh van tuoi vao /valves.json"));
  }
}
