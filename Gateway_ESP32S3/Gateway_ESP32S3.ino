// ═══════════════════════════════════════════════════════════════════════════
//  HỆ THỐNG NHÀ MÀNG THÔNG MINH (SMART GREENHOUSE IoT - TinyML)
//  NODE 3: GATEWAY NODE - PHIÊN BẢN TƯƠNG THÍCH ESP32-S3 (TỰ CHỨA 1 FILE)
// ═══════════════════════════════════════════════════════════════════════════
//  - Chức năng: Trạm thu LoRa E32 từ Master Node, phát Web REST API cục bộ
//  (Cổng 80)
//    phục vụ Cloudflare Dashboard, đồng bộ thời gian NTP (GMT+7), ghi CSV
//    LittleFS.
//  - Phần cứng: ESP32-S3 (thay thế tạm thời ESP32 DevKit V1 cho khối Gateway).
//
//  ★ LƯU Ý PHẦN CỨNG & SƠ ĐỒ NỐI CHÂN TRÊN ESP32-S3:
//    1. Tránh GPIO 19 & 20: Trên ESP32-S3, GPIO 19 là USB_D- và GPIO 20 là
//    USB_D+.
//       Nếu nối LoRa vào chân 19 như bo DevKit V1 cũ sẽ làm hỏng kết nối USB
//       CDC.
//    2. Cặp chân LoRa UART1 an toàn trên ESP32-S3:
//       - ESP32-S3 GPIO 17 (TX1) ---> Chân RXD của LoRa E32
//       - ESP32-S3 GPIO 18 (RX1) ---> Chân TXD của LoRa E32
//       - Chân M0 và M1 của LoRa E32: Nối cố định xuống mass GND (Chế độ Normal
//       Mode 0).
//       - Chân VCC LoRa: Cấp nguồn 5V (từ mạch LM2596) kèm tụ lọc 470uF 16V sát
//       chân.
//    3. Đèn LED báo WiFi: Chân GPIO 2 (hoặc chân 21 / LED rời kèm trở 220 Ohm).
//    4. USB CDC Timeout: Code đã thêm timeout 2.5s cho Serial để ESP32-S3 không
//    bị
//       treo khởi động khi cắm nguồn Adapter / LM2596 độc lập (không cắm máy
//       tính).
// ═══════════════════════════════════════════════════════════════════════════

#include <ArduinoJson.h>
#include <LittleFS.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Wire.h>
#include <time.h>
#include <vector>

// ─────────────────────────────────────────────────────────────
//  1. CẤU HÌNH WIFI & MẠNG
// ─────────────────────────────────────────────────────────────
#define WIFI_SSID "Xoi Banh My Chi Nga"
#define WIFI_PASSWORD "13071982"

// ─────────────────────────────────────────────────────────────
//  2. CẤU HÌNH GIAO THỨC MQTT (TÙY CHỌN BROKER TRUNG TÂM)
// ─────────────────────────────────────────────────────────────
#define MQTT_ENABLED false          // =true: Kích hoạt đẩy dữ liệu MQTT
#define MQTT_SERVER "192.168.1.228" // Địa chỉ IP của MQTT Broker
#define MQTT_PORT 1883              // Cổng mặc định MQTT
#define MQTT_CLIENT_ID "ESP32S3_SmartGreenhouse_GW" 

#define MQTT_TOPIC_SENSOR "greenhouse/sensor"
#define MQTT_TOPIC_FAN "greenhouse/fan"
#define MQTT_TOPIC_PUMP "greenhouse/pump"
#define MQTT_TOPIC_FORECAST "greenhouse/forecast"

// ─────────────────────────────────────────────────────────────
//  3. CẤU HÌNH CHÂN GIAO TIẾP LORA E32 DÀNH CHO ESP32-S3
//  Sử dụng UART1 phần cứng với chân GPIO 17 và GPIO 18
// ─────────────────────────────────────────────────────────────
#define LORA_TX_PIN 17   // ESP32-S3 TX1 (GPIO 17) -> E32 RXD
#define LORA_RX_PIN 18   // ESP32-S3 RX1 (GPIO 18) -> E32 TXD
#define LORA_BAUD 115200 // Tốc độ Baud truyền nhận không dây LoRa

// ─────────────────────────────────────────────────────────────
//  4. CẤU HÌNH LƯU TRỮ FLASH LITTLEFS
// ─────────────────────────────────────────────────────────────
#define CSV_FILE "/data.csv"
#define MAX_FILE_SIZE (1.5 * 1024 * 1024) // Giới hạn tối đa 1.5 MB

// ─────────────────────────────────────────────────────────────
//  5. ĐỒNG BỘ THỜI GIAN THỰC TẾ QUA MẠNG (NTP TIME SYNC)
// ─────────────────────────────────────────────────────────────
#define NTP_SERVER "pool.ntp.org"
#define NTP_SERVER2 "time.google.com"
#define NTP_SERVER3 "time.windows.com"
#define GMT_OFFSET (7 * 3600) // GMT+7 (25200 giây)
#define DAYLIGHT_OFFSET 0

// ─────────────────────────────────────────────────────────────
//  6. CHU KỲ HOẠT ĐỘNG & CHÂN ĐÈN BÁO
// ─────────────────────────────────────────────────────────────
#define WIFI_RECONNECT 5000 // Thử kết nối lại WiFi mỗi 5 giây
#define MQTT_RECONNECT 5000 // Thử kết nối lại MQTT mỗi 5 giây
#define CSV_INTERVAL 10000  // Ghi CSV mỗi 10 giây khi có gói LoRa mới
#define LED_PIN 2 // Chân LED báo trạng thái WiFi (hoặc GPIO 21 tùy bo S3)

// ─── Đối tượng mạng & phần cứng ────────────────────────────
// Trên ESP32-S3 dùng HardwareSerial(1) với GPIO Matrix linh hoạt
HardwareSerial LoRaSerial(1);
WiFiClient espClient;
PubSubClient mqttClient(espClient);
WebServer webServer(80);

// ─── Dữ liệu hệ thống (Được cập nhật từ LoRa) ──────────────
float g_temperature = 0;
float g_humidity = 0;
float g_tempExt = 0;
float g_humidExt = 0;
float g_lightLux = 0;
float g_soilMoisture = 0;
float g_soilMoisture2 = 0;

// Hệ thống tưới thông minh (Bơm & 2 Van)
bool g_pumpRunning = false;
bool g_pumpEnabled = true;
bool g_pumpEmergencyStop = false;
unsigned long g_pumpTime = 0;
bool g_pumpSeq = true;

// Van 1
bool g_v1_run = false;
int g_v1_mode = 1;
float g_v1_soil = 0;
float g_v1_dry = 40.0;
float g_v1_wet = 65.0;
int g_v1_to = 300;
int g_v1_rest = 60;
unsigned long g_v1_time = 0;
bool g_v1_err = false;
String g_v1_name = "Van 1 (Luống 1)";

// Van 2
bool g_v2_run = false;
int g_v2_mode = 1;
float g_v2_soil = 0;
float g_v2_dry = 45.0;
float g_v2_wet = 70.0;
int g_v2_to = 300;
int g_v2_rest = 60;
unsigned long g_v2_time = 0;
bool g_v2_err = false;
String g_v2_name = "Van 2 (Luống 2)";

// Biến tương thích ngược
bool g_pumpManual = false;
float g_pumpOnThresh = 40.0;
float g_pumpOffThresh = 65.0;
bool g_valve1Running = false;
bool g_valve2Running = false;
bool g_valveManual = false;

