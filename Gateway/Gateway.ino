/*
 * ============================================================
 *  HỆ THỐNG NHÀ MÀNG THÔNG MINH - GATEWAY ESP32
 * ============================================================
 *  Chức năng:
 *    - Đọc nhiệt độ & độ ẩm từ DHT11/DHT22
 *    - Gửi dữ liệu qua MQTT dạng JSON
 *    - Đồng bộ thời gian NTP
 *    - Tự động reconnect WiFi & MQTT
 *    - LED báo trạng thái
 *
 *  Phần cứng:
 *    - ESP32 DevKit
 *    - DHT11 (hoặc DHT22) → GPIO 4
 *    - LED_BUILTIN → GPIO 2
 *
 *  Libraries cần cài (Arduino IDE → Library Manager):
 *    - DHT sensor library (Adafruit)
 *    - Adafruit Unified Sensor
 *    - PubSubClient (Nick O'Leary)
 *    - ArduinoJson (Benoit Blanchon)
 * ============================================================
 */

#include <ArduinoJson.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <time.h>

// ======================== CẤU HÌNH ========================

// --- WiFi ---
#define WIFI_SSID "Xoi Banh My Chi Nga" // ← Thay đổi
#define WIFI_PASSWORD "13071982"        // ← Thay đổi

// --- MQTT Broker ---
#define MQTT_SERVER "192.168.1.227" // ← IP máy chạy Mosquitto
#define MQTT_PORT 1883
#define MQTT_TOPIC "greenhouse/sensor"
#define MQTT_CLIENT_ID "ESP32_Greenhouse"
// Nếu broker có auth, uncomment và điền:
// #define MQTT_USER    "username"
// #define MQTT_PASS    "password"

// --- DHT Sensor ---
#define DHTPIN 4      // GPIO 4
#define DHTTYPE DHT11 // Đổi thành DHT22 nếu dùng DHT22

// --- Timing ---
#define SENSOR_INTERVAL 30000      // 30 giây đọc sensor 1 lần
#define MQTT_RECONNECT_DELAY 5000  // 5 giây thử reconnect
#define WIFI_RECONNECT_DELAY 10000 // 10 giây thử reconnect WiFi

// --- LED Status ---
#define LED_PIN 2 // LED_BUILTIN trên ESP32

// --- NTP ---
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET 25200 // GMT+7 (Việt Nam) = 7 * 3600
#define DAYLIGHT_OFFSET 0

// ======================== OBJECTS ========================

WiFiClient espClient;
PubSubClient mqttClient(espClient);
DHT dht(DHTPIN, DHTTYPE);

// ======================== VARIABLES ========================

unsigned long lastSensorRead = 0;
unsigned long lastMqttReconnect = 0;
unsigned long lastWifiReconnect = 0;
bool ntpSynced = false;

// Bộ đếm lỗi đọc sensor liên tiếp
int sensorErrorCount = 0;
#define MAX_SENSOR_ERRORS 10

// ======================== SETUP ========================

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println("============================================");
  Serial.println("  NHÀ MÀNG THÔNG MINH - ESP32 GATEWAY");
  Serial.println("============================================");

  // LED setup
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // DHT setup
  dht.begin();
  Serial.println("[DHT] Sensor initialized on GPIO " + String(DHTPIN));

  // WiFi setup
  setupWiFi();

  // NTP setup
  configTime(GMT_OFFSET, DAYLIGHT_OFFSET, NTP_SERVER);
  Serial.println("[NTP] Đang đồng bộ thời gian...");

  // MQTT setup
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setBufferSize(512); // Tăng buffer cho JSON lớn
  Serial.println("[MQTT] Server: " + String(MQTT_SERVER) + ":" +
                 String(MQTT_PORT));

  Serial.println("============================================");
  Serial.println("[SYSTEM] Khởi động hoàn tất!");
  Serial.println("============================================");
}

// ======================== LOOP ========================

void loop() {
  unsigned long now = millis();

  // --- Kiểm tra WiFi ---
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, LOW); // LED tắt = mất WiFi
    if (now - lastWifiReconnect >= WIFI_RECONNECT_DELAY) {
      lastWifiReconnect = now;
      Serial.println("[WiFi] Mất kết nối! Đang reconnect...");
      setupWiFi();
    }
    return; // Không làm gì khác nếu chưa có WiFi
  }

  // --- Kiểm tra & kết nối MQTT ---
  if (!mqttClient.connected()) {
    if (now - lastMqttReconnect >= MQTT_RECONNECT_DELAY) {
      lastMqttReconnect = now;
      reconnectMQTT();
    }
  } else {
    mqttClient.loop(); // Xử lý MQTT messages
  }

  // --- Đồng bộ NTP (chỉ 1 lần) ---
  if (!ntpSynced) {
    syncNTP();
  }

  // --- Đọc sensor và gửi dữ liệu ---
  if (now - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = now;
    readAndPublish();
  }

  // --- LED blink khi MQTT connected ---
  if (mqttClient.connected()) {
    // LED sáng liên tục = đang hoạt động bình thường
    digitalWrite(LED_PIN, HIGH);
  }
}

