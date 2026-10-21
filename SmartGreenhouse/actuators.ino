// ═════════════════════════════════════════════════════════════
//  SMART GREENHOUSE - QUẢN LÝ CƠ CẤU CHẤP HÀNH (ACTUATORS)
//  - Quạt DC 12V: Điều khiển PWM 20kHz qua Module MOSFET D4184 (PID Nhiệt độ)
//  - Bóng sưởi: Điều khiển PWM 1kHz qua Module MOSFET D4184 (Công suất nhiệt)
//  - Bơm phun sương ẩm & Đèn quang hợp: Đóng ngắt qua MOSFET
//  - Mái che Servo: Điều khiển góc mở 0° - 180° qua xung PWM 50Hz
//  - Hệ thống tưới thông minh (Bơm & 2 Van liên động an toàn):
//    + Hysteresis độ ẩm đất riêng cho từng van
//    + Trễ an toàn 1.5s (Interlock Delay) chống búa nước và quá áp vỡ ống
//    + Giới hạn an toàn: Timeout 300s tối đa, thời gian nghỉ tối thiểu 60s
//    + Chức năng ngắt khẩn cấp (Emergency Stop)
// ═════════════════════════════════════════════════════════════

void initFan() {
  // Điều khiển bằng Module MOSFET D4184 (Không cần chân đảo chiều AIN1, AIN2)
  ledcAttach(FAN_PIN, FAN_PWM_FREQ, FAN_PWM_RES);
  ledcWrite(FAN_PIN, 0);

  fanPID.SetOutputLimits(0, FAN_PWM_MAX);
  fanPID.SetSampleTime(SENSOR_INTERVAL);
  fanPID.SetMode(AUTOMATIC);

  g_fanMode = FAN_MODE_AUTO;
  g_fanPWM = 0;
  g_fanRunning = false;
}

void updateFan() {
  // 1. Chế độ BẬT THỦ CÔNG (Manual): Giữ nguyên tốc độ quạt tay, không bị phụ
  // thuộc cảm biến
  if (g_fanMode == FAN_MODE_MANUAL) {
    setFanPWM(g_manualPWM);
    return;
  }

  // 2. Chế độ TẮT THỦ CÔNG: Tắt quạt
  if (g_fanMode == FAN_MODE_OFF) {
    setFanPWM(0);
    return;
  }

  // 3. Chế độ TỰ ĐỘNG (AUTO): Cần cảm biến nhiệt độ SHT30 để chạy thuật toán
  // PID
  if (!g_sht30OK || isnan(g_temperature)) {
    setFanPWM(0);
    return;
  }

  pidInput = (double)g_temperature;
  pidSetpoint = (double)g_fanSetpoint;
  if (g_temperature <= (g_fanSetpoint - FAN_DEADBAND)) {
    fanPID.SetMode(MANUAL);
    pidOutput = 0;
    setFanPWM(0);
    fanPID.SetMode(AUTOMATIC);
  } else {
    fanPID.Compute();
    int pwm = (int)pidOutput;
    if (pwm > 0 && pwm < FAN_PWM_MIN)
      pwm = FAN_PWM_MIN;
    setFanPWM(pwm);
  }
}

void setFanPWM(int pwm) {
  pwm = constrain(pwm, 0, FAN_PWM_MAX);
  ledcWrite(FAN_PIN, pwm);
  g_fanPWM = pwm;
  if (pwm == 0)
    g_fanSpeed = 0;
  else if (pwm <= 125)
    g_fanSpeed = 40;
  else if (pwm <= 180)
    g_fanSpeed = 60;
  else if (pwm <= 230)
    g_fanSpeed = 80;
  else
    g_fanSpeed = 100;
  g_fanRunning = (pwm > 0);
}

