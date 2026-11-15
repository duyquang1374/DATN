#pragma once

// ═════════════════════════════════════════════════════════════
//  HỆ THỐNG NHÀ MÀNG THÔNG MINH - NODE 1: MASTER NODE (ESP32)
//  Tệp cấu hình phần cứng, chân GPIO, tham số cảm biến & thuật toán
// ═════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────
//  1. GIAO TIẾP KHÔNG DÂY LORA E32 (UART2) - KẾT NỐI GATEWAY
//  Module: E32-433T20D (Chuẩn truyền thông UART trong suốt Mode 0)
//  Lưu ý phần cứng: Chân M0 và M1 của E32 BẮT BUỘC nối GND để chạy Mode 0
// ─────────────────────────────────────────────────────────────
#define LORA_TX_PIN 18   // ESP32 TX2 (GPIO 18) -> E32 RXD
#define LORA_RX_PIN 19   // ESP32 RX2 (GPIO 19) -> E32 TXD
#define LORA_BAUD 115200 // Tốc độ Baud giao tiếp LoRa

// ─────────────────────────────────────────────────────────────
//  2. GIAO TIẾP CÓ DÂY UART HMI (UART1) - KẾT NỐI TỦ ĐIỆN HMI
//  Giao tiếp hai chiều: Master gửi cảm biến/trạng thái; HMI gửi lệnh nút/biến
//  trở
// ─────────────────────────────────────────────────────────────
#define HMI_TX_PIN 17   // ESP32 TX1 (GPIO 17) -> HMI RX2 (GPIO 16)
#define HMI_RX_PIN 16   // ESP32 RX1 (GPIO 16) -> HMI TX2 (GPIO 17)
#define HMI_BAUD 115200 // Tốc độ Baud giao tiếp HMI

// ─────────────────────────────────────────────────────────────
//  3. I2C BUS 0 (CHÍNH) - CẢM BIẾN TRONG NHÀ MÀNG
//  Bao gồm: SHT30 Trong nhà (Địa chỉ 0x44) & BH1750 Ánh sáng (Địa chỉ 0x23)
//  Nguồn cấp: 3.3V sạch từ IC nguồn AMS1117-3.3V
// ─────────────────────────────────────────────────────────────
#define I2C_SDA 21 // Chân dữ liệu I2C0 (GPIO 21)
#define I2C_SCL 22 // Chân xung clock I2C0 (GPIO 22)

// ─────────────────────────────────────────────────────────────
//  4. I2C BUS 1 (PHỤ) - CẢM BIẾN NGOÀI TRỜI
//  Bao gồm: SHT30 Ngoài trời (Địa chỉ 0x44)
//  Nguồn cấp: 3.3V sạch từ IC nguồn AMS1117-3.3V
// ─────────────────────────────────────────────────────────────
#define I2C1_SDA 5  // Chân dữ liệu I2C1 (GPIO 5)
#define I2C1_SCL 23 // Chân xung clock I2C1 (GPIO 23)

// ─────────────────────────────────────────────────────────────
//  5. QUẠT THÔNG GIÓ DC 12V - MODULE CÔNG SUẤT MOSFET D4184 (MẠCH 1)
//  Điều khiển tốc độ bằng PWM qua MOSFET (ngắt cực âm GND, không cần đảo chiều)
// ─────────────────────────────────────────────────────────────
#define FAN_PIN 27 // Chân xung PWM điều khiển tốc độ quạt (GPIO 27 an toàn)
#define FAN_PWMA FAN_PIN   // Tương thích ngược tên biến cũ
#define FAN_PWM_FREQ 20000 // Tần số 20 kHz (loại bỏ tiếng rít động cơ DC)
#define FAN_PWM_RES 8      // Độ phân giải 8-bit (Giá trị PWM: 0 - 255)

// ─────────────────────────────────────────────────────────────
//  6. BÓNG SƯỞI ẤM (CỤM 5 BÓNG SỢI ĐỐT //) - MODULE MOSFET D4184 (MẠCH 2)
//  Điều khiển công suất nhiệt tỏa ra bằng xung PWM
// ─────────────────────────────────────────────────────────────
#define HEATER_PIN 14 // Chân xung PWM điều khiển bóng sưởi (GPIO 14 an toàn)
#define HEATER_PWMB HEATER_PIN // Tương thích ngược tên biến cũ
#define HEATER_PWM_FREQ 1000 // Tần số 1 kHz cho tải nhiệt trở (đóng ngắt dứt khoát, MOSFET mát)

