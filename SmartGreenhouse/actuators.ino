void initFan() {
  pinMode(FAN_AIN1, OUTPUT);
  pinMode(FAN_AIN2, OUTPUT);
  digitalWrite(FAN_AIN1, HIGH);
  digitalWrite(FAN_AIN2, LOW);
  ledcAttach(FAN_PWMA, FAN_PWM_FREQ, FAN_PWM_RES);
  ledcWrite(FAN_PWMA, 0);

  fanPID.SetOutputLimits(0, FAN_PWM_MAX);
  fanPID.SetSampleTime(SENSOR_INTERVAL);
  fanPID.SetMode(AUTOMATIC);

  g_fanMode = FAN_MODE_AUTO;
  g_fanPWM = 0;
  g_fanRunning = false;
}

void updateFan() {
  if (!g_sht30OK || isnan(g_temperature)) {
    setFanPWM(0);
    return;
  }
  switch (g_fanMode) {
    case FAN_MODE_OFF:
      setFanPWM(0);
      break;
    case FAN_MODE_AUTO:
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
        if (pwm > 0 && pwm < FAN_PWM_MIN) pwm = FAN_PWM_MIN;
        setFanPWM(pwm);
      }
      break;
    case FAN_MODE_MANUAL:
      setFanPWM(g_manualPWM);
      break;
  }
}

void setFanPWM(int pwm) {
  pwm = constrain(pwm, 0, FAN_PWM_MAX);
  ledcWrite(FAN_PWMA, pwm);
  g_fanPWM = pwm;
  g_fanSpeed = map(pwm, 0, FAN_PWM_MAX, 0, 100);
  g_fanRunning = (pwm > 0);
}

void setFanMode(int mode) { g_fanMode = mode; if (mode == FAN_MODE_AUTO) fanPID.SetMode(AUTOMATIC); }
void setFanManualSpeed(int pwm) { g_manualPWM = constrain(pwm, 0, FAN_PWM_MAX); g_fanMode = FAN_MODE_MANUAL; setFanPWM(g_manualPWM); }
void setFanSetpoint(float sp) { g_fanSetpoint = sp; pidSetpoint = sp; heaterSetpoint = sp - 2.0; }

void initHeater() {
  pinMode(HEATER_BIN1, OUTPUT);
  pinMode(HEATER_BIN2, OUTPUT);
  digitalWrite(HEATER_BIN1, HIGH);
  digitalWrite(HEATER_BIN2, LOW);
  ledcAttach(HEATER_PWMB, FAN_PWM_FREQ, FAN_PWM_RES);
  ledcWrite(HEATER_PWMB, 0);
  
  heaterPID.SetOutputLimits(0, 255);
  heaterPID.SetSampleTime(SENSOR_INTERVAL);
  heaterPID.SetMode(AUTOMATIC);

  g_heaterRunning = false;
  g_heaterManual = false;
}

void updateHeater() {
  if (g_heaterManual) return;
  if (!g_sht30OK || isnan(g_temperature)) {
    setHeaterPWM(0);
    return;
  }
  
  pidInput = (double)g_temperature;
  heaterSetpoint = (double)g_fanSetpoint - 2.0; // Giữ khoảng cách 2 độ để không bị đụng độ với quạt
  
  if (g_temperature >= heaterSetpoint) {
    heaterPID.SetMode(MANUAL);
    heaterPidOutput = 0;
    setHeaterPWM(0);
    heaterPID.SetMode(AUTOMATIC);
  } else {
    heaterPID.Compute();
    int pwm = (int)heaterPidOutput;
    if (pwm > 0 && pwm < FAN_PWM_MIN) pwm = FAN_PWM_MIN;
    setHeaterPWM(pwm);
  }
}

void setHeaterPWM(int pwm) {
  pwm = constrain(pwm, 0, 255);
  ledcWrite(HEATER_PWMB, pwm);
  g_heaterPWM = pwm;
  g_heaterRunning = (pwm > 0);
}

void setHeaterManual(bool on) {
  g_heaterManual = true;
  setHeaterPWM(on ? 255 : 0);
}

void initPump() {
  pinMode(PUMP_PIN, OUTPUT);
  setPumpState(false);
}

