#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// ═════════════════════════════════════════════════════════════
//  HỆ THỐNG NHÀ MÀNG THÔNG MINH - NODE 2: HMI NODE (ESP32)
//  Tủ điện điều khiển tại chỗ, giám sát thông số & hiển thị LCD
// ═════════════════════════════════════════════════════════════

// ─────────────────────────────────────────────────────────────
//  1. GIAO TIẾP UART KẾT NỐI VỚI MASTER NODE (SERIAL2 PHẦN CỨNG)
//  - Chân 16 (RX2): Nhận dữ liệu cảm biến & trạng thái thiết bị từ Master (TX
//  17)
//  - Chân 17 (TX2): Gửi lệnh điều khiển nút bấm & góc mở rèm sang Master (RX
//  16)
//  - Kết nối Header H17 trên bo mạch HMI
// ─────────────────────────────────────────────────────────────
#define RX2_PIN 16
#define TX2_PIN 17

// ─────────────────────────────────────────────────────────────
//  2. MÀN HÌNH LCD 2004 I2C (BACKPACK PCF8574 - ĐỊA CHỈ 0x27)
// ─────────────────────────────────────────────────────────────
#define I2C_SDA 21
#define I2C_SCL 22
LiquidCrystal_I2C lcd(0x27, 20, 4);

// ─────────────────────────────────────────────────────────────
//  3. CẤU HÌNH THIẾT BỊ, NÚT NHẤN CƠ & ĐÈN LED CHỈ BÁO
//  Danh sách 6 thiết bị:
//  0: Quạt, 1: Đèn, 2: Sưởi, 3: Phun sương, 4: Bơm, 5: Rèm Cửa
// ─────────────────────────────────────────────────────────────
#define NUM_DEVICES 6 // 6 cơ cấu chấp hành trong nhà màng
#define NUM_BUTTONS                                                            \
  5 // 5 nút nhấn cơ đóng/ngắt (Rèm cửa dùng biến trở xoay riêng)
#define NUM_LEDS                                                               \
  5 // 5 cặp LED (Xanh: Đang Bật / Đỏ: Đang Tắt)
    // Ghi chú: ĐÃ BỎ LED & Nút Rèm để giải phóng GPIO 0, 1, 3 nạp code an toàn

// ─────────────────────────────────────────────────────────────
//  4. BIẾN TRỞ XOAY ĐIỀU CHỈNH GÓC MỞ RÈM CỬA (0 - 75 độ = 0 - 100%)
//  - Xoay hết cỡ biến trở = 100% = 75 độ (khớp ROOF_MAX_ANGLE bên Master)
//  - LCD hiển thị theo %: VD 10 độ -> 13%
//  - Chân giữa biến trở vào GPIO 34 (ADC1_CH6, Input-only an toàn tuyệt đối)
//  - 2 chân ngoài nối 3.3V và GND
//  - Đặt ENABLE_POT_ROOF = false khi CHƯA cắm biến trở (chống thả nổi ADC làm
//  giật rèm)
// ─────────────────────────────────────────────────────────────
#define ENABLE_POT_ROOF                                                        \
  true // =true: Đã cắm biến trở | =false: Chưa cắm (tránh thả nổi ADC)
#define POT_ROOF_PIN 34
#define ROOF_MAX_ANGLE 70 // Góc mở tối đa rèm (PHẢI trùng ROOF_MAX_ANGLE trong MasterNode/config.h = 70 độ)
// Quy đổi góc rèm (0 - 75 độ) sang phần trăm mở (0 - 100%), làm tròn
#define ROOF_ANGLE_TO_PCT(a) ((int)(((a) * 100 + ROOF_MAX_ANGLE / 2) / ROOF_MAX_ANGLE))

// Mảng chân GPIO 5 Nút bấm (Cấu hình INPUT_PULLUP nội, nhấn xuống nối GND)
const int BTNS[NUM_BUTTONS] = {13, 14, 27, 26, 15};
// 13: Nút Quạt | 14: Nút Đèn | 27: Nút Sưởi | 26: Nút Phun sương | 15: Nút Bơm

// Mảng chân GPIO 5 Đèn LED Xanh (Báo trạng thái thiết bị ĐANG BẬT)
const int LED_G[NUM_LEDS] = {32, 33, 25, 12, 19};
// 32: LED Quạt | 33: LED Đèn | 25: LED Sưởi | 12: LED Phun sương | 19: LED Bơm

