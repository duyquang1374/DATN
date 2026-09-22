/*
 * ============================================================
 *  HỆ THỐNG NHÀ MÀNG THÔNG MINH - GATEWAY ESP32
 * ============================================================
 *  Chức năng:
 *    - Đọc nhiệt độ & độ ẩm từ SHT30 (I2C)
 *    - Đọc cường độ ánh sáng từ BH1750 (I2C)
 *    - Lưu dữ liệu vào bộ nhớ flash (LittleFS) dạng CSV
 *    - Chạy Web Server để tải CSV về máy tính khi cần
 *    - Gửi dữ liệu qua MQTT (tùy chọn, nếu broker có sẵn)
 *    - Đồng bộ thời gian NTP
 *
 *  Không cần treo laptop! ESP32 tự thu thập 24/7.
 *  Khi cần train AI → mở trình duyệt → vào IP ESP32 → tải CSV
 *
 *  Phần cứng:
 *    - ESP32 DevKit
 *    - SHT30 → I2C (SDA: GPIO 21, SCL: GPIO 22)
 *    - BH1750 → I2C (SDA: GPIO 21, SCL: GPIO 22)
 *    - LED_BUILTIN → GPIO 2
 *
 *  Libraries cần cài (Arduino IDE → Library Manager):
 *    - Adafruit SHT31 Library (by Adafruit)
 *    - BH1750 (by Christopher Laws)
 *    - PubSubClient (Nick O'Leary)
 *    - ArduinoJson (Benoit Blanchon)
 *    - LittleFS (built-in ESP32)
 * ============================================================
 */

#include <Adafruit_SHT31.h>
#include <ArduinoJson.h>
#include <BH1750.h>
#include <LittleFS.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Wire.h>
#include <time.h>

// ======================== CẤU HÌNH ========================

// --- WiFi ---
#define WIFI_SSID "Xoi Banh My Chi Nga"
#define WIFI_PASSWORD "13071982"

// --- MQTT Broker (tùy chọn) ---
#define MQTT_ENABLED true // Đặt false nếu không dùng MQTT
#define MQTT_SERVER "192.168.1.227"
#define MQTT_PORT 1883
#define MQTT_TOPIC "greenhouse/sensor"
#define MQTT_CLIENT_ID "ESP32_Greenhouse"

// --- I2C Pins ---
#define I2C_SDA 21
#define I2C_SCL 22

// --- Timing ---
#define SENSOR_INTERVAL 30000 // 30 giây đọc sensor
#define MQTT_RECONNECT_DELAY 5000
#define WIFI_RECONNECT_DELAY 10000

// --- LED Status ---
#define LED_PIN 2

// --- NTP ---
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET 25200 // GMT+7
#define DAYLIGHT_OFFSET 0

// --- Storage ---
#define CSV_FILE "/data.csv"
#define MAX_FILE_SIZE 1400000 // ~1.4MB (để lại 100KB cho hệ thống)

// ======================== OBJECTS ========================

WiFiClient espClient;
PubSubClient mqttClient(espClient);
Adafruit_SHT31 sht30 = Adafruit_SHT31(); // SHT30
BH1750 lightMeter;
WebServer server(80);

// ======================== VARIABLES ========================

unsigned long lastSensorRead = 0;
unsigned long lastMqttReconnect = 0;
unsigned long lastWifiReconnect = 0;
bool ntpSynced = false;
int sensorErrorCount = 0;
unsigned long totalRecords = 0;

// Lưu giá trị đọc gần nhất để hiển thị trên web
float lastTemperature = NAN;
float lastHumidity = NAN;
float lastLight = NAN;

#define MAX_SENSOR_ERRORS 10

