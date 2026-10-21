#pragma once

// ═════════════════════════════════════════════════════════════
//  HỆ THỐNG NHÀ MÀNG THÔNG MINH - NODE 3: GATEWAY NODE (ESP32)
//  Tệp cấu hình WiFi, MQTT, LoRa E32, bộ nhớ Flash & thời gian NTP
// ═════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────
//  1. KẾT NỐI MẠNG KHÔNG DÂY WIFI (TRẠM THU THẬP & PHÁT API)
// ─────────────────────────────────────────────────────────────
#define WIFI_SSID        "Xoi Banh My Chi Nga"
#define WIFI_PASSWORD    "13071982"

// ─────────────────────────────────────────────────────────────
//  2. GIAO THỨC MQTT (TÙY CHỌN ĐẨY LÊN BROKER TRUNG TÂM)
// ─────────────────────────────────────────────────────────────
#define MQTT_ENABLED     false               // =true: Kích hoạt đẩy dữ liệu MQTT
#define MQTT_SERVER      "192.168.1.228"     // Địa chỉ IP của MQTT Broker nội bộ
#define MQTT_PORT        1883                // Cổng mặc định MQTT
#define MQTT_CLIENT_ID   "ESP32_SmartGreenhouse_GW" // Client ID định danh Gateway

// Các chủ đề (Topics) xuất bản dữ liệu
#define MQTT_TOPIC_SENSOR   "greenhouse/sensor"   // Topic dữ liệu cảm biến thời gian thực
#define MQTT_TOPIC_FAN      "greenhouse/fan"      // Topic trạng thái quạt
#define MQTT_TOPIC_PUMP     "greenhouse/pump"     // Topic trạng thái bơm & van
#define MQTT_TOPIC_FORECAST "greenhouse/forecast" // Topic dự báo TinyML 3 bước tương lai

// ─────────────────────────────────────────────────────────────
//  3. GIAO TIẾP KHÔNG DÂY LORA E32 (UART2) - THU DỮ LIỆU TỪ MASTER
//  Module: E32-433T20D (Chuẩn truyền thông UART trong suốt Mode 0)
//  Lưu ý phần cứng: Chân M0 và M1 của E32 BẮT BUỘC nối GND để chạy Mode 0
// ─────────────────────────────────────────────────────────────
#define LORA_TX_PIN      18    // ESP32 TX2 (GPIO 18) -> E32 RXD
#define LORA_RX_PIN      19    // ESP32 RX2 (GPIO 19) -> E32 TXD
#define LORA_BAUD        115200 // Tốc độ Baud truyền nhận không dây LoRa

// ─────────────────────────────────────────────────────────────
//  4. CẤU HÌNH LƯU TRỮ DỮ LIỆU (DATA LOGGER VÀO FLASH LITTLEFS)
// ─────────────────────────────────────────────────────────────
#define CSV_FILE         "/data.csv"         // Đường dẫn file nhật ký cảm biến
#define MAX_FILE_SIZE    (1.5 * 1024 * 1024) // Giới hạn tối đa 1.5 MB (Tự xoay vòng cắt 1/2 file cũ khi đầy)

// ─────────────────────────────────────────────────────────────
//  5. ĐỒNG BỘ THỜI GIAN THỰC TẾ QUA MẠNG (NTP TIME SYNC)
// ─────────────────────────────────────────────────────────────
#define NTP_SERVER       "pool.ntp.org"
#define NTP_SERVER2      "time.google.com"
#define NTP_SERVER3      "time.windows.com"
#define GMT_OFFSET       (7 * 3600)  // Múi giờ Việt Nam: GMT+7 (25200 giây)
#define DAYLIGHT_OFFSET  0           // Không có giờ mùa hè

// ─────────────────────────────────────────────────────────────
//  6. CÁC KHOẢNG THỜI GIAN ĐỊNH KỲ (milliseconds)
// ─────────────────────────────────────────────────────────────
#define WIFI_RECONNECT   5000  // Thử kết nối lại WiFi mỗi 5 giây nếu mất mạng
#define MQTT_RECONNECT   5000  // Thử kết nối lại MQTT mỗi 5 giây
#define CSV_INTERVAL     10000 // Ghi CSV mỗi 10 giây khi nhận được gói tin mới từ LoRa
#define LED_PIN          2     // Đèn LED tích hợp trên ESP32 báo trạng thái hoạt động
