#pragma once

// ─────────────────────────────────────────────────────────────
//  WIFI
// ─────────────────────────────────────────────────────────────
#define WIFI_SSID        "Xoi Banh My Chi Nga"
#define WIFI_PASSWORD    "13071982"

// ─────────────────────────────────────────────────────────────
//  MQTT
// ─────────────────────────────────────────────────────────────
#define MQTT_ENABLED     true
#define MQTT_SERVER      "192.168.1.228"
#define MQTT_PORT        1883
#define MQTT_CLIENT_ID   "ESP32_SmartGreenhouse_GW"

// MQTT Topics
#define MQTT_TOPIC_SENSOR   "greenhouse/sensor"
#define MQTT_TOPIC_FAN      "greenhouse/fan"
#define MQTT_TOPIC_PUMP     "greenhouse/pump"
#define MQTT_TOPIC_FORECAST "greenhouse/forecast"

// ─────────────────────────────────────────────────────────────
//  LORA E32 (UART2)
// ─────────────────────────────────────────────────────────────
#define LORA_TX_PIN      18
#define LORA_RX_PIN      19
#define LORA_BAUD        115200

// ─────────────────────────────────────────────────────────────
//  CẤU HÌNH DATA LOGGER
// ─────────────────────────────────────────────────────────────
#define CSV_FILE         "/data.csv"
#define MAX_FILE_SIZE    (1.5 * 1024 * 1024)  // 1.5 MB

// ─────────────────────────────────────────────────────────────
//  THỜI GIAN NTP
// ─────────────────────────────────────────────────────────────
#define NTP_SERVER       "pool.ntp.org"
#define NTP_SERVER2      "time.google.com"
#define NTP_SERVER3      "time.windows.com"
#define GMT_OFFSET       (7 * 3600)  // GMT+7
#define DAYLIGHT_OFFSET  0

// Các khoảng thời gian (ms)
#define WIFI_RECONNECT   5000
#define MQTT_RECONNECT   5000
#define CSV_INTERVAL     10000 // Ghi CSV mỗi 10s (chỉ ghi khi nhận được dữ liệu mới từ LoRa)
#define LED_PIN          2