int g_fanMode = 1;
int g_fanSpeed = 0;
int g_fanPWM = 0;
float g_fanSetpoint = 30.5;

bool g_mistRunning = false;
bool g_mistManual = false;
float g_mistHumidThresh = 65.0;

bool g_lightRunning = false;
bool g_lightManual = false;
float g_lightThresh = 1000.0;

bool g_heaterRunning = false;
bool g_heaterManual = false;
int g_heaterPWM = 0;
float g_heaterSetpoint = 28.5;

int g_roofAngle = 0;
bool g_roofManual = false;

bool g_sht30OK = false;
bool g_sht30ExtOK = false;
bool g_bh1750OK = false;
unsigned long g_nodeUptime = 0;

// Dự báo ML TinyML
float g_fc_temp[3] = {NAN, NAN, NAN};
float g_fc_humid[3] = {NAN, NAN, NAN};
float g_fc_lux[3] = {NAN, NAN, NAN};
bool g_forecast_ready = false;

// ─── Trạng thái Gateway ──────────────────────────────────────
bool g_ntpSynced = false;
bool g_wifiOK = false;
unsigned long g_totalRecords = 0;
unsigned long g_lastCSVTime = 0;
unsigned long g_lastMqttReconn = 0;
unsigned long g_lastWifiReconn = 0;
unsigned long g_lastLoraReceive =
    0; // Thời điểm cuối cùng nhận được data từ LoRa
unsigned long g_lastCommandTime = 0; // Thời điểm gửi lệnh điều khiển xuống Node

// ─── Theo dõi trạng thái & Smart Retry cho lệnh điều khiển ───
String g_lastSentCmd = "";
unsigned long g_lastCmdSentTime = 0;
bool g_cmdAckPending = false;
int g_cmdRetryCount = 0;

// ─── Khai báo nguyên mẫu hàm (Forward Declarations) ─────────
void connectWiFi();
void reconnectMQTT();
void syncNTPSetup();
void getTimestamp(char *buf, size_t len);
void publishMQTT();
void initDataLogger();
void trimFile();
void saveToCSV();
void checkLoRaMessage();
void processLoRaPacket(String rawJson);
void sendLoRaCommand(String cmdJson, bool isNewCmd = true);
void setupWebServer();
void setCorsHeaders();
void handleApiStatus();
void handleApiControl();
void handleApiForecast();
void handleDownloadCSV();
void handleDeleteCSV();

// ═══════════════════════════════════════════════════════════════
//  SETUP
// ═══════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);

  // Cơ chế chống treo trên ESP32-S3 (Native USB CDC):
  // Chờ Serial tối đa 2.5 giây. Nếu chạy độc lập không cắm máy tính, sẽ tự vượt
  // qua
  unsigned long startSerial = millis();
  while (!Serial && (millis() - startSerial < 2500)) {
    delay(10);
  }

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println(F("================================================="));
  Serial.println(F(" GATEWAY ESP32-S3 - SMART GREENHOUSE (LORA E32)"));
  Serial.println(F("================================================="));

  // Khởi tạo UART1 cho LoRa E32 với chân chỉ định riêng cho ESP32-S3
  LoRaSerial.begin(LORA_BAUD, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);
  Serial.printf("[LoRa S3] Da khoi tao UART1 (RX=%d, TX=%d) cho E32\n",
                LORA_RX_PIN, LORA_TX_PIN);

  // LittleFS Data Logger
  initDataLogger();

  // Kết nối WiFi
  connectWiFi();

  // Đồng bộ NTP
  if (g_wifiOK) {
    syncNTPSetup();
  }

  // Khởi tạo MQTT (nếu bật)
  if (MQTT_ENABLED) {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setBufferSize(512);
  }

  // Khởi tạo Web Server REST API
  setupWebServer();

  Serial.println(F("[SYSTEM] ESP32-S3 Gateway Da San Sang!"));
}

// ═══════════════════════════════════════════════════════════════
//  MAIN LOOP
// ═══════════════════════════════════════════════════════════════
void loop() {
  unsigned long now = millis();

  // Web server xử lý request từ Cloudflare Dashboard
  webServer.handleClient();

  // Đọc và giải mã gói tin từ sóng LoRa
  checkLoRaMessage();

  // ─ Smart Retry: Tự động gửi lại lệnh nếu sau 2500ms chưa nhận được phản hồi từ Master (cho phép truyền nhận LoRa hoàn tất) ─
  if (g_cmdAckPending && (now - g_lastCmdSentTime >= 2500)) {
    if (g_cmdRetryCount < 2) {
      g_cmdRetryCount++;
      g_lastCmdSentTime = now;
      Serial.printf("[LoRa Retry %d] Chưa nhận phản hồi -> Phát lại lệnh: %s\n",
                    g_cmdRetryCount, g_lastSentCmd.c_str());
      sendLoRaCommand(g_lastSentCmd, false); // Gửi lại (không tạo timer mới)
    } else {
      g_cmdAckPending = false;
      g_cmdRetryCount = 0;
    }
  }

  // ─ WiFi watchdog ─
  if (WiFi.status() != WL_CONNECTED) {
    g_wifiOK = false;
    digitalWrite(LED_PIN, LOW);
    if (now - g_lastWifiReconn >= WIFI_RECONNECT) {
      g_lastWifiReconn = now;
      WiFi.reconnect();
      Serial.println(F("[WiFi] Reconnecting..."));
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
    struct tm t;
    if (getLocalTime(&t, 500)) {
      g_ntpSynced = true;
      Serial.println(F("[NTP] Đã sync trong loop"));
    }
  }

  // ─ Lưu CSV định kỳ ─
  // Chỉ lưu khi ĐÃ nhận được ít nhất 1 gói LoRa và gói đó trong vòng 30s
  if (now - g_lastCSVTime >= CSV_INTERVAL) {
    g_lastCSVTime = now;
    if (g_lastLoraReceive > 0 && (now - g_lastLoraReceive) < 30000) {
      saveToCSV();
      if (MQTT_ENABLED && mqttClient.connected()) {
        publishMQTT();
      }
    }
  }
}

// ═══════════════════════════════════════════════════════════════
//  PHẦN 1: KẾT NỐI WIFI, MQTT & THỜI GIAN NTP
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
    Serial.println(F("\n[WiFi] Thất bại → chạy offline mode"));
  }
}

void reconnectMQTT() {
  if (mqttClient.connect(MQTT_CLIENT_ID)) {
    Serial.println(F("[MQTT] Kết nối thành công ✓"));
  } else {
    Serial.println("[MQTT] Kết nối thất bại, rc=" + String(mqttClient.state()));
  }
}

void syncNTPSetup() {
  configTime(GMT_OFFSET, DAYLIGHT_OFFSET, NTP_SERVER, NTP_SERVER2, NTP_SERVER3);
  Serial.print(F("[NTP] Đồng bộ"));
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
  Serial.println(F("\n[NTP] Chưa sync, dùng millis()"));
}

void getTimestamp(char *buf, size_t len) {
  struct tm t;
  if (g_ntpSynced && getLocalTime(&t)) {
    strftime(buf, len, "%Y-%m-%d %H:%M:%S", &t);
  } else {
    snprintf(buf, len, "uptime:%lus", millis() / 1000);
  }
}