void setFanMode(int mode) {
  g_fanMode = mode;
  if (mode == FAN_MODE_OFF)
    setFanPWM(0);
  else if (mode == FAN_MODE_AUTO)
    fanPID.SetMode(AUTOMATIC);
}
void setFanManualSpeed(int pwm) {
  g_manualPWM = constrain(pwm, 0, FAN_PWM_MAX);
  g_fanMode = FAN_MODE_MANUAL;
  setFanPWM(g_manualPWM);
}
void setFanSetpoint(float sp) {
  g_fanSetpoint = sp;
  pidSetpoint = sp;
  heaterSetpoint = sp - 2.0;
}

void cycleFanHMI() {
  // Điều khiển bằng nút nhấn HMI -> Chuyển sang chế độ THỦ CÔNG (MANUAL)
  // Chu trình 4 mức PWM: Mức 1 (40% - PWM 102) -> Mức 2 (60% - PWM 153) -> Mức 3 (80% - PWM 204) -> Mức 4 (100% - PWM 255) -> TẮT (Mức 0)
  if (g_fanMode != FAN_MODE_MANUAL || !g_fanRunning || g_fanPWM == 0) {
    // Lần nhấn 1: Mức 1: 40% (PWM 102)
    setFanManualSpeed(102);
    Serial.println(F("[HMI] Quat -> Muc 1: 40% (PWM 102) [TAY]"));
  } else if (g_fanPWM < 125) {
    // Lần nhấn 2: Mức 2: 60% (PWM 153)
    setFanManualSpeed(153);
    Serial.println(F("[HMI] Quat -> Muc 2: 60% (PWM 153) [TAY]"));
  } else if (g_fanPWM < 180) {
    // Lần nhấn 3: Mức 3: 80% (PWM 204)
    setFanManualSpeed(204);
    Serial.println(F("[HMI] Quat -> Muc 3: 80% (PWM 204) [TAY]"));
  } else if (g_fanPWM < 230) {
    // Lần nhấn 4: Mức 4: 100% (PWM 255)
    setFanManualSpeed(255);
    Serial.println(F("[HMI] Quat -> Muc 4: 100% (PWM 255) [TAY]"));
  } else {
    // Lần nhấn 5: Mức 0: TẮT (PWM 0)
    setFanMode(FAN_MODE_OFF);
    Serial.println(F("[HMI] Quat -> Muc 0: TAT (0%)"));
  }
}

void initHeater() {
  // Điều khiển bằng Module MOSFET D4184 (Giải phóng hoàn toàn GPIO 12 & 13)
  ledcAttach(HEATER_PIN, HEATER_PWM_FREQ, FAN_PWM_RES);
  ledcWrite(HEATER_PIN, 0);

  heaterPID.SetOutputLimits(0, 255);
  heaterPID.SetSampleTime(SENSOR_INTERVAL);
  heaterPID.SetMode(AUTOMATIC);

  g_heaterRunning = false;
  g_heaterManual = false;
}

void updateHeater() {
  if (g_heaterManual)
    return;
  if (!g_sht30OK || isnan(g_temperature)) {
    setHeaterPWM(0);
    return;
  }

  pidInput = (double)g_temperature;
  heaterSetpoint = (double)g_fanSetpoint -
                   2.0; // Giữ khoảng cách 2 độ để không bị đụng độ với quạt

  if (g_temperature >= heaterSetpoint) {
    heaterPID.SetMode(MANUAL);
    heaterPidOutput = 0;
    setHeaterPWM(0);
    heaterPID.SetMode(AUTOMATIC);
  } else {
    heaterPID.Compute();
    int pwm = (int)heaterPidOutput;
    if (pwm > 0 && pwm < FAN_PWM_MIN)
      pwm = FAN_PWM_MIN;
    setHeaterPWM(pwm);
  }
}

void setHeaterPWM(int pwm) {
  pwm = constrain(pwm, 0, 255);
  ledcWrite(HEATER_PIN, pwm);
  g_heaterPWM = pwm;
  g_heaterRunning = (pwm > 0);
}

void setHeaterManual(bool on) {
  g_heaterManual = true;
  setHeaterPWM(on ? 255 : 0);
}

void setHeaterManualSpeed(int pwm) {
  g_heaterManual = true;
  setHeaterPWM(pwm);
}

