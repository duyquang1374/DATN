// ═════════════════════════════════════════════════════════════
//  HỆ THỐNG NHÀ MÀNG THÔNG MINH - SENSOR NODE (WSN END-NODE)
//  Bo mạch: ESP32-C3 SuperMini + Module LoRa E32-433T20D
//  Cảm biến:
//    - 1x SHT30 (Nhiệt độ & Độ ẩm không khí) qua I2C (SDA 8, SCL 9)
//    - 1x Cảm biến độ ẩm đất điện dung qua ADC (GPIO 2)
//  Chức năng:
//    - Thu thập dữ liệu định kỳ mỗi 10 giây (lệch pha theo Node ID)
//    - Đóng gói dữ liệu dạng nhị phân siêu gọn có mã kiểm tra CRC-16
//    - Phát qua LoRa E32 và lắng nghe gói xác nhận ACK từ Master Node
//    - Tự động phát lại (Retry) tối đa 2 lần nếu mất gói
// ═════════════════════════════════════════════════════════════

#include <Wire.h>
#include <Adafruit_SHT31.h>

// ─── CẤU HÌNH ĐỊNH DANH NODE (ĐỔI ID CHO TỪNG NODE) ──────────
// Đặt = 1 cho Node 1 (Luống 1)
// Đặt = 2 cho Node 2 (Luống 2)
#define NODE_ID 1

// ─── CHÂN KẾT NỐI TRÊN BO ESP32-C3 SUPERMINI ──────────────────
// I2C Cảm biến SHT30
#define I2C_SDA_PIN     8
#define I2C_SCL_PIN     9
#define SHT30_I2C_ADDR  0x44  // ADDR nối GND hoặc để trống

// ADC Cảm biến độ ẩm đất (Capacitive v1.2)
// Lưu ý: GPIO 2 là strapping pin trên ESP32-C3, có thể ảnh hưởng ADC khi boot.
// Nếu raw luôn = 4095 dù đã cấp 5V -> thử đổi sang GPIO 3 hoặc GPIO 4.
#define SOIL_ADC_PIN    3     // Thử GPIO 3 (ADC1_CH3) nếu GPIO 2 không ổn định

// Giao tiếp UART1 với LoRa E32-433T20D (Mode 0: M0=GND, M1=GND)
#define LORA_RX_PIN     20    // ESP32-C3 RX1 <- E32 TXD
#define LORA_TX_PIN     21    // ESP32-C3 TX1 -> E32 RXD
#define LORA_BAUD       115200

// ─── THAM SỐ TRUYỀN THÔNG VÔ TUYẾN ───────────────────────────
#define SENSOR_PACKET_HEADER 0xAA
#define ACK_PACKET_HEADER    0x55
#define TX_INTERVAL_MS       10000UL // Chu kỳ phát 10 giây
#define ACK_TIMEOUT_MS       800     // Tăng lên 800ms để Master kịp xử lý I2C/PID và phát LoRa
#define MAX_RETRIES          2       // Số lần thử gửi lại khi mất gói

// Lệch pha phát sóng chống va chạm (Time-Slots):
// Master gửi status ở giây 0s
// Node 1 phát ở giây 3s (3000ms)
// Node 2 phát ở giây 6.5s (6500ms)
#define PHASE_OFFSET_MS      ((NODE_ID == 1) ? 3000UL : 6500UL)

