/*
 * lora_comm.ino
 * Quản lý giao tiếp qua UART với module LoRa E32
 * Node (Nhà màng) <---> Gateway
 */

// ═══════════════════════════════════════════════════════════════
//  GỬI DỮ LIỆU CẢM BIẾN LÊN GATEWAY
// ═══════════════════════════════════════════════════════════════
void sendDataToGateway() {
  StaticJsonDocument<512> doc;
  doc["type"]        = "status";
  doc["temp"]        = isnan(g_temperature)  ? 0 : round(g_temperature  * 10) / 10.0;
  doc["humid"]       = isnan(g_humidity)     ? 0 : round(g_humidity     * 10) / 10.0;
  doc["temp_ext"]    = isnan(g_tempExt)      ? 0 : round(g_tempExt      * 10) / 10.0;
  doc["humid_ext"]   = isnan(g_humidExt)     ? 0 : round(g_humidExt     * 10) / 10.0;
  doc["lux"]         = isnan(g_lightLux)     ? 0 : (int)g_lightLux;
  doc["soil"]        = isnan(g_soilMoisture) ? 0 : round(g_soilMoisture * 10) / 10.0;
  
  doc["fan_mode"]    = g_fanMode;
  doc["fan_speed"]   = g_fanSpeed;
  doc["fan_pwm"]     = g_fanPWM;
  doc["fan_sp"]      = g_fanSetpoint;
  
  doc["pump"]        = g_pumpRunning;
  doc["pump_m"]      = g_pumpManual;
  doc["pump_on"]     = g_pumpOnThresh;
  doc["pump_off"]    = g_pumpOffThresh;
  
  doc["mist"]        = g_mistRunning;
  doc["mist_m"]      = g_mistManual;
  
  doc["light"]       = g_lightRunning;
  doc["light_m"]     = g_lightManual;
  doc["light_th"]    = g_lightThresh;
  
  doc["s_ok"]        = g_sht30OK;
  doc["se_ok"]       = g_sht30ExtOK;
  doc["b_ok"]        = g_bh1750OK;
  
  doc["uptime"]      = millis() / 1000;

  char buf[512];
  size_t len = serializeJson(doc, buf);
  
  // Gửi qua LoRa (thêm \n để gateway dễ parse)
  buf[len] = '\n';
  buf[len+1] = '\0';
  LoRaSerial.print(buf);
  
  Serial.println("[LoRa] Sent status");
}

// ═══════════════════════════════════════════════════════════════
//  GỬI DỮ LIỆU DỰ BÁO LÊN GATEWAY
// ═══════════════════════════════════════════════════════════════
void sendForecastToGateway() {
  if (!g_forecast_ready) return;
  
  StaticJsonDocument<384> doc;
  doc["type"] = "forecast";
  
  JsonArray arrT = doc.createNestedArray("temp");
  JsonArray arrH = doc.createNestedArray("humid");
  JsonArray arrL = doc.createNestedArray("lux");
  
  for (int h = 0; h < FC_HORIZONS; h++) {
    arrT.add(isnan(g_fc_temp[h])  ? 0 : round(g_fc_temp[h]  * 10) / 10.0f);
    arrH.add(isnan(g_fc_humid[h]) ? 0 : round(g_fc_humid[h] * 10) / 10.0f);
    arrL.add(isnan(g_fc_lux[h])   ? 0 : (int)g_fc_lux[h]);
  }

  char buf[384];
  size_t len = serializeJson(doc, buf);
  buf[len] = '\n';
  buf[len+1] = '\0';
  LoRaSerial.print(buf);
  
  Serial.println("[LoRa] Sent forecast");
}

// ═══════════════════════════════════════════════════════════════
//  NHẬN VÀ XỬ LÝ LỆNH TỪ GATEWAY
// ═══════════════════════════════════════════════════════════════
void checkLoRaCommand() {
  while (LoRaSerial.available()) {
    String msg = LoRaSerial.readStringUntil('\n');
    msg.trim();
    if (msg.length() == 0) continue;
    
    Serial.println("[LoRa] Nhận: " + msg);
    
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, msg);
    if (error) {
      Serial.println("[LoRa] Lỗi parse JSON!");
      continue;
    }
    
    if (doc.containsKey("cmd")) {
      String cmd = doc["cmd"].as<String>();
      
      // -- Bơm --
      if (cmd == "pump_on") {
        g_pumpManual = true;
        setPumpState(true);
        Serial.println("[LoRa] CMD: Bật bơm");
      } 
      else if (cmd == "pump_off") {
        g_pumpManual = true;
        setPumpState(false);
        Serial.println("[LoRa] CMD: Tắt bơm");
      } 
      else if (cmd == "pump_auto") {
        g_pumpManual = false;
        Serial.println("[LoRa] CMD: Bơm Auto");
      }
      
      // -- Quạt --
      else if (cmd == "fan") {
        if (doc.containsKey("mode")) {
          g_fanMode = doc["mode"];
        }
        // speed đã là 0-255 (PWM) từ slider web, gán trực tiếp
        if (doc.containsKey("speed") && g_fanMode == FAN_MODE_MANUAL) {
          g_manualPWM = constrain((int)doc["speed"], 0, 255);
        }
        if (doc.containsKey("setpoint") && g_fanMode == FAN_MODE_AUTO) {
          g_fanSetpoint = doc["setpoint"];
          pidSetpoint = g_fanSetpoint;
        }
        Serial.printf("[LoRa] CMD: Fan mode=%d manualPWM=%d\n", g_fanMode, g_manualPWM);
      }
      
      // -- Cấu hình --
      else if (cmd == "config") {
        if (doc.containsKey("pump_on")) g_pumpOnThresh = doc["pump_on"];
        if (doc.containsKey("pump_off")) g_pumpOffThresh = doc["pump_off"];
        if (doc.containsKey("fan_sp")) {
          g_fanSetpoint = doc["fan_sp"];
          pidSetpoint = g_fanSetpoint;
        }
        if (doc.containsKey("light_th")) g_lightThresh = doc["light_th"];
        saveConfig();
        Serial.println("[LoRa] CMD: Cập nhật cấu hình");
      }
      
      // -- Phun sương --
      else if (cmd == "mist_on") {
        g_mistManual = true;
        setMistState(true);
        Serial.println("[LoRa] CMD: Bật phun sương");
      }
      else if (cmd == "mist_off") {
        g_mistManual = true;
        setMistState(false);
        Serial.println("[LoRa] CMD: Tắt phun sương");
      }
      else if (cmd == "mist_auto") {
        g_mistManual = false;
        Serial.println("[LoRa] CMD: Phun sương Auto");
      }
      
      // -- Đèn --
      else if (cmd == "light_on") {
        g_lightManual = true;
        setLightState(true);
        Serial.println("[LoRa] CMD: Bật đèn");
      }
      else if (cmd == "light_off") {
        g_lightManual = true;
        setLightState(false);
        Serial.println("[LoRa] CMD: Tắt đèn");
      }
      else if (cmd == "light_auto") {
        g_lightManual = false;
        Serial.println("[LoRa] CMD: Đèn Auto");
      }
      
      // Cập nhật ngay cơ cấu chấp hành
      updateFan();
      updatePump();
      updateMist();
      updateLight();
      
      // Gửi lại trạng thái ngay lập tức để Gateway/Web cập nhật nhanh
      sendDataToGateway();
    }
  }
}
