// ═════════════════════════════════════════════════════════════
//  SMART GREENHOUSE - THU THẬP & XỬ LÝ DỮ LIỆU CẢM BIẾN (SENSORS)
//  - SHT30 Trong nhà (I2C0: SDA 21, SCL 22 - Địa chỉ 0x44)
//  - SHT30 Ngoài trời (I2C1: SDA 5, SCL 23 - Địa chỉ 0x44)
//  - BH1750 Ánh sáng (I2C0: SDA 21, SCL 22 - Địa chỉ 0x23)
//  - Cảm biến độ ẩm đất điện dung Capacitive v1.2 (2 Kênh độc lập):
//    + Kênh 1: GPIO 34 (ADC1_CH6, Header H23) + Tụ gốm C2 1µF xuống GND
//    + Kênh 2: GPIO 35 (ADC1_CH7, Header H28) + Tụ gốm C1 1µF xuống GND
//    + Lọc trung bình 10 mẫu (chu kỳ nạp tụ 5ms) chống răng cưa tín hiệu
//    + Phát hiện lỗi hở mạch (Raw > 4050) và chập mass (Raw < 80)
//  - Tích hợp hàm quét I2C Scanner hỗ trợ gỡ lỗi phần cứng
// ═════════════════════════════════════════════════════════════

/*
 * sensors.ino
 * Đọc cảm biến: SHT30 (nhiệt độ, độ ẩm), BH1750 (ánh sáng),
 * cảm biến độ ẩm đất (analog).
 *
 * ─── CALIBRATE CẢM BIẾN ĐẤT ─────────────────────────────────
 *  Bật SOIL_CALIBRATION_MODE = 1 trong config.h
 *  → Serial Monitor sẽ in ra giá trị ADC RAW mỗi 2 giây
 *
 *  Bước 1: Đặt cảm biến trong KHÔNG KHÍ (hoặc đất khô nhất)
 *          Ghi lại giá trị ADC raw → đặt SOIL_DRY_VALUE
 *  Bước 2: Nhúng cảm biến vào NƯỚC (hoặc đất ướt nhất)
 *          Ghi lại giá trị ADC raw → đặt SOIL_WET_VALUE
 *  Bước 3: Tắt SOIL_CALIBRATION_MODE = 0
 * ─────────────────────────────────────────────────────────────
 */

// Lưu ADC raw gần nhất để debug (được khai báo trong SmartGreenhouse.ino)
extern int g_soilRawADC;

// ═══════════════════════════════════════════════════════════════
//  KHỞI TẠO CẢM BIẾN
// ═══════════════════════════════════════════════════════════════
void initSensors() {
  I2C_Ext.begin(I2C1_SDA, I2C1_SCL);
  I2C_Ext.setTimeOut(50); // Chống treo I2C1

  // ─ SHT30 Trong Nhà (I2C 0) ─
  if (sht30.begin(0x44)) {
    sht30.heater(false);
    g_sht30OK = true;
    Serial.println(F("[SHT30 IN] Init OK"));
  } else {
    g_sht30OK = false;
    Serial.println(F("[SHT30 IN] FAILED!"));
  }

  // ─ SHT30 Ngoài Trời (I2C 1, dùng I2C_Ext, địa chỉ 0x44) ─
  if (sht30_ext.begin(0x44)) {
    sht30_ext.heater(false);
    g_sht30ExtOK = true;
    Serial.println(F("[SHT30 OUT] Init OK"));
  } else {
    g_sht30ExtOK = false;
    Serial.println(F("[SHT30 OUT] FAILED!"));
  }

  // SHT30 Warm-up: cảm biến cần ~0.5s để ổn định sau khi cấp nguồn.
  // Lần đọc đầu tiên thường trả về -45°C (giá trị raw=0x0000 = lỗi CRC)
  // Giải pháp: đọc dummy lần đầu và bỏ qua
  if (g_sht30OK) {
    delay(500); // Đợi cảm biến khởi động
    sht30.readTemperature();
    sht30.readHumidity();
    Serial.println(F("[SHT30 IN] Warm-up OK"));
  }
  if (g_sht30ExtOK) {
    sht30_ext.readTemperature();
    sht30_ext.readHumidity();
    Serial.println(F("[SHT30 OUT] Warm-up OK"));
  }

  // ─ BH1750 ─
  // Địa chỉ I2C: 0x23 (ADDR=GND, mặc định) hoặc 0x5C (ADDR=VCC)
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    g_bh1750OK = true;
    Serial.println(F("[BH1750] Init OK  (CONTINUOUS_HIGH_RES_MODE)"));
  } else {
    g_bh1750OK = false;
    Serial.println(F("[BH1750] FAILED! Kiem tra ket noi I2C."));
  }

  // ─ Cảm biến độ ẩm đất (ADC1: GPIO 34 & GPIO 35) ─
  // Cả GPIO 34 và GPIO 35 đều là input-only trên ESP32 (không xung đột WiFi)
  analogReadResolution(12);       // 12-bit ADC: 0 – 4095
  analogSetAttenuation(ADC_11db); // Dải đo: 0 – 3.3V (full range)

  Serial.printf("[SOIL]   Khoi tao 2 cam bien: Van 1 (Pin %d), Van 2 (Pin %d)\n",
                SOIL1_PIN, SOIL2_PIN);

