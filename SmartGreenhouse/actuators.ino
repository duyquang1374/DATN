/*
 * actuators.ino
 * Điều khiển Quạt DC (TB6612FNG + PID) và Bơm 5V.
 *
 * Quạt DC:
 *   - TB6612FNG: AIN1/AIN2 chọn chiều, PWMA điều tốc, STBY kích hoạt
 *   - ESP32 LEDC (ledcWrite) tạo PWM tần số 20kHz
 *   - PID: Input=nhiệt độ, Setpoint=FAN_SETPOINT, Output=PWM (0-255)
 *   - REVERSE direction: nhiệt độ tăng → PWM tăng → quạt nhanh hơn
 *
 * Bơm 5V:
 *   - Bật khi độ ẩm đất < PUMP_ON_THRESHOLD
 *   - Tắt khi độ ẩm đất > PUMP_OFF_THRESHOLD (hysteresis)
 *   - Bảo vệ: tắt sau PUMP_MAX_RUNTIME, cooldown PUMP_COOLDOWN
 */

// ═══════════════════════════════════════════════════════════════
//  KHỞI TẠO QUẠT DC
// ═══════════════════════════════════════════════════════════════
void initFan() {
  // Chân direction TB6612FNG
  pinMode(FAN_AIN1, OUTPUT);
  pinMode(FAN_AIN2, OUTPUT);
  pinMode(FAN_STBY, OUTPUT);

  // Đặt chiều quay cố định: AIN1=HIGH, AIN2=LOW → chiều thuận
  digitalWrite(FAN_AIN1, HIGH);
  digitalWrite(FAN_AIN2, LOW);
  digitalWrite(FAN_STBY, HIGH);  // Kích hoạt driver

  // Cấu hình PWM (LEDC) cho ESP32 Core v3.x
  // API mới: ledcAttach(pin, freq, resolution) — không cần channel riêng
  ledcAttach(FAN_PWMA, FAN_PWM_FREQ, FAN_PWM_RES);
  ledcWrite(FAN_PWMA, 0);  // Bắt đầu với quạt tắt

  // Cấu hình PID
  fanPID.SetOutputLimits(0, FAN_PWM_MAX);
  fanPID.SetSampleTime(SENSOR_INTERVAL);
  fanPID.SetMode(AUTOMATIC);  // Bật PID

  g_fanMode    = FAN_MODE_AUTO;
  g_fanPWM     = 0;
  g_fanRunning = false;

  Serial.printf("[FAN] Init OK  STBY=%d AIN1=%d AIN2=%d PWM=%d\n",
                FAN_STBY, FAN_AIN1, FAN_AIN2, FAN_PWMA);
  Serial.printf("[PID] Kp=%.1f Ki=%.1f Kd=%.1f  SP=%.1f°C\n",
                PID_KP, PID_KI, PID_KD, g_fanSetpoint);
}

// ═══════════════════════════════════════════════════════════════
//  KHỞI TẠO BƠM
// ═══════════════════════════════════════════════════════════════
void initPump() {
  pinMode(PUMP_PIN, OUTPUT);

  // Đảm bảo bơm tắt lúc khởi động
  setPumpState(false);

  g_pumpRunning = false;
  g_pumpStart   = 0;
  g_pumpStop    = 0;

  Serial.printf("[PUMP] Init OK  pin=%d  ActiveHigh=%d\n",
                PUMP_PIN, PUMP_ACTIVE_HIGH);
  Serial.printf("[PUMP] ON<%.0f%%  OFF>%.0f%%  MaxRT=%lus\n",
                g_pumpOnThresh, g_pumpOffThresh,
                (unsigned long)PUMP_MAX_RUNTIME / 1000);
}

// ═══════════════════════════════════════════════════════════════
//  CẬP NHẬT QUẠT (Gọi mỗi SENSOR_INTERVAL)
// ═══════════════════════════════════════════════════════════════
void updateFan() {
  if (!g_sht30OK || isnan(g_temperature)) {
    // Sensor lỗi → tắt quạt an toàn
    setFanPWM(0);
    return;
  }

  switch (g_fanMode) {
    // ── OFF ──────────────────────────────────────────────────
    case FAN_MODE_OFF:
      setFanPWM(0);
      break;

    // ── AUTO (PID) ───────────────────────────────────────────
    case FAN_MODE_AUTO:
      pidInput    = (double)g_temperature;
      pidSetpoint = (double)g_fanSetpoint;

      // Dưới ngưỡng start temp → tắt quạt, reset integral
      if (g_temperature <= (g_fanSetpoint - FAN_DEADBAND)) {
        fanPID.SetMode(MANUAL);
        pidOutput = 0;
        setFanPWM(0);
        fanPID.SetMode(AUTOMATIC);  // Reset tích phân
      } else {
        fanPID.Compute();

        // Áp dụng PWM tối thiểu khi đang chạy (vượt ma sát tĩnh)
        int pwm = (int)pidOutput;
        if (pwm > 0 && pwm < FAN_PWM_MIN) {
          pwm = FAN_PWM_MIN;
        }
        setFanPWM(pwm);
      }
      break;

    // ── MANUAL ──────────────────────────────────────────────
    case FAN_MODE_MANUAL:
      setFanPWM(g_manualPWM);
      break;
  }

  // Log
  const char* modeStr[] = {"OFF", "AUTO", "MAN"};
  Serial.printf("[FAN] Mode=%-4s  PWM=%3d  Speed=%3d%%  T=%.1f°C  SP=%.1f°C\n",
                modeStr[g_fanMode], g_fanPWM, g_fanSpeed,
                g_temperature, (float)pidSetpoint);
}