// Mảng chân GPIO 5 Đèn LED Đỏ (Báo trạng thái thiết bị ĐANG TẮT)
const int LED_R[NUM_LEDS] = {18, 5, 4, 2, 23};
// 18: LED Quạt | 5: LED Đèn  | 4: LED Sưởi  | 2: LED Phun sương  | 23: LED Bơm

// Danh sách mã lệnh gửi sang Master khi nhấn nút
const char *CMD_NAMES[NUM_BUTTONS] = {
    "CMD:TOGGLE_FAN",    // Đảo trạng thái Quạt
    "CMD:TOGGLE_LIGHT",  // Đảo trạng thái Đèn
    "CMD:TOGGLE_HEATER", // Đảo trạng thái Sưởi
    "CMD:TOGGLE_MIST",   // Đảo trạng thái Phun sương
    "CMD:TOGGLE_PUMP"    // Đảo trạng thái Bơm tưới
};

// ─── Biến quản lý trạng thái chống dội phím (Debounce 50ms) ───
int buttonState[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH};
int lastButtonReading[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[NUM_BUTTONS] = {0, 0, 0, 0, 0};
const unsigned long debounceDelay = 50; // Trễ 50ms lọc nhiễu dội phím cơ

// ─── Trạng thái 6 thiết bị nhận từ Master Node (0: Tắt/Đóng, 1: Bật/Mở) ───
int deviceStatus[NUM_DEVICES] = {0, 0, 0, 0, 0, 0};

// ─── Biến quản lý góc rèm cửa & bộ lọc biến trở (EMA filter) ───
int currentRoofAngle = 0;
int lastSentRoofAngle = -1;
unsigned long lastPotSampleTime = 0;
float smoothedPot = -1.0;

// ─── Biến lưu trạng thái cảm biến & thiết bị hiển thị LCD ───
int g_fanLevel = 0;       // 0 (Tắt), 1 (40%), 2 (60%), 3 (80%), 4 (100%)
int g_heaterLevel = 0;    // 0 (Tắt), 1 (40%), 2 (60%), 3 (80%), 4 (100%)
bool g_sht30OK = true;
bool g_sht30ExtOK = true;
float g_lastT1 = 0, g_lastH1 = 0, g_lastT2 = 0, g_lastH2 = 0, g_lastLux = 0;

// Khai báo trước các hàm con
void updateLEDs();
void updateLCD(float t1, float h1, float t2, float h2, float lux);
void updateLCDRow3();
void updateLCDActuators();
void parseMasterData(String json);

void setup() {
  // Serial USB debug trên máy tính
  Serial.begin(115200);

  // Serial2 giao tiếp với bo mạch Master Node
  Serial2.begin(115200, SERIAL_8N1, RX2_PIN, TX2_PIN);

  // Khởi động màn hình LCD 2004 I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setTimeOut(50); // Chống treo I2C khi lỏng dây hoặc nhiễu tải công suất
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HMI Node Starting...");

  // Cấu hình chân 5 nút nhấn
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(BTNS[i], INPUT_PULLUP);
  }

  // Cấu hình chân 10 đèn LED (5 Xanh, 5 Đỏ)
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_G[i], OUTPUT);
    pinMode(LED_R[i], OUTPUT);
    digitalWrite(LED_G[i], LOW); // Khởi động: LED Xanh Tắt
    digitalWrite(LED_R[i],
                 HIGH); // Khởi động: LED Đỏ Sáng (báo thiết bị đang Tắt)
  }

  // Cấu hình chân đọc chiết áp xoay rèm cửa
#if ENABLE_POT_ROOF
  pinMode(POT_ROOF_PIN, INPUT);
#endif

  delay(300);
  lcd.clear();
  updateLCDActuators();
}

