// =====================================================================
//  CHƯƠNG TRÌNH TEST PHẦN CỨNG TỦ ĐIỆN HMI (HMI HARDWARE SELF-TEST)
//  Dự án: Nhà Màng Thông Minh ĐATN
//  Vị trí lưu: Hardware/test_hmi/test_hmi.ino
//  Mục đích:
//    1. Kiểm tra 5 Nút nhấn cơ xem có nhận tín hiệu và đúng chân không
//    2. Kiểm tra 10 Đèn LED (5 Xanh, 5 Đỏ) xem có sáng đúng cặp thiết bị không
//    3. Kiểm tra Biến trở xoay rèm cửa (GPIO 34) xem ADC có thay đổi khi xoay không
//    4. Kiểm tra Màn hình LCD 2004 I2C (địa chỉ 0x27)
//  ĐẶC BIỆT: Chạy độc lập, KHÔNG CẦN kết nối UART với Master Node!
// =====================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ─────────────────────────────────────────────────────────────────────
//  1. CẤU HÌNH CHÂN PHẦN CỨNG HMI (Chuẩn 100% theo tủ điện thực tế)
// ─────────────────────────────────────────────────────────────────────

#define NUM_CHANNELS 5 // 5 kênh thiết bị có nút bấm & LED

// Tên 5 thiết bị
const char* DEVICE_NAMES[NUM_CHANNELS] = {
  "1. QUAT (Fan)   ",
  "2. DEN (Light)  ",
  "3. SUOI (Heater)",
  "4. PHUN (Mist)  ",
  "5. BOM (Pump)   "
};

// 5 Chân Nút nhấn (Cấu hình INPUT_PULLUP: Bình thường HIGH, nhấn xuống nối GND = LOW)
const int PIN_BTNS[NUM_CHANNELS]  = {13, 14, 27, 26, 15};
// 13: Quạt | 14: Đèn | 27: Sưởi | 26: Phun sương | 15: Bơm

// 5 Chân LED Xanh (Báo BẬT - Xuất HIGH sáng đèn)
const int PIN_LED_G[NUM_CHANNELS] = {32, 33, 25, 12, 19};

// 5 Chân LED Đỏ (Báo TẮT - Xuất HIGH sáng đèn)
const int PIN_LED_R[NUM_CHANNELS] = {18, 5, 4, 2, 23};

// Biến trở xoay rèm cửa (Chân giữa vào GPIO 34, 2 chân ngoài vào 3.3V và GND)
#define PIN_POT_ROOF   34
#define ROOF_MAX_ANGLE 70 // Góc mở rèm tối đa (70 độ)

// Màn hình LCD 2004 I2C (SDA = GPIO 21, SCL = GPIO 22)
#define I2C_SDA 21
#define I2C_SCL 22
LiquidCrystal_I2C lcd(0x27, 20, 4);
bool hasLCD = false;