#if SOIL_CALIBRATION_MODE
  Serial.println();
  Serial.println(F("╔══════════════════════════════════════╗"));
  Serial.println(F("║  CHE DO CALIBRATE CAM BIEN DAT       ║"));
  Serial.println(F("║  Xem ADC RAW trong Serial Monitor    ║"));
  Serial.println(F("║  Dat cam bien KHO  -> ghi SOIL_DRY   ║"));
  Serial.println(F("║  Nhung vao NUOC   -> ghi SOIL_WET    ║"));
  Serial.println(F("╚══════════════════════════════════════╝"));
#endif
}

// ═══════════════════════════════════════════════════════════════
//  ĐỌC ĐỘ ẨM ĐẤT (ADC) CHO TỪNG KÊNH CẢM BIẾN
//  Phát hiện lỗi hở mạch/chập mạch, tính toán % theo calib riêng
static bool s_lastSoilFault1 = false;
static bool s_lastSoilFault2 = false;

float readSoilChannel(int pin, int dryVal, int wetVal, int &outRaw, bool &outError) {
  long sum = 0;
  for (int i = 0; i < SOIL_SAMPLES; i++) {
    sum += analogRead(pin);
    delay(5); // Nghỉ 5ms để nạp tụ ADC
  }
  int raw = (int)(sum / SOIL_SAMPLES);
  outRaw = raw;

  bool &lastFault = (pin == SOIL2_PIN) ? s_lastSoilFault2 : s_lastSoilFault1;

  // Kiểm tra lỗi cảm biến (đứt dây nguồn hoặc chập mass)
  if (raw < SOIL_RAW_FAULT_LOW || raw > SOIL_RAW_FAULT_HIGH) {
    outError = true;
    if (!lastFault) {
      lastFault = true;
      Serial.printf("[SOIL] Pin %d: Mat tin hieu hoac cham mass (Raw=%d < %d)\n",
                    pin, raw, SOIL_RAW_FAULT_LOW);
    }
    return NAN;
  }
  
  if (lastFault) {
    lastFault = false;
    Serial.printf("[SOIL OK] Pin %d: Da ket noi lai binh thuong (Raw=%d)\n", pin, raw);
  }
  outError = false;

  // Tính % bằng floating point (Capacitive v1.2: khô = Raw cao, ướt = Raw thấp)
  float span = (float)(dryVal - wetVal);
  if (fabsf(span) < 1.0f) span = 1.0f;

  float pct = ((float)(dryVal - raw) / span) * 100.0f;
  return constrain(pct, 0.0f, 100.0f);
}

float readSoilMoisture() {
  // 1. Đọc kênh 1 (Van 1 - GPIO 34)
  int raw1 = 0;
  bool err1 = false;
  float s1 = readSoilChannel(g_valves[0].soilPin, g_valves[0].dryRaw, g_valves[0].wetRaw, raw1, err1);
  g_valves[0].rawADC = raw1;
  g_valves[0].sensorError = err1;
  g_valves[0].currentSoil = s1;
  g_soilMoisture = s1; // Tương thích ngược

#if NUM_SOIL_SENSORS >= 2
  // 2. Đọc kênh 2 (Van 2 - GPIO 35)
  int raw2 = 0;
  bool err2 = false;
  float s2 = readSoilChannel(g_valves[1].soilPin, g_valves[1].dryRaw, g_valves[1].wetRaw, raw2, err2);
  g_valves[1].rawADC = raw2;
  g_valves[1].sensorError = err2;
  g_valves[1].currentSoil = s2;
  g_soilMoisture2 = s2;
#else
  // Chế độ 1 cảm biến: Van 2 dùng chung dữ liệu với Van 1 (không lo chân GPIO 35 thả nổi)
  g_valves[1].rawADC = raw1;
  g_valves[1].sensorError = err1;
  g_valves[1].currentSoil = s1;
  g_soilMoisture2 = s1;
#endif

  return s1;
}

