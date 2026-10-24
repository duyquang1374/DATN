// ═════════════════════════════════════════════════════════════
//  SMART GREENHOUSE - GIAO TIẾP KHÔNG DÂY LORA E32 (LORA_COMM)
//  - Module LoRa E32-433T20D (UART2: TX 18, RX 19, Baud 115200, M0/M1=GND)
//  - Phát gói tin:
//    + 'state' (phát tức thì khi thay đổi trạng thái): Báo ngay thiết bị bật/tắt
//    + 'status' (mỗi 10s): Toàn bộ thông số cảm biến, 2 van, bơm & thiết bị
//    + 'forecast' (mỗi 5 phút): Kết quả dự báo TinyML 3 bước thời gian tương lai
// ═════════════════════════════════════════════════════════════

void sendStateEventToGateway() {
  StaticJsonDocument<256> doc;
  doc["type"] = "state";
  doc["light"] = g_lightRunning ? 1 : 0;
  doc["light_m"] = g_lightManual ? 1 : 0;
  doc["mist"] = g_mistRunning ? 1 : 0;
  doc["mist_m"] = g_mistManual ? 1 : 0;
  doc["fan_mode"] = g_fanMode;
  doc["fan_speed"] = g_fanSpeed;
  doc["fan_pwm"] = g_fanPWM;
  doc["heater"] = g_heaterRunning ? 1 : 0;
  doc["heater_m"] = g_heaterManual ? 1 : 0;
  doc["heater_pwm"] = g_heaterPWM;
  doc["roof"] = g_roofAngle;
  doc["roof_m"] = g_roofManual ? 1 : 0;
  doc["pump"] = g_pumpRunning ? 1 : 0;
  doc["p_en"] = g_pumpEnabled ? 1 : 0;
  doc["p_emg"] = g_pumpEmergencyStop ? 1 : 0;
  doc["v1_run"] = g_valves[0].isRunning ? 1 : 0;
  doc["v1_mode"] = g_valves[0].mode;
  doc["v2_run"] = g_valves[1].isRunning ? 1 : 0;
  doc["v2_mode"] = g_valves[1].mode;

  char buf[256];
  size_t len = serializeJson(doc, buf);
  buf[len] = '\n';
  buf[len + 1] = '\0';
  LoRaSerial.print(buf);
  LoRaSerial.flush();
  Serial.printf("[LoRa Tx] Da phat state event (%d bytes)\n", (int)len);
}