void loop() {
  unsigned long now = millis();

  // ─── 1. NHẬN VÀ XỬ LÝ DỮ LIỆU TỪ MASTER QUA SERIAL2 ───
  if (Serial2.available()) {
    String msg = Serial2.readStringUntil('\n');
    msg.trim();
    if (msg.length() > 0) {
      parseMasterData(msg);
    }
  }

  // ─── 2. QUÉT VÀ XỬ LÝ NÚT BẤM (CHỐNG DỘI PHÍM DEBOUNCE 50ms) ───
  for (int i = 0; i < NUM_BUTTONS; i++) {
    int reading = digitalRead(BTNS[i]);

    if (reading != lastButtonReading[i]) {
      lastDebounceTime[i] = now;
    }

    if ((now - lastDebounceTime[i]) > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;

        // Bắt sự kiện nhấn xuống (LOW do sử dụng trở kéo lên INPUT_PULLUP)
        if (buttonState[i] == LOW) {
          Serial.println("Nhan nut: " + String(CMD_NAMES[i]));
          Serial2.println(CMD_NAMES[i]); // Gửi lệnh sang Master

          // Cập nhật phản hồi nhanh tại chỗ trên LCD & LED ngay lập tức (<1ms)
          if (i == 0) { // Quạt: chu trình mức 0 -> 1 -> 2 -> 3 -> 4 -> 0
            g_fanLevel = (g_fanLevel + 1) % 5;
            deviceStatus[0] = (g_fanLevel > 0) ? 1 : 0;
          } else if (i == 2) { // Sưởi: chu trình mức 0 -> 1 -> 2 -> 3 -> 4 -> 0
            g_heaterLevel = (g_heaterLevel + 1) % 5;
            deviceStatus[2] = (g_heaterLevel > 0) ? 1 : 0;
          } else {
            deviceStatus[i] = !deviceStatus[i];
          }
          updateLEDs();
          updateLCDActuators();
        }
      }
    }
    lastButtonReading[i] = reading;
  }

  // ─── 3. ĐỌC BIẾN TRỞ VÀ ĐIỀU CHỈNH GÓC MỞ RÈM CỬA BẰNG TAY (0 - 75 độ) ───
#if ENABLE_POT_ROOF
  if (now - lastPotSampleTime >=
      35) { // Lấy mẫu 35ms (~28 lần/giây: mượt mà, bám sát tay xoay)
    lastPotSampleTime = now;

    // Lấy trung bình 8 mẫu ADC nhanh (chỉ mất ~0.2ms)
    long sum = 0;
    for (int s = 0; s < 8; s++) {
      sum += analogRead(POT_ROOF_PIN);
      delayMicroseconds(25);
    }
    int raw = (int)(sum / 8);

    // Bộ lọc EMA: phản hồi tức thì nhưng triệt tiêu hoàn toàn rung số
    if (smoothedPot < 0)
      smoothedPot = raw;
    else
      smoothedPot = (smoothedPot * 0.75f) + (raw * 0.25f);

    // Ánh xạ ADC sang góc 0 - 75 độ (xoay hết cỡ = 100% = 75 độ, ~53 đơn vị ADC / 1 độ)
    int targetAngle = map((int)smoothedPot, 40, 4050, 0, ROOF_MAX_ANGLE);
    targetAngle = constrain(targetAngle, 0, ROOF_MAX_ANGLE);

    // Vùng chết Deadband 1°: Bám sát từng độ xoay, servo chuyển động liên tục
    // không giật khấc
    if (lastSentRoofAngle < 0) {
      lastSentRoofAngle =
          targetAngle; // Ghi nhận vị trí ban đầu của biến trở khi vừa khởi động
    } else if (abs(targetAngle - lastSentRoofAngle) >= 1) {
      lastSentRoofAngle = targetAngle;
      currentRoofAngle = targetAngle;

      // Gửi lệnh góc rèm sang Master
      Serial2.printf("CMD:SET_ROOF:%d\n", targetAngle);

      deviceStatus[5] = (targetAngle > 0) ? 1 : 0;
      updateLEDs();
    }
  }

  // Cập nhật LCD Dòng 3 riêng biệt (tối thiểu 80ms) khi xoay chiết áp rèm
  static unsigned long lastLcdRoofTime = 0;
  static int lastDisplayedRoofAngle = -1;
  if (currentRoofAngle != lastDisplayedRoofAngle &&
      (now - lastLcdRoofTime >= 80)) {
    lastLcdRoofTime = now;
    lastDisplayedRoofAngle = currentRoofAngle;
    updateLCDRow3();
  }