// ─────────────────────────────────────────────────────────────────────
//  2. BIẾN QUẢN LÝ TRẠNG THÁI NÚT BẤM & LED
// ─────────────────────────────────────────────────────────────────────
bool deviceState[NUM_CHANNELS] = {false, false, false, false, false}; // false: Đang Tắt (LED Đỏ), true: Đang Bật (LED Xanh)
int lastButtonReading[NUM_CHANNELS] = {HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[NUM_CHANNELS] = {0, 0, 0, 0, 0};
const unsigned long DEBOUNCE_DELAY = 50; // Chống dội phím 50ms

// Cập nhật trạng thái đèn LED theo mảng deviceState
void updateLEDs() {
  for (int i = 0; i < NUM_CHANNELS; i++) {
    if (deviceState[i]) {
      digitalWrite(PIN_LED_G[i], HIGH); // Bật LED Xanh
      digitalWrite(PIN_LED_R[i], LOW);  // Tắt LED Đỏ
    } else {
      digitalWrite(PIN_LED_G[i], LOW);  // Tắt LED Xanh
      digitalWrite(PIN_LED_R[i], HIGH); // Bật LED Đỏ
    }
  }
}

// ─────────────────────────────────────────────────────────────────────
//  3. TỰ ĐỘNG KIỂM TRA TOÀN BỘ ĐÈN LED KHI KHỞI ĐỘNG (LED TEST ON BOOT)
// ─────────────────────────────────────────────────────────────────────
void runLedTestSequence() {
  Serial.println("\n[1/3] DANG TEST TOAN BO DEN LED...");

  // Bước 1: Bật tất cả LED XANH trong 1.2 giây
  Serial.println("  -> Bat tat ca 5 LED XANH (Kiem tra xem co sang deu khong)");
  if (hasLCD) { lcd.clear(); lcd.setCursor(0, 0); lcd.print("TEST: 5 LED XANH"); }
  for (int i = 0; i < NUM_CHANNELS; i++) digitalWrite(PIN_LED_G[i], HIGH);
  for (int i = 0; i < NUM_CHANNELS; i++) digitalWrite(PIN_LED_R[i], LOW);
  delay(1200);

  // Bước 2: Bật tất cả LED ĐỎ trong 1.2 giây
  Serial.println("  -> Bat tat ca 5 LED DO (Kiem tra xem co sang deu khong)");
  if (hasLCD) { lcd.setCursor(0, 0); lcd.print("TEST: 5 LED DO  "); }
  for (int i = 0; i < NUM_CHANNELS; i++) digitalWrite(PIN_LED_G[i], LOW);
  for (int i = 0; i < NUM_CHANNELS; i++) digitalWrite(PIN_LED_R[i], HIGH);
  delay(1200);

  // Bước 3: Chớp lần lượt từng thiết bị (Xanh -> Đỏ)
  Serial.println("  -> Kiem tra lan luot tung thiet bi (Xanh / Do):");
  for (int i = 0; i < NUM_CHANNELS; i++) {
    // Tắt hết
    for (int j = 0; j < NUM_CHANNELS; j++) {
      digitalWrite(PIN_LED_G[j], LOW);
      digitalWrite(PIN_LED_R[j], LOW);
    }
    Serial.printf("     Dang test: %s | Xanh (GPIO %d) -> Do (GPIO %d)\n", 
                  DEVICE_NAMES[i], PIN_LED_G[i], PIN_LED_R[i]);
    if (hasLCD) {
      lcd.setCursor(0, 1);
      lcd.print("Test: "); lcd.print(DEVICE_NAMES[i]);
    }
    // Sáng Xanh
    digitalWrite(PIN_LED_G[i], HIGH);
    delay(300);
    digitalWrite(PIN_LED_G[i], LOW);
    // Sáng Đỏ
    digitalWrite(PIN_LED_R[i], HIGH);
    delay(300);
    digitalWrite(PIN_LED_R[i], LOW);
  }

  // Kết thúc test: Đưa tất cả về trạng thái TẮT mặc định (5 LED ĐỎ SÁNG)
  updateLEDs();
  Serial.println("  -> Test LED hoan tat! Mac dinh: 5 LED DO DANG SANG (Bao tat ca thiet bi dang TAT).");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n========================================================");
  Serial.println("  CHUONG TRINH TEST PHAN CUNG TU DIEN HMI (STANDALONE)  ");
  Serial.println("========================================================");

  // 1. Cấu hình các chân Nút bấm (INPUT_PULLUP)
  for (int i = 0; i < NUM_CHANNELS; i++) {
    pinMode(PIN_BTNS[i], INPUT_PULLUP);
  }

  // 2. Cấu hình các chân LED (OUTPUT)
  for (int i = 0; i < NUM_CHANNELS; i++) {
    pinMode(PIN_LED_G[i], OUTPUT);
    pinMode(PIN_LED_R[i], OUTPUT);
    digitalWrite(PIN_LED_G[i], LOW);
    digitalWrite(PIN_LED_R[i], LOW);
  }

  // 3. Khởi tạo màn hình LCD 2004 (I2C)
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.beginTransmission(0x27);
  if (Wire.endTransmission() == 0) {
    hasLCD = true;
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(2, 0); lcd.print("TEST TU DIEN HMI");
    lcd.setCursor(0, 1); lcd.print("Dang kiem tra...");
    Serial.println("[OK] Da tim thay man hinh LCD 2004 tai I2C 0x27.");
  } else {
    hasLCD = false;
    Serial.println("[CHUY] Khong tim thay LCD 2004 tai I2C 0x27 (Chua cam hoac sai dia chi).");
    Serial.println("       Chuong trinh van tiep tuc test nut bam va LED qua Serial!");
  }

  // 4. Chạy chuỗi kiểm tra LED tự động
  runLedTestSequence();

  // 5. Cấu hình chân Biến trở
  pinMode(PIN_POT_ROOF, INPUT);

  if (hasLCD) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("HMI HARDWARE TEST");
    lcd.setCursor(0, 1); lcd.print("Nhan nut: Dao mau LED");
    lcd.setCursor(0, 2); lcd.print("Xoay bien tro de xem");
  }

  Serial.println("\n[HUONG DAN SU DUNG TEST PHAN CUNG]:");
  Serial.println("1. TEST NUT BAM:");
  Serial.println("   - Nhan nut bat ky tren tu dien -> LED cua thiet bi do se doi mau:");
  Serial.println("     + Tu DO (Tat) sang XANH (Bat)");
  Serial.println("     + Nhan lai -> Chuyen tu XANH sang DO");
  Serial.println("   - Serial Monitor se in ngay thiet bi nao vua duoc nhan!");
  Serial.println("2. TEST BIEN TRO:");
  Serial.println("   - Xoay bien tro -> Serial Monitor va LCD se in gia tri ADC va Goc 0 - 70 do.");
  Serial.println("========================================================\n");
}