void updatePump() {
  if (g_pumpManual) return;
  unsigned long now = millis();
  if (g_pumpRunning && (now - g_pumpStart >= 30000)) { setPumpState(false); return; }
  if (isnan(g_soilMoisture)) return;
  if (!g_pumpRunning && g_soilMoisture < g_pumpOnThresh) setPumpState(true);
  else if (g_pumpRunning && g_soilMoisture > g_pumpOffThresh) setPumpState(false);
}

void setPumpState(bool on) {
  digitalWrite(PUMP_PIN, on ? (PUMP_ACTIVE_HIGH ? HIGH : LOW) : (PUMP_ACTIVE_HIGH ? LOW : HIGH));
  if (on && !g_pumpRunning) g_pumpStart = millis();
  if (!on && g_pumpRunning) g_pumpStop = millis();
  g_pumpRunning = on;
}

void setPumpManual(bool on) { g_pumpManual = true; setPumpState(on); }

void initMist() {
  pinMode(MIST_PIN, OUTPUT);
  setMistState(false);
}

void updateMist() {
  if (g_mistManual) return;
  bool shouldRun = g_fanRunning;
  if (shouldRun && !g_mistRunning) setMistState(true);
  else if (!shouldRun && g_mistRunning) setMistState(false);
}

void setMistState(bool on) {
  digitalWrite(MIST_PIN, on ? (MIST_ACTIVE_HIGH ? HIGH : LOW) : (MIST_ACTIVE_HIGH ? LOW : HIGH));
  g_mistRunning = on;
}

void setMistManual(bool on) { g_mistManual = true; setMistState(on); }

void initLight() {
  pinMode(LIGHT_PIN, OUTPUT);
  setLightState(false);
}

void updateLight() {
  if (g_lightManual) return;
  if (!g_bh1750OK || isnan(g_lightLux)) return;
  if (!g_lightRunning && g_lightLux < g_lightThresh) setLightState(true);
  else if (g_lightRunning && g_lightLux > (g_lightThresh + 200)) setLightState(false);
}

void setLightState(bool on) {
  digitalWrite(LIGHT_PIN, on ? (LIGHT_ACTIVE_HIGH ? HIGH : LOW) : (LIGHT_ACTIVE_HIGH ? LOW : HIGH));
  g_lightRunning = on;
}

void setLightManual(bool on) { g_lightManual = true; setLightState(on); }

void initValves() {
  pinMode(VALVE1_PIN, OUTPUT);
  pinMode(VALVE2_PIN, OUTPUT);
  setValveState(1, false);
  setValveState(2, false);
}

void updateValves() {
  if (g_valveManual) return;
  // Nếu bơm chạy thì bật van 1, mặc định
  if (g_pumpRunning && !g_valve1Running) setValveState(1, true);
  else if (!g_pumpRunning && g_valve1Running) setValveState(1, false);
}

void setValveState(int v, bool on) {
  if (v == 1) {
    digitalWrite(VALVE1_PIN, on ? (PUMP_ACTIVE_HIGH ? HIGH : LOW) : (PUMP_ACTIVE_HIGH ? LOW : HIGH));
    g_valve1Running = on;
  } else if (v == 2) {
    digitalWrite(VALVE2_PIN, on ? (PUMP_ACTIVE_HIGH ? HIGH : LOW) : (PUMP_ACTIVE_HIGH ? LOW : HIGH));
    g_valve2Running = on;
  }
}
void setValveManual(int v, bool on) { g_valveManual = true; setValveState(v, on); }

void initRoof() {
  // Servo using ESP32Servo
  roofServo.attach(SERVO_PIN);
  setRoofAngle(0); // Đóng mái
}

void updateRoof() {
  if (g_roofManual) return;
  if (!g_sht30OK || isnan(g_temperature)) return;
  // Bật quạt thì mở mái (chỉ mở khi quá nóng)
  if (g_temperature > g_fanSetpoint + 2.0 && g_roofAngle != 180) {
    setRoofAngle(180);
  } else if (g_temperature <= g_fanSetpoint && g_roofAngle != 0) {
    setRoofAngle(0);
  }
}

void setRoofAngle(int angle) {
  angle = constrain(angle, 0, 180);
  roofServo.write(angle);
  g_roofAngle = angle;
}
void setRoofManual(int angle) {
  g_roofManual = true;
  setRoofAngle(angle);
}