// ═══════════════════════════════════════════════════════════════
// ═══════════════════════════════════════════════════════════════
//  HÀM GIẢI PHÓNG & PHỤC HỒI BUS I2C KHI BỊ TREO (I2C BUS RECOVERY)
//  Nhấp 9 xung clock SCL để giải phóng chân SDA bị slave giữ mức LOW
// ═══════════════════════════════════════════════════════════════
void recoverI2CBus(int sdaPin, int sclPin, TwoWire &wireBus) {
  wireBus.end();
  pinMode(sdaPin, INPUT_PULLUP);
  pinMode(sclPin, OUTPUT);

  // Nhấp 9 xung SCL để buộc slave giải phóng SDA
  for (int i = 0; i < 9; i++) {
    digitalWrite(sclPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sclPin, LOW);
    delayMicroseconds(10);
  }

  // Tạo xung STOP hợp lệ trên bus I2C
  pinMode(sdaPin, OUTPUT);
  digitalWrite(sdaPin, LOW);
  delayMicroseconds(10);
  digitalWrite(sclPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sdaPin, HIGH);
  delayMicroseconds(10);

  pinMode(sdaPin, INPUT);
  pinMode(sclPin, INPUT);

  wireBus.begin(sdaPin, sclPin);
  wireBus.setTimeOut(50);
}