void publishMQTT() {
  char timeStr[25];
  getTimestamp(timeStr, sizeof(timeStr));

  StaticJsonDocument<512> doc;
  doc["ts"] = timeStr;
  doc["temp"] = g_temperature;
  doc["humid"] = g_humidity;
  doc["temp_ext"] = g_tempExt;
  doc["humid_ext"] = g_humidExt;
  doc["lux"] = g_lightLux;
  doc["soil"] = g_soilMoisture;
  doc["fan_mode"] = g_fanMode;
  doc["fan_speed"] = g_fanSpeed;
  doc["pump"] = g_pumpRunning;
  doc["wifi_rssi"] = WiFi.RSSI();

  char buf[512];
  serializeJson(doc, buf);
  mqttClient.publish(MQTT_TOPIC_SENSOR, buf);
}

void loadGatewayConfig() {
  if (LittleFS.exists("/config.json")) {
    File f = LittleFS.open("/config.json", "r");
    if (f) {
      StaticJsonDocument<512> doc;
      DeserializationError err = deserializeJson(doc, f);
      if (!err) {
        if (doc.containsKey("fan_sp"))
          g_fanSetpoint = doc["fan_sp"];
        if (doc.containsKey("heater_sp"))
          g_heaterSetpoint = doc["heater_sp"];
        else
          g_heaterSetpoint = 28.5;
        if (doc.containsKey("mist_th"))
          g_mistHumidThresh = doc["mist_th"];
        if (doc.containsKey("light_th"))
          g_lightThresh = doc["light_th"];
        if (doc.containsKey("pump_on"))
          g_pumpOnThresh = doc["pump_on"];
        if (doc.containsKey("pump_off"))
          g_pumpOffThresh = doc["pump_off"];
        if (doc.containsKey("v1_dry"))
          g_v1_dry = doc["v1_dry"];
        if (doc.containsKey("v1_wet"))
          g_v1_wet = doc["v1_wet"];
        if (doc.containsKey("v1_to"))
          g_v1_to = doc["v1_to"];
        if (doc.containsKey("v1_rest"))
          g_v1_rest = doc["v1_rest"];
        if (doc.containsKey("v2_dry"))
          g_v2_dry = doc["v2_dry"];
        if (doc.containsKey("v2_wet"))
          g_v2_wet = doc["v2_wet"];
        if (doc.containsKey("v2_to"))
          g_v2_to = doc["v2_to"];
        if (doc.containsKey("v2_rest"))
          g_v2_rest = doc["v2_rest"];
        Serial.printf("[FS] Gateway da load cau hinh Flash: FanSP=%.1f MistTH=%.0f LightTH=%.0f V1Dry=%.0f V2Dry=%.0f\n",
                      g_fanSetpoint, g_mistHumidThresh, g_lightThresh, g_v1_dry, g_v2_dry);
      }
      f.close();
    }
  }
}

void saveGatewayConfig() {
  File f = LittleFS.open("/config.json", "w");
  if (f) {
    StaticJsonDocument<512> doc;
    doc["fan_sp"] = g_fanSetpoint;
    doc["heater_sp"] = g_heaterSetpoint;
    doc["mist_th"] = g_mistHumidThresh;
    doc["light_th"] = g_lightThresh;
    doc["pump_on"] = g_pumpOnThresh;
    doc["pump_off"] = g_pumpOffThresh;
    doc["v1_dry"] = g_v1_dry;
    doc["v1_wet"] = g_v1_wet;
    doc["v1_to"] = g_v1_to;
    doc["v1_rest"] = g_v1_rest;
    doc["v2_dry"] = g_v2_dry;
    doc["v2_wet"] = g_v2_wet;
    doc["v2_to"] = g_v2_to;
    doc["v2_rest"] = g_v2_rest;
    serializeJson(doc, f);
    f.close();
    Serial.println(F("[FS] Gateway da luu tat ca cau hinh nguong vao Flash (/config.json)"));
  }
}

// ═══════════════════════════════════════════════════════════════
//  PHẦN 2: LƯU TRỮ DỮ LIỆU NHẬT KÝ (DATA LOGGER LITTLEFS)
// ═══════════════════════════════════════════════════════════════
void initDataLogger() {
  if (!LittleFS.begin(true)) {
    Serial.println(F("[FS] LittleFS MOUNT FAILED!"));
    return;
  }

  // Khôi phục toàn bộ cấu hình ngưỡng từ Flash
  loadGatewayConfig();

  // Khôi phục g_totalRecords bằng cách đếm dòng
  if (LittleFS.exists(CSV_FILE)) {
    File f = LittleFS.open(CSV_FILE, "r");
    g_totalRecords = 0;
    while (f.available()) {
      if (f.read() == '\n')
        g_totalRecords++;
    }
    if (g_totalRecords > 0)
      g_totalRecords--; // Trừ dòng header
    f.close();
    Serial.printf("[FS] File CSV đã có: %lu bản ghi\n", g_totalRecords);
  } else {
    File f = LittleFS.open(CSV_FILE, "w");
    if (f) {
      f.println(F("timestamp,temperature,humidity,light,soil,fan_mode,fan_"
                  "speed,pump,temp_ext,humid_ext"));
      f.close();
      Serial.println(F("[FS] Tạo file CSV mới ✓"));
    }
  }
}

void trimFile() {
  File f = LittleFS.open(CSV_FILE, "r");
  if (!f)
    return;

  String header = f.readStringUntil('\n');
  size_t half = f.size() / 2;
  f.seek(half);
  f.readStringUntil('\n');

  File temp = LittleFS.open("/temp.csv", "w");
  temp.println(header);

  char buf[512];
  while (f.available()) {
    size_t n = f.readBytes(buf, sizeof(buf));
    temp.write((const uint8_t *)buf, n);
  }

  f.close();
  temp.close();                                                                                                                                                                                                                                                                                                                                                                                                

  LittleFS.remove(CSV_FILE);
  LittleFS.rename("/temp.csv", CSV_FILE);

  initDataLogger();
}

void saveToCSV() {
  if (LittleFS.exists(CSV_FILE)) {
    File f = LittleFS.open(CSV_FILE, "r");
    if (f) {
      size_t sz = f.size();
      f.close();
      if (sz >= MAX_FILE_SIZE) {
        Serial.println(F("[FS] File đầy! Cắt bớt dữ liệu cũ..."));
        trimFile();
      }
    }
  }

  char timeStr[25];
  getTimestamp(timeStr, sizeof(timeStr));

  File f = LittleFS.open(CSV_FILE, "a");
  if (f) {
    f.printf("%s,%.1f,%.1f,%.0f,%.1f,%d,%d,%d,%.1f,%.1f\n", timeStr,
             g_temperature, g_humidity, g_lightLux, g_soilMoisture, g_fanMode,
             g_fanSpeed, g_pumpRunning ? 1 : 0, g_tempExt, g_humidExt);
    f.close();
    g_totalRecords++;
    Serial.printf("[FS] Saved #%lu  T=%.1f H=%.1f Te=%.1f He=%.1f\n",
                  g_totalRecords, g_temperature, g_humidity, g_tempExt,
                  g_humidExt);
  } else {
    Serial.println(F("[FS] Ghi CSV thất bại!"));
  }
}

// ═══════════════════════════════════════════════════════════════
//  PHẦN 3: GIAO TIẾP LORA E32 & XỬ LÝ GÓI TIN TỪ MASTER NODE
// ═══════════════════════════════════════════════════════════════

