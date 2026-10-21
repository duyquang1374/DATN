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
#define HEATER_PWM_FREQ                                                        \
  1000 // Tần số 1 kHz cho tải nhiệt trở (đóng ngắt dứt khoát, MOSFET mát)

// ─────────────────────────────────────────────────────────────
//  7. CÁC THIẾT BỊ CHẤP HÀNH KHÁC (RELAY & MOSFET)
//  Tất cả được gán vào các chân an toàn, tránh hoàn toàn chân Strapping (0, 2,
//  12, 15)
// ─────────────────────────────────────────────────────────────
#define PUMP_PIN 32 // Relay kích nguồn Máy bơm tưới (GPIO 32, Active HIGH)
#define PUMP_ACTIVE_HIGH true

#define MIST_PIN 33 // MOSFET kích Bơm phun sương ẩm (GPIO 33, Active HIGH)
#define MIST_ACTIVE_HIGH true

#define LIGHT_PIN 4 // MOSFET kích Dàn đèn LED quang hợp (GPIO 4, Active HIGH)
#define LIGHT_ACTIVE_HIGH true
#define DEFAULT_LIGHT_THRESH                                                   \
  1000.0 // Ngưỡng ánh sáng mặc định (Lux): < 1000 Lux thì bật đèn

// ─── CHUYỂN ĐỔI CHÂN AN TOÀN CHO VAN TƯỚI VÀ SERVO MÁI CHE ───
#define VALVE1_PIN                                                             \
  26 // Relay Van điện từ 1 - Luống 1 (Chuyển sang GPIO 26 an toàn)
#define VALVE2_PIN                                                             \
  13 // Relay Van điện từ 2 - Luống 2 (Chuyển sang GPIO 13 an toàn)
#define SERVO_PIN                                                              \
  25 // Tín hiệu Servo mở rèm mái che (Chuyển sang GPIO 25, tránh kẹt boot chân
     // 0)

// ─────────────────────────────────────────────────────────────
//  8. CẢM BIẾN ĐỘ ẨM ĐẤT 2 KÊNH (CHO 2 LUỐNG RIÊNG BIỆT)
//  Sử dụng cảm biến điện dung Capacitive v1.2 chống ăn mòn
//  Phần cứng: H23 (Luống 1) & H28 (Luống 2), mỗi kênh có tụ gốm 1µF lọc nhiễu
//  ADC xuống GND
// ─────────────────────────────────────────────────────────────
#define SOIL1_PIN                                                              \
  34 // GPIO 34 (ADC1_CH6, Input-only) - Đo ẩm đất Van 1 (Luống 1, Header H23)
#define SOIL2_PIN                                                              \
  35 // GPIO 35 (ADC1_CH7, Input-only) - Đo ẩm đất Van 2 (Luống 2, Header H28)
#define SOIL_PIN SOIL1_PIN // Tương thích ngược với hệ thống 1 cảm biến cũ

#define NUM_SOIL_SENSORS 2 // =1: Chỉ dùng 1 cảm biến (GPIO 34, Van 2 dùng chung dữ liệu); =2: Cắm đủ 2 cảm biến (GPIO 34 & 35)

#define SOIL_SAMPLES 10 // Số mẫu lấy trung bình (lọc nhiễu ADC bằng phần mềm)

// Cảm biến độ ẩm đất điện dung (Capacitive v1.2 / v2.0):
// - Trong KHÔNG KHÍ (Khô): Tần số cao nhất -> Điện áp cao nhất -> ADC Raw lớn (~3000 - 4095)
// - Trong NƯỚC (Ướt):      Điện dung tăng   -> Điện áp tụt       -> ADC Raw nhỏ (~1300 - 1800)
#define SOIL_DRY_VALUE 3200 // Giá trị ADC Raw khi đất KHÔ (Calib thực tế: trong không khí)
#define SOIL_WET_VALUE 1500 // Giá trị ADC Raw khi đất ƯỚT (Calib thực tế: nhúng cốc nước)
#define SOIL_CALIBRATION_MODE 0 // =1: In ADC Raw ra Serial để hiệu chuẩn

// Giới hạn kiểm tra lỗi phần cứng cảm biến:
// Cảm biến điện dung bình thường trong không khí có thể xuất áp kịch trần (ADC = 4095), đây là trạng thái KHÔ bình thường chứ không phải lỗi!
// Chỉ coi là lỗi khi chân bị chập mass hoặc đứt dây nguồn làm áp về 0 (ADC < 20).
#define SOIL_RAW_FAULT_LOW 20    // Raw < 20: Chập mass GND hoặc mất nguồn VCC
#define SOIL_RAW_FAULT_HIGH 4096 // Tắt bắt lỗi trần cao (giá trị 4095 là hoàn toàn bình thường ở trạng thái đất khô)

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
#define VALVE_MODE_ON                                                          \
  2 // Chế độ: Ép Bật thủ công (vẫn bảo vệ bởi Timeout an toàn)

#define PUMP_INTERLOCK_DELAY                                                   \
  1500 // 1.5 giây: Độ trễ an toàn liên động giữa van và bơm

#define DEFAULT_VALVE_TIMEOUT                                                  \
  300 // 300 giây (5 phút): Thời gian tưới tối đa cho mỗi lần mở
#define DEFAULT_VALVE_REST_TIME                                                \
  60 // 60 giây: Thời gian nghỉ tối thiểu giữa 2 lần tưới liên tiếp

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
#define SENSOR_INTERVAL 2000 // Đọc cảm biến môi trường & đất mỗi 2 giây
#define HMI_INTERVAL 1000    // Đồng bộ dữ liệu sang màn hình LCD HMI mỗi 1 giây
#define LORA_INTERVAL                                                          \
  10000 // Gửi gói tin trạng thái lên Gateway qua LoRa mỗi 10 giây
#define FORECAST_INTERVAL                                                      \
  300000 // Chạy mô hình TinyML dự báo mỗi 5 phút (1 bước Horizon)

// ─────────────────────────────────────────────────────────────
//  11. BỘ THUẬT TOÁN PID - ĐIỀU KHIỂN TỰ ĐỘNG TỐC ĐỘ QUẠT THÔNG GIÓ
// ─────────────────────────────────────────────────────────────
#define PID_KP 8.0                // Hệ số tỷ lệ P
#define PID_KI 0.5                // Hệ số tích phân I
#define PID_KD 2.0                // Hệ số vi phân D
#define DEFAULT_FAN_SETPOINT 28.0 // Nhiệt độ mục tiêu cài đặt (°C)
#define FAN_DEADBAND                                                           \
  0.5 // Vùng chết chống đóng/ngắt liên tục quanh ngưỡng (±0.5°C)
#define FAN_PWM_MIN                                                            \
  50 // Ngưỡng PWM tối thiểu để quạt bắt đầu quay (chống kẹt trục)
#define FAN_PWM_MAX 255 // Giá trị PWM tối đa (100% công suất)

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
#define MAX_SENSOR_ERRORS                                                      \
  5 // Số lần đọc lỗi liên tiếp tối đa trước khi reset I2C

void sendDataToGateway(bool force = false);
void sendValveConfigToGateway();
void sendForecastToGateway();
void saveValvesConfig();
int countRunningValves();
void setPumpManual(bool on);
void cycleFanHMI();
void cycleHeaterHMI();

