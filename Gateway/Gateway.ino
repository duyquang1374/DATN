/*
 * Gateway.ino
 * Gateway trung tâm cho Nhà màng thông minh
 * - Giao tiếp với Node qua LoRa E32
 * - Kết nối WiFi, đồng bộ thời gian NTP
 * - Chạy WebServer cung cấp API cho Cloudflare Dashboard
 * - Lưu CSV vào LittleFS
 * - Push dữ liệu lên MQTT
 */

#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <time.h>
#include "config.h"

// ─── Đối tượng mạng & phần cứng ────────────────────────────
HardwareSerial    LoRaSerial(2);
WiFiClient        espClient;
PubSubClient      mqttClient(espClient);
WebServer         webServer(80);

// ─── Dữ liệu hệ thống (Được cập nhật từ LoRa) ──────────────
float g_temperature  = 0;
float g_humidity     = 0;
float g_tempExt      = 0;
float g_humidExt     = 0;
float g_lightLux     = 0;
float g_soilMoisture = 0;

int   g_fanMode      = 1;
int   g_fanSpeed     = 0;
int   g_fanPWM       = 0;
float g_fanSetpoint  = 30.0;

bool  g_pumpRunning  = false;
bool  g_pumpManual   = false;
float g_pumpOnThresh = 40.0;
float g_pumpOffThresh= 80.0;

bool  g_mistRunning  = false;
bool  g_mistManual   = false;

bool  g_lightRunning = false;
bool  g_lightManual  = false;
float g_lightThresh  = 1000.0;

bool  g_sht30OK      = false;
bool  g_sht30ExtOK   = false;
bool  g_bh1750OK     = false;
unsigned long g_nodeUptime = 0;

// Dự báo ML
float g_fc_temp[3]  = {NAN, NAN, NAN};
float g_fc_humid[3] = {NAN, NAN, NAN};
float g_fc_lux[3]   = {NAN, NAN, NAN};
bool  g_forecast_ready = false;

// ─── Trạng thái Gateway ──────────────────────────────────────
bool          g_ntpSynced    = false;
bool          g_wifiOK       = false;
unsigned long g_totalRecords = 0;
unsigned long g_lastCSVTime  = 0;
unsigned long g_lastMqttReconn = 0;
unsigned long g_lastWifiReconn = 0;
unsigned long g_lastLoraReceive = 0; // Thời điểm cuối cùng nhận được data từ LoRa
unsigned long g_lastCommandTime = 0; // Thời điểm gửi lệnh điều khiển xuống Node

// ═══════════════════════════════════════════════════════════════
//  SETUP
// ═══════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println(F("======================================="));
  Serial.println(F(" GATEWAY - SMART GREENHOUSE (LORA)"));
  Serial.println(F("======================================="));

  // UART LoRa
  LoRaSerial.begin(LORA_BAUD, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);
  Serial.println("[LoRa] Da khoi tao UART2 cho E32");

  // LittleFS
  initDataLogger();

  // WiFi
  connectWiFi();

  // NTP
  if (g_wifiOK) {
    syncNTPSetup();
  }

  // MQTT
  if (MQTT_ENABLED) {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setBufferSize(512);
  }

  // Web server
  setupWebServer();

  Serial.println("[SYSTEM] Gateway Ready!");
}

// ═══════════════════════════════════════════════════════════════
//  MAIN LOOP
// ═══════════════════════════════════════════════════════════════
void loop() {
  unsigned long now = millis();

  // Web server xử lý request
  webServer.handleClient();

  // Đọc dữ liệu từ LoRa
  checkLoRaMessage();

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
    struct tm t;
    if (getLocalTime(&t, 500)) {
      g_ntpSynced = true;
      Serial.println("[NTP] Đã sync trong loop");
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
//  WIFI & MQTT
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

void syncNTPSetup() {
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

void getTimestamp(char* buf, size_t len) {
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
  doc["ts"]          = timeStr;
  doc["temp"]        = g_temperature;
  doc["humid"]       = g_humidity;
  doc["temp_ext"]    = g_tempExt;
  doc["humid_ext"]   = g_humidExt;
  doc["lux"]         = g_lightLux;
  doc["soil"]        = g_soilMoisture;
  doc["fan_mode"]    = g_fanMode;
  doc["fan_speed"]   = g_fanSpeed;
  doc["pump"]        = g_pumpRunning;
  doc["wifi_rssi"]   = WiFi.RSSI();

  char buf[512];
  serializeJson(doc, buf);
  mqttClient.publish(MQTT_TOPIC_SENSOR, buf);
}
