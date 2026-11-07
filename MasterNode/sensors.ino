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

  // ─ Cảm biến độ ẩm đất ─
  // Master Node không đo ADC đất trực tiếp; độ ẩm đất Luống 1 & Luống 2
  // được nhận không dây từ Sensor Node 1 & Sensor Node 2 qua LoRa E32.
  Serial.println(F("[SOIL WSN] Van 1 & Van 2 nhan du lieu khong day tu Sensor Node 1 & 2"));
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
// ═══════════════════════════════════════════════════════════════
//  THUẬT TOÁN DATA FUSION & CHỊU LỖI CHO VI KHÍ HẬU VÀ ĐẤT
//  - Tổng hợp 3 cảm biến SHT30 bên trong nhà: Master Local, Node 1, Node 2
//  - Tự động phát hiện Timeout (>35s) của từng Sensor Node
//  - Cập nhật độ ẩm đất cho 2 Luống tưới từ 2 Node không dây
// ═══════════════════════════════════════════════════════════════
void updateMicroclimateFusion() {
  unsigned long now = millis();

  // 1. Kiểm tra timeout cho từng Sensor Node
  for (int i = 0; i < 2; i++) {
    if (g_sensorNode[i].isOnline && (now - g_sensorNode[i].lastSeen > NODE_TIMEOUT_MS)) {
      g_sensorNode[i].isOnline = false;
      Serial.printf("[WSN WARN] Sensor Node %d mat ket noi (TIMEOUT > 35s)!\n", i + 1);
    }
  }

  // 2. Cập nhật độ ẩm đất cho 2 luống tưới
  // Node 1 -> Van 1 (Luống 1)
  if (g_sensorNode[0].isOnline && !isnan(g_sensorNode[0].soilMoisture)) {
    g_valves[0].currentSoil = g_sensorNode[0].soilMoisture;
    g_valves[0].sensorError = false;
    g_soilMoisture = g_sensorNode[0].soilMoisture;
  } else {
    g_valves[0].sensorError = true;
    g_valves[0].currentSoil = NAN;
    g_soilMoisture = NAN;
  }

  // Node 2 -> Van 2 (Luống 2)
  if (g_sensorNode[1].isOnline && !isnan(g_sensorNode[1].soilMoisture)) {
    g_valves[1].currentSoil = g_sensorNode[1].soilMoisture;
    g_valves[1].sensorError = false;
    g_soilMoisture2 = g_sensorNode[1].soilMoisture;
  } else {
    g_valves[1].sensorError = true;
    g_valves[1].currentSoil = NAN;
    g_soilMoisture2 = NAN;
  }

  // 3. Tính toán trung bình thích nghi nhiệt độ & độ ẩm không khí bên trong từ 3 nguồn
  float sumTemp = 0.0f;
  float sumHumid = 0.0f;
  int validCount = 0;

  // Nguồn 1: SHT30 Trong Nhà tại Master
  if (g_sht30OK && !isnan(g_tempInLocal) && g_tempInLocal >= TEMP_MIN && g_tempInLocal <= TEMP_MAX &&
      !isnan(g_humidInLocal) && g_humidInLocal >= HUMID_MIN && g_humidInLocal <= HUMID_MAX) {
    sumTemp += g_tempInLocal;
    sumHumid += g_humidInLocal;
    validCount++;
  }

  // Nguồn 2: SHT30 tại Sensor Node 1
  if (g_sensorNode[0].isOnline && !g_sensorNode[0].shtError &&
      !isnan(g_sensorNode[0].temperature) && g_sensorNode[0].temperature >= TEMP_MIN && g_sensorNode[0].temperature <= TEMP_MAX &&
      !isnan(g_sensorNode[0].humidity) && g_sensorNode[0].humidity >= HUMID_MIN && g_sensorNode[0].humidity <= HUMID_MAX) {
    sumTemp += g_sensorNode[0].temperature;
    sumHumid += g_sensorNode[0].humidity;
    validCount++;
  }

  // Nguồn 3: SHT30 tại Sensor Node 2
  if (g_sensorNode[1].isOnline && !g_sensorNode[1].shtError &&
      !isnan(g_sensorNode[1].temperature) && g_sensorNode[1].temperature >= TEMP_MIN && g_sensorNode[1].temperature <= TEMP_MAX &&
      !isnan(g_sensorNode[1].humidity) && g_sensorNode[1].humidity >= HUMID_MIN && g_sensorNode[1].humidity <= HUMID_MAX) {
    sumTemp += g_sensorNode[1].temperature;
    sumHumid += g_sensorNode[1].humidity;
    validCount++;
  }

  // Gán giá trị trung bình đại diện toàn bộ nhà màng
  if (validCount > 0) {
    g_temperature = sumTemp / validCount;
    g_humidity = sumHumid / validCount;
  } else {
    g_temperature = NAN;
    g_humidity = NAN;
  }
}

