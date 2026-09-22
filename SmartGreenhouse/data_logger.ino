/*
 * data_logger.ino
 * Lưu dữ liệu cảm biến vào flash ESP32 (LittleFS) dạng CSV.
 * Tương thích với code Collect_data.ino cũ.
 *
 * CSV Format:
 *   timestamp,temperature,humidity,light,soil,fan_mode,fan_speed,pump
 */

// ═══════════════════════════════════════════════════════════════
//  KHỞI TẠO DATA LOGGER
// ═══════════════════════════════════════════════════════════════
void initDataLogger() {
  if (!LittleFS.begin(true)) {  // true = format nếu mount fail
    Serial.println("[FS] LittleFS MOUNT FAILED!");
    return;
  }

  checkStorage();

  // Tạo file CSV với header nếu chưa tồn tại
  if (!LittleFS.exists(CSV_FILE)) {
    File f = LittleFS.open(CSV_FILE, "w");
    if (f) {
      f.println(F("timestamp,temperature,humidity,light,soil,fan_mode,fan_speed,pump"));
      f.close();
      Serial.println("[FS] Tạo file CSV mới ✓");
    } else {
      Serial.println("[FS] Không thể tạo file CSV!");
    }
    g_totalRecords = 0;
  } else {
    g_totalRecords = countRecords();
    Serial.printf("[FS] File CSV đã có: %lu bản ghi\n", g_totalRecords);
  }
}

// ═══════════════════════════════════════════════════════════════
//  LƯU DỮ LIỆU VÀO CSV
// ═══════════════════════════════════════════════════════════════
void saveToCSV() {
  // Chỉ lưu khi có dữ liệu hợp lệ
  if (!g_sht30OK || isnan(g_temperature) || isnan(g_humidity)) {
    Serial.println("[FS] Bỏ qua lưu CSV: SHT30 lỗi");
    return;
  }

  // Kiểm tra dung lượng
  if (LittleFS.exists(CSV_FILE)) {
    File f = LittleFS.open(CSV_FILE, "r");
    if (f) {
      size_t sz = f.size();
      f.close();
      if (sz >= MAX_FILE_SIZE) {
        Serial.println("[FS] File đầy! Cắt bớt dữ liệu cũ...");
        trimFile();
      }
    }
  }

  // Lấy timestamp
  char timeStr[25];
  getTimestamp(timeStr, sizeof(timeStr));

  // Làm tròn dữ liệu
  float t = round(g_temperature  * 10) / 10.0f;
  float h = round(g_humidity     * 10) / 10.0f;
  float l = round(g_lightLux);
  float s = round(g_soilMoisture * 10) / 10.0f;

  // Ghi dòng CSV
  File f = LittleFS.open(CSV_FILE, "a");
  if (f) {
    f.printf("%s,%.1f,%.1f,%.0f,%.1f,%d,%d,%d\n",
             timeStr, t, h, l, s,
             g_fanMode, g_fanSpeed, g_pumpRunning ? 1 : 0);
    f.close();
    g_totalRecords++;
    Serial.printf("[FS] Saved #%lu  T=%.1f H=%.1f Lux=%.0f Soil=%.0f\n",
                  g_totalRecords, t, h, l, s);
  } else {
    Serial.println("[FS] Ghi CSV thất bại!");
  }
}

// ═══════════════════════════════════════════════════════════════
//  CẮT BỚT FILE (Giữ nửa sau - dữ liệu mới)
// ═══════════════════════════════════════════════════════════════
void trimFile() {
  File f = LittleFS.open(CSV_FILE, "r");
  if (!f) return;

  // Đọc và lưu header
  String header = f.readStringUntil('\n');

  // Nhảy đến nửa file
  size_t half = f.size() / 2;
  f.seek(half);
  f.readStringUntil('\n');  // Bỏ dòng bị cắt đứt

  // Đọc phần còn lại
  String remaining = f.readString();
  f.close();

  // Ghi lại file
  f = LittleFS.open(CSV_FILE, "w");
  if (f) {
    f.println(header);
    f.print(remaining);
    f.close();
    g_totalRecords = countRecords();
    Serial.printf("[FS] Đã cắt. Còn lại: %lu bản ghi\n", g_totalRecords);
  }
}

// ═══════════════════════════════════════════════════════════════
//  ĐẾM BẢN GHI
// ═══════════════════════════════════════════════════════════════
unsigned long countRecords() {
  if (!LittleFS.exists(CSV_FILE)) return 0;
  File f = LittleFS.open(CSV_FILE, "r");
  if (!f) return 0;

  unsigned long count = 0;
  f.readStringUntil('\n');  // Bỏ header
  while (f.available()) {
    f.readStringUntil('\n');
    count++;
  }
  f.close();
  return count;
}

// ═══════════════════════════════════════════════════════════════
//  KIỂM TRA DUNG LƯỢNG
// ═══════════════════════════════════════════════════════════════
void checkStorage() {
  size_t total = LittleFS.totalBytes();
  size_t used  = LittleFS.usedBytes();
  Serial.printf("[FS] Tổng: %d KB  Dùng: %d KB  Trống: %d KB\n",
                total / 1024, used / 1024, (total - used) / 1024);
}

// ═══════════════════════════════════════════════════════════════
//  LẤY KÍCH THƯỚC FILE CSV
// ═══════════════════════════════════════════════════════════════
size_t getCSVFileSize() {
  if (!LittleFS.exists(CSV_FILE)) return 0;
  File f = LittleFS.open(CSV_FILE, "r");
  if (!f) return 0;
  size_t sz = f.size();
  f.close();
  return sz;
}