void processLoRaPacket(String rawJson) {
  StaticJsonDocument<2048> doc;
  DeserializationError error = deserializeJson(doc, rawJson);

  if (error) {
    Serial.println("[LoRa] JSON parse failed: " + String(error.c_str()) +
                   " (len=" + String(rawJson.length()) + " B)");
    if (rawJson.length() > 450) {
      Serial.println(
          F("[LoRa] CANH BAO: Goi tin >450B bi rot byte o giua do E32 tran bo "
            "dem 512B! Can nap code toi uu (<400B) cho Master Node!"));
    }
    return;
  }

  if (!doc.containsKey("type"))
    return;
  String type = doc["type"].as<String>();

  g_lastLoraReceive = millis();
  if (type == "state") {
    g_cmdAckPending = false; // Đã nhận gói tin phản hồi tức thì 'state' từ Master Node -> Hủy cờ chờ phát lại
    g_cmdRetryCount = 0;
  }

  if (type == "state" || type == "status") {
    // Nếu vừa gửi lệnh điều khiển trong vòng 3.5s:
    // CHỈ cho phép gói 'state' (phản hồi sự kiện tức thì từ Master) cập nhật thiết bị.
    // Bỏ qua việc ghi đè thiết bị từ gói 'status' định kỳ cũ để chống giật dội trạng thái trên Web.
    bool allowActuatorUpdate = (type == "state") || (millis() - g_lastCommandTime >= 3500);

    if (allowActuatorUpdate) {
      if (doc.containsKey("light"))
        g_lightRunning = doc["light"].as<bool>();
      if (doc.containsKey("light_m"))
        g_lightManual = doc["light_m"].as<bool>();

      if (doc.containsKey("mist"))
        g_mistRunning = doc["mist"].as<bool>();
      if (doc.containsKey("mist_m"))
        g_mistManual = doc["mist_m"].as<bool>();

      if (doc.containsKey("heater"))
        g_heaterRunning = doc["heater"].as<bool>();
      if (doc.containsKey("heater_m"))
        g_heaterManual = doc["heater_m"].as<bool>();
      if (doc.containsKey("heater_pwm"))
        g_heaterPWM = (int)doc["heater_pwm"];

      if (doc.containsKey("roof"))
        g_roofAngle = doc["roof"] | 0;
      if (doc.containsKey("roof_m"))
        g_roofManual = doc["roof_m"].as<bool>();

      if (doc.containsKey("fan_mode"))
        g_fanMode = doc["fan_mode"] | 1;
      if (doc.containsKey("fan_speed"))
        g_fanSpeed = doc["fan_speed"] | 0;
      else if (doc.containsKey("fan_spd"))
        g_fanSpeed = doc["fan_spd"] | 0;
      if (doc.containsKey("fan_pwm"))
        g_fanPWM = (int)doc["fan_pwm"];
      else if (doc.containsKey("fan_speed") || doc.containsKey("fan_spd"))
        g_fanPWM = map(g_fanSpeed, 0, 100, 0, 255);

      if (doc.containsKey("pump"))
        g_pumpRunning = doc["pump"].as<bool>();
      if (doc.containsKey("p_en"))
        g_pumpEnabled = doc["p_en"].as<bool>();
      if (doc.containsKey("p_emg"))
        g_pumpEmergencyStop = doc["p_emg"].as<bool>();

      if (doc.containsKey("v1_run")) {
        g_v1_run = doc["v1_run"].as<bool>();
        g_valve1Running = g_v1_run;
      }
      if (doc.containsKey("v1_mode"))
        g_v1_mode = doc["v1_mode"] | 1;

      if (doc.containsKey("v2_run")) {
        g_v2_run = doc["v2_run"].as<bool>();
        g_valve2Running = g_v2_run;
      }
      if (doc.containsKey("v2_mode"))
        g_v2_mode = doc["v2_mode"] | 1;
    }

    if (type == "state") {
      bool cfgChanged = false;
      if (doc.containsKey("heater_sp")) {
        float hsp = doc["heater_sp"];
        if (g_heaterSetpoint != hsp && hsp > 0) {
          g_heaterSetpoint = hsp;
          cfgChanged = true;
        }
      }
      if (doc.containsKey("fan_sp")) {
        float fsp = doc["fan_sp"];
        if (g_fanSetpoint != fsp && fsp > 0) {
          g_fanSetpoint = fsp;
          cfgChanged = true;
        }
      }
      if (doc.containsKey("v1_dry")) {
        float vd = doc["v1_dry"];
        if (g_v1_dry != vd && vd > 0) { g_v1_dry = vd; cfgChanged = true; }
      }
      if (doc.containsKey("v1_wet")) {
        float vw = doc["v1_wet"];
        if (g_v1_wet != vw && vw > 0) { g_v1_wet = vw; cfgChanged = true; }
      }
      if (doc.containsKey("v2_dry")) {
        float vd2 = doc["v2_dry"];
        if (g_v2_dry != vd2 && vd2 > 0) { g_v2_dry = vd2; cfgChanged = true; }
      }
      if (doc.containsKey("v2_wet")) {
        float vw2 = doc["v2_wet"];
        if (g_v2_wet != vw2 && vw2 > 0) { g_v2_wet = vw2; cfgChanged = true; }
      }
      if (cfgChanged) {
        saveGatewayConfig();
      }
      Serial.printf("[LoRa] State Sync OK: Light=%d (m=%d) Mist=%d (m=%d) Heat=%d Fan=%d Roof=%d Pump=%d HeaterSP=%.1f V1Dry=%.0f V1Wet=%.0f\n",
                    g_lightRunning, g_lightManual, g_mistRunning, g_mistManual,
                    g_heaterRunning, g_fanMode, g_roofAngle, g_pumpRunning, g_heaterSetpoint, g_v1_dry, g_v1_wet);
      return; // Gói state chỉ chứa cơ cấu chấp hành, xử lý xong return ngay
    }

    // Các trường cảm biến (chỉ có trong gói type == "status")
    g_temperature = doc["temp"] | 0.0f;
    g_humidity = doc["humid"] | 0.0f;
    g_tempExt = doc.containsKey("te") ? (float)doc["te"] : (doc["temp_ext"] | 0.0f);
    g_humidExt = doc.containsKey("he") ? (float)doc["he"] : (doc["humid_ext"] | 0.0f);
    g_lightLux = doc["lux"] | 0.0f;

    g_soilMoisture = doc["soil"] | 0.0f;
    g_soilMoisture2 = doc["soil2"] | 0.0f;

    g_pumpTime = doc.containsKey("p_t") ? doc["p_t"].as<unsigned long>() : (doc["p_time"] | 0UL);
    g_pumpSeq = doc.containsKey("p_seq") ? doc["p_seq"].as<bool>() : true;

    g_v1_soil = doc.containsKey("v1_soil") ? (float)doc["v1_soil"] : g_soilMoisture;
    g_v1_time = doc.containsKey("v1_t") ? doc["v1_t"].as<unsigned long>() : (doc["v1_time"] | 0UL);
    g_v1_err = doc.containsKey("v1_err") ? doc["v1_err"].as<bool>() : false;
    if (doc.containsKey("v1_name"))
      g_v1_name = doc["v1_name"].as<String>();

    g_v2_soil = doc.containsKey("v2_soil") ? (float)doc["v2_soil"] : g_soilMoisture2;
    g_v2_time = doc.containsKey("v2_t") ? doc["v2_t"].as<unsigned long>() : (doc["v2_time"] | 0UL);
    g_v2_err = doc.containsKey("v2_err") ? doc["v2_err"].as<bool>() : false;
    if (doc.containsKey("v2_name"))
      g_v2_name = doc["v2_name"].as<String>();

    if (doc.containsKey("fan_pwm"))
      g_fanPWM = (int)doc["fan_pwm"];
    else
      g_fanPWM = map(g_fanSpeed, 0, 100, 0, 255);
    bool cfgChanged = false;
    if (doc.containsKey("fan_sp")) {
      float fsp = doc["fan_sp"];
      if (millis() - g_lastCommandTime >= 15000) {
        if (g_fanSetpoint != fsp && fsp > 0) {
          g_fanSetpoint = fsp;
          cfgChanged = true;
        }
      }
    }

    if (doc.containsKey("light_th")) {
      if (g_lightThresh != (float)doc["light_th"]) {
        g_lightThresh = doc["light_th"];
        cfgChanged = true;
      }
    }

    if (doc.containsKey("mist_th")) {
      if (g_mistHumidThresh != (float)doc["mist_th"]) {
        g_mistHumidThresh = doc["mist_th"];
        cfgChanged = true;
      }
    }

    // Các ngưỡng van, quạt, sưởi được lưu cố định trong Flash Gateway và chỉ đồng bộ qua gói 'state' (ACK) hoặc 'cfg', không bị ghi đè bởi gói status định kỳ

    if (cfgChanged) {
      saveGatewayConfig();
    }

    g_sht30OK = doc.containsKey("s_ok") ? doc["s_ok"].as<bool>() : false;
    g_sht30ExtOK = doc.containsKey("se_ok") ? doc["se_ok"].as<bool>() : false;
    g_bh1750OK = doc.containsKey("b_ok") ? doc["b_ok"].as<bool>() : false;

    g_nodeUptime = doc.containsKey("up") ? doc["up"].as<unsigned long>() : (doc["uptime"] | 0UL);

    Serial.printf(
        "[LoRa] Status OK: T=%.1f H=%.1f L=%.0f Soil=%.1f Mist=%d (TH=%.0f) Light=%d (TH=%.0f) FanSP=%.1f\n",
        g_temperature, g_humidity, g_lightLux, g_soilMoisture, g_mistRunning,
        g_mistHumidThresh, g_lightRunning, g_lightThresh, g_fanSetpoint);
  } else if (type == "cfg") {
    if (doc.containsKey("v1_dry"))
      g_v1_dry = doc["v1_dry"];
    if (doc.containsKey("v1_wet"))
      g_v1_wet = doc["v1_wet"];
    if (doc.containsKey("v1_to"))
      g_v1_to = doc["v1_to"];
    if (doc.containsKey("v1_rest"))
      g_v1_rest = doc["v1_rest"];
    if (doc.containsKey("v2_dry"))
      g_v2_dry = doc["v2_dry"];
    if (doc.containsKey("v2_wet"))
      g_v2_wet = doc["v2_wet"];
    if (doc.containsKey("v2_to"))
      g_v2_to = doc["v2_to"];
    if (doc.containsKey("v2_rest"))
      g_v2_rest = doc["v2_rest"];
    saveGatewayConfig();
    Serial.println(F("[LoRa] Da dong bo va luu cau hinh van tu Master Node ✓"));
  } else if (type == "forecast") {
    JsonArray arrT = doc["temp"];
    JsonArray arrH = doc["humid"];
    JsonArray arrL = doc["lux"];
    for (int i = 0; i < 3; i++) {
      g_fc_temp[i] = arrT[i];
      g_fc_humid[i] = arrH[i];
      g_fc_lux[i] = arrL[i];
    }
    g_forecast_ready = true;
    Serial.println(F("[LoRa] Forecast OK"));
  }
}

