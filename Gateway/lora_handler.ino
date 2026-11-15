// ═════════════════════════════════════════════════════════════
//  GATEWAY LORA HANDLER - GIAO TIẾP KHÔNG DÂY VỚI MASTER NODE
//  - Thu nhận định kỳ gói tin 'status' (10s), 'state' và 'forecast' (5 phút)
//  - Ghép nối buffer dữ liệu lớn theo cặp ngoặc nhọn JSON {}
//  - Cơ chế Timeout 1500ms dọn đệm chống kẹt byte rác
//  - Chống dội trạng thái (Debounce) khi vừa gửi lệnh điều khiển
//  - Smart Retry: Phát lệnh điều khiển từ API Web xuống Master Node qua sóng LoRa
// ═════════════════════════════════════════════════════════════

void processLoRaPacket(String rawJson) {
  StaticJsonDocument<2048> doc;
  DeserializationError error = deserializeJson(doc, rawJson);

  if (error) {
    Serial.println("[LoRa] JSON parse failed: " + String(error.c_str()) +
                   " (len=" + String(rawJson.length()) + " B)");
    if (rawJson.length() > 450) {
      Serial.println(
          F("[LoRa] CANH BAO: Goi tin >450B bi rot byte o giua do E32 tran bo "
            "dem 512B! Can nap code toi uu (<400B) cho Master Node!"));
    }
    return;
  }

  if (!doc.containsKey("type"))
    return;
  String type = doc["type"].as<String>();

  g_lastLoraReceive = millis();
  if (type == "state") {
    g_cmdAckPending = false; // Đã nhận gói tin phản hồi tức thì 'state' từ Master Node -> Hủy cờ chờ phát lại
    g_cmdRetryCount = 0;
  }

  if (type == "state" || type == "status") {
    // Nếu vừa gửi lệnh điều khiển trong vòng 3.5s:
    // CHỈ cho phép gói 'state' (phản hồi sự kiện tức thì từ Master) cập nhật thiết bị.
    // Bỏ qua việc ghi đè thiết bị từ gói 'status' định kỳ cũ để chống giật dội trạng thái trên Web.
    bool allowActuatorUpdate = (type == "state") || (millis() - g_lastCommandTime >= 3500);

    if (allowActuatorUpdate) {
      if (doc.containsKey("light"))
        g_lightRunning = doc["light"].as<bool>();
      if (doc.containsKey("light_m"))
        g_lightManual = doc["light_m"].as<bool>();

      if (doc.containsKey("mist"))
        g_mistRunning = doc["mist"].as<bool>();
      if (doc.containsKey("mist_m"))
        g_mistManual = doc["mist_m"].as<bool>();

      if (doc.containsKey("heater"))
        g_heaterRunning = doc["heater"].as<bool>();
      if (doc.containsKey("heater_m"))
        g_heaterManual = doc["heater_m"].as<bool>();
      if (doc.containsKey("heater_pwm"))
        g_heaterPWM = (int)doc["heater_pwm"];

      if (doc.containsKey("roof"))
        g_roofAngle = doc["roof"] | 0;
      if (doc.containsKey("roof_m"))
        g_roofManual = doc["roof_m"].as<bool>();

      if (doc.containsKey("fan_mode"))
        g_fanMode = doc["fan_mode"] | 1;
      if (doc.containsKey("fan_speed"))
        g_fanSpeed = doc["fan_speed"] | 0;
      else if (doc.containsKey("fan_spd"))
        g_fanSpeed = doc["fan_spd"] | 0;
      if (doc.containsKey("fan_pwm"))
        g_fanPWM = (int)doc["fan_pwm"];
      else if (doc.containsKey("fan_speed") || doc.containsKey("fan_spd"))
        g_fanPWM = map(g_fanSpeed, 0, 100, 0, 255);

      if (doc.containsKey("pump"))
        g_pumpRunning = doc["pump"].as<bool>();
      if (doc.containsKey("p_en"))
        g_pumpEnabled = doc["p_en"].as<bool>();
      if (doc.containsKey("p_emg"))
        g_pumpEmergencyStop = doc["p_emg"].as<bool>();

      if (doc.containsKey("v1_run")) {
        g_v1_run = doc["v1_run"].as<bool>();
        g_valve1Running = g_v1_run;
      }
      if (doc.containsKey("v1_mode"))
        g_v1_mode = doc["v1_mode"] | 1;

      if (doc.containsKey("v2_run")) {
        g_v2_run = doc["v2_run"].as<bool>();
        g_valve2Running = g_v2_run;
      }
      if (doc.containsKey("v2_mode"))
        g_v2_mode = doc["v2_mode"] | 1;
    }

    if (type == "state") {
      bool cfgChanged = false;
      if (doc.containsKey("heater_sp")) {
        float hsp = doc["heater_sp"];
        if (g_heaterSetpoint != hsp && hsp > 0) {
          g_heaterSetpoint = hsp;
          cfgChanged = true;
        }
      }
      if (doc.containsKey("fan_sp")) {
        float fsp = doc["fan_sp"];
        if (g_fanSetpoint != fsp && fsp > 0) {
          g_fanSetpoint = fsp;
          cfgChanged = true;
        }
      }
      if (doc.containsKey("v1_dry")) {
        float vd = doc["v1_dry"];
        if (g_v1_dry != vd && vd > 0) { g_v1_dry = vd; cfgChanged = true; }
      }
      if (doc.containsKey("v1_wet")) {
        float vw = doc["v1_wet"];
        if (g_v1_wet != vw && vw > 0) { g_v1_wet = vw; cfgChanged = true; }
      }
      if (doc.containsKey("v2_dry")) {
        float vd2 = doc["v2_dry"];
        if (g_v2_dry != vd2 && vd2 > 0) { g_v2_dry = vd2; cfgChanged = true; }
      }
      if (doc.containsKey("v2_wet")) {
        float vw2 = doc["v2_wet"];
        if (g_v2_wet != vw2 && vw2 > 0) { g_v2_wet = vw2; cfgChanged = true; }
      }
      if (cfgChanged) {
        saveGatewayConfig();
      }
      Serial.printf("[LoRa] State Sync OK: Light=%d (m=%d) Mist=%d (m=%d) Heat=%d Fan=%d Roof=%d Pump=%d HeaterSP=%.1f V1Dry=%.0f V1Wet=%.0f\n",
                    g_lightRunning, g_lightManual, g_mistRunning, g_mistManual,
                    g_heaterRunning, g_fanMode, g_roofAngle, g_pumpRunning, g_heaterSetpoint, g_v1_dry, g_v1_wet);
      return; // Gói state chỉ chứa cơ cấu chấp hành, xử lý xong return ngay
    }

    // Các trường cảm biến (chỉ có trong gói type == "status")
    g_temperature = doc["temp"] | 0.0f;
    g_humidity = doc["humid"] | 0.0f;
    g_tempExt = doc.containsKey("te") ? (float)doc["te"] : (doc["temp_ext"] | 0.0f);
    g_humidExt = doc.containsKey("he") ? (float)doc["he"] : (doc["humid_ext"] | 0.0f);
    g_lightLux = doc["lux"] | 0.0f;

    g_soilMoisture = doc["soil"] | 0.0f;
    g_soilMoisture2 = doc["soil2"] | 0.0f;

    g_pumpTime = doc.containsKey("p_t") ? doc["p_t"].as<unsigned long>() : (doc["p_time"] | 0UL);
    g_pumpSeq = doc.containsKey("p_seq") ? doc["p_seq"].as<bool>() : true;

    g_v1_soil = doc.containsKey("v1_soil") ? (float)doc["v1_soil"] : g_soilMoisture;
    g_v1_time = doc.containsKey("v1_t") ? doc["v1_t"].as<unsigned long>() : (doc["v1_time"] | 0UL);
    g_v1_err = doc.containsKey("v1_err") ? doc["v1_err"].as<bool>() : false;
    if (doc.containsKey("v1_name"))
      g_v1_name = doc["v1_name"].as<String>();

    g_v2_soil = doc.containsKey("v2_soil") ? (float)doc["v2_soil"] : g_soilMoisture2;
    g_v2_time = doc.containsKey("v2_t") ? doc["v2_t"].as<unsigned long>() : (doc["v2_time"] | 0UL);
    g_v2_err = doc.containsKey("v2_err") ? doc["v2_err"].as<bool>() : false;
    if (doc.containsKey("v2_name"))
      g_v2_name = doc["v2_name"].as<String>();

    if (doc.containsKey("fan_pwm"))
      g_fanPWM = (int)doc["fan_pwm"];
    else
      g_fanPWM = map(g_fanSpeed, 0, 100, 0, 255);
    bool cfgChanged = false;
    if (doc.containsKey("fan_sp")) {
      float fsp = doc["fan_sp"];
      if (millis() - g_lastCommandTime >= 15000) {
        if (g_fanSetpoint != fsp && fsp > 0) {
          g_fanSetpoint = fsp;
          cfgChanged = true;
        }
      }
    }

    if (doc.containsKey("light_th")) {
      if (g_lightThresh != (float)doc["light_th"]) {
        g_lightThresh = doc["light_th"];
        cfgChanged = true;
      }
    }

    if (doc.containsKey("mist_th")) {
      if (g_mistHumidThresh != (float)doc["mist_th"]) {
        g_mistHumidThresh = doc["mist_th"];
        cfgChanged = true;
      }
    }

    if (cfgChanged) {
      saveGatewayConfig();
    }

    // Chú ý: Dùng .as<bool>() để chuyển đổi chính xác integer 1/0 sang boolean trong ArduinoJson
    g_sht30OK = doc.containsKey("s_ok") ? doc["s_ok"].as<bool>() : false;
    g_sht30ExtOK = doc.containsKey("se_ok") ? doc["se_ok"].as<bool>() : false;
    g_bh1750OK = doc.containsKey("b_ok") ? doc["b_ok"].as<bool>() : false;

    g_nodeUptime = doc.containsKey("up") ? doc["up"].as<unsigned long>() : (doc["uptime"] | 0UL);

    Serial.printf(
        "[LoRa] Status OK: T=%.1f H=%.1f L=%.0f Soil=%.1f Mist=%d (TH=%.0f) Light=%d (TH=%.0f) FanSP=%.1f (SHT_OK=%d SHT_Ext=%d)\n",
        g_temperature, g_humidity, g_lightLux, g_soilMoisture, g_mistRunning,
        g_mistHumidThresh, g_lightRunning, g_lightThresh, g_fanSetpoint,
        g_sht30OK, g_sht30ExtOK);
  } else if (type == "cfg") {
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
    saveGatewayConfig();
    Serial.println(F("[LoRa] Da dong bo va luu cau hinh van tu Master Node ✓"));
  } else if (type == "forecast") {
    JsonArray arrT = doc["temp"];
    JsonArray arrH = doc["humid"];
    JsonArray arrL = doc["lux"];
    for (int i = 0; i < 3; i++) {
      g_fc_temp[i] = arrT[i];
      g_fc_humid[i] = arrH[i];
      g_fc_lux[i] = arrL[i];
    }
    g_forecast_ready = true;
    Serial.println(F("[LoRa] Forecast OK"));
  }
}