#endif

  // ─── 4. TỰ ĐỘNG PHỤC HỒI MÀN HÌNH LCD NẾU BỊ NHIỄU XUNG RƠ-LE / ĐỘNG CƠ ───
  static unsigned long lastLcdHealthCheck = 0;
  if (now - lastLcdHealthCheck >= 30000) { // Kiểm tra mỗi 30 giây
    lastLcdHealthCheck = now;
    Wire.beginTransmission(0x27);
    if (Wire.endTransmission() != 0) {
      Serial.println(
          F("[LCD] PCF8574 bi treo do nhieu -> Khoi phuc I2C & LCD"));
      Wire.end();
      delay(10);
      Wire.begin(I2C_SDA, I2C_SCL);
      Wire.setTimeOut(50);
      lcd.init();
      lcd.backlight();
      updateLCD(g_lastT1, g_lastH1, g_lastT2, g_lastH2, g_lastLux);
    }
  }
}

// ═══════════════════════════════════════════════════════════════
//  GIẢI MÃ DỮ LIỆU ĐỊNH KỲ TỪ MASTER GỬI SANG (JSON FORMAT)
// ═══════════════════════════════════════════════════════════════
void parseMasterData(String json) {
  // Lọc bỏ các ký tự rác trước '{' và sau '}' nếu có nhiễu đường truyền
  int startIndex = json.indexOf('{');
  int endIndex = json.lastIndexOf('}');
  if (startIndex == -1 || endIndex == -1 || endIndex <= startIndex) {
    return;
  }
  String cleanJson = json.substring(startIndex, endIndex + 1);

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, cleanJson);
  if (err) {
    Serial.print("JSON Parse Error: ");
    Serial.println(err.c_str());
    return;
  }

  // Dữ liệu cảm biến từ Master (-999 biểu thị cảm biến mất kết nối)
  float t1 = doc["t1"] | -999.0f;
  float h1 = doc["h1"] | -999.0f;
  float t2 = doc["t2"] | -999.0f;
  float h2 = doc["h2"] | -999.0f;
  float lux = doc["lux"] | -999.0f;

  if (doc.containsKey("s_ok")) {
    g_sht30OK = doc["s_ok"].as<int>() == 1;
  } else {
    g_sht30OK = (t1 > -900);
  }

  if (doc.containsKey("se_ok")) {
    g_sht30ExtOK = doc["se_ok"].as<int>() == 1;
  } else {
    g_sht30ExtOK = (t2 > -900);
  }

  if (doc.containsKey("fan_lvl")) {
    g_fanLevel = doc["fan_lvl"];
  } else if (doc.containsKey("fan_spd")) {
    int spd = doc["fan_spd"];
    if (spd == 0) g_fanLevel = 0;
    else if (spd <= 50) g_fanLevel = 1;
    else if (spd <= 70) g_fanLevel = 2;
    else if (spd <= 90) g_fanLevel = 3;
    else g_fanLevel = 4;
  }

  if (doc.containsKey("heat_lvl")) {
    g_heaterLevel = doc["heat_lvl"];
  } else if (doc.containsKey("heat_pwm")) {
    int pwm = doc["heat_pwm"];
    if (pwm == 0) g_heaterLevel = 0;
    else if (pwm <= 125) g_heaterLevel = 1;
    else if (pwm <= 180) g_heaterLevel = 2;
    else if (pwm <= 230) g_heaterLevel = 3;
    else g_heaterLevel = 4;
  } else if (doc.containsKey("st")) {
    if (deviceStatus[2] == 0) g_heaterLevel = 0;
    else if (g_heaterLevel == 0) g_heaterLevel = 1;
  }

  // Mảng trạng thái 6 thiết bị từ Master
  if (doc.containsKey("st")) {
    int count = doc["st"].size();
    for (int i = 0; i < NUM_DEVICES && i < count; i++) {
      deviceStatus[i] = doc["st"][i];
    }
    updateLEDs();
  }

  // Góc mở rèm hiện tại
  if (doc.containsKey("roof")) {
    currentRoofAngle = doc["roof"];
  }

  // Cập nhật toàn bộ thông tin lên LCD 2004
  updateLCD(t1, h1, t2, h2, lux);
}

