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
  
  doc["heater"]      = g_heaterRunning;
  doc["heater_m"]    = g_heaterManual;
  doc["heater_pwm"]  = g_heaterPWM;

  doc["valve1"]      = g_valve1Running;
  doc["valve2"]      = g_valve2Running;
  doc["valve_m"]     = g_valveManual;
  
  doc["roof"]        = g_roofAngle;
  doc["roof_m"]      = g_roofManual;
  
  doc["s_ok"]        = g_sht30OK;
  doc["se_ok"]       = g_sht30ExtOK;
  doc["b_ok"]        = g_bh1750OK;
  
  doc["uptime"]      = millis() / 1000;

  char buf[512];
  size_t len = serializeJson(doc, buf);
  buf[len] = '\n';
  buf[len+1] = '\0';
  LoRaSerial.print(buf);
}

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
}

void checkLoRaCommand() {
  static String buf = "";
  while (LoRaSerial.available()) {
    char c = LoRaSerial.read();
    if (c == '\n') {
      buf.trim();
      if (buf.length() > 0) {
        StaticJsonDocument<256> doc;
        DeserializationError error = deserializeJson(doc, buf);
        if (!error && doc.containsKey("cmd")) {
          String cmd = doc["cmd"].as<String>();
          
          if (cmd == "pump_on") setPumpManual(true);
          else if (cmd == "pump_off") setPumpManual(false);
          else if (cmd == "pump_auto") g_pumpManual = false;
          
          else if (cmd == "fan") {
            if (doc.containsKey("mode")) g_fanMode = doc["mode"];
            if (doc.containsKey("speed") && g_fanMode == FAN_MODE_MANUAL) setFanManualSpeed((int)doc["speed"]);
            if (doc.containsKey("setpoint") && g_fanMode == FAN_MODE_AUTO) setFanSetpoint(doc["setpoint"]);
          }
          else if (cmd == "config") {
            if (doc.containsKey("pump_on")) g_pumpOnThresh = doc["pump_on"];
            if (doc.containsKey("pump_off")) g_pumpOffThresh = doc["pump_off"];
            if (doc.containsKey("fan_sp")) setFanSetpoint(doc["fan_sp"]);
            if (doc.containsKey("light_th")) g_lightThresh = doc["light_th"];
            saveConfig();
          }
          
          else if (cmd == "mist_on") setMistManual(true);
          else if (cmd == "mist_off") setMistManual(false);
          else if (cmd == "mist_auto") g_mistManual = false;
          
          else if (cmd == "light_on") setLightManual(true);
          else if (cmd == "light_off") setLightManual(false);
          else if (cmd == "light_auto") g_lightManual = false;
          
          else if (cmd == "heater_on") setHeaterManual(true);
          else if (cmd == "heater_off") setHeaterManual(false);
          else if (cmd == "heater_auto") g_heaterManual = false;
          
          else if (cmd == "valve1_on") setValveManual(1, true);
          else if (cmd == "valve1_off") setValveManual(1, false);
          else if (cmd == "valve2_on") setValveManual(2, true);
          else if (cmd == "valve2_off") setValveManual(2, false);
          else if (cmd == "valve_auto") g_valveManual = false;
          
          else if (cmd == "roof_open") setRoofManual(180);
          else if (cmd == "roof_close") setRoofManual(0);
          else if (cmd == "roof_auto") g_roofManual = false;

          updateFan();
          updatePump();
          updateMist();
          updateLight();
          updateHeater();
          updateValves();
          updateRoof();
          
          sendDataToGateway();
          sendHMIData();
        }
      }
      buf = "";
    } else {
      buf += c;
      if (buf.length() > 256) buf = "";
    }
  }
}