void sendDataToGateway(bool force) {
  static unsigned long lastSend = 0;
  // Chống phát dồn dập làm nghẽn sóng và tràn đệm phần cứng module LoRa E32 (trừ khi có cờ force)
  if (!force && (millis() - lastSend < 2000)) {
    return;
  }

  // CSMA/CA (Listen-Before-Talk): Nếu cổng LoRa đang có dữ liệu từ Gateway (lệnh điều khiển Web tới),
  // ưu tiên nhận và thực thi lệnh trước, hoãn phát status để không đè sóng làm mất lệnh!
  if (LoRaSerial.available() > 0) {
    checkLoRaCommand();
    return;
  }

  lastSend = millis();

  StaticJsonDocument<768> doc;
  doc["type"] = "status";
  doc["temp"] = isnan(g_temperature) ? 0 : round(g_temperature * 10) / 10.0;
  doc["humid"] = isnan(g_humidity) ? 0 : round(g_humidity * 10) / 10.0;
  doc["temp_ext"] = isnan(g_tempExt) ? 0 : round(g_tempExt * 10) / 10.0;
  doc["humid_ext"] = isnan(g_humidExt) ? 0 : round(g_humidExt * 10) / 10.0;
  doc["lux"] = isnan(g_lightLux) ? 0 : (int)g_lightLux;

  // Cảm biến độ ẩm đất 1 & 2
  doc["soil"] = isnan(g_valves[0].currentSoil) ? 0 : round(g_valves[0].currentSoil * 10) / 10.0;
  doc["soil2"] = isnan(g_valves[1].currentSoil) ? 0 : round(g_valves[1].currentSoil * 10) / 10.0;

  // Bơm nước & An toàn (1/0 tiết kiệm dung lượng)
  doc["pump"] = g_pumpRunning ? 1 : 0;
  doc["p_en"] = g_pumpEnabled ? 1 : 0;
  doc["p_emg"] = g_pumpEmergencyStop ? 1 : 0;
  doc["p_time"] = g_pumpRunning ? (millis() - g_pumpRunStartTime) / 1000 : 0;
  doc["p_seq"] = g_sequentialIrrigation ? 1 : 0;

  // Van 1
  doc["v1_run"] = g_valves[0].isRunning ? 1 : 0;
  doc["v1_mode"] = g_valves[0].mode;
  doc["v1_time"] = g_valves[0].isRunning ? (millis() - g_valves[0].runStartTime) / 1000 : 0;
  doc["v1_err"] = g_valves[0].sensorError ? 1 : 0;
  doc["v1_dry"] = g_valves[0].dryThresh;
  doc["v1_wet"] = g_valves[0].wetThresh;

  // Van 2
  doc["v2_run"] = g_valves[1].isRunning ? 1 : 0;
  doc["v2_mode"] = g_valves[1].mode;
  doc["v2_time"] = g_valves[1].isRunning ? (millis() - g_valves[1].runStartTime) / 1000 : 0;
  doc["v2_err"] = g_valves[1].sensorError ? 1 : 0;
  doc["v2_dry"] = g_valves[1].dryThresh;
  doc["v2_wet"] = g_valves[1].wetThresh;

  // Khối môi trường & chế độ thủ công / tự động kèm các ngưỡng cài đặt
  doc["fan_mode"] = g_fanMode;
  doc["fan_speed"] = g_fanSpeed;
  doc["fan_pwm"] = g_fanPWM;
  doc["fan_sp"] = g_fanSetpoint;
  doc["mist"] = g_mistRunning ? 1 : 0;
  doc["mist_m"] = g_mistManual ? 1 : 0;
  doc["mist_th"] = g_mistHumidThresh;
  doc["light"] = g_lightRunning ? 1 : 0;
  doc["light_m"] = g_lightManual ? 1 : 0;
  doc["light_th"] = g_lightThresh;
  doc["heater"] = g_heaterRunning ? 1 : 0;
  doc["heater_m"] = g_heaterManual ? 1 : 0;
  doc["heater_pwm"] = g_heaterPWM;
  doc["heater_sp"] = heaterSetpoint;
  doc["roof"] = g_roofAngle;
  doc["roof_m"] = g_roofManual ? 1 : 0;

  doc["s_ok"] = g_sht30OK ? 1 : 0;
  doc["se_ok"] = g_sht30ExtOK ? 1 : 0;
  doc["b_ok"] = g_bh1750OK ? 1 : 0;
  doc["uptime"] = millis() / 1000;

  char buf[768];
  size_t len = serializeJson(doc, buf);
  buf[len] = '\n';
  buf[len + 1] = '\0';
  LoRaSerial.print(buf);
  Serial.printf("[LoRa Tx] Da phat status sang Gateway (%d bytes)\n", (int)len);
}

void sendValveConfigToGateway() {
  StaticJsonDocument<256> doc;
  doc["type"] = "cfg";
  doc["v1_dry"] = g_valves[0].dryThresh;
  doc["v1_wet"] = g_valves[0].wetThresh;
  doc["v1_to"]  = g_valves[0].maxDuration;
  doc["v1_rest"]= g_valves[0].minRestTime;
  doc["v2_dry"] = g_valves[1].dryThresh;
  doc["v2_wet"] = g_valves[1].wetThresh;
  doc["v2_to"]  = g_valves[1].maxDuration;
  doc["v2_rest"]= g_valves[1].minRestTime;

  char buf[256];
  size_t len = serializeJson(doc, buf);
  buf[len] = '\n';
  buf[len + 1] = '\0';
  LoRaSerial.print(buf);
  Serial.printf("[LoRa Tx] Da phat valve config sang Gateway (%d bytes)\n", (int)len);
}