// ======================== SETUP ========================

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println("============================================");
  Serial.println("  NHÀ MÀNG THÔNG MINH - ESP32 GATEWAY v3");
  Serial.println("  SHT30 + BH1750 | Tự lưu + Web Server");
  Serial.println("============================================");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println("[I2C] SDA=" + String(I2C_SDA) + " SCL=" + String(I2C_SCL));

  // SHT30
  if (sht30.begin(0x44)) { // Địa chỉ mặc định SHT30 = 0x44
    Serial.println("[SHT30] Init OK ✓");
    sht30.heater(false); // Tắt heater
  } else {
    Serial.println("[SHT30] Init FAILED! Kiểm tra kết nối I2C.");
  }

  // BH1750
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("[BH1750] Init OK ✓");
  } else {
    Serial.println("[BH1750] Init FAILED! Kiểm tra kết nối I2C.");
  }

  // LittleFS
  if (!LittleFS.begin(true)) { // true = format if failed
    Serial.println("[FS] LittleFS MOUNT FAILED!");
  } else {
    Serial.println("[FS] LittleFS OK");
    checkStorage();

    // Tạo CSV header nếu file chưa tồn tại
    if (!LittleFS.exists(CSV_FILE)) {
      File f = LittleFS.open(CSV_FILE, "w");
      if (f) {
        f.println("timestamp,temperature,humidity,light");
        f.close();
        Serial.println("[FS] Created new CSV file");
      }
    } else {
      // Đếm số dòng hiện có
      totalRecords = countRecords();
      Serial.println("[FS] Existing records: " + String(totalRecords));
    }
  }

  // WiFi
  setupWiFi();

  // NTP - Đợi sync xong trước khi thu thập
  configTime(GMT_OFFSET, DAYLIGHT_OFFSET, NTP_SERVER);
  Serial.print("[NTP] Đang đồng bộ thời gian");
  struct tm timeinfo;
  int ntpAttempts = 0;
  while (!getLocalTime(&timeinfo, 1000) && ntpAttempts < 15) {
    Serial.print(".");
    ntpAttempts++;
    delay(1000);
  }
  if (ntpAttempts < 15) {
    ntpSynced = true;
    char buf[25];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
    Serial.println("\n[NTP] ✓ Đồng bộ thành công: " + String(buf));
  } else {
    Serial.println("\n[NTP] ✗ Chưa sync được! Timestamp sẽ dùng millis.");
    Serial.println("[NTP] Sẽ thử lại trong loop...");
  }

  // MQTT (tùy chọn)
  if (MQTT_ENABLED) {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setBufferSize(512);
    Serial.println("[MQTT] Server: " + String(MQTT_SERVER));
  }

  // Web Server
  setupWebServer();

  Serial.println("============================================");
  Serial.println("[SYSTEM] Khởi động hoàn tất!");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[WEB] Truy cập: http://" + WiFi.localIP().toString());
    Serial.println("[WEB] Tải CSV:  http://" + WiFi.localIP().toString() +
                   "/download");
  }
  Serial.println("============================================");
}

// ======================== LOOP ========================

void loop() {
  unsigned long now = millis();

  // Web server luôn xử lý
  server.handleClient();

  // Kiểm tra WiFi
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, LOW);
    if (now - lastWifiReconnect >= WIFI_RECONNECT_DELAY) {
      lastWifiReconnect = now;
      setupWiFi();
    }
    // VẪN đọc sensor và lưu local dù mất WiFi!
  }

  // MQTT (nếu bật)
  if (MQTT_ENABLED && WiFi.status() == WL_CONNECTED) {
    if (!mqttClient.connected()) {
      if (now - lastMqttReconnect >= MQTT_RECONNECT_DELAY) {
        lastMqttReconnect = now;
        reconnectMQTT();
      }
    } else {
      mqttClient.loop();
    }
  }

  // NTP sync
  if (!ntpSynced) {
    syncNTP();
  }

  // Đọc sensor và lưu
  if (now - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = now;
    readAndSave();
  }

  // LED
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_PIN, HIGH);
  }
}

// ======================== WEB SERVER ========================

void setupWebServer() {
  // Trang chủ - Dashboard mini
  server.on("/", HTTP_GET, handleRoot);

  // Tải CSV
  server.on("/download", HTTP_GET, handleDownload);

  // API lấy dữ liệu JSON
  server.on("/api/data", HTTP_GET, handleApiData);

  // API thống kê
  server.on("/api/stats", HTTP_GET, handleApiStats);

  // Xóa dữ liệu (sau khi đã tải về)
  server.on("/delete", HTTP_GET, handleDelete);

  server.begin();
  Serial.println("[WEB] Server started on port 80");
}