void checkLoRaMessage() {
  static String rawBuf = "";
  static int braceCount = 0;
  static bool inJson = false;
  static unsigned long lastPrint = 0;
  static unsigned long lastWarn = 0;
  static unsigned long lastCharTime = 0;

  // Timeout dọn đệm nếu nhận dở dang quá 3000ms
  if (inJson && (millis() - lastCharTime > 3000)) {
    Serial.println("[LoRa RX Timeout] Huy bo: " + rawBuf);
    rawBuf = "";
    braceCount = 0;
    inJson = false;
  }

  while (LoRaSerial.available()) {
    char c = LoRaSerial.read();
    lastPrint = millis();
    lastCharTime = millis();

    // Bắt đầu JSON khi gặp '{' (bỏ qua mọi byte rác / nhiễu vô tuyến phía trước)
    if (c == '{') {
      if (!inJson) {
        rawBuf = "";
        braceCount = 0;
        inJson = true;
      }
      braceCount++;
    }

    if (inJson) {
      rawBuf += c;

      if (c == '}') {
        braceCount--;

        // Gói JSON hoàn chỉnh khi số dấu { và } cân bằng
        if (braceCount <= 0) {
          inJson = false;
          rawBuf.trim();
          Serial.println("[LoRa RAW] " + rawBuf);
          processLoRaPacket(rawBuf);
          rawBuf = "";
          braceCount = 0;
        }
      }

      // Bảo vệ tràn bộ đệm (chống nhiễu kéo dài không ngắt)
      if (rawBuf.length() > 1500) {
        rawBuf = "";
        braceCount = 0;
        inJson = false;
      }
    }
  }

  // Nếu quá lâu không nhận được byte nào từ Node
  if (millis() - lastPrint > 15000 && millis() - lastWarn > 15000) {
    lastWarn = millis();
    if (g_lastLoraReceive == 0 && millis() > 25000) {
      Serial.println(F("[LoRa] WARN: Khong nhan duoc du lieu tu Node! Kiem tra "
                       "day noi TX/RX va M0/M1=GND."));
    }
  }
}

// Hàm gửi lệnh điều khiển từ API xuống Node
void sendLoRaCommand(String cmdJson, bool isNewCmd) {
  cmdJson.trim();
  if (cmdJson.length() == 0) return;

  // CSMA/CA (Listen-Before-Talk): Đợi cổng LoRa sạch dữ liệu và yên lặng ít nhất 80ms trước khi phát
  unsigned long lastByte = millis();
  unsigned long start = millis();
  while ((LoRaSerial.available() > 0 || (millis() - lastByte < 80)) && (millis() - start < 1200)) {
    if (LoRaSerial.available() > 0) {
      checkLoRaMessage();
      lastByte = millis();
    }
    delay(5);
  }

  if (isNewCmd) {
    g_lastSentCmd = cmdJson;
    g_lastCmdSentTime = millis();
    g_cmdAckPending = true;
    g_cmdRetryCount = 0;
  }

  cmdJson += "\n";
  LoRaSerial.print(cmdJson);
  LoRaSerial.flush();
  Serial.print("[LoRa Tx] " + cmdJson);
  g_lastCommandTime = millis();
}

// ═══════════════════════════════════════════════════════════════
//  PHẦN 4: WEBSERVER REST API (PHỤC VỤ CLOUDFLARE DASHBOARD)
// ═══════════════════════════════════════════════════════════════
void setupWebServer() {
  webServer.on("/", HTTP_GET, []() {
    webServer.send(200, "text/html",
                   "<h1>Smart Greenhouse Gateway (ESP32-S3)</h1><p>UI is "
                   "hosted on Cloudflare.</p>");
  });

  // CORS headers
  webServer.onNotFound([]() {
    webServer.sendHeader("Connection", "close");
    if (webServer.method() == HTTP_OPTIONS) {
      webServer.sendHeader("Access-Control-Allow-Origin", "*");
      webServer.sendHeader("Access-Control-Allow-Methods",
                           "GET, POST, OPTIONS");
      webServer.sendHeader("Access-Control-Allow-Headers", "Content-Type");
      webServer.send(204);
    } else {
      webServer.send(404, "text/plain", "Not Found");
    }
  });

  webServer.on("/api/status", HTTP_GET, handleApiStatus);
  webServer.on("/api/control", HTTP_GET, handleApiControl);
  webServer.on("/api/forecast", HTTP_GET, handleApiForecast);
  webServer.on("/download", HTTP_GET, handleDownloadCSV);
  webServer.on("/delete_csv", HTTP_POST, handleDeleteCSV);

  webServer.begin();
  Serial.println(F("[WEB] Web Server started"));
}