// ======================== FUNCTIONS ========================

/**
 * Kết nối WiFi
 */
void setupWiFi() {
  Serial.println("[WiFi] Đang kết nối đến: " + String(WIFI_SSID));

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("[WiFi] Kết nối thành công!");
    Serial.println("[WiFi] IP: " + WiFi.localIP().toString());
    Serial.println("[WiFi] RSSI: " + String(WiFi.RSSI()) + " dBm");
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.println();
    Serial.println("[WiFi] Kết nối thất bại! Sẽ thử lại...");
  }
}

/**
 * Kết nối MQTT broker
 */
void reconnectMQTT() {
  Serial.println("[MQTT] Đang kết nối broker...");

  bool connected = false;

#ifdef MQTT_USER
  connected = mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS);
#else
  connected = mqttClient.connect(MQTT_CLIENT_ID);
#endif

  if (connected) {
    Serial.println("[MQTT] Kết nối thành công!");

    // Gửi message báo online
    StaticJsonDocument<128> onlineDoc;
    onlineDoc["status"] = "online";
    onlineDoc["ip"] = WiFi.localIP().toString();

    char onlineBuffer[128];
    serializeJson(onlineDoc, onlineBuffer);
    mqttClient.publish("greenhouse/status", onlineBuffer, true); // retained
  } else {
    Serial.print("[MQTT] Kết nối thất bại, rc=");
    Serial.println(mqttClient.state());
    Serial.println("[MQTT] Sẽ thử lại sau " +
                   String(MQTT_RECONNECT_DELAY / 1000) + "s");
  }
}

/**
 * Đồng bộ thời gian NTP
 */
void syncNTP() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo, 1000)) { // timeout 1s
    ntpSynced = true;
    Serial.println("[NTP] Đồng bộ thành công: " + formatTime(&timeinfo));
  }
}

/**
 * Format thời gian thành chuỗi
 */
String formatTime(struct tm *timeinfo) {
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}

/**
 * Lấy Unix timestamp hiện tại
 */
unsigned long getTimestamp() {
  time_t now;
  time(&now);
  return (unsigned long)now;
}

/**
 * Đọc sensor DHT và publish lên MQTT
 */
void readAndPublish() {
  // Đọc sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  // Kiểm tra lỗi đọc
  if (isnan(humidity) || isnan(temperature)) {
    sensorErrorCount++;
    Serial.println("[DHT] Lỗi đọc sensor! (lần " + String(sensorErrorCount) +
                   "/" + String(MAX_SENSOR_ERRORS) + ")");

    if (sensorErrorCount >= MAX_SENSOR_ERRORS) {
      Serial.println("[DHT] QUÁ NHIỀU LỖI! Kiểm tra kết nối sensor.");
      // Có thể restart ESP32:
      // ESP.restart();
    }
    return;
  }

  // Reset bộ đếm lỗi khi đọc thành công
  sensorErrorCount = 0;

  // Validate dữ liệu (loại bỏ giá trị bất thường)
  if (temperature < -10 || temperature > 60) {
    Serial.println("[DHT] Nhiệt độ bất thường: " + String(temperature) +
                   "°C → Bỏ qua");
    return;
  }
  if (humidity < 0 || humidity > 100) {
    Serial.println("[DHT] Độ ẩm bất thường: " + String(humidity) +
                   "% → Bỏ qua");
    return;
  }

  // Lấy thời gian
  unsigned long timestamp = getTimestamp();
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  // In ra Serial
  Serial.println("─────────────────────────────────");
  Serial.println("[DATA] " + formatTime(&timeinfo));
  Serial.println("[DATA] Nhiệt độ: " + String(temperature, 1) + " °C");
  Serial.println("[DATA] Độ ẩm:    " + String(humidity, 1) + " %");

  // Tạo JSON payload
  StaticJsonDocument<256> doc;
  doc["temperature"] = round(temperature * 10.0) / 10.0; // Làm tròn 1 chữ số
  doc["humidity"] = round(humidity * 10.0) / 10.0;
  doc["timestamp"] = timestamp;
  doc["device"] = MQTT_CLIENT_ID;

  char jsonBuffer[256];
  size_t jsonLen = serializeJson(doc, jsonBuffer);

  // Publish lên MQTT
  if (mqttClient.connected()) {
    bool published = mqttClient.publish(MQTT_TOPIC, jsonBuffer);
    if (published) {
      Serial.println("[MQTT] ✓ Đã gửi (" + String(jsonLen) + " bytes)");
      Serial.println("[MQTT] → " + String(jsonBuffer));
    } else {
      Serial.println("[MQTT] ✗ Gửi thất bại!");
    }
  } else {
    Serial.println("[MQTT] Chưa kết nối → Dữ liệu mất!");
  }
}