// ─────────────────────────────────────────────────────────────
//  7. CÁC THIẾT BỊ CHẤP HÀNH KHÁC (RELAY & MOSFET)
//  Tất cả được gán vào các chân an toàn, tránh hoàn toàn chân Strapping (0, 2,
//  12, 15)
// ─────────────────────────────────────────────────────────────
#define PUMP_PIN 32 // Relay kích nguồn Máy bơm tưới (GPIO 32, Active HIGH)
#define PUMP_ACTIVE_HIGH true

#define MIST_PIN 33 // MOSFET kích Bơm phun sương ẩm (GPIO 33, Active HIGH)
#define MIST_ACTIVE_HIGH true
#define DEFAULT_MIST_HUMID_THRESH 65.0 // Ngưỡng độ ẩm không khí (%): dưới 65% thì bật phun sương cấp ẩm
#define MIST_HUMID_HYSTERESIS     5.0  // Độ trễ chống đóng ngắt liên tục (đạt 70% thì dừng phun)
#define MIST_TEMP_HYSTERESIS      0.5  // Độ trễ nhiệt độ làm mát (T_in <= Setpoint - 0.5 mới ngắt làm mát)
#define MIST_MIN_RUN_TIME_MS      60000UL // Thời gian chạy tối thiểu 1 phút (60 giây) trước khi cho phép ngắt
extern unsigned long g_mistStartTime;

#define LIGHT_PIN 4 // MOSFET kích Dàn đèn LED quang hợp (GPIO 4, Active HIGH)
#define LIGHT_ACTIVE_HIGH true
#define DEFAULT_LIGHT_THRESH 1000.0 // Ngưỡng ánh sáng mặc định (Lux): < 1000 Lux thì bật đèn

// ─── CHUYỂN ĐỔI CHÂN AN TOÀN CHO VAN TƯỚI VÀ SERVO MÁI CHE ───
#define VALVE1_PIN 26 // Relay Van điện từ 1 - Luống 1 (Chuyển sang GPIO 26 an toàn)
#define VALVE2_PIN 13 // Relay Van điện từ 2 - Luống 2 (Chuyển sang GPIO 13 an toàn)
#define SERVO_PIN      25 // Tín hiệu Servo 1 mở rèm mái che (Thuận chiều - GPIO 25)
#define SERVO_2_PIN    12 // Tín hiệu Servo 2 mở rèm mái che (Ngược chiều - GPIO 12)
#define ROOF_MAX_ANGLE 75 // Góc mở tối đa của rèm chớp (75 độ = 100%)
// Quy đổi góc rèm (0 - 75 độ) sang phần trăm mở (0 - 100%), làm tròn. VD: 10 độ -> 13%
#define ROOF_ANGLE_TO_PCT(a) ((int)(((a) * 100 + ROOF_MAX_ANGLE / 2) / ROOF_MAX_ANGLE))

// ─────────────────────────────────────────────────────────────
//  8. CẢM BIẾN ĐỘ ẨM ĐẤT (WSN DISTRIBUTED SENSING)
//  Đã giải phóng GPIO 34 và 35 trên Master Node!
//  Độ ẩm đất Luống 1 được nhận từ Sensor Node 1 qua LoRa
//  Độ ẩm đất Luống 2 được nhận từ Sensor Node 2 qua LoRa
// ─────────────────────────────────────────────────────────────
#define NUM_SOIL_SENSORS 2 // 2 kênh luống: Van 1 (Node 1), Van 2 (Node 2)
#define SOIL1_PIN -1       // Không dùng chân cứng trên Master
#define SOIL2_PIN -1       // Không dùng chân cứng trên Master
#define SOIL_PIN SOIL1_PIN

#define SOIL1_DRY_VALUE 2500
#define SOIL1_WET_VALUE 1500
#define SOIL2_DRY_VALUE 2420
#define SOIL2_WET_VALUE 1500
#define SOIL_DRY_VALUE SOIL1_DRY_VALUE
#define SOIL_WET_VALUE SOIL1_WET_VALUE

// ─────────────────────────────────────────────────────────────
//  8.1. ĐỊNH NGHĨA GIAO THỨC WSN & DỮ LIỆU SENSOR NODES
// ─────────────────────────────────────────────────────────────
#define SENSOR_PACKET_HEADER 0xAA
#define ACK_PACKET_HEADER    0x55
#define NODE_TIMEOUT_MS      35000UL // Quá 35s không nhận tin coi như node mất kết nối