void cycleHeaterHMI() {
  // Điều khiển bằng nút nhấn HMI -> Chuyển sang chế độ THỦ CÔNG (MANUAL)
  // Chu trình 4 mức PWM: Mức 1 (40% - PWM 102) -> Mức 2 (60% - PWM 153) -> Mức 3 (80% - PWM 204) -> Mức 4 (100% - PWM 255) -> TẮT (Mức 0)
  if (!g_heaterManual || !g_heaterRunning || g_heaterPWM == 0) {
    // Lần nhấn 1: Mức 1: 40% (PWM 102)
    setHeaterManualSpeed(102);
    Serial.println(F("[HMI] Suoi -> Muc 1: 40% (PWM 102) [TAY]"));
  } else if (g_heaterPWM < 125) {
    // Lần nhấn 2: Mức 2: 60% (PWM 153)
    setHeaterManualSpeed(153);
    Serial.println(F("[HMI] Suoi -> Muc 2: 60% (PWM 153) [TAY]"));
  } else if (g_heaterPWM < 180) {
    // Lần nhấn 3: Mức 3: 80% (PWM 204)
    setHeaterManualSpeed(204);
    Serial.println(F("[HMI] Suoi -> Muc 3: 80% (PWM 204) [TAY]"));
  } else if (g_heaterPWM < 230) {
    // Lần nhấn 4: Mức 4: 100% (PWM 255)
    setHeaterManualSpeed(255);
    Serial.println(F("[HMI] Suoi -> Muc 4: 100% (PWM 255) [TAY]"));
  } else {
    // Lần nhấn 5: Mức 0: TẮT (PWM 0)
    setHeaterManual(false);
    Serial.println(F("[HMI] Suoi -> Muc 0: TAT (0%)"));
  }
}

// (Khối Bơm & Van được hợp nhất trong hệ thống tưới thông minh bên dưới)
void initMist() {
  pinMode(MIST_PIN, OUTPUT);
  setMistState(false);
}

void updateMist() {
  if (g_mistManual)
    return;
  bool shouldRun = g_fanRunning;
  if (shouldRun && !g_mistRunning)
    setMistState(true);
  else if (!shouldRun && g_mistRunning)
    setMistState(false);
}

void setMistState(bool on) {
  digitalWrite(MIST_PIN, on ? (MIST_ACTIVE_HIGH ? HIGH : LOW)
                            : (MIST_ACTIVE_HIGH ? LOW : HIGH));
  g_mistRunning = on;
}

void setMistManual(bool on) {
  g_mistManual = true;
  setMistState(on);
}

void initLight() {
  pinMode(LIGHT_PIN, OUTPUT);
  setLightState(false);
}

void updateLight() {
  if (g_lightManual)
    return;
  if (!g_bh1750OK || isnan(g_lightLux))
    return;
  if (!g_lightRunning && g_lightLux < g_lightThresh)
    setLightState(true);
  else if (g_lightRunning && g_lightLux > (g_lightThresh + 200))
    setLightState(false);
}

void setLightState(bool on) {
  digitalWrite(LIGHT_PIN, on ? (LIGHT_ACTIVE_HIGH ? HIGH : LOW)
                             : (LIGHT_ACTIVE_HIGH ? LOW : HIGH));
  g_lightRunning = on;
}

void setLightManual(bool on) {
  g_lightManual = true;
  setLightState(on);
}

// ═══════════════════════════════════════════════════════════════
//  HỆ THỐNG TƯỚI THÔNG MINH (BƠM & CÁC VAN ĐIỀU KHIỂN ĐỘC LẬP)
//  - Trình tự an toàn: Mở van -> trễ 1.5s -> Bật bơm
//                      Tắt bơm -> trễ 1.5s -> Đóng van
//  - Tuyệt đối không để bơm chạy khi 0 van mở
//  - Hysteresis: độ ẩm < dryThresh mở van, > wetThresh đóng van
//  - Timeout bảo vệ cho từng van (kể cả bật tay)
//  - Hàng đợi tưới tuần tự ưu tiên vùng khô nhất
//  - Phát hiện lỗi cảm biến tự đóng van và cảnh báo
// ═══════════════════════════════════════════════════════════════