void sendForecastToGateway() {
  if (!g_forecast_ready)
    return;
  StaticJsonDocument<384> doc;
  doc["type"] = "forecast";
  JsonArray arrT = doc.createNestedArray("temp");
  JsonArray arrH = doc.createNestedArray("humid");
  JsonArray arrL = doc.createNestedArray("lux");
  for (int h = 0; h < FC_HORIZONS; h++) {
    arrT.add(isnan(g_fc_temp[h]) ? 0 : round(g_fc_temp[h] * 10) / 10.0f);
    arrH.add(isnan(g_fc_humid[h]) ? 0 : round(g_fc_humid[h] * 10) / 10.0f);
    arrL.add(isnan(g_fc_lux[h]) ? 0 : (int)g_fc_lux[h]);
  }
  char buf[384];
  size_t len = serializeJson(doc, buf);
  buf[len] = '\n';
  buf[len + 1] = '\0';
  LoRaSerial.print(buf);
}

void checkLoRaCommand() {
  static String rawBuf = "";
  static int braceCount = 0;
  static bool inJson = false;
  static unsigned long lastByteTime = 0;

  // Timeout an toàn: Nếu quá 1500ms kể từ byte cuối mà JSON chưa đóng ngoặc }, tự động hủy buffer lỗi để không bị deadlock
  if (inJson && (millis() - lastByteTime > 1500)) {
    Serial.printf("[LoRa RX Timeout] Huy bo goi tin loi/thieu ngoac: %s\n", rawBuf.c_str());
    rawBuf = "";
    braceCount = 0;
    inJson = false;
  }

  while (LoRaSerial.available()) {
    char c = LoRaSerial.read();
    lastByteTime = millis();

    // Bắt đầu JSON khi gặp '{' (bỏ qua mọi byte rác / nhiễu vô tuyến phía trước)
    if (c == '{') {
      if (!inJson) {
        rawBuf = "";
        braceCount = 0;
        inJson = true;
      }
      braceCount++;
    }

    if (inJson) {
      rawBuf += c;

      if (c == '}') {
        braceCount--;

        // Gói JSON hoàn chỉnh khi số dấu { và } cân bằng
        if (braceCount <= 0) {
          inJson = false;
          rawBuf.trim();

          Serial.printf("[LoRa RX RAW] Nhan: %s\n", rawBuf.c_str());

          StaticJsonDocument<384> doc;
          DeserializationError error = deserializeJson(doc, rawBuf);
          if (error) {
            Serial.printf("[LoRa RX] JSON loi: %s | buf: %s\n", error.c_str(), rawBuf.c_str());
          } else if (doc.containsKey("cmd")) {
            String cmd = doc["cmd"].as<String>();
            Serial.printf("[LoRa RX] Nhan lenh: %s\n", cmd.c_str());

            // ─── Lệnh điều khiển Van tưới mới ───
            if (cmd == "valve_set") {
              int id = doc["id"] | 1;
              int mode = doc["mode"] | 1;
              setValveMode(id - 1, mode);
              saveValvesConfig();
              Serial.printf("[LoRa RX] Van %d -> Mode %d\n", id, mode);
            } else if (cmd == "valve_cfg") {
              int id = doc["id"] | 1;
              int idx = id - 1;
              if (idx >= 0 && idx < NUM_VALVES) {
                if (doc.containsKey("name")) strncpy(g_valves[idx].name, doc["name"], sizeof(g_valves[idx].name) - 1);
                if (doc.containsKey("dry")) g_valves[idx].dryThresh = doc["dry"];
                if (doc.containsKey("wet")) g_valves[idx].wetThresh = doc["wet"];
                if (doc.containsKey("to")) g_valves[idx].maxDuration = doc["to"];
                if (doc.containsKey("rest")) g_valves[idx].minRestTime = doc["rest"];
                saveValvesConfig();
                sendValveConfigToGateway();
              }
            } else if (cmd == "valve_cal") {
              int id = doc["id"] | 1;
              int idx = id - 1;
              if (idx >= 0 && idx < NUM_VALVES) {
                if (doc.containsKey("dry_raw")) g_valves[idx].dryRaw = doc["dry_raw"];
                if (doc.containsKey("wet_raw")) g_valves[idx].wetRaw = doc["wet_raw"];
                saveValvesConfig();
              }
            } else if (cmd == "pump_emg") {
              triggerEmergencyStop();
              Serial.println(F("[LoRa RX] DUNG KHAN CAP BOM & VAN!"));
            } else if (cmd == "pump_emg_reset") {
              resetEmergencyStop();
              Serial.println(F("[LoRa RX] KHOI PHUC KHAN CAP BOM & VAN"));
            } else if (cmd == "pump_lock") {
              g_pumpEnabled = true;
              saveValvesConfig();
              Serial.println(F("[LoRa RX] Khoa bom: CHO PHEP"));
            } else if (cmd == "pump_seq") {
              g_sequentialIrrigation = (doc["val"] != 0);
              saveValvesConfig();
              Serial.printf("[LoRa RX] Tuoi tuan tu: %s\n", g_sequentialIrrigation ? "BAT" : "TAT");
            }

            // ─── Lệnh Van tưới cũ (tương thích ngược) ───
            else if (cmd == "valve1_on") { setValveMode(0, VALVE_MODE_ON); saveValvesConfig(); }
            else if (cmd == "valve1_off") { setValveMode(0, VALVE_MODE_OFF); saveValvesConfig(); }
            else if (cmd == "valve2_on") { setValveMode(1, VALVE_MODE_ON); saveValvesConfig(); }
            else if (cmd == "valve2_off") { setValveMode(1, VALVE_MODE_OFF); saveValvesConfig(); }
            else if (cmd == "valve_auto") { setValveMode(0, VALVE_MODE_AUTO); setValveMode(1, VALVE_MODE_AUTO); saveValvesConfig(); }
            else if (cmd == "pump_on") { setPumpManual(true); }
            else if (cmd == "pump_off") { setPumpManual(false); }
            else if (cmd == "pump_auto") { setValveMode(0, VALVE_MODE_AUTO); setValveMode(1, VALVE_MODE_AUTO); saveValvesConfig(); }

            // ─── Quạt ───
            else if (cmd == "fan") {
              if (doc.containsKey("mode"))
                setFanMode((int)doc["mode"]);
              if (doc.containsKey("speed") && g_fanMode == FAN_MODE_MANUAL)
                setFanManualSpeed((int)doc["speed"]);
              if (doc.containsKey("setpoint")) {
                setFanSetpoint(doc["setpoint"]);
                saveConfig();
              }
              Serial.printf("[LoRa RX] Quat -> Mode=%d, Speed=%d, Setpoint=%.1f\n", g_fanMode, g_fanSpeed, g_fanSetpoint);
            } else if (cmd == "config") {
              if (doc.containsKey("fan_sp"))
                setFanSetpoint(doc["fan_sp"]);
              if (doc.containsKey("light_th"))
                g_lightThresh = doc["light_th"];
              if (doc.containsKey("mist_th"))
                g_mistHumidThresh = doc["mist_th"];
              if (doc.containsKey("pump_on"))
                g_pumpOnThresh = doc["pump_on"];
              if (doc.containsKey("pump_off"))
                g_pumpOffThresh = doc["pump_off"];
              saveConfig();
              Serial.println(F("[LoRa RX] Luu cau hinh nguong"));
            }

            // ─── Phun sương ───
            else if (cmd == "mist_on") {
              setMistManual(true);
              Serial.printf("[LoRa RX] Phun suong -> %s (GPIO %d)\n", g_mistRunning ? "BAT" : "TAT", MIST_PIN);
            } else if (cmd == "mist_off") {
              setMistManual(false);
              Serial.printf("[LoRa RX] Phun suong -> %s\n", g_mistRunning ? "BAT" : "TAT");
            } else if (cmd == "mist_auto") {
              g_mistManual = false;
              Serial.println(F("[LoRa RX] Phun suong -> AUTO"));
            }

            // ─── Đèn chiếu sáng ───
            else if (cmd == "light_on") {
              setLightManual(true);
              Serial.printf("[LoRa RX] Den chieu sang -> %s (GPIO %d)\n", g_lightRunning ? "BAT" : "TAT", LIGHT_PIN);
            } else if (cmd == "light_off") {
              setLightManual(false);
              Serial.printf("[LoRa RX] Den chieu sang -> %s\n", g_lightRunning ? "BAT" : "TAT");
            } else if (cmd == "light_auto") {
              g_lightManual = false;
              Serial.println(F("[LoRa RX] Den chieu sang -> AUTO"));
            }

            // ─── Sưởi ấm ───
            else if (cmd == "heater_on") {
              setHeaterManual(true);
              Serial.printf("[LoRa RX] Suoi am -> %s (PWM %d)\n", g_heaterRunning ? "BAT" : "TAT", g_heaterPWM);
            } else if (cmd == "heater_off") {
              setHeaterManual(false);
              Serial.println(F("[LoRa RX] Suoi am -> TAT"));
            } else if (cmd == "heater_auto") {
              g_heaterManual = false;
              Serial.println(F("[LoRa RX] Suoi am -> AUTO"));
            } else if (cmd == "heater") {
              if (doc.containsKey("speed")) {
                setHeaterManualSpeed(doc["speed"]);
                Serial.printf("[LoRa RX] Suoi am -> Muc PWM %d\n", g_heaterPWM);
              }
            } else if (cmd == "heater_cfg") {
              if (doc.containsKey("setpoint")) {
                setHeaterSetpoint(doc["setpoint"]);
                saveConfig();
                Serial.printf("[LoRa RX] Suoi am -> Setpoint=%.1f\n", heaterSetpoint);
              }
            }

            // ─── Mái che ───
            else if (cmd == "roof_open") {
              setRoofManual(180);
              Serial.println(F("[LoRa RX] Rem cua -> MO (180 deg)"));
            } else if (cmd == "roof_close") {
              setRoofManual(0);
              Serial.println(F("[LoRa RX] Rem cua -> DONG (0 deg)"));
            } else if (cmd == "roof_auto") {
              g_roofManual = false;
              Serial.println(F("[LoRa RX] Rem cua -> AUTO"));
            } else if (cmd == "roof_set") {
              if (doc.containsKey("angle")) {
                setRoofManual(doc["angle"].as<int>());
                Serial.printf("[LoRa RX] Rem cua -> %d deg\n", g_roofAngle);
              }
            }

            updateFan();
            updateIrrigation();
            updateMist();
            updateLight();
            updateHeater();
            updateRoof();

            // Cập nhật phản hồi ngay cho màn hình LCD và LED tủ HMI
            sendHMIData();

            // Đồng bộ ngay lập tức sang Gateway để Web Dashboard cập nhật tức thì
            sendStateEventToGateway();
          }
          rawBuf = "";
          braceCount = 0;
          inJson = false;
        }
      }

      if (rawBuf.length() > 384) {
        rawBuf = "";
        braceCount = 0;
        inJson = false;
      }
    }
  }
}