void loop() {
  unsigned long now = millis();

  // ───────────────────────────────────────────────────────────────────
  //  PHẦN 1: QUÉT VÀ KIỂM TRA 5 NÚT NHẤN CƠ
  // ───────────────────────────────────────────────────────────────────
  for (int i = 0; i < NUM_CHANNELS; i++) {
    int reading = digitalRead(PIN_BTNS[i]);

    // Nếu trạng thái chân thay đổi, reset bộ đếm dội phím
    if (reading != lastButtonReading[i]) {
      lastDebounceTime[i] = now;
    }

    if ((now - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
      // Khi nút bấm được nhấn xuống (chuyển sang LOW)
      static int currentStableState[NUM_CHANNELS] = {HIGH, HIGH, HIGH, HIGH, HIGH};
      if (reading != currentStableState[i]) {
        currentStableState[i] = reading;

        if (currentStableState[i] == LOW) {
          // Đảo trạng thái thiết bị
          deviceState[i] = !deviceState[i];
          updateLEDs();

          // In thông báo rõ ràng ra Serial Monitor
          Serial.println("--------------------------------------------------");
          Serial.printf("[NUT BAM DUOC NHAN] -> %s (Chan GPIO %d)\n", DEVICE_NAMES[i], PIN_BTNS[i]);
          if (deviceState[i]) {
            Serial.printf("   => Trang thai: BAT  | LED XANH (GPIO %d) SANG | LED DO (GPIO %d) TAT\n", 
                          PIN_LED_G[i], PIN_LED_R[i]);
          } else {
            Serial.printf("   => Trang thai: TAT  | LED DO (GPIO %d) SANG   | LED XANH (GPIO %d) TAT\n", 
                          PIN_LED_R[i], PIN_LED_G[i]);
          }
          Serial.println("--------------------------------------------------");

          // Cập nhật lên màn hình LCD
          if (hasLCD) {
            lcd.setCursor(0, 1);
            lcd.print("Vua nhan: ");
            lcd.print(DEVICE_NAMES[i]);
            lcd.setCursor(0, 2);
            lcd.print("Trang thai: ");
            lcd.print(deviceState[i] ? "BAT [XANH] " : "TAT [DO]   ");
          }
        }
      }
    }
    lastButtonReading[i] = reading;
  }

  // ───────────────────────────────────────────────────────────────────
  //  PHẦN 2: QUÉT VÀ KIỂM TRA BIẾN TRỞ XOAY RÈM CỬA (GPIO 34)
  // ───────────────────────────────────────────────────────────────────
  static unsigned long lastPotCheck = 0;
  static int lastAngle = -999;

  if (now - lastPotCheck >= 100) { // Đọc biến trở mỗi 100ms
    lastPotCheck = now;

    // Lấy mẫu trung bình 4 lần để chống rung
    long sum = 0;
    for (int k = 0; k < 4; k++) {
      sum += analogRead(PIN_POT_ROOF);
      delayMicroseconds(20);
    }
    int rawADC = sum / 4;

    // Quy đổi từ dải ADC (khoảng 40 - 4050) sang góc 0 - 70 độ
    int angle = map(rawADC, 40, 4050, 0, ROOF_MAX_ANGLE);
    angle = constrain(angle, 0, ROOF_MAX_ANGLE);
    int pct = (angle * 100) / ROOF_MAX_ANGLE; // Phần trăm mở 0 - 100%

    // Chỉ in khi góc xoay thay đổi từ 1 độ trở lên (tránh spam Serial)
    if (abs(angle - lastAngle) >= 1) {
      lastAngle = angle;

      Serial.printf("[BIEN TRO GPIO 34] ADC: %4d | Goc quy doi: %2d do (%3d%%)\n", 
                    rawADC, angle, pct);

      if (hasLCD) {
        lcd.setCursor(0, 3);
        char buf[21];
        snprintf(buf, sizeof(buf), "Rem: %2d deg (%3d%%) ", angle, pct);
        lcd.print(buf);
      }
    }
  }

  // ───────────────────────────────────────────────────────────────────
  //  PHẦN 3: NHẬN PHÍM BẤM TEST TỪ SERIAL MONITOR
  // ───────────────────────────────────────────────────────────────────
  if (Serial.available() > 0) {
    char ch = Serial.read();
    if (ch == 'a' || ch == 'A') {
      Serial.println("\n[LENH SERIAL] -> BAT TAT CA 5 LED XANH");
      for (int i = 0; i < NUM_CHANNELS; i++) deviceState[i] = true;
      updateLEDs();
    }
    else if (ch == 'r' || ch == 'R') {
      Serial.println("\n[LENH SERIAL] -> BAT TAT CA 5 LED DO");
      for (int i = 0; i < NUM_CHANNELS; i++) deviceState[i] = false;
      updateLEDs();
    }
    else if (ch >= '1' && ch <= '5') {
      int idx = ch - '1';
      deviceState[idx] = !deviceState[idx];
      updateLEDs();
      Serial.printf("\n[LENH SERIAL] -> Dao trang thai thiet bi: %s\n", DEVICE_NAMES[idx]);
    }
  }

  delay(10);
}
