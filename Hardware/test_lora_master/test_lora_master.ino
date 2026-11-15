// =====================================================================
//  SKETCH CHẨN ĐOÁN LORA E32 TRÊN MASTER NODE (KHÔNG CÓ SERVO / CẢM BIẾN)
//  - Lần lượt thử UART2 ở các baud: 9600, 19200, 38400, 57600, 115200
//  - Mỗi baud: nghe 12 giây (SensorNode phát mỗi 10s) và in HEX mọi byte nhận được
//  - Mỗi baud: phát 1 gói JSON ping -> nếu Gateway in "[LoRa RAW] {"type":"ping"...}"
//    thì chiều Master -> Gateway hoạt động ở baud đó
//  Chân giống MasterNode: ESP32 TX2 = GPIO 18 -> E32 RXD, RX2 = GPIO 19 <- E32 TXD
// =====================================================================

#define LORA_TX_PIN 18
#define LORA_RX_PIN 19
#define LISTEN_MS   12000

HardwareSerial LoRaSerial(2);
const long BAUDS[] = {9600, 19200, 38400, 57600, 115200};
const int NUM_BAUDS = sizeof(BAUDS) / sizeof(BAUDS[0]);

void testBaud(long baud) {
  LoRaSerial.end();
  delay(50);
  LoRaSerial.begin(baud, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);
  delay(100);
  while (LoRaSerial.available()) LoRaSerial.read(); // Xóa rác khi đổi baud

  Serial.printf("\n===== THU BAUD %ld =====\n", baud);

  // Phát gói ping (Gateway chỉ in [LoRa RAW] nếu nhận đúng JSON)
  char ping[64];

  snprintf(ping, sizeof(ping), "{\"type\":\"ping\",\"baud\":%ld}\n", baud);
  LoRaSerial.print(ping);
  LoRaSerial.flush();
  Serial.printf("[TX] Da phat: %s", ping);

  // Nghe và in HEX
  unsigned long start = millis();
  int total = 0, aa = 0, brace = 0, col = 0;
  while (millis() - start < LISTEN_MS) {
    while (LoRaSerial.available()) {
      uint8_t b = LoRaSerial.read();
      total++;
      if (b == 0xAA) aa++;
      if (b == '{') brace++;
      if (col == 0) Serial.print("[RX] ");
      Serial.printf("%02X ", b);
      if (++col >= 24) { Serial.println(); col = 0; }
    }
    delay(2);
  }
  if (col) Serial.println();
  Serial.printf("[KET QUA %ld] Tong %d byte | so byte 0xAA (goi SensorNode): %d | so dau '{' (JSON Gateway): %d\n",
                baud, total, aa, brace);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n##### CHAN DOAN LORA E32 - MASTER NODE #####");
  Serial.println("Luu y: M0, M1 phai noi GND. SensorNode phai dang bat de co du lieu nghe.");
}

void loop() {
  for (int i = 0; i < NUM_BAUDS; i++) testBaud(BAUDS[i]);
  Serial.println("\n##### HET 1 VONG - LAP LAI #####");
}
