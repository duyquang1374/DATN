// ═════════════════════════════════════════════════════════════
//  GATEWAY WEBSERVER REST API - PHỤC VỤ CLOUDFLARE DASHBOARD
//  Các Endpoint chính:
//  - GET  /api/status   : Trả về JSON toàn bộ cảm biến, hệ thống tưới & thiết
//  bị
//  - GET  /api/control  : Nhận lệnh điều khiển từ Web (Van, Bơm, Quạt, Đèn,
//  Sưởi, Rèm)
//  - GET  /api/forecast : Trả về dữ liệu dự báo 3 bước thời gian từ TinyML
//  - GET  /download     : Tải xuống file dữ liệu lịch sử data.csv
//  - POST /delete_csv   : Xóa file dữ liệu lịch sử data.csv
// ═════════════════════════════════════════════════════════════

void setupWebServer() {
  webServer.on("/", HTTP_GET, []() {
    webServer.send(
        200, "text/html",
        "<h1>Smart Greenhouse Gateway</h1><p>UI is hosted on Cloudflare.</p>");
  });

  // CORS headers
  webServer.onNotFound([]() {
    webServer.sendHeader("Connection", "close");
    if (webServer.method() == HTTP_OPTIONS) {
      webServer.sendHeader("Access-Control-Allow-Origin", "*");
      webServer.sendHeader("Access-Control-Allow-Methods",
                           "GET, POST, OPTIONS");
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
  webServer.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  webServer.sendHeader("Access-Control-Allow-Headers", "*");
  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");
  webServer.sendHeader("Connection", "close");
}

void handleApiStatus() {
  StaticJsonDocument<1536> doc;

  // Dữ liệu mới nhất lưu ở Gateway
  doc["temperature"] = g_temperature;
  doc["humidity"] = g_humidity;
  doc["light"] = g_lightLux;
  doc["soil"] = g_soilMoisture;
  doc["soil2"] = g_soilMoisture2;

  // Bơm & An toàn
  doc["pump_running"] = g_pumpRunning;
  doc["pump_enabled"] = g_pumpEnabled;
  doc["pump_emergency"] = g_pumpEmergencyStop;
  doc["pump_time"] = g_pumpTime;
  doc["pump_seq"] = g_pumpSeq;

  // Van 1
  doc["v1_run"] = g_v1_run;
  doc["v1_mode"] = g_v1_mode;
  doc["v1_soil"] = g_v1_soil;
  doc["v1_dry"] = g_v1_dry;
  doc["v1_wet"] = g_v1_wet;
  doc["v1_to"] = g_v1_to;
  doc["v1_rest"] = g_v1_rest;
  doc["v1_time"] = g_v1_time;
  doc["v1_err"] = g_v1_err;
  doc["v1_name"] = g_v1_name;

  // Van 2
  doc["v2_run"] = g_v2_run;
  doc["v2_mode"] = g_v2_mode;
  doc["v2_soil"] = g_v2_soil;
  doc["v2_dry"] = g_v2_dry;
  doc["v2_wet"] = g_v2_wet;
  doc["v2_to"] = g_v2_to;
  doc["v2_rest"] = g_v2_rest;
  doc["v2_time"] = g_v2_time;
  doc["v2_err"] = g_v2_err;
  doc["v2_name"] = g_v2_name;

  // Khối môi trường
  doc["fan_mode"] = g_fanMode;
  doc["fan_speed"] = g_fanSpeed;
  doc["fan_pwm"] = g_fanPWM;
  doc["fan_setpoint"] = g_fanSetpoint;

  doc["mist_running"] = g_mistRunning;
  doc["mist_manual"] = g_mistManual;
  doc["mist_thresh"] = g_mistHumidThresh;

  doc["light_running"] = g_lightRunning;
  doc["light_manual"] = g_lightManual;
  doc["light_thresh"] = g_lightThresh;

  doc["pump_on"] = g_pumpOnThresh;
  doc["pump_off"] = g_pumpOffThresh;

  doc["heater_running"] = g_heaterRunning;
  doc["heater_manual"] = g_heaterManual;
  doc["heater_pwm"] = g_heaterPWM;
  doc["heater_setpoint"] = g_heaterSetpoint;

  doc["valve1_running"] = g_v1_run;
  doc["valve2_running"] = g_v2_run;
  doc["valve_manual"] = g_valveManual;

  doc["roof_angle"] = g_roofAngle;
  doc["roof_manual"] = g_roofManual;

  doc["sht30_ok"] = g_sht30OK;
  doc["sht30_ext_ok"] = g_sht30ExtOK;
  doc["temp_ext"] = g_tempExt;
  doc["humid_ext"] = g_humidExt;
  doc["bh1750_ok"] = g_bh1750OK;

  doc["wifi_rssi"] = g_wifiOK ? WiFi.RSSI() : 0;
  doc["ip"] = g_wifiOK ? WiFi.localIP().toString() : "0.0.0.0";
  doc["uptime"] = millis() / 1000;   // Uptime của Gateway
  doc["node_uptime"] = g_nodeUptime; // Uptime của Node
  doc["total_records"] = g_totalRecords;

  File f = LittleFS.open(CSV_FILE, "r");
  doc["file_kb"] = f ? (f.size() / 1024) : 0;
  if (f)
    f.close();
  doc["flash_total_kb"] = LittleFS.totalBytes() / 1024;
  doc["ntp_synced"] = g_ntpSynced;

  char timeStr[25];
  getTimestamp(timeStr, sizeof(timeStr));
  doc["timestamp"] = timeStr;

  char buf[1536];
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
    if (val == "on") {
      loraDoc["cmd"] = "pump_on";
      hasCmd = true;
    } else if (val == "off") {
      loraDoc["cmd"] = "pump_off";
      hasCmd = true;
    } else if (val == "auto") {
      loraDoc["cmd"] = "pump_auto";
      hasCmd = true;
    }
  }

  // -- Quạt mode --
  if (webServer.hasArg("fan_mode")) {
    loraDoc["cmd"] = "fan";
    loraDoc["mode"] = webServer.arg("fan_mode").toInt();
    hasCmd = true;
  }

  // -- Điều khiển Van tưới mới (AUTO / ON / OFF) --
  if (webServer.hasArg("valve") && webServer.hasArg("state")) {
    int vId = webServer.arg("valve").toInt();
    String st = webServer.arg("state");
    int mode = 1;
    if (st == "off")
      mode = 0;
    else if (st == "auto")
      mode = 1;
    else if (st == "on")
      mode = 2;

    loraDoc["cmd"] = "valve_set";
    loraDoc["id"] = vId;
    loraDoc["mode"] = mode;
    hasCmd = true;

    if (vId == 1) {
      g_v1_mode = mode;
      if (mode == 0)
        g_v1_run = false;
      else if (mode == 2)
        g_v1_run = true;
    }
    if (vId == 2) {
      g_v2_mode = mode;
      if (mode == 0)
        g_v2_run = false;
      else if (mode == 2)
        g_v2_run = true;
    }
    g_valve1Running = g_v1_run;
    g_valve2Running = g_v2_run;
    g_pumpRunning = (g_v1_run || g_v2_run);
  }

  // -- Cấu hình ngưỡng & thời gian van tưới --
  if (webServer.hasArg("valve_cfg")) {
    int vId = webServer.arg("valve_cfg").toInt();
    loraDoc["cmd"] = "valve_cfg";
    loraDoc["id"] = vId;
    if (webServer.hasArg("dry"))
      loraDoc["dry"] = webServer.arg("dry").toFloat();
    if (webServer.hasArg("wet"))
      loraDoc["wet"] = webServer.arg("wet").toFloat();
    if (webServer.hasArg("timeout"))
      loraDoc["to"] = webServer.arg("timeout").toInt();
    if (webServer.hasArg("rest"))
      loraDoc["rest"] = webServer.arg("rest").toInt();
    if (webServer.hasArg("name"))
      loraDoc["name"] = webServer.arg("name");
    hasCmd = true;

    if (vId == 1) {
      if (webServer.hasArg("dry"))
        g_v1_dry = webServer.arg("dry").toFloat();
      if (webServer.hasArg("wet"))
        g_v1_wet = webServer.arg("wet").toFloat();
      if (webServer.hasArg("timeout"))
        g_v1_to = webServer.arg("timeout").toInt();
      if (webServer.hasArg("rest"))
        g_v1_rest = webServer.arg("rest").toInt();
      if (webServer.hasArg("name"))
        g_v1_name = webServer.arg("name");
    } else if (vId == 2) {
      if (webServer.hasArg("dry"))
        g_v2_dry = webServer.arg("dry").toFloat();
      if (webServer.hasArg("wet"))
        g_v2_wet = webServer.arg("wet").toFloat();
      if (webServer.hasArg("timeout"))
        g_v2_to = webServer.arg("timeout").toInt();
      if (webServer.hasArg("rest"))
        g_v2_rest = webServer.arg("rest").toInt();
      if (webServer.hasArg("name"))
        g_v2_name = webServer.arg("name");
    }
    saveGatewayConfig();
  }

  // -- Dừng khẩn cấp --
  if (webServer.hasArg("pump_emg")) {
    int val = webServer.arg("pump_emg").toInt();
    loraDoc["cmd"] = (val == 1) ? "pump_emg" : "pump_emg_reset";
    hasCmd = true;
    g_pumpEmergencyStop = (val == 1);
    if (val == 1) {
      g_pumpRunning = false;
      g_v1_run = false;
      g_v2_run = false;
    }
  }

  // -- Khóa an toàn bơm (1: Cho phép, 0: Khóa) --
  if (webServer.hasArg("pump_lock")) {
    int val = webServer.arg("pump_lock").toInt();
    loraDoc["cmd"] = "pump_lock";
    loraDoc["val"] = val;
    hasCmd = true;
    g_pumpEnabled = (val != 0);
    if (!g_pumpEnabled) {
      g_pumpRunning = false;
      g_v1_run = false;
      g_v2_run = false;
    }
  }

  // -- Chế độ tưới tuần tự --
  if (webServer.hasArg("pump_seq")) {
    int val = webServer.arg("pump_seq").toInt();
    loraDoc["cmd"] = "pump_seq";
    loraDoc["val"] = val;
    hasCmd = true;
    g_pumpSeq = (val != 0);
  }

  // -- Quạt speed (manual) --
  if (webServer.hasArg("fan_speed")) {
    loraDoc["cmd"] = "fan";
    loraDoc["mode"] = 2; // MANUAL
    loraDoc["speed"] = webServer.arg("fan_speed").toInt();
    hasCmd = true;
  }

  // -- Setpoint quạt Auto --
  if (webServer.hasArg("setpoint")) {
    loraDoc["cmd"] = "fan";
    loraDoc["mode"] = 1; // AUTO
    loraDoc["setpoint"] = webServer.arg("setpoint").toFloat();
    hasCmd = true;
    g_fanMode = 1;
    g_fanSetpoint = webServer.arg("setpoint").toFloat();
    saveGatewayConfig();
  }

  // -- Cấu hình ngưỡng bơm --
  if (webServer.hasArg("pump_on") || webServer.hasArg("pump_off")) {
    loraDoc["cmd"] = "config";
    if (webServer.hasArg("pump_on")) {
      loraDoc["pump_on"] = webServer.arg("pump_on").toFloat();
      g_pumpOnThresh = loraDoc["pump_on"];
    }
    if (webServer.hasArg("pump_off")) {
      loraDoc["pump_off"] = webServer.arg("pump_off").toFloat();
      g_pumpOffThresh = loraDoc["pump_off"];
    }
    hasCmd = true;
    saveGatewayConfig();
  }

  // -- Ngưỡng đèn --
  if (webServer.hasArg("light_thresh")) {
    loraDoc["cmd"] = "config";
    loraDoc["light_th"] = webServer.arg("light_thresh").toFloat();
    hasCmd = true;
    g_lightThresh = webServer.arg("light_thresh").toFloat();
    saveGatewayConfig();
  }

  // -- Ngưỡng phun sương cấp ẩm --
  if (webServer.hasArg("mist_thresh")) {
    loraDoc["cmd"] = "config";
    loraDoc["mist_th"] = webServer.arg("mist_thresh").toFloat();
    hasCmd = true;
    g_mistHumidThresh = webServer.arg("mist_thresh").toFloat();
    saveGatewayConfig();
  }

  // -- Phun sương --
  if (webServer.hasArg("mist")) {
    String val = webServer.arg("mist");
    if (val == "on") {
      loraDoc["cmd"] = "mist_on";
      hasCmd = true;
    } else if (val == "off") {
      loraDoc["cmd"] = "mist_off";
      hasCmd = true;
    } else if (val == "auto") {
      loraDoc["cmd"] = "mist_auto";
      hasCmd = true;
    }
  }

  // -- Đèn --
  if (webServer.hasArg("lamp")) {
    String val = webServer.arg("lamp");
    if (val == "on") {
      loraDoc["cmd"] = "light_on";
      hasCmd = true;
    } else if (val == "off") {
      loraDoc["cmd"] = "light_off";
      hasCmd = true;
    } else if (val == "auto") {
      loraDoc["cmd"] = "light_auto";
      hasCmd = true;
    }
  }

  // -- Sưởi --
  if (webServer.hasArg("heater")) {
    String val = webServer.arg("heater");
    if (val == "on") {
      loraDoc["cmd"] = "heater_on";
      hasCmd = true;
    } else if (val == "off") {
      loraDoc["cmd"] = "heater_off";
      hasCmd = true;
    } else if (val == "auto") {
      loraDoc["cmd"] = "heater_auto";
      hasCmd = true;
    }
  }
  if (webServer.hasArg("heater_speed")) {
    loraDoc["cmd"] = "heater";
    loraDoc["speed"] = constrain(webServer.arg("heater_speed").toInt(), 0, 255);
    hasCmd = true;
  }
  if (webServer.hasArg("heater_setpoint")) {
    g_heaterSetpoint = webServer.arg("heater_setpoint").toFloat();
    saveGatewayConfig();
    loraDoc["cmd"] = "heater_cfg";
    loraDoc["setpoint"] = g_heaterSetpoint;
    hasCmd = true;
  }

  // -- Van điện từ 1 --
  if (webServer.hasArg("valve1")) {
    String val = webServer.arg("valve1");
    if (val == "on") {
      loraDoc["cmd"] = "valve1_on";
      hasCmd = true;
    } else if (val == "off") {
      loraDoc["cmd"] = "valve1_off";
      hasCmd = true;
    } else if (val == "auto") {
      loraDoc["cmd"] = "valve_auto";
      hasCmd = true;
    }
  }

  // -- Van điện từ 2 --
  if (webServer.hasArg("valve2")) {
    String val = webServer.arg("valve2");
    if (val == "on") {
      loraDoc["cmd"] = "valve2_on";
      hasCmd = true;
    } else if (val == "off") {
      loraDoc["cmd"] = "valve2_off";
      hasCmd = true;
    } else if (val == "auto") {
      loraDoc["cmd"] = "valve_auto";
      hasCmd = true;
    }
  }

  // -- Mái che --
  if (webServer.hasArg("roof")) {
    String val = webServer.arg("roof");
    if (val == "open") {
      loraDoc["cmd"] = "roof_open";
      hasCmd = true;
    } else if (val == "close") {
      loraDoc["cmd"] = "roof_close";
      hasCmd = true;
    } else if (val == "auto") {
      loraDoc["cmd"] = "roof_auto";
      hasCmd = true;
    }
  }
  if (webServer.hasArg("roof_angle")) {
    loraDoc["cmd"] = "roof_set";
    loraDoc["angle"] = constrain(webServer.arg("roof_angle").toInt(), 0, 180);
    hasCmd = true;
  }

  if (hasCmd) {
    String loraStr;
    serializeJson(loraDoc, loraStr);
    sendLoRaCommand(loraStr);
    delay(80);
    sendLoRaCommand(loraStr); // Phát đúp 2 lần chống va chạm sóng/rớt gói trên không

    // ─── Cập nhật trạng thái Gateway ngay lập tức (không đợi Node xác nhận)
    // ─── Điều này đảm bảo Web poll 1 giây sau sẽ nhận được state đúng
    String cmd = loraDoc["cmd"].as<String>();
    if (cmd == "pump_on") {
      g_pumpRunning = true;
      g_pumpManual = true;
    }
    if (cmd == "pump_off") {
      g_pumpRunning = false;
      g_pumpManual = true;
    }
    if (cmd == "pump_auto") {
      g_pumpManual = false;
    }
    if (cmd == "mist_on") {
      g_mistRunning = true;
      g_mistManual = true;
    }
    if (cmd == "mist_off") {
      g_mistRunning = false;
      g_mistManual = true;
    }
    if (cmd == "mist_auto") {
      g_mistManual = false;
    }
    if (cmd == "light_on") {
      g_lightRunning = true;
      g_lightManual = true;
    }
    if (cmd == "light_off") {
      g_lightRunning = false;
      g_lightManual = true;
    }
    if (cmd == "light_auto") {
      g_lightManual = false;
    }
    if (cmd == "heater_on") {
      g_heaterRunning = true;
      g_heaterManual = true;
      g_heaterPWM = 255;
    }
    if (cmd == "heater_off") {
      g_heaterRunning = false;
      g_heaterManual = true;
      g_heaterPWM = 0;
    }
    if (cmd == "heater_auto") {
      g_heaterManual = false;
    }
    if (cmd == "heater") {
      if (loraDoc.containsKey("speed")) {
        g_heaterPWM = loraDoc["speed"];
        g_heaterRunning = (g_heaterPWM > 0);
        g_heaterManual = true;
      }
    }
    if (cmd == "valve1_on") {
      g_valve1Running = true;
      g_valveManual = true;
    }
    if (cmd == "valve1_off") {
      g_valve1Running = false;
      g_valveManual = true;
    }
    if (cmd == "valve2_on") {
      g_valve2Running = true;
      g_valveManual = true;
    }
    if (cmd == "valve2_off") {
      g_valve2Running = false;
      g_valveManual = true;
    }
    if (cmd == "valve_auto") {
      g_valveManual = false;
    }
    if (cmd == "roof_open") {
      g_roofAngle = 180;
      g_roofManual = true;
    }
    if (cmd == "roof_close") {
      g_roofAngle = 0;
      g_roofManual = true;
    }
    if (cmd == "roof_auto") {
      g_roofManual = false;
    }
    if (cmd == "roof_set") {
      if (loraDoc.containsKey("angle")) {
        g_roofAngle = loraDoc["angle"];
        g_roofManual = true;
      }
    }
    if (cmd == "fan") {
      if (loraDoc.containsKey("mode"))
        g_fanMode = loraDoc["mode"];
      if (loraDoc.containsKey("speed")) {
        int spd = loraDoc["speed"];
        if (spd == 0) g_fanSpeed = 0;
        else if (spd <= 125) g_fanSpeed = 40;
        else if (spd <= 180) g_fanSpeed = 60;
        else if (spd <= 230) g_fanSpeed = 80;
        else g_fanSpeed = 100;
      }
      if (loraDoc.containsKey("setpoint"))
        g_fanSetpoint = loraDoc["setpoint"];
    }
    if (cmd == "config") {
      if (loraDoc.containsKey("pump_on"))
        g_pumpOnThresh = loraDoc["pump_on"];
      if (loraDoc.containsKey("pump_off"))
        g_pumpOffThresh = loraDoc["pump_off"];
      if (loraDoc.containsKey("light_th"))
        g_lightThresh = loraDoc["light_th"];
    }

    StaticJsonDocument<384> resDoc;
    resDoc["status"] = "success";
    resDoc["light_running"] = g_lightRunning;
    resDoc["light_manual"] = g_lightManual;
    resDoc["mist_running"] = g_mistRunning;
    resDoc["mist_manual"] = g_mistManual;
    resDoc["heater_running"] = g_heaterRunning;
    resDoc["heater_manual"] = g_heaterManual;
    resDoc["heater_pwm"] = g_heaterPWM;
    resDoc["fan_mode"] = g_fanMode;
    resDoc["fan_speed"] = g_fanSpeed;
    resDoc["roof_angle"] = g_roofAngle;
    resDoc["roof_manual"] = g_roofManual;
    resDoc["pump_running"] = g_pumpRunning;
    resDoc["pump_manual"] = g_pumpManual;
    resDoc["v1_run"] = g_v1_run;
    resDoc["v1_mode"] = g_v1_mode;
    resDoc["v2_run"] = g_v2_run;
    resDoc["v2_mode"] = g_v2_mode;
    String resStr;
    serializeJson(resDoc, resStr);
    webServer.send(200, "application/json", resStr);
  } else {
    webServer.send(400, "application/json",
                   "{\"status\":\"error\",\"msg\":\"Unknown params\"}");
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
      arrT.add(isnan(g_fc_temp[h]) ? 0 : round(g_fc_temp[h] * 10) / 10.0f);
      arrH.add(isnan(g_fc_humid[h]) ? 0 : round(g_fc_humid[h] * 10) / 10.0f);
      arrL.add(isnan(g_fc_lux[h]) ? 0 : (int)g_fc_lux[h]);
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
    webServer.send(404, "application/json",
                   "{\"status\":\"error\",\"msg\":\"File không tồn tại\"}");
  }
}