// ═══════════════════════════════════════════════════════════════
//  CẬP NHẬT BƠM (Gọi mỗi SENSOR_INTERVAL)
// ═══════════════════════════════════════════════════════════════
void updatePump() {
  if (g_pumpManual) return;  // Đang điều khiển tay, bỏ qua logic tự động

  unsigned long now = millis();

  // ── Bảo vệ: tắt sau PUMP_MAX_RUNTIME ────────────────────
  if (g_pumpRunning && (now - g_pumpStart >= PUMP_MAX_RUNTIME)) {
    Serial.println("[PUMP] Đã chạy quá lâu → tắt (bảo vệ)");
    setPumpState(false);
    return;
  }

  // ── Cooldown: chưa đủ thời gian nghỉ ────────────────────
  if (!g_pumpRunning && g_pumpStop > 0
      && (now - g_pumpStop < PUMP_COOLDOWN)) {
    return;  // Đang trong thời gian cooldown
  }

  if (isnan(g_soilMoisture)) return;

  // ── Logic hysteresis ──────────────────────────────────────
  if (!g_pumpRunning && g_soilMoisture < g_pumpOnThresh) {
    Serial.printf("[PUMP] Đất khô (%.0f%% < %.0f%%) → BẬT BƠM\n",
                  g_soilMoisture, g_pumpOnThresh);
    setPumpState(true);

  } else if (g_pumpRunning && g_soilMoisture > g_pumpOffThresh) {
    Serial.printf("[PUMP] Đất đủ ẩm (%.0f%% > %.0f%%) → TẮT BƠM\n",
                  g_soilMoisture, g_pumpOffThresh);
    setPumpState(false);
  }

  // Log trạng thái bơm
  if (g_pumpRunning) {
    Serial.printf("[PUMP] ON  Dat=%.0f%%  Runtime=%lus\n",
                  g_soilMoisture, (now - g_pumpStart) / 1000);
  }
}

// ═══════════════════════════════════════════════════════════════
//  HÀM ĐIỀU KHIỂN CẤP THẤP
// ═══════════════════════════════════════════════════════════════

// Đặt PWM quạt (0-255) và cập nhật các biến trạng thái
void setFanPWM(int pwm) {
  pwm = constrain(pwm, 0, FAN_PWM_MAX);
  ledcWrite(FAN_PWMA, pwm);
  g_fanPWM     = pwm;
  g_fanSpeed   = map(pwm, 0, FAN_PWM_MAX, 0, 100);
  g_fanRunning = (pwm > 0);
}

// Bật/tắt bơm, ghi timestamp
void setPumpState(bool on) {
  unsigned long now = millis();
  if (on) {
    digitalWrite(PUMP_PIN, PUMP_ACTIVE_HIGH ? HIGH : LOW);
    if (!g_pumpRunning) g_pumpStart = now;
    g_pumpRunning = true;
  } else {
    digitalWrite(PUMP_PIN, PUMP_ACTIVE_HIGH ? LOW : HIGH);
    if (g_pumpRunning) g_pumpStop = now;
    g_pumpRunning = false;
  }
}

// ═══════════════════════════════════════════════════════════════
//  API ĐIỀU KHIỂN (gọi từ web_server_local.ino)
// ═══════════════════════════════════════════════════════════════

// Đặt chế độ quạt: 0=OFF, 1=AUTO, 2=MANUAL
void setFanMode(int mode) {
  if (mode < FAN_MODE_OFF || mode > FAN_MODE_MANUAL) return;
  g_fanMode = mode;
  if (mode == FAN_MODE_AUTO) {
    fanPID.SetMode(AUTOMATIC);
  }
  Serial.printf("[FAN] Chế độ → %d\n", mode);
}

// Đặt tốc độ thủ công (0-255), tự chuyển sang MANUAL
void setFanManualSpeed(int pwm) {
  g_manualPWM = constrain(pwm, 0, FAN_PWM_MAX);
  g_fanMode   = FAN_MODE_MANUAL;
  Serial.printf("[FAN] Thủ công PWM=%d\n", g_manualPWM);
}

// Đặt setpoint PID nhiệt độ
void setFanSetpoint(float sp) {
  g_fanSetpoint = constrain(sp, 20.0f, 45.0f);
  pidSetpoint = (double)g_fanSetpoint;
  Serial.printf("[PID] Setpoint → %.1f°C\n", g_fanSetpoint);
}

// Bật/tắt bơm thủ công từ web
void setPumpManual(bool on) {
  g_pumpManual = true;
  setPumpState(on);
  Serial.printf("[PUMP] Điều khiển tay → %s\n", on ? "ON" : "OFF");
}

// Trả bơm về chế độ tự động
void setPumpAuto() {
  g_pumpManual = false;
  Serial.println("[PUMP] Chế độ tự động");
}