void setCorsHeaders() {
  webServer.sendHeader("Access-Control-Allow-Origin", "*");
  webServer.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  webServer.sendHeader("Access-Control-Allow-Headers", "*");
  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");
  webServer.sendHeader("Connection", "close");
}

void handleApiStatus() {
  StaticJsonDocument<1536> doc;

  // Dữ liệu mới nhất lưu ở Gateway
  doc["temperature"] = g_temperature;
  doc["humidity"] = g_humidity;
  doc["light"] = g_lightLux;
  doc["soil"] = g_soilMoisture;
  doc["soil2"] = g_soilMoisture2;

  // Bơm & An toàn
  doc["pump_running"] = g_pumpRunning;
  doc["pump_enabled"] = g_pumpEnabled;
  doc["pump_emergency"] = g_pumpEmergencyStop;
  doc["pump_time"] = g_pumpTime;
  doc["pump_seq"] = g_pumpSeq;

  // Van 1
  doc["v1_run"] = g_v1_run;
  doc["v1_mode"] = g_v1_mode;
  doc["v1_soil"] = g_v1_soil;
  doc["v1_dry"] = g_v1_dry;
  doc["v1_wet"] = g_v1_wet;
  doc["v1_to"] = g_v1_to;
  doc["v1_rest"] = g_v1_rest;
  doc["v1_time"] = g_v1_time;
  doc["v1_err"] = g_v1_err;
  doc["v1_name"] = g_v1_name;

  // Van 2
  doc["v2_run"] = g_v2_run;
  doc["v2_mode"] = g_v2_mode;
  doc["v2_soil"] = g_v2_soil;
  doc["v2_dry"] = g_v2_dry;
  doc["v2_wet"] = g_v2_wet;
  doc["v2_to"] = g_v2_to;
  doc["v2_rest"] = g_v2_rest;
  doc["v2_time"] = g_v2_time;
  doc["v2_err"] = g_v2_err;
  doc["v2_name"] = g_v2_name;

  // Khối môi trường
  doc["fan_mode"] = g_fanMode;
  doc["fan_speed"] = g_fanSpeed;
  doc["fan_pwm"] = g_fanPWM;
  doc["fan_setpoint"] = g_fanSetpoint;

  doc["mist_running"] = g_mistRunning;
  doc["mist_manual"] = g_mistManual;
  doc["mist_thresh"] = g_mistHumidThresh;

  doc["light_running"] = g_lightRunning;
  doc["light_manual"] = g_lightManual;
  doc["light_thresh"] = g_lightThresh;

  doc["pump_on"] = g_pumpOnThresh;
  doc["pump_off"] = g_pumpOffThresh;

  doc["heater_running"] = g_heaterRunning;
  doc["heater_manual"] = g_heaterManual;
  doc["heater_pwm"] = g_heaterPWM;
  doc["heater_setpoint"] = g_heaterSetpoint;

  doc["valve1_running"] = g_v1_run;
  doc["valve2_running"] = g_v2_run;
  doc["valve_manual"] = g_valveManual;

  doc["roof_angle"] = g_roofAngle;
  doc["roof_manual"] = g_roofManual;

  doc["sht30_ok"] = g_sht30OK;
  doc["sht30_ext_ok"] = g_sht30ExtOK;
  doc["temp_ext"] = g_tempExt;
  doc["humid_ext"] = g_humidExt;
  doc["bh1750_ok"] = g_bh1750OK;

  doc["wifi_rssi"] = g_wifiOK ? WiFi.RSSI() : 0;
  doc["ip"] = g_wifiOK ? WiFi.localIP().toString() : "0.0.0.0";
  doc["uptime"] = millis() / 1000;   // Uptime của Gateway
  doc["node_uptime"] = g_nodeUptime; // Uptime của Node
  doc["total_records"] = g_totalRecords;

  File f = LittleFS.open(CSV_FILE, "r");
  doc["file_kb"] = f ? (f.size() / 1024) : 0;
  if (f)
    f.close();
  doc["flash_total_kb"] = LittleFS.totalBytes() / 1024;
  doc["ntp_synced"] = g_ntpSynced;

  char timeStr[25];
  getTimestamp(timeStr, sizeof(timeStr));
  doc["timestamp"] = timeStr;

  char buf[1536];
  serializeJson(doc, buf);

  setCorsHeaders();
  webServer.send(200, "application/json", buf);
}