// Trang chủ
void handleRoot() {
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  size_t fileSize = 0;
  if (LittleFS.exists(CSV_FILE)) {
    File f = LittleFS.open(CSV_FILE, "r");
    fileSize = f.size();
    f.close();
  }

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='30'>"; // Auto refresh 30s
  html += "<title>Nhà Màng Thông Minh</title>";
  html += "<style>";
  html += "* {margin:0;padding:0;box-sizing:border-box;}";
  html += "body {font-family:'Segoe "
          "UI',sans-serif;background:#0f1117;color:#f0f0f5;padding:20px;}";
  html += ".container {max-width:600px;margin:0 auto;}";
  html += "h1 {text-align:center;margin-bottom:20px;font-size:22px;";
  html += "background:linear-gradient(135deg,#22c55e,#06b6d4);-webkit-"
          "background-clip:text;-webkit-text-fill-color:transparent;}";
  html += ".card {background:#1e2130;border:1px solid "
          "rgba(255,255,255,0.06);border-radius:12px;padding:20px;margin-"
          "bottom:12px;}";
  html += ".card h2 {font-size:14px;color:#8b8fa3;margin-bottom:8px;}";
  html += ".value {font-size:32px;font-weight:700;}";
  html += ".temp {color:#f59e0b;} .humid {color:#3b82f6;} .light "
          "{color:#eab308;} .green {color:#22c55e;}";
  html += ".btn {display:inline-block;padding:12px "
          "24px;border-radius:8px;text-decoration:none;";
  html += "font-weight:600;margin:4px;text-align:center;font-size:14px;}";
  html += ".btn-green "
          "{background:linear-gradient(135deg,#22c55e,#16a34a);color:#fff;}";
  html += ".btn-blue "
          "{background:linear-gradient(135deg,#3b82f6,#2563eb);color:#fff;}";
  html += ".btn-red "
          "{background:linear-gradient(135deg,#ef4444,#dc2626);color:#fff;}";
  html += ".actions {text-align:center;margin-top:16px;}";
  html += ".info {font-size:13px;color:#5a5e72;margin-top:8px;}";
  html += "</style></head><body><div class='container'>";
  html += "<h1>🌿 Nhà Màng Thông Minh</h1>";

  // Nhiệt độ
  html += "<div class='card'><h2>🌡️ NHIỆT ĐỘ</h2>";
  if (!isnan(lastTemperature)) {
    html +=
        "<div class='value temp'>" + String(lastTemperature, 1) + " °C</div>";
  } else {
    html += "<div class='value'>-- °C</div>";
  }
  html += "</div>";

  // Độ ẩm
  html += "<div class='card'><h2>💧 ĐỘ ẨM</h2>";
  if (!isnan(lastHumidity)) {
    html += "<div class='value humid'>" + String(lastHumidity, 1) + " %</div>";
  } else {
    html += "<div class='value'>-- %</div>";
  }
  html += "</div>";

  // Ánh sáng
  html += "<div class='card'><h2>☀️ ÁNH SÁNG</h2>";
  if (!isnan(lastLight)) {
    html += "<div class='value light'>" + String(lastLight, 1) + " lux</div>";
  } else {
    html += "<div class='value'>-- lux</div>";
  }
  html += "</div>";

  // Thống kê
  html += "<div class='card'><h2>📊 DỮ LIỆU THU THẬP</h2>";
  html += "<div class='value green'>" + String(totalRecords) + " mẫu</div>";
  html += "<div class='info'>File: " + String(fileSize / 1024) + " KB / " +
          String(MAX_FILE_SIZE / 1024) + " KB</div>";
  html += "<div class='info'>Tần suất: mỗi 30 giây</div>";

  float daysOfData = totalRecords * 30.0 / 86400.0;
  html += "<div class='info'>Thời gian: ~" + String(daysOfData, 1) +
          " ngày dữ liệu</div>";
  html += "</div>";

  // Actions
  html += "<div class='actions'>";
  html += "<a class='btn btn-green' href='/download'>💾 Tải CSV</a> ";
  html += "<a class='btn btn-blue' href='/api/stats'>📊 API Stats</a> ";
  html += "<a class='btn btn-red' href='/delete' onclick=\"return confirm('Xóa "
          "toàn bộ dữ liệu?')\">🗑️ Xóa Data</a>";
  html += "</div>";

  html += "<div class='info' style='text-align:center;margin-top:16px;'>";
  html += "Tự động cập nhật mỗi 30 giây | IP: " + WiFi.localIP().toString();
  html += "</div>";

  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

// Tải CSV
void handleDownload() {
  if (!LittleFS.exists(CSV_FILE)) {
    server.send(404, "text/plain", "No data file found!");
    return;
  }

  File f = LittleFS.open(CSV_FILE, "r");
  server.streamFile(f, "text/csv");
  f.close();
  Serial.println("[WEB] CSV downloaded by client");
}

// API dữ liệu (100 dòng cuối)
void handleApiData() {
  if (!LittleFS.exists(CSV_FILE)) {
    server.send(404, "application/json", "{\"error\":\"no data\"}");
    return;
  }

  // Đọc 100 dòng cuối
  File f = LittleFS.open(CSV_FILE, "r");
  String lines[100];
  int lineCount = 0;
  int idx = 0;

  f.readStringUntil('\n'); // Skip header
  while (f.available()) {
    String line = f.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      lines[idx % 100] = line;
      idx++;
      lineCount++;
    }
  }
  f.close();

  // Build JSON
  String json = "{\"total\":" + String(lineCount) + ",\"data\":[";
  int start = (idx > 100) ? idx - 100 : 0;
  int count = min(lineCount, 100);

  for (int i = 0; i < count; i++) {
    int li = (start + i) % 100;
    if (lines[li].length() == 0)
      continue;

    // Parse CSV line: timestamp,temperature,humidity,light
    int c1 = lines[li].indexOf(',');
    int c2 = lines[li].indexOf(',', c1 + 1);
    int c3 = lines[li].indexOf(',', c2 + 1);

    if (c1 > 0 && c2 > 0 && c3 > 0) {
      if (i > 0)
        json += ",";
      json += "{\"t\":\"" + lines[li].substring(0, c1) + "\"";
      json += ",\"temp\":" + lines[li].substring(c1 + 1, c2);
      json += ",\"humid\":" + lines[li].substring(c2 + 1, c3);
      json += ",\"light\":" + lines[li].substring(c3 + 1) + "}";
    }
  }
  json += "]}";

  server.send(200, "application/json", json);
}