// ═══════════════════════════════════════════════════════════════
//  CẬP NHẬT TRẠNG THÁI 5 CẶP ĐÈN LED (XANH = BẬT, ĐỎ = TẮT)
// ═══════════════════════════════════════════════════════════════
void updateLEDs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (deviceStatus[i] == 1) {
      digitalWrite(LED_G[i], HIGH); // Thiết bị bật -> LED Xanh Sáng
      digitalWrite(LED_R[i], LOW);  // LED Đỏ Tắt
    } else {
      digitalWrite(LED_G[i], LOW);  // LED Xanh Tắt
      digitalWrite(LED_R[i], HIGH); // Thiết bị tắt -> LED Đỏ Sáng
    }
  }
}

// ═══════════════════════════════════════════════════════════════
//  CẬP NHẬT THÔNG TIN HIỂN THỊ MÀN HÌNH LCD 2004 (4 DÒNG x 20 CỘT)
// ═══════════════════════════════════════════════════════════════
void updateLCD(float t1, float h1, float t2, float h2, float lux) {
  g_lastT1 = t1;
  g_lastH1 = h1;
  g_lastT2 = t2;
  g_lastH2 = h2;
  g_lastLux = lux;

  char buf[21];

  // Dòng 1: Cảm biến Trong nhà (SHT30 Inside) - chỉ hiển thị chữ C
  lcd.setCursor(0, 0);
  if (g_sht30OK && t1 > -900 && h1 >= 0) {
    snprintf(buf, sizeof(buf), "In:  %4.1fC   %4.1f%%  ", t1, h1);
  } else {
    snprintf(buf, sizeof(buf), "In:  [LOI CAM BIEN] ");
  }
  lcd.print(buf);

  // Dòng 2: Cảm biến Ngoài trời (SHT30 Outside) - chỉ hiển thị chữ C
  lcd.setCursor(0, 1);
  if (g_sht30ExtOK && t2 > -900 && h2 >= 0) {
    snprintf(buf, sizeof(buf), "Out: %4.1fC   %4.1f%%  ", t2, h2);
  } else {
    snprintf(buf, sizeof(buf), "Out: [LOI CAM BIEN] ");
  }
  lcd.print(buf);

  // Dòng 3: Cảm biến Cường độ sáng (BH1750 Lux) & Góc mở Rèm cửa
  updateLCDRow3();

  // Dòng 4: Trạng thái 5 thiết bị chấp hành: Q: D: S: P: B:
  updateLCDActuators();
}

// ═══════════════════════════════════════════════════════════════
//  DÒNG 3 LCD: CƯỜNG ĐỘ SÁNG (LUX) & GÓC MỞ RÈM CỬA
// ═══════════════════════════════════════════════════════════════
void updateLCDRow3() {
  lcd.setCursor(0, 2);
  char buf3[21];
  if (g_lastLux >= 0) {
    snprintf(buf3, sizeof(buf3), "Lux:%4.0flx |Rem:%3d%%", g_lastLux,
             ROOF_ANGLE_TO_PCT(currentRoofAngle));
  } else {
    snprintf(buf3, sizeof(buf3), "Lux:----   |Rem:%3d%%",
             ROOF_ANGLE_TO_PCT(currentRoofAngle));
  }
  lcd.print(buf3);
}

// ═══════════════════════════════════════════════════════════════
//  DÒNG 4 LCD: TRẠNG THÁI 5 THIẾT BỊ THEO CÚ PHÁP: Q: D: S: P: B:
//  - Q (Quạt), S (Đèn sưởi): 0 (Tắt), 1 (40%), 2 (60%), 3 (80%), 4 (100%)
//  - D (Đèn), P (Phun sương), B (Bơm): 0 (Tắt), 1 (Bật)
//  Cập nhật tức thì khi bấm nút cơ để người dùng thấy ngay phản hồi
// ═══════════════════════════════════════════════════════════════
void updateLCDActuators() {
  lcd.setCursor(0, 3);
  char buf4[21];

  snprintf(buf4, sizeof(buf4), "Q:%d D:%d S:%d P:%d B:%d ",
           g_fanLevel,
           deviceStatus[1] ? 1 : 0,
           g_heaterLevel,
           deviceStatus[3] ? 1 : 0,
           deviceStatus[4] ? 1 : 0);
  lcd.print(buf4);
}