enum PumpInterlockState {
  PUMP_STATE_IDLE,         // Bơm tắt, không có van nào mở
  PUMP_STATE_START_DELAY,  // Van đã mở, đang đợi trễ 1.5s để bật bơm an toàn
  PUMP_STATE_RUNNING,      // Bơm đang chạy, có ít nhất 1 van mở
  PUMP_STATE_STOP_DELAY    // Bơm đã tắt, đang đợi trễ 1.5s để xả áp rồi mới đóng van cuối
};

PumpInterlockState g_interlockState = PUMP_STATE_IDLE;
unsigned long g_interlockTimer = 0;
int g_closingValveId = -1;

void initIrrigation() {
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, PUMP_ACTIVE_HIGH ? LOW : HIGH);
  g_pumpRunning = false;

  for (int i = 0; i < NUM_VALVES; i++) {
    pinMode(g_valves[i].pin, OUTPUT);
    digitalWrite(g_valves[i].pin, PUMP_ACTIVE_HIGH ? LOW : HIGH);
    g_valves[i].isRunning = false;
    g_valves[i].runStartTime = 0;
    g_valves[i].lastStopTime = 0;
  }

  g_interlockState = PUMP_STATE_IDLE;
  g_closingValveId = -1;
  Serial.println(F("[IRRIGATION] Khoi tao he thong tuoi thong minh (2 Van + 1 Bom) san sang."));
}

int countRunningValves() {
  int count = 0;
  for (int i = 0; i < NUM_VALVES; i++) {
    if (g_valves[i].isRunning) count++;
  }
  return count;
}

void openValveHardware(int idx) {
  if (idx < 0 || idx >= NUM_VALVES) return;
  digitalWrite(g_valves[idx].pin, PUMP_ACTIVE_HIGH ? HIGH : LOW);
  g_valves[idx].isRunning = true;
  g_valves[idx].runStartTime = millis();
  Serial.printf("[VALVE %d] MO: %s\n", idx + 1, g_valves[idx].name);
}

void closeValveHardware(int idx) {
  if (idx < 0 || idx >= NUM_VALVES) return;
  digitalWrite(g_valves[idx].pin, PUMP_ACTIVE_HIGH ? LOW : HIGH);
  g_valves[idx].isRunning = false;
  g_valves[idx].lastStopTime = millis();
  Serial.printf("[VALVE %d] DONG: %s (Da chay: %lus)\n",
                idx + 1, g_valves[idx].name,
                (millis() - g_valves[idx].runStartTime) / 1000);
}

void turnPumpOnHardware() {
  digitalWrite(PUMP_PIN, PUMP_ACTIVE_HIGH ? HIGH : LOW);
  g_pumpRunning = true;
  g_pumpRunStartTime = millis();
  Serial.println(F("[PUMP] >>> BAT BOM (Ap luc nuoc bat dau cap vao duong ong)"));
}

void turnPumpOffHardware() {
  digitalWrite(PUMP_PIN, PUMP_ACTIVE_HIGH ? LOW : HIGH);
  g_pumpRunning = false;
  Serial.println(F("[PUMP] <<< TAT BOM (Ngat dong dien bom)"));
}

void triggerEmergencyStop() {
  g_pumpEmergencyStop = true;
  turnPumpOffHardware();
  for (int i = 0; i < NUM_VALVES; i++) {
    closeValveHardware(i);
    g_valves[i].mode = VALVE_MODE_OFF;
  }
  g_interlockState = PUMP_STATE_IDLE;
  g_closingValveId = -1;
  Serial.println(F("[EMERGENCY STOP] DA NGAT TOAN BO BOM VA VAN TUOI!"));
}

void resetEmergencyStop() {
  g_pumpEmergencyStop = false;
  g_pumpEnabled = true;
  Serial.println(F("[EMERGENCY STOP] DA KHOI PHUC TRANG THAI BINH THUONG."));
}