// API thống kê
void handleApiStats() {
  size_t fileSize = 0;
  if (LittleFS.exists(CSV_FILE)) {
    File f = LittleFS.open(CSV_FILE, "r");
    fileSize = f.size();
    f.close();
  }

  size_t totalBytes = LittleFS.totalBytes();
  size_t usedBytes = LittleFS.usedBytes();

  String json = "{";
  json += "\"records\":" + String(totalRecords);
  json += ",\"file_size_kb\":" + String(fileSize / 1024);
  json += ",\"flash_total_kb\":" + String(totalBytes / 1024);
  json += ",\"flash_used_kb\":" + String(usedBytes / 1024);
  json += ",\"flash_free_kb\":" + String((totalBytes - usedBytes) / 1024);
  json += ",\"days\":" + String(totalRecords * 30.0 / 86400.0, 1);
  json += ",\"ip\":\"" + WiFi.localIP().toString() + "\"";
  json += ",\"uptime\":" + String(millis() / 1000);
  json += "}";

  server.send(200, "application/json", json);
}

// Xóa dữ liệu
void handleDelete() {
  LittleFS.remove(CSV_FILE);

  // Tạo file mới
  File f = LittleFS.open(CSV_FILE, "w");
  if (f) {
    f.println("timestamp,temperature,humidity,light");
    f.close();
  }
  totalRecords = 0;

  Serial.println("[FS] Data deleted by user");
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Redirecting...");
}

// ======================== SENSOR & STORAGE ========================

void readAndSave() {
  // === Đọc SHT30 ===
  float temperature = sht30.readTemperature();
  float humidity = sht30.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    sensorErrorCount++;
  } else {
    sensorErrorCount = 0;
  }

  if (sensorErrorCount > 0) {
    Serial.println("[SHT30] Read error! (" + String(sensorErrorCount) + "/" +
                   String(MAX_SENSOR_ERRORS) + ")");
    if (sensorErrorCount >= MAX_SENSOR_ERRORS) {
      Serial.println("[SHT30] TOO MANY ERRORS! Check wiring.");
    }
  }

  // === Đọc BH1750 ===
  float light = lightMeter.readLightLevel();

  // Kiểm tra lỗi tổng hợp
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("[SENSOR] SHT30 data invalid, skipping...");
    return;
  }

  if (light < 0) {
    Serial.println("[BH1750] Read error, skipping...");
    return;
  }
  sensorErrorCount = 0;

  // Validate
  if (temperature < -10 || temperature > 60 || humidity < 0 || humidity > 100) {
    Serial.println("[SHT30] Invalid data: " + String(temperature) + "°C, " +
                   String(humidity) + "%");
    return;
  }

  if (light > 65535) {
    Serial.println("[BH1750] Invalid light: " + String(light) + " lux");
    return;
  }

  // Lấy thời gian
  struct tm timeinfo;
  char timeStr[25];
  if (getLocalTime(&timeinfo)) {
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
  } else {
    // Fallback: dùng millis
    snprintf(timeStr, sizeof(timeStr), "%lu", millis() / 1000);
  }

  // Làm tròn
  temperature = round(temperature * 10.0) / 10.0;
  humidity = round(humidity * 10.0) / 10.0;
  light = round(light * 10.0) / 10.0;

  // Cập nhật giá trị hiển thị
  lastTemperature = temperature;
  lastHumidity = humidity;
  lastLight = light;

  // === LƯU VÀO FLASH ===
  saveToFlash(timeStr, temperature, humidity, light);

  // === GỬI MQTT (nếu bật) ===
  if (MQTT_ENABLED && mqttClient.connected()) {
    sendMQTT(timeStr, temperature, humidity, light);
  }

  // Print
  Serial.println("─────────────────────────────────");
  Serial.println("[#" + String(totalRecords) + "] " + String(timeStr));
  Serial.println("[DATA] " + String(temperature, 1) + "°C | " +
                 String(humidity, 1) + "% | " + String(light, 1) + " lux");
}

