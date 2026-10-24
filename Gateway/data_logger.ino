/*
 * data_logger.ino
 * Lưu dữ liệu vào flash ESP32 (LittleFS) dạng CSV trên Gateway
 */

void loadGatewayConfig() {
  if (LittleFS.exists("/config.json")) {
    File f = LittleFS.open("/config.json", "r");
    if (f) {
      StaticJsonDocument<512> doc;
      DeserializationError err = deserializeJson(doc, f);
      if (!err) {
        if (doc.containsKey("fan_sp"))
          g_fanSetpoint = doc["fan_sp"];
        if (doc.containsKey("heater_sp"))
          g_heaterSetpoint = doc["heater_sp"];
        if (doc.containsKey("mist_th"))
          g_mistHumidThresh = doc["mist_th"];
        if (doc.containsKey("light_th"))
          g_lightThresh = doc["light_th"];
        if (doc.containsKey("pump_on"))
          g_pumpOnThresh = doc["pump_on"];
        if (doc.containsKey("pump_off"))
          g_pumpOffThresh = doc["pump_off"];
        if (doc.containsKey("v1_dry"))
          g_v1_dry = doc["v1_dry"];
        if (doc.containsKey("v1_wet"))
          g_v1_wet = doc["v1_wet"];
        if (doc.containsKey("v1_to"))
          g_v1_to = doc["v1_to"];
        if (doc.containsKey("v1_rest"))
          g_v1_rest = doc["v1_rest"];
        if (doc.containsKey("v2_dry"))
          g_v2_dry = doc["v2_dry"];
        if (doc.containsKey("v2_wet"))
          g_v2_wet = doc["v2_wet"];
        if (doc.containsKey("v2_to"))
          g_v2_to = doc["v2_to"];
        if (doc.containsKey("v2_rest"))
          g_v2_rest = doc["v2_rest"];
        Serial.printf("[FS] Gateway da load cau hinh Flash: FanSP=%.1f MistTH=%.0f LightTH=%.0f V1Dry=%.0f V2Dry=%.0f\n",
                      g_fanSetpoint, g_mistHumidThresh, g_lightThresh, g_v1_dry, g_v2_dry);
      }
      f.close();
    }
  }
}

void saveGatewayConfig() {
  File f = LittleFS.open("/config.json", "w");
  if (f) {
    StaticJsonDocument<512> doc;
    doc["fan_sp"] = g_fanSetpoint;
    doc["heater_sp"] = g_heaterSetpoint;
    doc["mist_th"] = g_mistHumidThresh;
    doc["light_th"] = g_lightThresh;
    doc["pump_on"] = g_pumpOnThresh;
    doc["pump_off"] = g_pumpOffThresh;
    doc["v1_dry"] = g_v1_dry;
    doc["v1_wet"] = g_v1_wet;
    doc["v1_to"] = g_v1_to;
    doc["v1_rest"] = g_v1_rest;
    doc["v2_dry"] = g_v2_dry;
    doc["v2_wet"] = g_v2_wet;
    doc["v2_to"] = g_v2_to;
    doc["v2_rest"] = g_v2_rest;
    serializeJson(doc, f);
    f.close();
    Serial.println(F("[FS] Gateway da luu tat ca cau hinh nguong vao Flash (/config.json)"));
  }
}

void initDataLogger() {
  if (!LittleFS.begin(true)) {
    Serial.println("[FS] LittleFS MOUNT FAILED!");
    return;
  }
  loadGatewayConfig();

  // Khôi phục g_totalRecords bằng cách đếm dòng
  if (LittleFS.exists(CSV_FILE)) {
    File f = LittleFS.open(CSV_FILE, "r");
    g_totalRecords = 0;
    while (f.available()) {
      if (f.read() == '\n') g_totalRecords++;
    }
    if (g_totalRecords > 0) g_totalRecords--; // Trừ dòng header
    f.close();
    Serial.printf("[FS] File CSV đã có: %lu bản ghi\n", g_totalRecords);
  } else {
    File f = LittleFS.open(CSV_FILE, "w");
    if (f) {
      f.println(F("timestamp,temperature,humidity,light,soil,fan_mode,fan_speed,pump,temp_ext,humid_ext"));
      f.close();
      Serial.println("[FS] Tạo file CSV mới ✓");
    }
  }
}

void trimFile() {
  File f = LittleFS.open(CSV_FILE, "r");
  if (!f) return;

  String header = f.readStringUntil('\n');
  size_t half = f.size() / 2;
  f.seek(half);
  f.readStringUntil('\n');

  File temp = LittleFS.open("/temp.csv", "w");
  temp.println(header);
  
  char buf[512];
  while (f.available()) {
    size_t n = f.readBytes(buf, sizeof(buf));
    temp.write((const uint8_t*)buf, n);
  }
  
  f.close();
  temp.close();

  LittleFS.remove(CSV_FILE);
  LittleFS.rename("/temp.csv", CSV_FILE);

  initDataLogger();
}

void saveToCSV() {
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

  char timeStr[25];
  getTimestamp(timeStr, sizeof(timeStr));

  File f = LittleFS.open(CSV_FILE, "a");
  if (f) {
    f.printf("%s,%.1f,%.1f,%.0f,%.1f,%d,%d,%d,%.1f,%.1f\n",
             timeStr, g_temperature, g_humidity, g_lightLux, g_soilMoisture,
             g_fanMode, g_fanSpeed, g_pumpRunning ? 1 : 0, g_tempExt, g_humidExt);
    f.close();
    g_totalRecords++;
    Serial.printf("[FS] Saved #%lu  T=%.1f H=%.1f Te=%.1f He=%.1f\n",
                  g_totalRecords, g_temperature, g_humidity, g_tempExt, g_humidExt);
  } else {
    Serial.println("[FS] Ghi CSV thất bại!");
  }
}