typedef struct __attribute__((packed)) {
  uint8_t  header;       // 0xAA
  uint8_t  node_id;      // 1 hoặc 2
  uint16_t msg_id;       // Đếm tăng dần
  int16_t  temp_x10;     // Nhiệt độ * 10
  uint16_t hum_x10;      // Độ ẩm không khí * 10
  uint16_t soil_pct;     // Độ ẩm đất (%)
  uint8_t  status_mask;  // Bit 0: lỗi SHT30, Bit 1: lỗi đất
  uint16_t crc16;        // CRC-16
} SensorPacket;

typedef struct __attribute__((packed)) {
  uint8_t  header;       // 0x55
  uint8_t  target_id;    // 1 hoặc 2
  uint16_t ack_msg_id;   // Mã gói tin vừa nhận
} AckPacket;

struct SensorNodeData {
  bool isOnline;
  bool isConfigured;
  uint8_t nodeId;
  uint32_t lastSeen;
  float temperature;
  float humidity;
  float soilMoisture;
  uint16_t msgId;
  bool shtError;
  bool soilError;
};

// ─────────────────────────────────────────────────────────────
//  9. CẤU HÌNH HỆ THỐNG TƯỚI THÔNG MINH (BƠM & 2 VAN LIÊN ĐỘNG AN TOÀN)
//  Quy tắc an toàn chống vỡ ống & búa nước:
//  - Bơm KHÔNG có ngưỡng riêng: Bơm tự động chạy khi có >= 1 van mở, tự tắt khi
//  tất cả van đóng.
//  - Khi Mở: Mở van trước -> Trễ 1.5s -> Bật bơm (chống quá áp vỡ đường ống).
//  - Khi Đóng: Tắt bơm trước -> Trễ 1.5s xả áp -> Mới đóng van cuối cùng (chống
//  búa nước).
// ─────────────────────────────────────────────────────────────
#define NUM_VALVES 2      // Số van vật lý (Van 1 & Van 2)
#define VALVE_MODE_OFF 0  // Chế độ: Ép Tắt thủ công
#define VALVE_MODE_AUTO 1 // Chế độ: Tự động theo độ ẩm đất (Hysteresis)
#define VALVE_MODE_ON  2 // Chế độ: Ép Bật thủ công (vẫn bảo vệ bởi Timeout an toàn)

#define PUMP_INTERLOCK_DELAY 1500 // 1.5 giây: Độ trễ an toàn liên động giữa van và bơm

#define DEFAULT_VALVE_TIMEOUT 300 // 300 giây (5 phút): Thời gian tưới tối đa cho mỗi lần mở
#define DEFAULT_VALVE_REST_TIME 60 // 60 giây: Thời gian nghỉ tối thiểu giữa 2 lần tưới liên tiếp

// Cấu trúc cấu hình và trạng thái động của từng van tưới
struct ValveConfig {
  char name[24];        // Tên hiển thị (vd: "Van 1 (Luống 1)")
  int pin;              // Chân GPIO điều khiển relay van
  int soilPin;          // Chân GPIO cảm biến độ ẩm đất tương ứng
  float dryThresh;      // Ngưỡng khô (< %) bắt đầu mở van tưới
  float wetThresh;      // Ngưỡng ẩm (> %) đủ ẩm thì đóng van dừng tưới
  uint32_t maxDuration; // Thời gian tưới tối đa mỗi lần (giây)
  uint32_t minRestTime; // Thời gian nghỉ tối thiểu giữa 2 lần tưới (giây)
  int dryRaw; // Giá trị ADC khi đất khô (hiệu chuẩn riêng cho từng cảm biến)
  int wetRaw; // Giá trị ADC khi đất ướt (hiệu chuẩn riêng cho từng cảm biến)
  int mode;   // 0: OFF, 1: AUTO, 2: ON
  bool isRunning; // Trạng thái van thực tế (true: đang mở, false: đang đóng)
  unsigned long runStartTime; // Thời điểm bắt đầu mở van lần này (millis)
  unsigned long lastStopTime; // Thời điểm đóng van lần trước (millis)
  float currentSoil;          // Độ ẩm đất đo được hiện tại (%)
  int rawADC;                 // Giá trị ADC Raw đọc được hiện tại
  bool sensorError;           // Cờ báo lỗi cảm biến (hở mạch/chập mạch)
};