void saveToFlash(const char *timeStr, float temp, float humid, float light) {
  // Kiểm tra dung lượng
  if (LittleFS.exists(CSV_FILE)) {
    File f = LittleFS.open(CSV_FILE, "r");
    size_t size = f.size();
    f.close();

    if (size >= MAX_FILE_SIZE) {
      Serial.println("[FS] File đầy! Hãy tải CSV và xóa data.");
      Serial.println("[FS] Truy cập http://" + WiFi.localIP().toString() +
                     "/download");
      // Tự động xóa nửa đầu file (giữ lại nửa sau)
      trimFile();
    }
  }

  // Ghi dữ liệu
  File f = LittleFS.open(CSV_FILE, "a");
  if (f) {
    f.printf("%s,%.1f,%.1f,%.1f\n", timeStr, temp, humid, light);
    f.close();
    totalRecords++;
    Serial.println("[FS] Saved ✓ (Total: " + String(totalRecords) + ")");
  } else {
    Serial.println("[FS] Write FAILED!");
  }
}

void trimFile() {
  // Khi file đầy, xóa nửa đầu (giữ dữ liệu mới)
  Serial.println("[FS] Trimming old data...");

  File f = LittleFS.open(CSV_FILE, "r");
  if (!f)
    return;

  // Đọc header
  String header = f.readStringUntil('\n');

  // Skip nửa đầu
  size_t halfSize = f.size() / 2;
  f.seek(halfSize);
  f.readStringUntil('\n'); // Skip partial line

  // Đọc nửa sau
  String remaining = f.readString();
  f.close();

  // Ghi lại
  f = LittleFS.open(CSV_FILE, "w");
  if (f) {
    f.println(header);
    f.print(remaining);
    f.close();
    totalRecords = countRecords();
    Serial.println("[FS] Trimmed! Remaining: " + String(totalRecords));
  }
}

// ======================== HELPERS ========================

void sendMQTT(const char *timeStr, float temp, float humid, float light) {
  StaticJsonDocument<256> doc;
  doc["temperature"] = temp;
  doc["humidity"] = humid;
  doc["light"] = light;
  doc["timestamp"] = timeStr;
  doc["device"] = MQTT_CLIENT_ID;

  char buffer[256];
  serializeJson(doc, buffer);

  if (mqttClient.publish(MQTT_TOPIC, buffer)) {
    Serial.println("[MQTT] Sent ✓");
  }
}

unsigned long countRecords() {
  if (!LittleFS.exists(CSV_FILE))
    return 0;

  File f = LittleFS.open(CSV_FILE, "r");
  unsigned long count = 0;
  f.readStringUntil('\n'); // Skip header
  while (f.available()) {
    f.readStringUntil('\n');
    count++;
  }
  f.close();
  return count;
}

void checkStorage() {
  size_t total = LittleFS.totalBytes();
  size_t used = LittleFS.usedBytes();
  Serial.printf("[FS] Total: %d KB | Used: %d KB | Free: %d KB\n", total / 1024,
                used / 1024, (total - used) / 1024);
}

void setupWiFi() {
  Serial.println("[WiFi] Connecting to: " + String(WIFI_SSID));
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Connected! IP: " + WiFi.localIP().toString());
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.println("\n[WiFi] Failed! Will retry...");
  }
}

void reconnectMQTT() {
  if (mqttClient.connect(MQTT_CLIENT_ID)) {
    Serial.println("[MQTT] Connected ✓");
  }
}

void syncNTP() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo, 1000)) {
    ntpSynced = true;
    char buf[25];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
    Serial.println("[NTP] Synced: " + String(buf));
  }
}
