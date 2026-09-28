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
  webServer.on("/api/control", HTTP_POST, handleApiControl);
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
  if (webServer.hasArg("plain") == false) {
    webServer.send(400, "application/json", "{\"status\":\"error\",\"msg\":\"No payload\"}");
    return;
  }

  String body = webServer.arg("plain");
  StaticJsonDocument<256> reqDoc;
  DeserializationError err = deserializeJson(reqDoc, body);

  if (err) {
    webServer.send(400, "application/json", "{\"status\":\"error\",\"msg\":\"JSON parse error\"}");
    return;
  }

  // Chuyển lệnh từ Web -> Gateway -> JSON -> gửi qua LoRa -> Node
  StaticJsonDocument<256> loraDoc;
  
  if (reqDoc.containsKey("action")) {
    String action = reqDoc["action"].as<String>();
    
    // -- Bơm --
    if (action == "pump_on") {
      loraDoc["cmd"] = "pump_on";
    } 
    else if (action == "pump_off") {
      loraDoc["cmd"] = "pump_off";
    } 
    else if (action == "pump_auto") {
      loraDoc["cmd"] = "pump_auto";
    }
    
    // -- Quạt --
    else if (action == "fan_mode") {
      loraDoc["cmd"] = "fan";
      loraDoc["mode"] = reqDoc["mode"].as<int>();
    }
    else if (action == "fan_speed") {
      loraDoc["cmd"] = "fan";
      loraDoc["speed"] = reqDoc["speed"].as<int>();
    }
    else if (action == "fan_setpoint") {
      loraDoc["cmd"] = "fan";
      loraDoc["setpoint"] = reqDoc["setpoint"].as<float>();
    }
    
    // -- Cấu hình Bơm --
    else if (action == "config_pump") {
      loraDoc["cmd"] = "config";
      loraDoc["pump_on"] = reqDoc["pump_on"].as<float>();
      loraDoc["pump_off"] = reqDoc["pump_off"].as<float>();
    }
  }

  // Gửi lệnh qua LoRa nếu có
  if (loraDoc.containsKey("cmd")) {
    String loraStr;
    serializeJson(loraDoc, loraStr);
    sendLoRaCommand(loraStr);
    webServer.send(200, "application/json", "{\"status\":\"success\"}");
  } else {
    webServer.send(400, "application/json", "{\"status\":\"error\",\"msg\":\"Unknown action\"}");
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