// ─── HIỆU CHUẨN ĐỘ ẨM ĐẤT ─────────────────────────────────────
// QUAN TRỌNG: Cảm biến điện dung v1.2 yêu cầu nguồn VCC = 5V để hoạt động đúng.
//   Nếu cấp 3.3V: chân AOUT sẽ luôn trả về ~4095 (lỗi bão hòa ADC).
//   Giải pháp: cắm VCC cảm biến vào pin 5V (VIN) trên ESP32-C3.
//              Chân AOUT vẫn kết nối bình thường vào GPIO 2 (an toàn ≤3.3V).
//
// Hướng dẫn hiệu chuẩn (sau khi dùng đúng nguồn 5V):
//   Bật CALIBRATION_MODE = 1 -> nạp code -> mở Serial Monitor (115200 baud)
//   - Đặt cảm biến trong KHÔNG KHÍ -> ghi raw -> đặt SOIL_DRY_RAW
//   - Nhúng vào CỐC NƯỚC         -> ghi raw -> đặt SOIL_WET_RAW
//   Với nguồn 5V: Khô thường ~2800-3500, Ướt thường ~1200-1800
#define CALIBRATION_MODE     0       // 1: In raw ADC mỗi 2s để hiệu chuẩn
#define SOIL_DRY_RAW         3200    // Giá trị raw khi cảm biến trong KHÔNG KHÍ (5V)
#define SOIL_WET_RAW         1500    // Giá trị raw khi cảm biến trong CỐC NƯỚC (5V)

// ─── ĐỊNH DẠNG GÓI TIN NHỊ PHÂN (PACKED STRUCT) ───────────────
typedef struct __attribute__((packed)) {
  uint8_t  header;       // 0xAA
  uint8_t  node_id;      // 1 hoặc 2
  uint16_t msg_id;       // Số thứ tự gói tin tự tăng
  int16_t  temp_x10;     // Nhiệt độ * 10 (ví dụ 28.5 độ C -> 285)
  uint16_t hum_x10;      // Độ ẩm không khí * 10 (ví dụ 65.2% -> 652)
  uint16_t soil_pct;     // Độ ẩm đất (0 - 100%)
  uint8_t  status_mask;  // Bit 0: SHT30 lỗi, Bit 1: Cảm biến đất lỗi
  uint16_t crc16;        // Mã kiểm tra lỗi CRC-16
} SensorPacket;

typedef struct __attribute__((packed)) {
  uint8_t  header;       // 0x55 (Đánh dấu gói ACK)
  uint8_t  target_id;    // 1 hoặc 2
  uint16_t ack_msg_id;   // Mã gói tin được xác nhận
} AckPacket;

// ─── ĐỐI TƯỢNG PHẦN CỨNG ─────────────────────────────────────
Adafruit_SHT31 sht30;
HardwareSerial LoRaSerial(1);

uint16_t g_msgCounter = 0;
unsigned long g_lastTxTime = 0;
bool g_shtOK = false;

// ─── HÀM TÍNH CRC-16 (MODBUS POLYNOMIAL 0xA001) ──────────────
uint16_t calcCRC16(const uint8_t *data, size_t len) {
  uint16_t crc = 0xFFFF;
  for (size_t i = 0; i < len; i++) {
    crc ^= (uint16_t)data[i];
    for (int j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        crc = (crc >> 1) ^ 0xA001;
      } else {
        crc = crc >> 1;
      }
    }
  }
  return crc;
}

// ─── ĐỌC ĐỘ ẨM ĐẤT LỌC TRUNG BÌNH ────────────────────────────
float readSoilMoisture(uint16_t &rawOut, bool &errOut) {
  long sum = 0;
  const int SAMPLES = 10;
  for (int i = 0; i < SAMPLES; i++) {
    sum += analogRead(SOIL_ADC_PIN);
    delay(4);
  }
  int raw = sum / SAMPLES;
  rawOut = (uint16_t)raw;

  // Chỉ phát hiện lỗi chập mass (raw quá thấp, cảm biến bị ngắn với GND)
  // Không chặn raw cao vì raw=4095 là hợp lệ khi cảm biến trong không khí khô
  // với nguồn 3.3V sẽ luôn bão hòa ở 4095 -> cần dùng nguồn 5V!
  if (raw < 150) {
    errOut = true;
    return 0.0f;
  }
  errOut = false;

  // Công thức: Khô = raw cao, Ướt = raw thấp
  // Kết quả được clamp về [0%, 100%] để xử lý trường hợp ngoài vùng hiệu chuẩn
  float pct = (float)(SOIL_DRY_RAW - raw) * 100.0f / (float)(SOIL_DRY_RAW - SOIL_WET_RAW);
  if (pct < 0.0f) pct = 0.0f;
  if (pct > 100.0f) pct = 100.0f;
  return pct;
}