// ═══════════════════════════════════════════════════════════════
//  ĐỌC TẤT CẢ CẢM BIẾN (Chạy ngầm mỗi 2s, in tóm tắt mỗi 10s chống spam)
//  Tích hợp cơ chế tự phục hồi bus I2C và sấy màng SHT30 chống đọng ẩm
// ═══════════════════════════════════════════════════════════════
void readSensors() {
  // ─ 1. SHT30 Trong Nhà (I2C 0: SDA 21, SCL 22) ─
  static unsigned long lastInRetry = 0;
  static unsigned long lastHeaterDry = 0;

  if (g_sht30OK) {
    float t = sht30.readTemperature();
    float h = sht30.readHumidity();
    bool tOK = !isnan(t) && t > -44.0f && t >= TEMP_MIN && t <= TEMP_MAX;
    bool hOK = !isnan(h) && h >= HUMID_MIN && h <= HUMID_MAX;

    if (tOK && hOK) {
      g_temperature = t;
      g_humidity = h;
      g_sensorErrors = 0;

      // Sấy nhẹ tự động: nếu ẩm trong nhà >= 92% (phun sương mạnh dễ đọng nước)
      if (h >= 92.0f && (millis() - lastHeaterDry > 180000)) {
        lastHeaterDry = millis();
        sht30.heater(true);
        delay(40);
        sht30.heater(false);
      }
    } else {
      g_sensorErrors++;
      if (g_sensorErrors == 2) {
        // Nhấp sấy nhanh 40ms nếu chớm có lỗi đọc do sương bám
        sht30.heater(true);
        delay(40);
        sht30.heater(false);
      } else if (g_sensorErrors >= MAX_SENSOR_ERRORS) {
        Serial.println(F("[SHT30 IN] Loi doc lien tiep -> Reset I2C Bus & Re-init"));
        recoverI2CBus(I2C_SDA, I2C_SCL, Wire);
        g_sht30OK = sht30.begin(0x44) || sht30.begin(0x45);
        g_sensorErrors = 0;
        if (!g_sht30OK) {
          g_temperature = NAN;
          g_humidity = NAN;
        }
      }
    }
  } else {
    // Tự động kết nối lại mỗi 5 giây nếu đang bị mất cảm biến
    if (millis() - lastInRetry >= 5000) {
      lastInRetry = millis();
      recoverI2CBus(I2C_SDA, I2C_SCL, Wire);
      if (sht30.begin(0x44) || sht30.begin(0x45)) {
        sht30.heater(false);
        g_sht30OK = true;
        g_sensorErrors = 0;
        Serial.println(F("[SHT30 IN OK] Da phuc hoi ket noi thanh cong!"));
      } else {
        g_temperature = NAN;
        g_humidity = NAN;
      }
    }
  }

  // ─ 2. SHT30 Ngoài Trời (I2C 1: SDA 5, SCL 23) ─
  static unsigned long lastExtRetry = 0;
  static int s_sensorExtErrors = 0;

  if (g_sht30ExtOK) {
    float t = sht30_ext.readTemperature();
    float h = sht30_ext.readHumidity();
    bool tOK = !isnan(t) && t > -44.0f && t >= TEMP_MIN && t <= TEMP_MAX;
    bool hOK = !isnan(h) && h >= HUMID_MIN && h <= HUMID_MAX;

    if (tOK && hOK) {
      g_tempExt = t;
      g_humidExt = h;
      s_sensorExtErrors = 0;
    } else {
      s_sensorExtErrors++;
      if (s_sensorExtErrors >= MAX_SENSOR_ERRORS) {
        Serial.println(F("[SHT30 OUT] Loi doc lien tiep -> Reset I2C1 Bus"));
        recoverI2CBus(I2C1_SDA, I2C1_SCL, I2C_Ext);
        g_sht30ExtOK = sht30_ext.begin(0x44);
        s_sensorExtErrors = 0;
        if (!g_sht30ExtOK) {
          g_tempExt = NAN;
          g_humidExt = NAN;
        }
      }
    }
  } else {
    // Tự động kết nối lại mỗi 5 giây
    if (millis() - lastExtRetry >= 5000) {
      lastExtRetry = millis();
      recoverI2CBus(I2C1_SDA, I2C1_SCL, I2C_Ext);
      if (sht30_ext.begin(0x44)) {
        sht30_ext.heater(false);
        g_sht30ExtOK = true;
        s_sensorExtErrors = 0;
        Serial.println(F("[SHT30 OUT OK] Da phuc hoi ket noi thanh cong!"));
      } else {
        g_tempExt = NAN;
        g_humidExt = NAN;
      }
    }
  }

  // ─ 3. BH1750 Ánh sáng (I2C 0) ─
  static unsigned long lastBhRetry = 0;
  if (g_bh1750OK) {
    float lux = lightMeter.readLightLevel();
    if (lux >= 0.0f && lux <= LUX_MAX) {
      g_lightLux = lux;
    } else {
      g_bh1750OK = false;
      g_lightLux = NAN;
    }
  } else {
    if (millis() - lastBhRetry >= 5000) {
      lastBhRetry = millis();
      if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        g_bh1750OK = true;
        Serial.println(F("[BH1750 OK] Da phuc hoi ket noi anh sang!"));
      } else {
        g_lightLux = NAN;
      }
    }
  }

  // ─ 4. Cảm biến độ ẩm đất 2 luống ─
  float soil = readSoilMoisture();
  if (!isnan(soil) && soil >= 0.0f) {
    g_soilMoisture = soil;
  } else {
    g_soilMoisture = NAN;
  }

  // ─ In định kỳ 10 giây tóm tắt 1 dòng duy nhất, không spam Serial ─
  static unsigned long lastLogPrint = 0;
  unsigned long now = millis();
  if (now - lastLogPrint >= 10000) {
    lastLogPrint = now;
    Serial.printf("[SENSOR] In: %s | Out: %s | Lux: %s | V1: %s (ADC:%d) | V2: %s (ADC:%d)\n",
                  isnan(g_temperature) ? "ERR" : (String(g_temperature, 1) + "*C/" + String(g_humidity, 1) + "%").c_str(),
                  isnan(g_tempExt) ? "ERR" : (String(g_tempExt, 1) + "*C/" + String(g_humidExt, 1) + "%").c_str(),
                  isnan(g_lightLux) ? "ERR" : (String(g_lightLux, 0) + " lx").c_str(),
                  isnan(g_valves[0].currentSoil) ? "LOI" : (String(g_valves[0].currentSoil, 0) + "%").c_str(), g_valves[0].rawADC,
                  isnan(g_valves[1].currentSoil) ? "LOI" : (String(g_valves[1].currentSoil, 0) + "%").c_str(), g_valves[1].rawADC);
  }
}

// ═══════════════════════════════════════════════════════════════
//  I2C SCANNER — Quét để tìm địa chỉ thiết bị
//  Gọi scanI2C() từ setup() khi nghi ngờ I2C không nhận thiết bị
// ═══════════════════════════════════════════════════════════════
void scanI2C() {
  Serial.println(F("[I2C] ===== Bat dau quet I2C ====="));
  int found = 0;
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("[I2C] Tim thay thiet bi tai 0x%02X", addr);
      if (addr == 0x44 || addr == 0x45)
        Serial.print(" <- SHT30/SHT31");
      else if (addr == 0x23 || addr == 0x5C)
        Serial.print(" <- BH1750");
      else if (addr == 0x27 || addr == 0x3F)
        Serial.print(" <- LCD I2C PCF8574");
      else if (addr == 0x3C || addr == 0x3D)
        Serial.print(" <- OLED SSD1306");
      Serial.println();
      found++;
    }
  }
  Serial.printf("[I2C] Ket qua: %d thiet bi.\n", found);
  if (found == 0) {
    Serial.println(F("[I2C] Khong tim thay! Kiem tra day SDA/SCL va nguon."));
  }
  Serial.println(F("[I2C] ===== Ket thuc quet ====="));
}
