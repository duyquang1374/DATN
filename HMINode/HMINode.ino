#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

// --- UART ---
#define RX1_PIN 16
#define TX1_PIN 17

// --- I2C LCD ---
#define I2C_SDA 21
#define I2C_SCL 22
LiquidCrystal_I2C lcd(0x27, 20, 4);

// --- CẤU HÌNH CHÂN ---
// 0: Quạt, 1: Đèn, 2: Sưởi, 3: Phun sương, 4: Bơm (Đã chuyển chân Bơm sang 15 vì GPIO34 không có pull-up nội)
const int BTNS[5] = {13, 14, 27, 26, 15};
const int LED_G[5] = {32, 33, 25, 12, 19}; // Đang bật
const int LED_R[5] = {18, 5, 4, 2, 23};   // Đang tắt

const char* CMD_NAMES[5] = {
  "CMD:TOGGLE_FAN",
  "CMD:TOGGLE_LIGHT",
  "CMD:TOGGLE_HEATER",
  "CMD:TOGGLE_MIST",
  "CMD:TOGGLE_PUMP"
};

// Trạng thái debounce
unsigned long lastDebounceTime[5] = {0, 0, 0, 0, 0};
int lastButtonState[5] = {HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long debounceDelay = 50;

// Trạng thái thiết bị từ Master
int deviceStatus[5] = {0, 0, 0, 0, 0};

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);

  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HMI Node Starting...");

  for (int i = 0; i < 5; i++) {
    pinMode(BTNS[i], INPUT_PULLUP);
    pinMode(LED_G[i], OUTPUT);
    pinMode(LED_R[i], OUTPUT);
    // Khởi tạo trạng thái tắt cho tất cả
    digitalWrite(LED_G[i], LOW);
    digitalWrite(LED_R[i], HIGH);
  }
}

void loop() {
  // 1. Nhận dữ liệu từ Master
  if (Serial1.available()) {
    String msg = Serial1.readStringUntil('\n');
    msg.trim();
    if (msg.length() > 0) {
      parseMasterData(msg);
    }
  }

  // 2. Đọc nút nhấn
  for (int i = 0; i < 5; i++) {
    int reading = digitalRead(BTNS[i]);
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading == LOW && lastButtonState[i] == HIGH) { // Nhấn xuống
        Serial.println("Nhan nut: " + String(CMD_NAMES[i]));
        Serial1.println(CMD_NAMES[i]);
        
        // Cập nhật đèn trạng thái ngay lập tức để phản hồi nhanh
        deviceStatus[i] = !deviceStatus[i];
        updateLEDs();
      }
    }
    lastButtonState[i] = reading;
  }
}

void parseMasterData(String json) {
  StaticJsonDocument<512> doc;
  DeserializationError err = deserializeJson(doc, json);
  if (err) {
    Serial.println("JSON Parse Error: " + String(err.c_str()));
    Serial.println("Raw Data: " + json);
    return;
  }

  float t1 = doc["t1"];
  float h1 = doc["h1"];
  float t2 = doc["t2"];
  float h2 = doc["h2"];
  float lux = doc["lux"];

  if (doc.containsKey("st")) {
    for (int i = 0; i < 5; i++) {
      deviceStatus[i] = doc["st"][i];
    }
    updateLEDs();
  }

  updateLCD(t1, h1, t2, h2, lux);
}

void updateLEDs() {
  for (int i = 0; i < 5; i++) {
    if (deviceStatus[i] == 1) {
      digitalWrite(LED_G[i], HIGH);
      digitalWrite(LED_R[i], LOW);
    } else {
      digitalWrite(LED_G[i], LOW);
      digitalWrite(LED_R[i], HIGH);
    }
  }
}

void updateLCD(float t1, float h1, float t2, float h2, float lux) {
  char buf[21];
  
  // Dong 1: In 1
  lcd.setCursor(0, 0);
  snprintf(buf, sizeof(buf), "In:  %.1fC %.1f%%  ", t1, h1);
  lcd.print(buf);

  // Dong 2: Out 2
  lcd.setCursor(0, 1);
  snprintf(buf, sizeof(buf), "Out: %.1fC %.1f%%  ", t2, h2);
  lcd.print(buf);

  // Dong 3: Lux
  lcd.setCursor(0, 2);
  snprintf(buf, sizeof(buf), "Lux: %.0f        ", lux);
  lcd.print(buf);

  // Dong 4: LoRa/Status
  lcd.setCursor(0, 3);
  lcd.print("System Status: OK   ");
}
