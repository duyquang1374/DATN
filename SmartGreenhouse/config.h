/*
 * ============================================================
 *  NHÀ MÀNG THÔNG MINH - CẤU HÌNH HỆ THỐNG
 *  Smart Greenhouse Configuration File
 *
 *  Chỉnh sửa file này để thay đổi thông số phần cứng,
 *  ngưỡng điều khiển và thông tin mạng.
 * ============================================================
 */

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
#define MQTT_SERVER      "192.168.1.227"
#define MQTT_PORT        1883
#define MQTT_CLIENT_ID   "ESP32_SmartGreenhouse_v2"

// MQTT Topics
#define MQTT_TOPIC_SENSOR   "greenhouse/sensor"
#define MQTT_TOPIC_FAN      "greenhouse/fan"
#define MQTT_TOPIC_PUMP     "greenhouse/pump"
#define MQTT_TOPIC_FORECAST "greenhouse/forecast"

// ─────────────────────────────────────────────────────────────
//  I2C BUS (Chung cho SHT30, BH1750, LCD)
// ─────────────────────────────────────────────────────────────
#define I2C_SDA   21
#define I2C_SCL   22

// ─────────────────────────────────────────────────────────────
//  LCD 20x4 I2C
// ─────────────────────────────────────────────────────────────
#define LCD_ADDR  0x27   // Địa chỉ I2C: thử 0x27 hoặc 0x3F
#define LCD_COLS  20
#define LCD_ROWS  4

// ─────────────────────────────────────────────────────────────
//  QUẠT DC - TB6612FNG
// ─────────────────────────────────────────────────────────────
#define FAN_AIN1         25    // Chiều quay A (HIGH = thuận)
#define FAN_AIN2         26    // Chiều quay B (LOW  = thuận)
#define FAN_PWMA         27    // PWM tốc độ quạt
#define FAN_STBY         14    // Standby (HIGH = kích hoạt driver)
// ESP32 Core v3.x: ledcAttach(pin, freq, res) — không cần khai báo channel
#define FAN_PWM_FREQ     20000 // 20 kHz → không gây ồn cuộn cảm
#define FAN_PWM_RES      8     // 8-bit: 0-255

// ─────────────────────────────────────────────────────────────
//  BƠM 5V (Relay hoặc MOSFET)
// ─────────────────────────────────────────────────────────────
#define PUMP_PIN         32    // GPIO32 → relay/MOSFET
// Nếu dùng relay Active-LOW: đổi PUMP_ON=LOW, PUMP_OFF=HIGH
#define PUMP_ACTIVE_HIGH true  // true = HIGH bật bơm

// ─────────────────────────────────────────────────────────────
//  CAM BIEN DO AM DAT (Analog)
// ─────────────────────────────────────────────────────────────
#define SOIL_PIN        34   // GPIO34 (ADC1_CH6, input-only)
#define SOIL_SAMPLES    10   // So mau trung binh (loc nhieu ADC)

// !! QUAN TRONG - Gia tri ADC phu thuoc cam bien cu the cua ban !!
//
// Loai DIEN TRO (que sat, 2 chan): dat KHO -> ADC CAO, dat UIOT -> ADC THAP
//   Vi du: DRY = 3200, WET = 1200  (ESP32 12-bit ADC: 0-4095)
//
// Loai DIEN DUNG (capacitive, mau den/xanh la): dat KHO -> ADC THAP, dat UIOT -> ADC CAO
//   Vi du: DRY = 1500, WET = 3000  (dao nguoc so voi loai dien tro)
//
// De tim gia tri chinh xac: bat SOIL_CALIBRATION_MODE = 1
// Xem ADC raw trong Serial Monitor (115200 baud)
//
#define SOIL_DRY_VALUE  1500  // ADC raw khi dat KHO - do thuc te voi cam bien nay!
#define SOIL_WET_VALUE  3000  // ADC raw khi dat UIOT - do thuc te!
// !! Neu cam bien DIEN TRO (que sat): DRY=3200, WET=1200 (ADC cao = kho)
// !! Neu cam bien DIEN DUNG (capacitive, mau den): DRY < WET (ADC thap = kho)
// ADC=784 do duoc o tren khong khi -> day la cam bien dien dung
// -> DRY ~ 784 (trong khong khi), WET ~ 1500-1800 (trong nuoc)

