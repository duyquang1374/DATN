/*
 * web_server_local.ino
 * Chạy WebServer trên Gateway để phục vụ UI
 */

void setupWebServer() {
  webServer.on("/", HTTP_GET, []() {
    webServer.send(200, "text/html", "<h1>Smart Greenhouse Gateway</h1><p>UI is hosted on Cloudflare.</p>");
  });

  // CORS headers
  webServer.onNotFound([]() {
    if (webServer.method() == HTTP_OPTIONS) {
      webServer.sendHeader("Access-Control-Allow-Origin", "*");
      webServer.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      webServer.sendHeader("Access-Control-Allow-Headers", "Content-Type");
      webServer.send(204);
    } else {
      webServer.send(404, "text/plain", "Not Found");
    }
  });

  webServer.on("/api/status", HTTP_GET, handleApiStatus);
  webServer.on("/api/control", HTTP_GET, handleApiControl);
  webServer.on("/api/forecast", HTTP_GET, handleApiForecast);
  webServer.on("/download", HTTP_GET, handleDownloadCSV);
  webServer.on("/delete_csv", HTTP_POST, handleDeleteCSV);

  webServer.begin();
  Serial.println("[WEB] Web Server started");
}

void setCorsHeaders() {
  webServer.sendHeader("Access-Control-Allow-Origin", "*");
}

void handleApiStatus() {
  StaticJsonDocument<512> doc;
  
  // Dữ liệu mới nhất lưu ở Gateway
  doc["temperature"]     = g_temperature;
  doc["humidity"]        = g_humidity;
  doc["light"]           = g_lightLux;
  doc["soil"]            = g_soilMoisture;
  
  doc["fan_mode"]        = g_fanMode;
  doc["fan_speed"]       = g_fanSpeed;
  doc["fan_pwm"]         = g_fanPWM;
  doc["fan_setpoint"]    = g_fanSetpoint;
  
  doc["pump_running"]    = g_pumpRunning;
  doc["pump_on"]         = g_pumpOnThresh;
  doc["pump_off"]        = g_pumpOffThresh;
  
  doc["sht30_ok"]        = g_sht30OK;
  doc["sht30_ext_ok"]    = g_sht30ExtOK;
  doc["temp_ext"]        = g_tempExt;
  doc["humid_ext"]       = g_humidExt;
  doc["bh1750_ok"]       = g_bh1750OK;
  
  doc["wifi_rssi"]       = g_wifiOK ? WiFi.RSSI() : 0;
  doc["ip"]              = g_wifiOK ? WiFi.localIP().toString() : "0.0.0.0";
  doc["uptime"]          = millis() / 1000; // Uptime của Gateway
  doc["node_uptime"]     = g_nodeUptime;    // Uptime của Node
  doc["records"]         = g_totalRecords;
  
  char timeStr[25];
  getTimestamp(timeStr, sizeof(timeStr));
  doc["timestamp"] = timeStr;

  char buf[512];
  serializeJson(doc, buf);

  setCorsHeaders();
  webServer.send(200, "application/json", buf);
}