// ─── GỬI GÓI TIN VÀ CHỜ PHẢN HỒI ACK ─────────────────────────
bool sendPacketWithAck(SensorPacket &pkt) {
  for (int attempt = 0; attempt <= MAX_RETRIES; attempt++) {
    // 0. Xóa sạch mọi byte rác/gói JSON phát tán cũ trong đệm RX trước khi phát
    while (LoRaSerial.available()) {
      LoRaSerial.read();
    }

    // 1. Phát gói tin
    LoRaSerial.write((const uint8_t*)&pkt, sizeof(SensorPacket));
    LoRaSerial.flush();
    Serial.printf("[TX] Node %d gui Msg #%u (Lan thu %d)... ", pkt.node_id, pkt.msg_id, attempt + 1);

    // 2. Chờ ACK có cơ chế quét Header 0x55 thông minh
    unsigned long startWait = millis();
    while (millis() - startWait < ACK_TIMEOUT_MS) {
      // Bỏ qua mọi byte nhiễu đứng trước byte ACK_PACKET_HEADER (0x55)
      while (LoRaSerial.available() > 0 && LoRaSerial.peek() != ACK_PACKET_HEADER) {
        LoRaSerial.read();
      }

      if (LoRaSerial.available() >= (int)sizeof(AckPacket)) {
        AckPacket ack;
        LoRaSerial.readBytes((char*)&ack, sizeof(AckPacket));
        if (ack.header == ACK_PACKET_HEADER &&
            ack.target_id == NODE_ID &&
            ack.ack_msg_id == pkt.msg_id) {
          Serial.printf("OK! Da nhan ACK sau %lu ms\n", millis() - startWait);
          return true;
        }
      }
      delay(5);
    }

    Serial.println("TIMEOUT!");
    if (attempt < MAX_RETRIES) {
      // Chờ ngẫu nhiên 200 - 500ms trước khi thử lại để chống va chạm lặp lại
      delay(random(200, 500));
    }
  }

  Serial.println("[TX FAIL] Khong nhan duoc ACK tu Master sau cac lan thu!");
  return false;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println(F("\n══════════════════════════════════════════"));
  Serial.printf(  " SMART GREENHOUSE - SENSOR NODE %d (ESP32-C3)\n", NODE_ID);
  Serial.println(F("══════════════════════════════════════════"));

  // Khởi tạo UART LoRa E32
  LoRaSerial.begin(LORA_BAUD, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);
  Serial.printf("[LORA] Serial1 khoi tao RX=%d, TX=%d @ %d baud\n",
                LORA_RX_PIN, LORA_TX_PIN, LORA_BAUD);

  // Khởi tạo ADC đất (12-bit: 0 - 4095, 11dB: 0 - 3.3V)
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  Serial.printf("[SOIL] ADC khoi tao tai GPIO %d (Dry=%d, Wet=%d)\n",
                SOIL_ADC_PIN, SOIL_DRY_RAW, SOIL_WET_RAW);
#if CALIBRATION_MODE
  Serial.println(F("[CAL] CHE DO HIEU CHUAN DANG BAT! Dat cam bien va quan sat gia tri Raw sau:"));
#endif

  // Khởi tạo I2C SHT30
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  if (sht30.begin(SHT30_I2C_ADDR)) {
    g_shtOK = true;
    sht30.heater(false);
    Serial.println(F("[SHT30] Khoi tao I2C 0x44 THANH CONG"));
  } else {
    g_shtOK = false;
    Serial.println(F("[SHT30] Khoi tao THAT BAI! Kiem tra day SDA/SCL."));
  }

  // Khởi tạo mốc thời gian lệch pha
  g_lastTxTime = millis() - TX_INTERVAL_MS + PHASE_OFFSET_MS;
  randomSeed(analogRead(0));
  Serial.printf("[WSN] Phase Offset: %lu ms | Interval: %lu ms\n", PHASE_OFFSET_MS, TX_INTERVAL_MS);
}