// ═══════════════════════════════════════════════════════════════
//  ĐỌC TẤT CẢ CẢM BIẾN TẠI CHỖ & TỔNG HỢP WSN
// ═══════════════════════════════════════════════════════════════
void readSensors() {
  // ─ 1. SHT30 Trong Nhà #1 (I2C 0: SDA 21, SCL 22) ─
  static unsigned long lastInRetry = 0;
  static unsigned long lastHeaterDry = 0;

  if (g_sht30OK) {
    float t = sht30.readTemperature();
    float h = sht30.readHumidity();
    bool tOK = !isnan(t) && t > -44.0f && t >= TEMP_MIN && t <= TEMP_MAX;
    bool hOK = !isnan(h) && h >= HUMID_MIN && h <= HUMID_MAX;

    if (tOK && hOK) {
      g_tempInLocal = t;
      g_humidInLocal = h;
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
        sht30.heater(true);
        delay(40);
        sht30.heater(false);
      } else if (g_sensorErrors >= MAX_SENSOR_ERRORS) {
        Serial.println(F("[SHT30 IN] Loi doc lien tiep -> Reset I2C Bus & Re-init"));
        recoverI2CBus(I2C_SDA, I2C_SCL, Wire);
        g_sht30OK = sht30.begin(0x44) || sht30.begin(0x45);
        g_sensorErrors = 0;
        if (!g_sht30OK) {
          g_tempInLocal = NAN;
          g_humidInLocal = NAN;
        }
      }
    }
  } else {
    if (millis() - lastInRetry >= 5000) {
      lastInRetry = millis();
      recoverI2CBus(I2C_SDA, I2C_SCL, Wire);
      if (sht30.begin(0x44) || sht30.begin(0x45)) {
        sht30.heater(false);
        g_sht30OK = true;
        g_sensorErrors = 0;
        Serial.println(F("[SHT30 IN OK] Da phuc hoi ket noi thanh cong!"));
      } else {
        g_tempInLocal = NAN;
        g_humidInLocal = NAN;
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

  // ─ 4. Thực thi Data Fusion tổng hợp từ 3 SHT30 & 2 Sensor Node ─
  updateMicroclimateFusion();

  // ─ In định kỳ 10 giây tóm tắt 1 dòng duy nhất, không spam Serial ─
  static unsigned long lastLogPrint = 0;
  unsigned long now = millis();
  if (now - lastLogPrint >= 10000) {
    lastLogPrint = now;
    Serial.printf("[WSN STATUS] T_avg: %s | Out: %s | Lux: %s | V1(N1): %s | V2(N2): %s | N1:%s N2:%s\n",
                  isnan(g_temperature) ? "ERR" : (String(g_temperature, 1) + "*C/" + String(g_humidity, 1) + "%").c_str(),
                  isnan(g_tempExt) ? "ERR" : (String(g_tempExt, 1) + "*C/" + String(g_humidExt, 1) + "%").c_str(),
                  isnan(g_lightLux) ? "ERR" : (String(g_lightLux, 0) + " lx").c_str(),
                  isnan(g_valves[0].currentSoil) ? "LOI" : (String(g_valves[0].currentSoil, 0) + "%").c_str(),
                  isnan(g_valves[1].currentSoil) ? "LOI" : (String(g_valves[1].currentSoil, 0) + "%").c_str(),
                  g_sensorNode[0].isOnline ? "ON" : "OFF",
                  g_sensorNode[1].isOnline ? "ON" : "OFF");
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