void requestOpenValve(int idx) {
  if (idx < 0 || idx >= NUM_VALVES) return;
  if (g_valves[idx].isRunning) return;
  if (g_pumpEmergencyStop) return;

  // 1. Mở van vật lý trước để thông đường ống
  openValveHardware(idx);

  // 2. Kích hoạt trễ an toàn cho bơm nếu bơm chưa chạy
  if (!g_pumpRunning) {
    g_interlockState = PUMP_STATE_START_DELAY;
    g_interlockTimer = millis() + PUMP_INTERLOCK_DELAY;
    Serial.printf("[INTERLOCK] Van %d da mo -> Cho 1.5s de khoi dong bom an toan...\n", idx + 1);
  } else {
    g_interlockState = PUMP_STATE_RUNNING;
  }
}

void requestCloseValve(int idx) {
  if (idx < 0 || idx >= NUM_VALVES) return;
  if (!g_valves[idx].isRunning) return;
  // Tránh spam reset timer nếu van này đã đang trong quá trình chờ 1.5s xả áp
  if (g_interlockState == PUMP_STATE_STOP_DELAY && g_closingValveId == idx) return;

  int openCount = countRunningValves();
  if (openCount > 1) {
    // Vẫn còn van khác đang mở -> Đóng van này ngay, bơm tiếp tục cấp cho van còn lại
    closeValveHardware(idx);
  } else {
    // Đây là van cuối cùng! TẮT BƠM TRƯỚC -> CHỜ 1.5s xả áp -> MỚI ĐÓNG VAN
    turnPumpOffHardware();
    g_closingValveId = idx;
    g_interlockState = PUMP_STATE_STOP_DELAY;
    g_interlockTimer = millis() + PUMP_INTERLOCK_DELAY;
    Serial.printf("[INTERLOCK] Da tat bom -> Cho 1.5s xa ap truoc khi dong van %d...\n", idx + 1);
  }
}

void setValveMode(int idx, int mode) {
  if (idx < 0 || idx >= NUM_VALVES) return;
  g_valves[idx].mode = mode;

  if (mode == VALVE_MODE_OFF) {
    requestCloseValve(idx);
  } else if (mode == VALVE_MODE_ON) {
    if (!g_pumpEmergencyStop) {
      if (g_valves[idx].sensorError) {
        Serial.printf("[WARN] Van %d: Cam bien loi/chua cam, van mo theo che do THU CONG (bao ve timeout %ds)\n",
                      idx + 1, g_valves[idx].maxDuration);
      }
      requestOpenValve(idx);
    }
  }
}