void handleApiControl() {
  setCorsHeaders();

  StaticJsonDocument<256> loraDoc;
  bool hasCmd = false;

  // -- Bơm --
  if (webServer.hasArg("pump")) {
    String val = webServer.arg("pump");
    if (val == "on")   { loraDoc["cmd"] = "pump_on";   hasCmd = true; }
    else if (val == "off")  { loraDoc["cmd"] = "pump_off";  hasCmd = true; }
    else if (val == "auto") { loraDoc["cmd"] = "pump_auto"; hasCmd = true; }
  }

  // -- Quạt mode --
  if (webServer.hasArg("fan_mode")) {
    loraDoc["cmd"]  = "fan";
    loraDoc["mode"] = webServer.arg("fan_mode").toInt();
    hasCmd = true;
  }

  // -- Quạt speed (manual) --
  if (webServer.hasArg("fan_speed")) {
    loraDoc["cmd"]   = "fan";
    loraDoc["mode"]  = 2; // MANUAL
    loraDoc["speed"] = webServer.arg("fan_speed").toInt();
    hasCmd = true;
  }

  // -- Setpoint quạt Auto --
  if (webServer.hasArg("setpoint")) {
    loraDoc["cmd"]      = "fan";
    loraDoc["mode"]     = 1; // AUTO
    loraDoc["setpoint"] = webServer.arg("setpoint").toFloat();
    hasCmd = true;
  }

  // -- Cấu hình ngưỡng bơm --
  if (webServer.hasArg("pump_on") || webServer.hasArg("pump_off")) {
    loraDoc["cmd"] = "config";
    if (webServer.hasArg("pump_on"))  loraDoc["pump_on"]  = webServer.arg("pump_on").toFloat();
    if (webServer.hasArg("pump_off")) loraDoc["pump_off"] = webServer.arg("pump_off").toFloat();
    hasCmd = true;
  }

  if (hasCmd) {
    String loraStr;
    serializeJson(loraDoc, loraStr);
    sendLoRaCommand(loraStr);

    // ─── Cập nhật trạng thái Gateway ngay lập tức (không đợi Node xác nhận) ───
    // Điều này đảm bảo Web poll 1 giây sau sẽ nhận được state đúng
    String cmd = loraDoc["cmd"].as<String>();
    if (cmd == "pump_on")   { g_pumpRunning = true;  }
    if (cmd == "pump_off")  { g_pumpRunning = false; }
    if (cmd == "pump_auto") { /* giữ state hiện tại, để Node quyết định */ }
    if (cmd == "fan") {
      if (loraDoc.containsKey("mode"))     g_fanMode     = loraDoc["mode"];
      if (loraDoc.containsKey("speed"))    g_fanSpeed    = map((int)loraDoc["speed"], 0, 255, 0, 100);
      if (loraDoc.containsKey("setpoint")) g_fanSetpoint = loraDoc["setpoint"];
    }
    if (cmd == "config") {
      if (loraDoc.containsKey("pump_on"))  g_pumpOnThresh  = loraDoc["pump_on"];
      if (loraDoc.containsKey("pump_off")) g_pumpOffThresh = loraDoc["pump_off"];
    }

    webServer.send(200, "application/json", "{\"status\":\"success\"}");
  } else {
    webServer.send(400, "application/json", "{\"status\":\"error\",\"msg\":\"Unknown params\"}");
  }
}

void handleApiForecast() {
  StaticJsonDocument<512> doc;
  
  doc["ready"] = g_forecast_ready;
  if (g_forecast_ready) {
    JsonArray arrT = doc.createNestedArray("temp");
    JsonArray arrH = doc.createNestedArray("humid");
    JsonArray arrL = doc.createNestedArray("lux");
    JsonArray arrM = doc.createNestedArray("min");
    const int labels[] = {30, 60, 90};
    for (int h = 0; h < 3; h++) {
      arrT.add(isnan(g_fc_temp[h])  ? 0 : round(g_fc_temp[h]  * 10) / 10.0f);
      arrH.add(isnan(g_fc_humid[h]) ? 0 : round(g_fc_humid[h] * 10) / 10.0f);
      arrL.add(isnan(g_fc_lux[h])   ? 0 : (int)g_fc_lux[h]);
      arrM.add(labels[h]);
    }
  }

  char buf[512];
  serializeJson(doc, buf);
  setCorsHeaders();
  webServer.send(200, "application/json", buf);
}

void handleDownloadCSV() {
  setCorsHeaders();
  if (!LittleFS.exists(CSV_FILE)) {
    webServer.send(404, "text/plain", "File không tồn tại");
    return;
  }
  
  File f = LittleFS.open(CSV_FILE, "r");
  if (!f) {
    webServer.send(500, "text/plain", "Không thể mở file");
    return;
  }

  webServer.sendHeader("Content-Disposition", "attachment; filename=data.csv");
  webServer.streamFile(f, "text/csv");
  f.close();
}

void handleDeleteCSV() {
  setCorsHeaders();
  if (LittleFS.exists(CSV_FILE)) {
    LittleFS.remove(CSV_FILE);
    Serial.println("[FS] Đã xóa file CSV theo yêu cầu Web.");
    // Tạo lại file với header
    initDataLogger();
    webServer.send(200, "application/json", "{\"status\":\"success\"}");
  } else {
    webServer.send(404, "application/json", "{\"status\":\"error\",\"msg\":\"File không tồn tại\"}");
  }
}