// ─────────────────────────────────────────────────────────────
//  10. CHU KỲ LẤY MẪU & GỬI DỮ LIỆU (milliseconds)
// ─────────────────────────────────────────────────────────────
#define SENSOR_INTERVAL 10000 // Đọc cảm biến môi trường & đất mỗi 2 giây
#define HMI_INTERVAL 1000    // Đồng bộ dữ liệu sang màn hình LCD HMI mỗi 1 giây
#define LORA_INTERVAL 10000 // Gửi gói tin trạng thái lên Gateway qua LoRa mỗi 10 giây
#define FORECAST_INTERVAL 300000 // Chạy mô hình TinyML dự báo mỗi 5 phút (1 bước Horizon)

// ─────────────────────────────────────────────────────────────
//  11. BỘ THUẬT TOÁN PID - ĐIỀU KHIỂN TỰ ĐỘNG TỐC ĐỘ QUẠT THÔNG GIÓ
// ─────────────────────────────────────────────────────────────
#define PID_KP 75.0             // Hệ số tỷ lệ P (phản ứng tuyến tính 25 PWM mỗi 1 độ C quá nhiệt)
#define PID_KI 0.05               // Hệ số tích phân I (chống bão hòa tích phân)
#define PID_KD 0                // Hệ số vi phân D
#define DEFAULT_FAN_SETPOINT 30.5 // Nhiệt độ mục tiêu cài đặt (°C)
#define FAN_DEADBAND 1.0 // Vùng chết chống đóng/ngắt liên tục quanh ngưỡng (±0.5°C)
#define FAN_PWM_MIN  102 // Ngưỡng PWM tối thiểu (~40%) để quạt chạy ổn định (đồng bộ mức 1 nút nhấn)
#define FAN_PWM_MAX 255 // Giá trị PWM tối đa (100% công suất)
#define FAN_KICKSTART_PWM 255 // Xung PWM đề pa tối đa để thắng ma sát tĩnh của quạt DC
#define FAN_KICKSTART_MS  300 // Thời gian duy trì đề pa (ms) giúp quạt lấy trớn quay ngay
#define HEATER_PWM_MIN 50 // Ngưỡng PWM tối thiểu cho bóng sưởi ấm
#define HEATER_PWM_MAX 255 // Giá trị PWM tối đa (100% công suất sưởi)
#define DEFAULT_HEATER_SETPOINT 28.5 // Nhiệt độ mục tiêu bật sưởi ấm cài đặt (°C)
#define HEATER_DEADBAND 1.0 // Vùng chết chống đóng/ngắt liên tục quanh ngưỡng sưởi (±0.5°C)

#define FAN_MODE_OFF 0    // Quạt tắt thủ công
#define FAN_MODE_AUTO 1   // Quạt chạy tự động theo thuật toán PID nhiệt độ
#define FAN_MODE_MANUAL 2 // Quạt chạy theo tốc độ người dùng chỉ định bằng tay

// ─────────────────────────────────────────────────────────────
//  12. THAM SỐ TƯƠNG THÍCH NGƯỢC (LEGACY COMPATIBILITY)
//  Bơm hiện tại chạy tự động liên động theo 2 Van tưới.
//  Các define dưới đây giữ lại để tránh lỗi biên dịch các thư viện cũ nếu có
//  gọi.
// ─────────────────────────────────────────────────────────────
#define DEFAULT_PUMP_ON_THRESH 40.0
#define DEFAULT_PUMP_OFF_THRESH 65.0

// ─────────────────────────────────────────────────────────────
//  13. GIỚI HẠN KIỂM TRA HỢP LỆ DỮ LIỆU CẢM BIẾN (SANITY CHECK)
// ─────────────────────────────────────────────────────────────
#define TEMP_MIN -10.0  // Nhiệt độ tối thiểu hợp lệ (°C)
#define TEMP_MAX 60.0   // Nhiệt độ tối đa hợp lệ (°C)
#define HUMID_MIN 0.0   // Độ ẩm không khí tối thiểu (%)
#define HUMID_MAX 100.0 // Độ ẩm không khí tối đa (%)
#define LUX_MAX 65535.0 // Cường độ sáng tối đa (Lux)
#define MAX_SENSOR_ERRORS  5 // Số lần đọc lỗi liên tiếp tối đa trước khi reset I2C

void sendDataToGateway(bool force = false);
void sendValveConfigToGateway();
void sendForecastToGateway();
void saveValvesConfig();
int countRunningValves();
void setPumpManual(bool on);
void cycleFanHMI();
void cycleHeaterHMI();
void setHeaterSetpoint(float sp);

// Các hàm xử lý WSN & Data Fusion
void checkLoRaTraffic();
void updateMicroclimateFusion();
void sendAckToNode(uint8_t nodeId, uint16_t msgId);
uint16_t calcCRC16(const uint8_t *data, size_t len);