void updateIrrigation() {
  unsigned long now = millis();

  // 1. Kiểm tra Dừng khẩn cấp
  if (g_pumpEmergencyStop) {
    if (g_pumpRunning) turnPumpOffHardware();
    for (int i = 0; i < NUM_VALVES; i++) {
      if (g_valves[i].isRunning) closeValveHardware(i);
    }
    g_interlockState = PUMP_STATE_IDLE;
    g_closingValveId = -1;
    return;
  }

  // 2. Xử lý Interlock Delay State Machine
  if (g_interlockState == PUMP_STATE_START_DELAY) {
    if (now >= g_interlockTimer) {
      if (countRunningValves() > 0) {
        turnPumpOnHardware();
        g_interlockState = PUMP_STATE_RUNNING;
      } else {
        g_interlockState = PUMP_STATE_IDLE;
      }
    }
  } else if (g_interlockState == PUMP_STATE_STOP_DELAY) {
    if (now >= g_interlockTimer) {
      if (g_closingValveId >= 0 && g_closingValveId < NUM_VALVES) {
        closeValveHardware(g_closingValveId);
        g_closingValveId = -1;
      }
      g_interlockState = PUMP_STATE_IDLE;
    }
  }

  // 3. Khóa an toàn: Tuyệt đối không để bơm chạy khi 0 van mở!
  if (g_pumpRunning && countRunningValves() == 0 && g_interlockState != PUMP_STATE_STOP_DELAY) {
    Serial.println(F("[SAFETY WARN] 0 van mo khi bom dang chay -> Ngat bom tuc thi!"));
    turnPumpOffHardware();
    g_interlockState = PUMP_STATE_IDLE;
  }

  // 4. Kiểm tra Timeout và Lỗi cảm biến cho các van đang mở
  for (int i = 0; i < NUM_VALVES; i++) {
    if (g_valves[i].isRunning) {
      // Bỏ qua kiểm tra nếu van này đã đang trong quá trình xả áp chờ đóng
      if (g_interlockState == PUMP_STATE_STOP_DELAY && g_closingValveId == i) {
        continue;
      }

      // 4a. Cảm biến bị lỗi trong chế độ AUTO: Tự đóng van an toàn để tránh ngập úng
      if (g_valves[i].mode == VALVE_MODE_AUTO && g_valves[i].sensorError) {
        Serial.printf("[SAFETY] Van %d (AUTO) cam bien loi -> Tu dong dong van!\n", i + 1);
        requestCloseValve(i);
        continue;
      }

      // 4b. Timeout bảo vệ tưới tối đa mỗi lần (cho cả AUTO và BẬT tay)
      if (now - g_valves[i].runStartTime >= (unsigned long)g_valves[i].maxDuration * 1000UL) {
        Serial.printf("[TIMEOUT] Van %d dat gioi han thoi gian %ds -> Tu dong dong van!\n",
                      i + 1, g_valves[i].maxDuration);
        requestCloseValve(i);
        if (g_valves[i].mode == VALVE_MODE_ON) {
          g_valves[i].mode = VALVE_MODE_AUTO; // Trả về AUTO sau timeout
          saveValvesConfig();
        }
        continue;
      }

      // 4c. Trong chế độ AUTO: Hysteresis (Độ ẩm >= wetThresh thì đóng van)
      if (g_valves[i].mode == VALVE_MODE_AUTO) {
        if (!isnan(g_valves[i].currentSoil) && g_valves[i].currentSoil >= g_valves[i].wetThresh) {
          Serial.printf("[AUTO] Van %d dat do am %.1f%% (>= %.1f%%) -> Dung tuoi!\n",
                        i + 1, g_valves[i].currentSoil, g_valves[i].wetThresh);
          requestCloseValve(i);
          continue;
        }
      }
    }
  }

  // 5. Trong chế độ AUTO: Kiểm tra các van cần mở (Độ ẩm < Ngưỡng khô)
  // Khóa ổn định khởi động: Trong 15 giây đầu sau khi bật nguồn, chỉ đo lấy mẫu baseline, tuyệt đối không tự kích hoạt tưới AUTO
  if (now < 15000) {
    return;
  }

  if (g_interlockState == PUMP_STATE_IDLE || g_interlockState == PUMP_STATE_RUNNING) {
    if (g_sequentialIrrigation) {
      // CHẾ ĐỘ TƯỚI LẦN LƯỢT: Nếu đang có van chạy thì đợi xong mới tới van kế tiếp
      if (countRunningValves() == 0) {
        int bestCandidate = -1;
        float lowestMoisture = 999.0f;

        for (int i = 0; i < NUM_VALVES; i++) {
          if (g_valves[i].mode == VALVE_MODE_AUTO && !g_valves[i].sensorError && !isnan(g_valves[i].currentSoil)) {
            if (g_valves[i].currentSoil < g_valves[i].dryThresh) {
              if (now - g_valves[i].lastStopTime >= (unsigned long)g_valves[i].minRestTime * 1000UL) {
                if (g_valves[i].currentSoil < lowestMoisture) {
                  lowestMoisture = g_valves[i].currentSoil;
                  bestCandidate = i;
                }
              }
            }
          }
        }

        if (bestCandidate >= 0) {
          Serial.printf("[AUTO QUEUE] Uu tien tuoi vung kho nhat: Van %d (%.1f%%)\n",
                        bestCandidate + 1, lowestMoisture);
          requestOpenValve(bestCandidate);
        }
      }
    } else {
      // CHẾ ĐỘ TƯỚI ĐỒNG THỜI
      for (int i = 0; i < NUM_VALVES; i++) {
        if (g_valves[i].mode == VALVE_MODE_AUTO && !g_valves[i].isRunning && !g_valves[i].sensorError && !isnan(g_valves[i].currentSoil)) {
          if (g_valves[i].currentSoil < g_valves[i].dryThresh) {
            if (now - g_valves[i].lastStopTime >= (unsigned long)g_valves[i].minRestTime * 1000UL) {
              requestOpenValve(i);
            }
          }
        }
      }
    }
  }

  // Đồng bộ biến trạng thái tương thích ngược
  g_valve1Running = g_valves[0].isRunning;
  g_valve2Running = g_valves[1].isRunning;
}