// ─────────────────────────────────────────────────────────────
//  DEBUG / CALIBRATION
// ─────────────────────────────────────────────────────────────
// =1: Serial in ADC raw cam bien dat de calibrate. =0: chay binh thuong
#define SOIL_CALIBRATION_MODE  0

// =1: Quet I2C khi boot, in dia chi tat ca thiet bi I2C vao Serial
// Dung khi cam bien khong nhan, hoac khong biet dia chi LCD/SHT30
#define I2C_SCAN_ON_BOOT       0

// ─────────────────────────────────────────────────────────────
//  LED STATUS
// ─────────────────────────────────────────────────────────────
#define LED_PIN          2

// ─────────────────────────────────────────────────────────────
//  NTP
// ─────────────────────────────────────────────────────────────
#define NTP_SERVER       "pool.ntp.org"
#define NTP_SERVER2      "time.google.com"    // Server du phong
#define NTP_SERVER3      "vn.pool.ntp.org"    // Server VN
#define GMT_OFFSET       25200  // GMT+7 (giay)
#define DAYLIGHT_OFFSET  0

// ─────────────────────────────────────────────────────────────
//  LƯU TRỮ CSV (LittleFS)
// ─────────────────────────────────────────────────────────────
#define CSV_FILE        "/data.csv"
#define MAX_FILE_SIZE   1400000  // ~1.4 MB

// ─────────────────────────────────────────────────────────────
//  THỜI GIAN (milliseconds)
// ─────────────────────────────────────────────────────────────
#define SENSOR_INTERVAL     2000   // Đọc cảm biến & cập nhật cơ cấu
#define CSV_INTERVAL        30000  // Lưu CSV + gửi MQTT
#define LCD_INTERVAL        1000   // Cập nhật màn hình LCD
#define MQTT_RECONNECT      5000   // Thử kết nối lại MQTT
#define WIFI_RECONNECT      15000  // Thử kết nối lại WiFi
#define FORECAST_INTERVAL   300000 // Dự báo mỗi 5 phút (= 1 horizon step)

// ─────────────────────────────────────────────────────────────
//  PID - ĐIỀU KHIỂN TỐC ĐỘ QUẠT
// ─────────────────────────────────────────────────────────────
#define PID_KP           8.0    // Hệ số tỉ lệ
#define PID_KI           0.5    // Hệ số tích phân
#define PID_KD           2.0    // Hệ số vi phân
#define DEFAULT_FAN_SETPOINT 28.0 // Nhiệt độ mục tiêu mặc định (°C)
#define FAN_DEADBAND     0.5    // Dải chết ±0.5°C (không kích PID)
#define FAN_PWM_MIN      50     // PWM tối thiểu khi quạt khởi động
#define FAN_PWM_MAX      255    // PWM tối đa

// Nhiệt độ bắt đầu bật quạt (dưới ngưỡng này → OFF dù mode AUTO)
// Thay vì macro tĩnh, giờ tính toán trong code: start_temp = g_fanSetpoint - FAN_DEADBAND

// Chế độ quạt
#define FAN_MODE_OFF     0
#define FAN_MODE_AUTO    1
#define FAN_MODE_MANUAL  2

// ─────────────────────────────────────────────────────────────
//  BƠM - NGƯỠNG ĐỘ ẨM ĐẤT
// ─────────────────────────────────────────────────────────────
#define DEFAULT_PUMP_ON_THRESH   40.0   // Bật bơm mặc định: < 40%
#define DEFAULT_PUMP_OFF_THRESH  65.0   // Tắt bơm mặc định: > 65%
#define PUMP_MAX_RUNTIME         30000  // Tối đa 30s liên tục (bảo vệ bơm)
#define PUMP_COOLDOWN            120000 // Chờ 2 phút trước khi bật lại

// ─────────────────────────────────────────────────────────────
//  SENSOR VALIDATION
// ─────────────────────────────────────────────────────────────
#define TEMP_MIN         -10.0
#define TEMP_MAX          60.0
#define HUMID_MIN          0.0
#define HUMID_MAX        100.0
#define LUX_MAX          65535.0
#define MAX_SENSOR_ERRORS  5
