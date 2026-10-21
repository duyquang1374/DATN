// ═════════════════════════════════════════════════════════════
//  GATEWAY LORA HANDLER - GIAO TIẾP KHÔNG DÂY VỚI MASTER NODE
//  - Thu nhận định kỳ gói tin 'status' (10s) và 'forecast' (5 phút)
//  - Ghép nối buffer dữ liệu lớn (>256 bytes) theo cặp ngoặc nhọn JSON {}
//  - Chống dội trạng thái (Debounce 2s) khi vừa gửi lệnh điều khiển
//  - Phát lệnh điều khiển từ API Web xuống Master Node qua sóng LoRa
// ═════════════════════════════════════════════════════════════

void checkLoRaMessage() {
  // Buffer để ghép nối dữ liệu LoRa bị chia nhỏ (>256 bytes)
  static String rawBuf = "";
  static unsigned long lastPrint = 0;
  static unsigned long lastWarn  = 0;
  static int braceCount = 0;       // Đếm {} để biết JSON hoàn chỉnh
  static bool inJson = false;

  while (LoRaSerial.available()) {
    char c = LoRaSerial.read();
    lastPrint = millis();

    // Bắt đầu JSON khi gặp '{'
    if (c == '{') {
      if (!inJson) {
        rawBuf = "";   // Reset buffer khi bắt đầu JSON mới
        braceCount = 0;
        inJson = true;
      }
      braceCount++;
    }

    // Chỉ thu thập khi đang trong JSON
    if (inJson) {
      rawBuf += c;

      if (c == '}') {
        braceCount--;

        // JSON hoàn chỉnh khi braceCount == 0
        if (braceCount == 0) {
          inJson = false;
          rawBuf.trim();
          Serial.println("[LoRa RAW] " + rawBuf);

          // Parse JSON
          StaticJsonDocument<1024> doc;
          DeserializationError error = deserializeJson(doc, rawBuf);

          if (error) {
            Serial.println("[LoRa] JSON parse failed: " + String(error.c_str()));
          } else if (doc.containsKey("type")) {
            String type = doc["type"].as<String>();

            if (type == "status" || type == "state") {
              if (doc.containsKey("temp"))     g_temperature   = doc["temp"]     | 0.0f;
              if (doc.containsKey("humid"))    g_humidity      = doc["humid"]    | 0.0f;
              if (doc.containsKey("temp_ext")) g_tempExt       = doc["temp_ext"] | 0.0f;
              if (doc.containsKey("humid_ext"))g_humidExt      = doc["humid_ext"]| 0.0f;
              if (doc.containsKey("lux"))      g_lightLux      = doc["lux"]      | 0.0f;

              // Cảm biến ẩm đất 1 & 2
              if (doc.containsKey("soil"))     g_soilMoisture  = doc["soil"]     | 0.0f;
              if (doc.containsKey("soil2"))    g_soilMoisture2 = doc["soil2"]    | 0.0f;

              if (doc.containsKey("s_ok"))     g_sht30OK       = doc["s_ok"]     | false;
              if (doc.containsKey("se_ok"))    g_sht30ExtOK    = doc["se_ok"]    | false;

              // Chỉ cho phép cập nhật cơ cấu chấp hành nếu là gói 'state' (phản hồi sự kiện tức thì) hoặc sau lệnh >3.5s
              bool allowActuatorUpdate = (type == "state") || (millis() - g_lastCommandTime >= 3500);
              if (allowActuatorUpdate) {
                // Bơm & An toàn
                if (doc.containsKey("pump"))     g_pumpRunning       = doc["pump"]   | false;
                if (doc.containsKey("p_en"))     g_pumpEnabled       = doc["p_en"]   | true;
                if (doc.containsKey("p_emg"))    g_pumpEmergencyStop = doc["p_emg"]  | false;
                if (doc.containsKey("p_time"))   g_pumpTime          = doc["p_time"] | 0UL;
                if (doc.containsKey("p_seq"))    g_pumpSeq           = doc["p_seq"]  | true;

                // Van 1
                if (doc.containsKey("v1_run"))   g_v1_run  = doc["v1_run"]  | false;
                if (doc.containsKey("v1_mode"))  g_v1_mode = doc["v1_mode"] | 1;
                if (doc.containsKey("v1_soil"))  g_v1_soil = doc["v1_soil"] | 0.0f;
                if (doc.containsKey("v1_dry"))   g_v1_dry  = doc["v1_dry"]  | 40.0f;
                if (doc.containsKey("v1_wet"))   g_v1_wet  = doc["v1_wet"]  | 65.0f;
                if (doc.containsKey("v1_to"))    g_v1_to   = doc["v1_to"]   | 300;
                if (doc.containsKey("v1_rest"))  g_v1_rest = doc["v1_rest"] | 60;
                if (doc.containsKey("v1_time"))  g_v1_time = doc["v1_time"] | 0UL;
                if (doc.containsKey("v1_err"))   g_v1_err  = doc["v1_err"]  | false;
                if (doc.containsKey("v1_name"))  g_v1_name = doc["v1_name"].as<String>();

                // Van 2
                if (doc.containsKey("v2_run"))   g_v2_run  = doc["v2_run"]  | false;
                if (doc.containsKey("v2_mode"))  g_v2_mode = doc["v2_mode"] | 1;
                if (doc.containsKey("v2_soil"))  g_v2_soil = doc["v2_soil"] | 0.0f;
                if (doc.containsKey("v2_dry"))   g_v2_dry  = doc["v2_dry"]  | 45.0f;
                if (doc.containsKey("v2_wet"))   g_v2_wet  = doc["v2_wet"]  | 70.0f;
                if (doc.containsKey("v2_to"))    g_v2_to   = doc["v2_to"]   | 300;
                if (doc.containsKey("v2_rest"))  g_v2_rest = doc["v2_rest"] | 60;
                if (doc.containsKey("v2_time"))  g_v2_time = doc["v2_time"] | 0UL;
                if (doc.containsKey("v2_err"))   g_v2_err  = doc["v2_err"]  | false;
                if (doc.containsKey("v2_name"))  g_v2_name = doc["v2_name"].as<String>();

                // Tương thích ngược
                g_valve1Running = g_v1_run;
                g_valve2Running = g_v2_run;

                if (doc.containsKey("fan_mode")) g_fanMode       = doc["fan_mode"] | 1;
                if (doc.containsKey("fan_speed"))g_fanSpeed      = doc["fan_speed"]| 0;
                if (doc.containsKey("fan_pwm"))  g_fanPWM        = doc["fan_pwm"]  | 0;
                if (doc.containsKey("fan_sp"))   g_fanSetpoint   = doc["fan_sp"]   | 30.0f;

                if (doc.containsKey("mist"))     g_mistRunning   = doc["mist"]     | false;
                if (doc.containsKey("mist_m"))   g_mistManual    = doc["mist_m"]   | false;

                if (doc.containsKey("light"))    g_lightRunning  = doc["light"]    | false;
                if (doc.containsKey("light_m"))  g_lightManual   = doc["light_m"]  | false;
                if (doc.containsKey("light_th")) g_lightThresh   = doc["light_th"] | 1000.0f;

                if (doc.containsKey("heater"))   g_heaterRunning = doc["heater"]   | false;
                if (doc.containsKey("heater_m")) g_heaterManual  = doc["heater_m"] | false;
                if (doc.containsKey("heater_pwm")) g_heaterPWM   = doc["heater_pwm"]| 0;

                if (doc.containsKey("roof"))     g_roofAngle     = doc["roof"]     | 0;
                if (doc.containsKey("roof_m"))   g_roofManual    = doc["roof_m"]   | false;
              }
              if (doc.containsKey("b_ok"))     g_bh1750OK      = doc["b_ok"]     | false;

              if (doc.containsKey("uptime"))   g_nodeUptime    = doc["uptime"]   | 0UL;

              g_lastLoraReceive = millis();
              if (type == "state") {
                Serial.printf("[LoRa] State Event OK: Mist=%d Light=%d Fan=%d Heater=%d Roof=%d\n",
                              g_mistRunning, g_lightRunning, g_fanSpeed, g_heaterPWM, g_roofAngle);
              } else {
                Serial.printf("[LoRa] Status OK: T=%.1f H=%.1f L=%.0f Soil=%.1f Mist=%d Light=%d\n",
                              g_temperature, g_humidity, g_lightLux, g_soilMoisture,
                              g_mistRunning, g_lightRunning);
              }
            }
            else if (type == "forecast") {
              JsonArray arrT = doc["temp"];
              JsonArray arrH = doc["humid"];
              JsonArray arrL = doc["lux"];
              for (int i = 0; i < 3; i++) {
                g_fc_temp[i]  = arrT[i];
                g_fc_humid[i] = arrH[i];
                g_fc_lux[i]   = arrL[i];
              }
              g_forecast_ready = true;
              Serial.println("[LoRa] Forecast OK");
            }
          }
          rawBuf = "";
        }
      }

      // Bảo vệ: nếu buffer quá lớn (lỗi), reset
      if (rawBuf.length() > 600) {
        Serial.println("[LoRa] Buffer overflow, reset");
        rawBuf = "";
        braceCount = 0;
        inJson = false;
      }
    }
  }

  // Nếu 10 giây không nhận được byte nào, in cảnh báo mỗi 15 giây
  if (millis() - lastPrint > 10000 && millis() - lastWarn > 15000) {
    lastWarn = millis();
    Serial.println("[LoRa] WARN: Khong nhan duoc du lieu tu Node! Kiem tra day noi TX/RX va M0/M1=GND.");
  }
}

// Hàm gửi lệnh điều khiển từ API xuống Node
void sendLoRaCommand(String cmdJson) {
  cmdJson.trim();
  cmdJson += "\n";
  LoRaSerial.print(cmdJson);
  LoRaSerial.flush();
  Serial.print("[LoRa Tx] " + cmdJson);
  g_lastCommandTime = millis();
}