// Wrapper tương thích ngược
void initPump() { initIrrigation(); }
void updatePump() {}
void setPumpState(bool on) { if (on) { requestOpenValve(0); requestOpenValve(1); } else { requestCloseValve(0); requestCloseValve(1); } }
void setPumpManual(bool on) { 
  if (on) {
    setValveMode(0, VALVE_MODE_ON); 
    setValveMode(1, VALVE_MODE_ON); 
  } else { 
    requestCloseValve(0);
    requestCloseValve(1);
    g_valves[0].mode = VALVE_MODE_AUTO; 
    g_valves[1].mode = VALVE_MODE_AUTO; 
  } 
  saveValvesConfig();
}
void initValves() {}
void updateValves() {}
void setValveState(int v, bool on) { int idx = v - 1; if (idx >= 0 && idx < NUM_VALVES) { if (on) requestOpenValve(idx); else requestCloseValve(idx); } }
void setValveManual(int v, bool on) { int idx = v - 1; if (idx >= 0 && idx < NUM_VALVES) { setValveMode(idx, on ? VALVE_MODE_ON : VALVE_MODE_OFF); } }

void initRoof() {
  // Bắt buộc phân bổ Timer 2 & 3 cho ESP32Servo trên ESP32 Arduino Core 3.x
  // (Timer 0 đã dùng cho Quạt 20kHz, Timer 1 đã dùng cho Bóng sưởi 1kHz)
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  roofServo.setPeriodHertz(50); // Tần số chuẩn 50Hz cho servo
  int ch = roofServo.attach(SERVO_PIN, 500, 2400); // 500us - 2400us cho góc 0° - 180°
  Serial.printf("[SERVO] Khoi tao Servo GPIO %d (Channel: %d, Attached: %d)\n",
                SERVO_PIN, ch, roofServo.attached());
  setRoofAngle(0); // Đóng mái
}

void updateRoof() {
  if (g_roofManual)
    return;
  if (!g_sht30OK || isnan(g_temperature))
    return;
  // Bật quạt thì mở mái (chỉ mở khi quá nóng)
  if (g_temperature > g_fanSetpoint + 2.0 && g_roofAngle != 180) {
    setRoofAngle(180);
  } else if (g_temperature <= g_fanSetpoint && g_roofAngle != 0) {
    setRoofAngle(0);
  }
}

void setRoofAngle(int angle) {
  angle = constrain(angle, 0, 180);
  if (roofServo.attached()) {
    roofServo.write(angle);
  } else {
    Serial.printf("[SERVO WARN] Servo chua attach vao GPIO %d!\n", SERVO_PIN);
  }
  static int s_lastPrintedAngle = -1;
  if (abs(angle - s_lastPrintedAngle) >= 15 || (angle == 0 && s_lastPrintedAngle != 0) || (angle == 180 && s_lastPrintedAngle != 180)) {
    s_lastPrintedAngle = angle;
    Serial.printf("[SERVO] Goc mai: %d do\n", angle);
  }
  g_roofAngle = angle;
}
void setRoofManual(int angle) {
  g_roofManual = true;
  setRoofAngle(angle);
}