void handleApiControl() {
  setCorsHeaders();

  std::vector<String> cmdList;

  // -- Bơm --
  if (webServer.hasArg("pump")) {
    String val = webServer.arg("pump");
    StaticJsonDocument<128> d;
    if (val == "on") {
      d["cmd"] = "pump_on";
      g_pumpRunning = true;
      g_pumpManual = true;
    } else if (val == "off") {
      d["cmd"] = "pump_off";
      g_pumpRunning = false;
      g_pumpManual = true;
    } else if (val == "auto") {
      d["cmd"] = "pump_auto";
      g_pumpManual = false;
    }
    if (d.containsKey("cmd")) {
      String s;
      serializeJson(d, s);
      cmdList.push_back(s);
    }
  }

  // -- Quạt mode --
  if (webServer.hasArg("fan_mode")) {
    int m = webServer.arg("fan_mode").toInt();
    StaticJsonDocument<128> d;
    d["cmd"] = "fan";
    d["mode"] = m;
    g_fanMode = m;
    if (m == 0) {
      g_fanPWM = 0;
      g_fanSpeed = 0;
    }
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);
  }

  // -- Điều khiển Van tưới mới (AUTO / ON / OFF) --
  if (webServer.hasArg("valve") && webServer.hasArg("state")) {
    int vId = webServer.arg("valve").toInt();
    String st = webServer.arg("state");
    int mode = 1;
    if (st == "off")
      mode = 0;
    else if (st == "auto")
      mode = 1;
    else if (st == "on")
      mode = 2;

    StaticJsonDocument<128> d;
    d["cmd"] = "valve_set";
    d["id"] = vId;
    d["mode"] = mode;
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);

    if (vId == 1) {
      g_v1_mode = mode;
      if (mode == 0)
        g_v1_run = false;
      else if (mode == 2)
        g_v1_run = true;
    }
    if (vId == 2) {
      g_v2_mode = mode;
      if (mode == 0)
        g_v2_run = false;
      else if (mode == 2)
        g_v2_run = true;
    }
    g_valve1Running = g_v1_run;
    g_valve2Running = g_v2_run;
    g_pumpRunning = (g_v1_run || g_v2_run);
  }

  // -- Cấu hình ngưỡng & thời gian van tưới --
  if (webServer.hasArg("valve_cfg")) {
    int vId = webServer.arg("valve_cfg").toInt();
    StaticJsonDocument<128> d;
    d["cmd"] = "config"; // Chuẩn hóa dùng cmd "config" để Master Node luôn nhận diện chắc chắn
    d["id"] = vId;
    if (webServer.hasArg("dry")) {
      int dryVal = (int)round(webServer.arg("dry").toFloat());
      d["dry"] = dryVal;
      if (vId == 1) d["v1_dry"] = dryVal;
      else if (vId == 2) d["v2_dry"] = dryVal;
    }
    if (webServer.hasArg("wet")) {
      int wetVal = (int)round(webServer.arg("wet").toFloat());
      d["wet"] = wetVal;
      if (vId == 1) d["v1_wet"] = wetVal;
      else if (vId == 2) d["v2_wet"] = wetVal;
    }
    if (webServer.hasArg("timeout")) {
      int toVal = webServer.arg("timeout").toInt();
      d["to"] = toVal;
      if (vId == 1) d["v1_to"] = toVal;
      else if (vId == 2) d["v2_to"] = toVal;
    }
    if (webServer.hasArg("rest"))
      d["rest"] = webServer.arg("rest").toInt();
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);

    if (vId == 1) {
      if (webServer.hasArg("dry"))
        g_v1_dry = webServer.arg("dry").toFloat();
      if (webServer.hasArg("wet"))
        g_v1_wet = webServer.arg("wet").toFloat();
      if (webServer.hasArg("timeout"))
        g_v1_to = webServer.arg("timeout").toInt();
      if (webServer.hasArg("rest"))
        g_v1_rest = webServer.arg("rest").toInt();
      if (webServer.hasArg("name"))
        g_v1_name = webServer.arg("name");
    } else if (vId == 2) {
      if (webServer.hasArg("dry"))
        g_v2_dry = webServer.arg("dry").toFloat();
      if (webServer.hasArg("wet"))
        g_v2_wet = webServer.arg("wet").toFloat();
      if (webServer.hasArg("timeout"))
        g_v2_to = webServer.arg("timeout").toInt();
      if (webServer.hasArg("rest"))
        g_v2_rest = webServer.arg("rest").toInt();
      if (webServer.hasArg("name"))
        g_v2_name = webServer.arg("name");
    }
    saveGatewayConfig();
  }

  // -- Dừng khẩn cấp --
  if (webServer.hasArg("pump_emg")) {
    int val = webServer.arg("pump_emg").toInt();
    StaticJsonDocument<128> d;
    d["cmd"] = (val == 1) ? "pump_emg" : "pump_emg_reset";
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);

    g_pumpEmergencyStop = (val == 1);
    if (val == 1) {
      g_pumpRunning = false;
      g_v1_run = false;
      g_v2_run = false;
    }
  }

  // -- Khóa an toàn bơm (1: Cho phép, 0: Khóa) --
  if (webServer.hasArg("pump_lock")) {
    int val = webServer.arg("pump_lock").toInt();
    StaticJsonDocument<128> d;
    d["cmd"] = "pump_lock";
    d["val"] = val;
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);

    g_pumpEnabled = (val != 0);
    if (!g_pumpEnabled) {
      g_pumpRunning = false;
      g_v1_run = false;
      g_v2_run = false;
    }
  }

  // -- Chế độ tưới tuần tự --
  if (webServer.hasArg("pump_seq")) {
    int val = webServer.arg("pump_seq").toInt();
    StaticJsonDocument<128> d;
    d["cmd"] = "pump_seq";
    d["val"] = val;
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);

    g_pumpSeq = (val != 0);
  }

  // -- Quạt speed (manual) --
  if (webServer.hasArg("fan_speed")) {
    int spd = webServer.arg("fan_speed").toInt();
    StaticJsonDocument<128> d;
    d["cmd"] = "fan";
    d["mode"] = 2; // MANUAL
    d["speed"] = spd;
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);

    g_fanMode = 2;
    g_fanPWM = spd;
    if (spd == 0)
      g_fanSpeed = 0;
    else if (spd <= 125)
      g_fanSpeed = 40;
    else if (spd <= 180)
      g_fanSpeed = 60;
    else if (spd <= 230)
      g_fanSpeed = 80;
    else
      g_fanSpeed = 100;
  }

  // -- Setpoint quạt Auto --
  if (webServer.hasArg("setpoint")) {
    float sp = webServer.arg("setpoint").toFloat();
    StaticJsonDocument<128> d;
    d["cmd"] = "fan";
    d["mode"] = 1; // AUTO
    d["setpoint"] = sp;
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);

    g_fanMode = 1;
    g_fanSetpoint = sp;
    saveGatewayConfig();
  }

  // -- Cấu hình ngưỡng bơm --
  if (webServer.hasArg("pump_on") || webServer.hasArg("pump_off")) {
    StaticJsonDocument<128> d;
    d["cmd"] = "config";
    if (webServer.hasArg("pump_on")) {
      d["pump_on"] = webServer.arg("pump_on").toFloat();
      g_pumpOnThresh = d["pump_on"];
    }
    if (webServer.hasArg("pump_off")) {
      d["pump_off"] = webServer.arg("pump_off").toFloat();
      g_pumpOffThresh = d["pump_off"];
    }
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);
    saveGatewayConfig();
  }

  // -- Ngưỡng đèn --
  if (webServer.hasArg("light_thresh")) {
    float lth = webServer.arg("light_thresh").toFloat();
    StaticJsonDocument<128> d;
    d["cmd"] = "config";
    d["light_th"] = lth;
    g_lightThresh = lth;
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);
    saveGatewayConfig();
  }

  // -- Ngưỡng phun sương cấp ẩm --
  if (webServer.hasArg("mist_thresh")) {
    float mth = webServer.arg("mist_thresh").toFloat();
    StaticJsonDocument<128> d;
    d["cmd"] = "config";
    d["mist_th"] = mth;
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);

    g_mistHumidThresh = mth;
    saveGatewayConfig();
  }

  // -- Phun sương --
  if (webServer.hasArg("mist")) {
    String val = webServer.arg("mist");
    StaticJsonDocument<128> d;
    if (val == "on") {
      d["cmd"] = "mist_on";
      g_mistRunning = true;
      g_mistManual = true;
    } else if (val == "off") {
      d["cmd"] = "mist_off";
      g_mistRunning = false;
      g_mistManual = true;
    } else if (val == "auto") {
      d["cmd"] = "mist_auto";
      g_mistManual = false;
    }
    if (d.containsKey("cmd")) {
      String s;
      serializeJson(d, s);
      cmdList.push_back(s);
    }
  }

  // -- Đèn --
  if (webServer.hasArg("lamp")) {
    String val = webServer.arg("lamp");
    StaticJsonDocument<128> d;
    if (val == "on") {
      d["cmd"] = "light_on";
      g_lightRunning = true;
      g_lightManual = true;
    } else if (val == "off") {
      d["cmd"] = "light_off";
      g_lightRunning = false;
      g_lightManual = true;
    } else if (val == "auto") {
      d["cmd"] = "light_auto";
      g_lightManual = false;
    }
    if (d.containsKey("cmd")) {
      String s;
      serializeJson(d, s);
      cmdList.push_back(s);
    }
  }

  // -- Sưởi --
  if (webServer.hasArg("heater")) {
    String val = webServer.arg("heater");
    StaticJsonDocument<128> d;
    if (val == "on") {
      d["cmd"] = "heater_on";
      g_heaterRunning = true;
      g_heaterManual = true;
      g_heaterPWM = 255;
    } else if (val == "off") {
      d["cmd"] = "heater_off";
      g_heaterRunning = false;
      g_heaterManual = true;
      g_heaterPWM = 0;
    } else if (val == "auto") {
      d["cmd"] = "heater_auto";
      g_heaterManual = false;
    }
    if (d.containsKey("cmd")) {
      String s;
      serializeJson(d, s);
      cmdList.push_back(s);
    }
  }
  if (webServer.hasArg("heater_speed")) {
    int spd = constrain(webServer.arg("heater_speed").toInt(), 0, 255);
    StaticJsonDocument<128> d;
    d["cmd"] = "heater";
    d["speed"] = spd;
    g_heaterPWM = spd;
    g_heaterRunning = (spd > 0);
    g_heaterManual = true;
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);
  }
  if (webServer.hasArg("heater_setpoint")) {
    float sp = webServer.arg("heater_setpoint").toFloat();
    g_heaterSetpoint = sp;
    saveGatewayConfig();
    StaticJsonDocument<128> d;
    d["cmd"] = "heater_cfg";
    d["setpoint"] = sp;
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);
  }

  // -- Van điện từ 1 --
  if (webServer.hasArg("valve1")) {
    String val = webServer.arg("valve1");
    StaticJsonDocument<128> d;
    if (val == "on") {
      d["cmd"] = "valve1_on";
      g_valve1Running = true;
      g_valveManual = true;
    } else if (val == "off") {
      d["cmd"] = "valve1_off";
      g_valve1Running = false;
      g_valveManual = true;
    } else if (val == "auto") {
      d["cmd"] = "valve_auto";
      g_valveManual = false;
    }
    if (d.containsKey("cmd")) {
      String s;
      serializeJson(d, s);
      cmdList.push_back(s);
    }
  }

  // -- Van điện từ 2 --
  if (webServer.hasArg("valve2")) {
    String val = webServer.arg("valve2");
    StaticJsonDocument<128> d;
    if (val == "on") {
      d["cmd"] = "valve2_on";
      g_valve2Running = true;
      g_valveManual = true;
    } else if (val == "off") {
      d["cmd"] = "valve2_off";
      g_valve2Running = false;
      g_valveManual = true;
    } else if (val == "auto") {
      d["cmd"] = "valve_auto";
      g_valveManual = false;
    }
    if (d.containsKey("cmd")) {
      String s;
      serializeJson(d, s);
      cmdList.push_back(s);
    }
  }

  // -- Mái che --
  if (webServer.hasArg("roof")) {
    String val = webServer.arg("roof");
    StaticJsonDocument<128> d;
    if (val == "open") {
      d["cmd"] = "roof_open";
      g_roofAngle = 180;
      g_roofManual = true;
    } else if (val == "close") {
      d["cmd"] = "roof_close";
      g_roofAngle = 0;
      g_roofManual = true;
    } else if (val == "auto") {
      d["cmd"] = "roof_auto";
      g_roofManual = false;
    }
    if (d.containsKey("cmd")) {
      String s;
      serializeJson(d, s);
      cmdList.push_back(s);
    }
  }
  if (webServer.hasArg("roof_angle")) {
    int ang = constrain(webServer.arg("roof_angle").toInt(), 0, 180);
    StaticJsonDocument<128> d;
    d["cmd"] = "roof_set";
    d["angle"] = ang;
    g_roofAngle = ang;
    g_roofManual = true;
    String s;
    serializeJson(d, s);
    cmdList.push_back(s);
  }

  if (!cmdList.empty()) {
    for (size_t i = 0; i < cmdList.size(); i++) {
      sendLoRaCommand(cmdList[i], true);
      if (i + 1 < cmdList.size()) {
        delay(250); // Đảm bảo E32 phát xong gói vô tuyến trước khi gửi lệnh tiếp theo
      }
    }

    StaticJsonDocument<512> resDoc;
    resDoc["status"] = "success";
    resDoc["light_running"] = g_lightRunning;
    resDoc["light_manual"] = g_lightManual;
    resDoc["mist_running"] = g_mistRunning;
    resDoc["mist_manual"] = g_mistManual;
    resDoc["heater_running"] = g_heaterRunning;
    resDoc["heater_manual"] = g_heaterManual;
    resDoc["heater_pwm"] = g_heaterPWM;
    resDoc["heater_setpoint"] = g_heaterSetpoint;
    resDoc["fan_mode"] = g_fanMode;
    resDoc["fan_speed"] = g_fanSpeed;
    resDoc["fan_setpoint"] = g_fanSetpoint;
    resDoc["roof_angle"] = g_roofAngle;
    resDoc["roof_manual"] = g_roofManual;
    resDoc["pump_running"] = g_pumpRunning;
    resDoc["pump_manual"] = g_pumpManual;
    resDoc["v1_run"] = g_v1_run;
    resDoc["v1_mode"] = g_v1_mode;
    resDoc["v1_dry"] = g_v1_dry;
    resDoc["v1_wet"] = g_v1_wet;
    resDoc["v1_to"] = g_v1_to;
    resDoc["v2_run"] = g_v2_run;
    resDoc["v2_mode"] = g_v2_mode;
    resDoc["v2_dry"] = g_v2_dry;
    resDoc["v2_wet"] = g_v2_wet;
    resDoc["v2_to"] = g_v2_to;
    String resStr;
    serializeJson(resDoc, resStr);
    webServer.send(200, "application/json", resStr);
  } else {
    webServer.send(400, "application/json",
                   "{\"status\":\"error\",\"msg\":\"Unknown params\"}");
  }
}

