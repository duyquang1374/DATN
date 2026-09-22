/*
 * lcd_display.ino
 * Giao diện LCD 20x4 I2C hiển thị trạng thái nhà màng.
 *
 * Layout 4 dòng (mỗi dòng 20 ký tự):
 * ┌────────────────────┐
 * │T: 28.5C   H: 75.3%│  ← Nhiệt độ & Độ ẩm không khí
 * │Lx:  1250  Dt:  65%│  ← Ánh sáng & Độ ẩm đất
 * │Fan: AUTO  Spd:  85%│  ← Chế độ quạt & Tốc độ
 * │Pmp: OFF   WiFi: OK│  ← Bơm & Trạng thái WiFi
 * └────────────────────┘
 */

// ═══════════════════════════════════════════════════════════════
//  KHỞI TẠO LCD
// ═══════════════════════════════════════════════════════════════
void initLCD() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Tạo ký tự tùy chỉnh (custom chars)
  // Degree symbol (°) vì LCD không có sẵn
  byte degree[8] = {
    0b00110,
    0b01001,
    0b01001,
    0b00110,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };
  // Ký tự WiFi OK (antenna)
  byte wifiOK[8] = {
    0b00100,
    0b01010,
    0b10001,
    0b00100,
    0b01010,
    0b00100,
    0b00100,
    0b00000
  };
  lcd.createChar(0, degree);  // \x00 = ký tự degree
  lcd.createChar(1, wifiOK);  // \x01 = ký tự wifi

  Serial.printf("[LCD] Init OK  Addr=0x%02X  %dx%d\n",
                LCD_ADDR, LCD_COLS, LCD_ROWS);
}

// ═══════════════════════════════════════════════════════════════
//  HIỂN THỊ MÀN HÌNH BOOT
// ═══════════════════════════════════════════════════════════════
void lcdShowBoot(const char* msg, int progressStep) {
  lcd.clear();

  // Dòng 0: Tiêu đề
  lcd.setCursor(0, 0);
  lcd.print(F("  NHAM MANG THONG"));

  // Dòng 1: Smart Greenhouse
  lcd.setCursor(0, 1);
  lcd.print(F("  MINH v2.0"));

  // Dòng 2: Thông báo hiện tại
  lcd.setCursor(0, 2);
  lcdPrintField(msg, LCD_COLS);

  // Dòng 3: Thanh tiến trình (0-4 bước → 0-20 ký tự '#')
  lcd.setCursor(0, 3);
  lcd.print("[");
  int bars = min(progressStep * 4, LCD_COLS - 2);
  for (int i = 0; i < bars; i++) lcd.print('#');
  for (int i = bars; i < LCD_COLS - 2; i++) lcd.print('-');
  lcd.print("]");

  Serial.println("[LCD] Boot: " + String(msg));
}

// ═══════════════════════════════════════════════════════════════
//  CẬP NHẬT LCD CHÍNH (Gọi mỗi LCD_INTERVAL)
// ═══════════════════════════════════════════════════════════════
void updateLCD() {
  char buf[21];

  // ─── Dòng 0: Nhiệt độ & Độ ẩm không khí ─────────────────
  lcd.setCursor(0, 0);
  if (g_sht30OK && !isnan(g_temperature)) {
    // "T: 28.5C   H: 75.3%"
    // Field T: 10 chars, Field H: 10 chars
    char tStr[8], hStr[8];
    snprintf(tStr, sizeof(tStr), "%.1f", g_temperature);
    snprintf(hStr, sizeof(hStr), "%.1f", g_humidity);
    snprintf(buf, sizeof(buf), "T:%-5sC H:%-5s%%", tStr, hStr);
  } else {
    snprintf(buf, sizeof(buf), "T: --- C H: --.-%%");
  }
  lcdPrintField(buf, LCD_COLS);

  // ─── Dòng 1: Ánh sáng & Độ ẩm đất ──────────────────────
  lcd.setCursor(0, 1);
  char luxStr[7], soilStr[6];
  if (g_bh1750OK && !isnan(g_lightLux)) {
    snprintf(luxStr, sizeof(luxStr), "%5.0f", g_lightLux);
  } else {
    strncpy(luxStr, " ----", sizeof(luxStr));
  }
  if (!isnan(g_soilMoisture)) {
    snprintf(soilStr, sizeof(soilStr), "%4.0f%%", g_soilMoisture);
  } else {
    strncpy(soilStr, " --% ", sizeof(soilStr));
  }
  snprintf(buf, sizeof(buf), "Lx:%s Dt:%s", luxStr, soilStr);
  lcdPrintField(buf, LCD_COLS);

  // ─── Dòng 2: Quạt - Chế độ & Tốc độ ─────────────────────
  lcd.setCursor(0, 2);
  const char* modeStr;
  switch (g_fanMode) {
    case FAN_MODE_OFF:    modeStr = "OFF "; break;
    case FAN_MODE_AUTO:   modeStr = "AUTO"; break;
    case FAN_MODE_MANUAL: modeStr = "MAN "; break;
    default:              modeStr = "????"; break;
  }
  snprintf(buf, sizeof(buf), "Fan:%-4s Spd:%3d%%", modeStr, g_fanSpeed);
  lcdPrintField(buf, LCD_COLS);

  // ─── Dòng 3: Bơm & WiFi ──────────────────────────────────
  lcd.setCursor(0, 3);
  const char* pumpStr  = g_pumpRunning  ? "ON " : "OFF";
  const char* manualMark = g_pumpManual ? "!" : " ";  // '!' = chế độ tay
  const char* wifiStr;
  if (g_wifiOK) {
    int rssi = WiFi.RSSI();
    if      (rssi >= -60) wifiStr = "OK  ";
    else if (rssi >= -75) wifiStr = "OK- ";
    else                  wifiStr = "WEAK";
  } else {
    wifiStr = "ERR ";
  }
  snprintf(buf, sizeof(buf), "Pmp:%s%s  WiFi:%-4s",
           pumpStr, manualMark, wifiStr);
  lcdPrintField(buf, LCD_COLS);
}

// ═══════════════════════════════════════════════════════════════
//  UTILITY: In chuỗi vào LCD với độ rộng cố định (đệm space)
// ═══════════════════════════════════════════════════════════════
void lcdPrintField(const char* str, int width) {
  int len = strnlen(str, width);
  for (int i = 0; i < len && i < width; i++) {
    lcd.print(str[i]);
  }
  // Đệm space để xóa ký tự cũ
  for (int i = len; i < width; i++) {
    lcd.print(' ');
  }
}

// ═══════════════════════════════════════════════════════════════
//  HIỂN THỊ CẢNH BÁO (Nhấp nháy dòng 2)
// ═══════════════════════════════════════════════════════════════
void lcdAlert(const char* msg) {
  for (int i = 0; i < 3; i++) {
    lcd.setCursor(0, 2);
    lcd.print(F("!! CANH BAO !!      "));
    delay(400);
    lcd.setCursor(0, 2);
    lcdPrintField(msg, LCD_COLS);
    delay(400);
  }
}

// ═══════════════════════════════════════════════════════════════
//  HIỂN THỊ IP (Sau khi kết nối WiFi)
// ═══════════════════════════════════════════════════════════════
void lcdShowIP() {
  if (!g_wifiOK) return;
  lcd.setCursor(0, 3);
  String ip = "IP:" + WiFi.localIP().toString();
  lcdPrintField(ip.c_str(), LCD_COLS);
}