void loop() {
  unsigned long now = millis();

#if CALIBRATION_MODE
  // Chế độ hiệu chuẩn: chỉ in raw ADC, không gửi LoRa
  {
    static unsigned long lastCal = 0;
    if (now - lastCal >= 2000) {
      lastCal = now;
      long sum = 0;
      for (int i = 0; i < 10; i++) { sum += analogRead(SOIL_ADC_PIN); delay(4); }
      int raw = (int)(sum / 10);
      Serial.printf("[CAL] GPIO%d Raw ADC = %d | Uoc tinh: %.1f%%\n",
                    SOIL_ADC_PIN, raw,
                    constrain((float)(SOIL_DRY_RAW - raw) * 100.0f / (float)(SOIL_DRY_RAW - SOIL_WET_RAW), 0.0f, 100.0f));
    }
    return; // Không phát LoRa khi đang hiệu chuẩn
  }
#endif

  // Kiểm tra chu kỳ phát dữ liệu định kỳ
  if (now - g_lastTxTime >= TX_INTERVAL_MS) {
    g_lastTxTime = now;
    g_msgCounter++;

    // 1. Đọc cảm biến nhiệt độ & độ ẩm SHT30
    float t = NAN;
    float h = NAN;
    bool shtError = false;

    if (g_shtOK) {
      t = sht30.readTemperature();
      h = sht30.readHumidity();
      if (isnan(t) || isnan(h) || t < -20.0f || t > 70.0f || h < 0.0f || h > 100.0f) {
        shtError = true;
      }
    } else {
      // Thử kết nối lại SHT30 nếu trước đó bị lỗi
      if (sht30.begin(SHT30_I2C_ADDR)) {
        g_shtOK = true;
        sht30.heater(false);
      }
      shtError = true;
    }

    // 2. Đọc cảm biến độ ẩm đất
    uint16_t soilRaw = 0;
    bool soilError = false;
    float soilPct = readSoilMoisture(soilRaw, soilError);

    // 3. Đóng gói dữ liệu
    SensorPacket pkt;
    pkt.header = SENSOR_PACKET_HEADER;
    pkt.node_id = NODE_ID;
    pkt.msg_id = g_msgCounter;

    if (!shtError) {
      pkt.temp_x10 = (int16_t)round(t * 10.0f);
      pkt.hum_x10  = (uint16_t)round(h * 10.0f);
    } else {
      pkt.temp_x10 = -999;
      pkt.hum_x10  = 0;
    }

    if (!soilError) {
      pkt.soil_pct = (uint16_t)round(soilPct);
    } else {
      pkt.soil_pct = 0;
    }

    pkt.status_mask = 0;
    if (shtError)  pkt.status_mask |= (1 << 0);
    if (soilError) pkt.status_mask |= (1 << 1);

    // Tính mã kiểm tra CRC-16
    pkt.crc16 = calcCRC16((const uint8_t*)&pkt, sizeof(SensorPacket) - 2);

    Serial.printf("[SENSOR] T=%.1f*C, H=%.1f%%, Dat=%.1f%% (Raw:%u) | Mask=0x%02X | CRC=0x%04X\n",
                  t, h, soilPct, soilRaw, pkt.status_mask, pkt.crc16);

    // 4. Phát dữ liệu qua LoRa và lắng nghe ACK
    sendPacketWithAck(pkt);
  }

  // Dọn dẹp dữ liệu rác nếu có byte lạc vào cổng LoRa ngoài phiên ACK
  while (LoRaSerial.available()) {
    LoRaSerial.read();
  }
}