void handleApiForecast() {
  StaticJsonDocument<512> doc;

  doc["ready"] = g_forecast_ready;
  if (g_forecast_ready) {
    JsonArray arrT = doc.createNestedArray("temp");
    JsonArray arrH = doc.createNestedArray("humid");
    JsonArray arrL = doc.createNestedArray("lux");
    JsonArray arrM = doc.createNestedArray("min");
    const int labels[] = {30, 60, 90};
    for (int h = 0; h < 3; h++) {
      arrT.add(isnan(g_fc_temp[h]) ? 0 : round(g_fc_temp[h] * 10) / 10.0f);
      arrH.add(isnan(g_fc_humid[h]) ? 0 : round(g_fc_humid[h] * 10) / 10.0f);
      arrL.add(isnan(g_fc_lux[h]) ? 0 : (int)g_fc_lux[h]);
      arrM.add(labels[h]);
    }
  }

  char buf[512];
  serializeJson(doc, buf);
  setCorsHeaders();
  webServer.send(200, "application/json", buf);
}

void handleDownloadCSV() {
  setCorsHeaders();
  if (!LittleFS.exists(CSV_FILE)) {
    webServer.send(404, "text/plain", "File không tồn tại");
    return;
  }

  File f = LittleFS.open(CSV_FILE, "r");
  if (!f) {
    webServer.send(500, "text/plain", "Không thể mở file");
    return;
  }

  webServer.sendHeader("Content-Disposition", "attachment; filename=data.csv");
  webServer.streamFile(f, "text/csv");
  f.close();
}

void handleDeleteCSV() {
  setCorsHeaders();
  if (LittleFS.exists(CSV_FILE)) {
    LittleFS.remove(CSV_FILE);
    Serial.println(F("[FS] Đã xóa file CSV theo yêu cầu Web."));
    initDataLogger();
    webServer.send(200, "application/json", "{\"status\":\"success\"}");
  } else {
    webServer.send(404, "application/json",
                   "{\"status\":\"error\",\"msg\":\"File không tồn tại\"}");
  }
}
