/*
 * lora_handler.ino
 * Nhận và gửi lệnh qua LoRa trên Gateway
 */

void checkLoRaMessage() {
  // DEBUG: In ra bất kỳ byte nào nhận được để kiểm tra kết nối vật lý
  static String rawBuf = "";
  static unsigned long lastPrint = 0;
  static unsigned long lastWarn  = 0;

  while (LoRaSerial.available()) {
    char c = LoRaSerial.read();
    rawBuf += c;
    lastPrint = millis();

    if (c == '\n' || rawBuf.length() > 256) {
      rawBuf.trim();
      if (rawBuf.length() > 0) {
        Serial.println("[LoRa RAW] " + rawBuf);

        // Parse JSON
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, rawBuf);

        if (error) {
          Serial.println("[LoRa] JSON parse failed: " + String(error.c_str()));
        } else if (doc.containsKey("type")) {
          String type = doc["type"].as<String>();

          if (type == "status") {
            g_temperature   = doc["temp"]     | 0.0f;
            g_humidity      = doc["humid"]    | 0.0f;
            g_tempExt       = doc["temp_ext"] | 0.0f;
            g_humidExt      = doc["humid_ext"]| 0.0f;
            g_lightLux      = doc["lux"]      | 0.0f;
            g_soilMoisture  = doc["soil"]     | 0.0f;

            g_fanMode       = doc["fan_mode"] | 1;
            g_fanSpeed      = doc["fan_speed"]| 0;
            g_fanPWM        = doc["fan_pwm"]  | 0;
            g_fanSetpoint   = doc["fan_sp"]   | 30.0f;

            g_pumpRunning   = doc["pump"]     | false;
            g_pumpOnThresh  = doc["pump_on"]  | 40.0f;
            g_pumpOffThresh = doc["pump_off"] | 65.0f;

            g_sht30OK       = doc["s_ok"]     | false;
            g_sht30ExtOK    = doc["se_ok"]    | false;
            g_bh1750OK      = doc["b_ok"]     | false;

            g_nodeUptime    = doc["uptime"]   | 0UL;

            g_lastLoraReceive = millis();
            Serial.printf("[LoRa] Status OK: T=%.1f H=%.1f L=%.0f Soil=%.1f\n",
                          g_temperature, g_humidity, g_lightLux, g_soilMoisture);
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
      }
      rawBuf = "";
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
  cmdJson += "\n";
  LoRaSerial.print(cmdJson);
  Serial.println("[LoRa Tx] " + cmdJson);
}
