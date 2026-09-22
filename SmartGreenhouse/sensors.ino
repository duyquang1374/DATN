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

// Lưu ADC raw gần nhất để debug
int g_soilRawADC = 0;

// ═══════════════════════════════════════════════════════════════
//  KHỞI TẠO CẢM BIẾN
// ═══════════════════════════════════════════════════════════════
void initSensors() {
  // ─ SHT30 ─
  // Địa chỉ I2C: 0x44 (chân ADDR=GND, mặc định) hoặc 0x45 (ADDR=VCC)
  if (sht30.begin(0x44)) {
    sht30.heater(false);
    g_sht30OK = true;
    Serial.println(F("[SHT30]  Init OK  (addr=0x44)"));
  } else if (sht30.begin(0x45)) {
    sht30.heater(false);
    g_sht30OK = true;
    Serial.println(F("[SHT30]  Init OK  (addr=0x45)"));
  } else {
    g_sht30OK = false;
    Serial.println(F("[SHT30]  FAILED! Kiem tra:"));
    Serial.println(F("  - Day SDA->21  SCL->22 dung chua?"));
    Serial.println(F("  - Nguon 3.3V du chua?"));
    Serial.println(F("  - Pull-up 4.7k tren SDA/SCL chua?"));
    Serial.println(F("  -> Goi scanI2C() de quet dia chi I2C"));
  }

  // SHT30 Warm-up: cảm biến cần ~0.5s để ổn định sau khi cấp nguồn.
  // Lần đọc đầu tiên thường trả về -45°C (giá trị raw=0x0000 = lỗi CRC)
  // Giải pháp: đọc dummy lần đầu và bỏ qua
  if (g_sht30OK) {
    delay(500);                  // Đợi cảm biến khởi động
    sht30.readTemperature();     // Dummy read — bỏ qua kết quả
    sht30.readHumidity();
    delay(100);
    Serial.println(F("[SHT30]  Warm-up OK"));
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

  // ─ Cảm biến độ ẩm đất (ADC) ─
  // GPIO34 là input-only trên ESP32, không dùng pinMode
  analogReadResolution(12);          // 12-bit ADC: 0 – 4095
  analogSetAttenuation(ADC_11db);    // Dải đo: 0 – 3.3V (full range)

  Serial.printf("[SOIL]   ADC pin=%d  12-bit (0-4095)  11dB (0-3.3V)\n",
                SOIL_PIN);
  Serial.printf("[SOIL]   Cau hinh: DRY=%d (kho=0%%)  WET=%d (uot=100%%)\n",
                SOIL_DRY_VALUE, SOIL_WET_VALUE);

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
//  ĐỌC ĐỘ ẨM ĐẤT (ADC)
//  Trả về phần trăm: 0% (khô) → 100% (ướt)
//  Trả về -1 khi đang ở Calibration Mode
// ═══════════════════════════════════════════════════════════════
float readSoilMoisture() {
  // Lấy trung bình SOIL_SAMPLES mẫu để lọc nhiễu ADC
  long sum = 0;
  for (int i = 0; i < SOIL_SAMPLES; i++) {
    sum += analogRead(SOIL_PIN);
    delay(10);  // Nghỉ để tụ ADC nạp lại hoàn toàn
  }
  g_soilRawADC = (int)(sum / SOIL_SAMPLES);

#if SOIL_CALIBRATION_MODE
  Serial.printf("[CAL] Soil ADC raw = %4d  (DRY=%d WET=%d)\n",
                g_soilRawADC, SOIL_DRY_VALUE, SOIL_WET_VALUE);
  return -1.0f;  // Báo hiệu calibration mode
#endif

  // Tính phần trăm bằng floating point (chính xác hơn integer map())
  // Cảm biến điện trở: đất KHÔ → điện trở cao → áp ADC CAO
  //   % = (DRY - raw) / (DRY - WET) * 100
  // Cảm biến capacitive: đất KHÔ → ADC THẤP → đổi SOIL_DRY < SOIL_WET
  float span = (float)(SOIL_DRY_VALUE - SOIL_WET_VALUE);
  if (fabsf(span) < 1.0f) return 50.0f;  // Tránh chia cho 0

  float pct = ((float)(SOIL_DRY_VALUE - g_soilRawADC) / span) * 100.0f;
  return constrain(pct, 0.0f, 100.0f);
}

// ═══════════════════════════════════════════════════════════════
//  ĐỌC TẤT CẢ CẢM BIẾN
// ═══════════════════════════════════════════════════════════════
void readSensors() {
  Serial.println(F("──────────── Sensor Reading ────────────"));

  // ─ SHT30 ─
  if (g_sht30OK) {
    float t = sht30.readTemperature();
    float h = sht30.readHumidity();

    // -45.0 = raw 0x0000 = loi CRC cua SHT30, phai reject (> -44 de chac)
    bool tOK = !isnan(t) && t > -44.0f && t >= TEMP_MIN && t <= TEMP_MAX;
    bool hOK = !isnan(h) && h >= HUMID_MIN && h <= HUMID_MAX;

    if (tOK && hOK) {
      g_temperature  = t;
      g_humidity     = h;
      g_sensorErrors = 0;
      Serial.printf("[SHT30]  T=%.2f*C  H=%.2f%%  OK\n", t, h);
    } else {
      g_sensorErrors++;
      Serial.printf("[SHT30]  LOI #%d: T=%s(%.2f)  H=%s(%.2f)\n",
                    g_sensorErrors,
                    tOK ? "OK" : "NG", isnan(t) ? -999.0f : t,
                    hOK ? "OK" : "NG", isnan(h) ? -999.0f : h);
      // Thử reinit nếu quá nhiều lỗi
      if (g_sensorErrors >= MAX_SENSOR_ERRORS) {
        Serial.println(F("[SHT30]  Thu reinit..."));
        g_sht30OK = sht30.begin(0x44) || sht30.begin(0x45);
        g_sensorErrors = 0;
      }
    }
  } else {
    // Thử reinit mỗi lần đọc nếu init ban đầu thất bại
    if (sht30.begin(0x44) || sht30.begin(0x45)) {
      sht30.heater(false);
      g_sht30OK = true;
      Serial.println(F("[SHT30]  Reinit thanh cong!"));
    } else {
      Serial.println(F("[SHT30]  OFFLINE"));
    }
  }

  // ─ BH1750 ─
  if (g_bh1750OK) {
    float lux = lightMeter.readLightLevel();
    if (lux >= 0.0f && lux <= LUX_MAX) {
      g_lightLux = lux;
      Serial.printf("[BH1750] Lux=%.1f lx  OK\n", lux);
    } else {
      Serial.printf("[BH1750] LOI: lux=%.1f (ngoai range)\n", lux);
    }
  } else {
    Serial.println(F("[BH1750] OFFLINE"));
  }

  // ─ Cảm biến độ ẩm đất ─
  float soil = readSoilMoisture();
  if (soil >= 0.0f) {
    g_soilMoisture = soil;
    const char* qual =
      soil < 20.0f ? "KHO KIET" :
      soil < 40.0f ? "KHO" :
      soil < 60.0f ? "BINH THUONG" :
      soil < 80.0f ? "AM" : "RAT AM";
    Serial.printf("[SOIL]   Dat=%.1f%%  ADC_raw=%d  [%s]\n",
                  soil, g_soilRawADC, qual);
  } else {
    g_soilMoisture = 50.0f;  // Placeholder khi đang calibrate
  }

  Serial.println(F("────────────────────────────────────────"));
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
      if      (addr == 0x44 || addr == 0x45) Serial.print(" <- SHT30/SHT31");
      else if (addr == 0x23 || addr == 0x5C) Serial.print(" <- BH1750");
      else if (addr == 0x27 || addr == 0x3F) Serial.print(" <- LCD I2C PCF8574");
      else if (addr == 0x3C || addr == 0x3D) Serial.print(" <- OLED SSD1306");
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