// ─── LỌC GÓI NHỊ PHÂN CỦA MẠNG CẢM BIẾN (WSN) LẪN VÀO LUỒNG JSON ───
// Gateway cùng kênh LoRa nên cũng nghe được gói SensorPacket (13B, header 0xAA)
// của Sensor Node và gói ACK (4B, header 0x55) mà Master Node trả lời Node.
// Gói Sensor Node thường chen vào giữa 2 block 54B của gói status từ Master
// -> làm hỏng JSON. Bỏ qua trọn gói nhị phân này trước khi ghép JSON.
#define WSN_SENSOR_HEADER 0xAA
#define WSN_SENSOR_PKT_LEN 13
#define WSN_ACK_HEADER 0x55
#define WSN_ACK_PKT_LEN 4

static void skipLoRaBytes(int n) {
  unsigned long t0 = millis();
  while (n > 0 && millis() - t0 < 50) {
    if (LoRaSerial.available()) {
      LoRaSerial.read();
      n--;
    }
  }
}

void checkLoRaMessage() {
  static String rawBuf = "";
  static int braceCount = 0;
  static bool inJson = false;
  static unsigned long lastPrint = 0;
  static unsigned long lastWarn = 0;
  static unsigned long lastCharTime = 0;

  // Timeout dọn đệm nếu nhận dở dang quá 3000ms (cho phép các block LoRa truyền tới)
  if (inJson && (millis() - lastCharTime > 3000)) {
    Serial.println("[LoRa RX Timeout] Huy bo: " + rawBuf);
    rawBuf = "";
    braceCount = 0;
    inJson = false;
  }

  while (LoRaSerial.available()) {
    char c = LoRaSerial.read();
    lastPrint = millis();
    lastCharTime = millis();

    // Gói SensorPacket (0xAA) có thể chen giữa JSON; gói ACK (0x55) chỉ nằm ngoài JSON
    if ((uint8_t)c == WSN_SENSOR_HEADER) {
      skipLoRaBytes(WSN_SENSOR_PKT_LEN - 1);
      continue;
    }
    if (!inJson && (uint8_t)c == WSN_ACK_HEADER) {
      skipLoRaBytes(WSN_ACK_PKT_LEN - 1);
      continue;
    }

    // Bắt đầu JSON khi gặp '{' (bỏ qua mọi byte rác / nhiễu vô tuyến phía trước)
    if (c == '{') {
      if (!inJson) {
        rawBuf = "";
        braceCount = 0;
        inJson = true;
      }
      braceCount++;
    }

    // Lọc nhiễu từ thiết bị LoRa lạ cùng kênh: dạng "N1,T=0,R=0,...\n" chen giữa JSON.
    // JSON của Master không bao giờ chứa '\n' bên trong -> gặp '\n' khi đang ghép JSON
    // nghĩa là có đoạn lạ: cắt từ vị trí "N<so>," gần nhất tới hết.
    if (inJson && (c == '\n' || c == '\r')) {
      int cut = -1;
      for (int i = rawBuf.length() - 2; i >= 0; i--) {
        if (rawBuf[i] == 'N' && isDigit(rawBuf[i + 1])) { cut = i; break; }
      }
      if (cut >= 0) {
        Serial.println("[LoRa] Loai bo nhieu la: " + rawBuf.substring(cut));
        rawBuf.remove(cut);
      }
      continue;
    }

    if (inJson) {
      rawBuf += c;

      if (c == '}') {
        braceCount--;

        // Gói JSON hoàn chỉnh khi số dấu { và } cân bằng
        if (braceCount <= 0) {
          inJson = false;
          rawBuf.trim();
          Serial.println("[LoRa RAW] " + rawBuf);
          processLoRaPacket(rawBuf);
          rawBuf = "";
          braceCount = 0;
        }
      }

      // Bảo vệ tràn bộ đệm (chống nhiễu kéo dài không ngắt)
      if (rawBuf.length() > 1500) {
        rawBuf = "";
        braceCount = 0;
        inJson = false;
      }
    }
  }

  // Nếu 10 giây không nhận được byte nào từ Node
  if (millis() - lastPrint > 10000 && millis() - lastWarn > 15000) {
    lastWarn = millis();
    if (g_lastLoraReceive == 0 && millis() > 25000) {
      Serial.println(F("[LoRa] WARN: Khong nhan duoc du lieu tu Node! Kiem tra day noi TX/RX va M0/M1=GND."));
    }
  }
}

// Hàm gửi lệnh điều khiển từ API xuống Node
void sendLoRaCommand(String cmdJson, bool isNewCmd) {
  cmdJson.trim();
  cmdJson += "\n";
  LoRaSerial.print(cmdJson);
  LoRaSerial.flush();
  Serial.print("[LoRa Tx] " + cmdJson);
  g_lastCommandTime = millis();

  if (isNewCmd) {
    g_lastSentCmd = cmdJson;
    g_lastSentCmd.trim();
    g_lastCmdSentTime = millis();
    g_cmdAckPending   = true;
    g_cmdRetryCount   = 0;
  } else {
    g_lastCmdSentTime = millis();
  }
}

// Hàm overload 1 tham số để tương thích gọi ở mọi nơi
void sendLoRaCommand(String cmdJson) {
  sendLoRaCommand(cmdJson, true);
}
