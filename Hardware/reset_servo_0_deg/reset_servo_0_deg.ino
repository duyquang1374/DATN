// =====================================================================
//  CHƯƠNG TRÌNH: RESET VÀ ĐỒNG BỘ 2 SERVO ĐẶT NGƯỢC CHIỀU NHAU
//  Dự án: Rèm Mái Chớp Tự Động - Nhà Màng Thông Minh ĐATN
//  Nguyên lý chuyển động cơ khí:
//    - Do 2 Servo lắp đối xứng / ngược chiều nhau:
//      + Servo 1 (Thuận): Quay từ 0 độ (Đóng) -> 70 độ (Mở)
//      + Servo 2 (Ngược): Quay ngược lại từ 70 độ (Đóng) -> 0 độ (Mở)
//      + Công thức góc: Goc_Servo_2 = 70 - Goc_Servo_1
//    - BẮT BUỘC dùng 2 chân GPIO riêng biệt (không câu chung 1 dây tín hiệu)
// =====================================================================

#if defined(ESP32)
  #include <ESP32Servo.h>
  // Chân GPIO trên ESP32
  #define SERVO_1_PIN  25    // Servo 1 (Thuận) - Chân GPIO 25 trên Master Node
  #define SERVO_2_PIN  26    // Servo 2 (Ngược) - Chân GPIO 12 trên Master Node
#else
  #include <Servo.h>
  // Chân trên Arduino Uno / Nano
  #define SERVO_1_PIN  9     // Servo 1 (Thuận)
  #define SERVO_2_PIN  10    // Servo 2 (Ngược)
#endif

// Góc mở tối đa của mái rèm chớp
const int MAX_ANGLE = 70;

Servo servo1;
Servo servo2;

// Hàm điều khiển đồng bộ cả 2 servo theo góc mở rèm (0 - 70 độ)
void setLouverAngle(int targetAngle) {
  targetAngle = constrain(targetAngle, 0, MAX_ANGLE);
  
  int angle1 = targetAngle;             // Servo 1: 0 -> 70
  int angle2 = MAX_ANGLE - targetAngle; // Servo 2: 70 -> 0 (đảo chiều góc)

  servo1.write(angle1);
  servo2.write(angle2);

  Serial.print("[GOC REM: ");
  Serial.print(targetAngle);
  Serial.print(" do] -> Servo 1 (Thuan): ");
  Serial.print(angle1);
  Serial.print(" do | Servo 2 (Nguoc): ");
  Serial.print(angle2);
  Serial.println(" do");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n========================================================");
  Serial.println("  CHUONG TRINH RESET & DONG BO 2 SERVO NGUOC CHIEU");
  Serial.println("========================================================");

  #if defined(ESP32)
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    servo1.setPeriodHertz(50);
    servo2.setPeriodHertz(50);
  #endif

  // Kết nối chân 2 servo
  Serial.print("Ket noi Servo 1 (Thuan) tai GPIO: ");
  Serial.println(SERVO_1_PIN);
  servo1.attach(SERVO_1_PIN, 500, 2400);

  Serial.print("Ket noi Servo 2 (Nguoc) tai GPIO: ");
  Serial.println(SERVO_2_PIN);
  servo2.attach(SERVO_2_PIN, 500, 2400);

  // RESET VỀ VỊ TRÍ ĐÓNG RÈM (Góc rèm = 0 độ)
  // Servo 1 sẽ về 0 độ, Servo 2 sẽ về 70 độ
  Serial.println("\n-> DANG RESET 2 SERVO VE VI TRI DONG REM...");
  setLouverAngle(0);
  delay(1500); // Chờ 2 servo quay xong

  Serial.println("-> DA XONG! 2 SERVO DANG DUOC CO DINH O TRANG THAI DONG REM.");
  Serial.println("-> Hay lap 2 tay don vao 2 truc servo o tu the rem DONG KIN!");
  Serial.println("--------------------------------------------------------");
  Serial.println("Phim lenh test qua Serial Monitor:");
  Serial.println("  Nhap '0' : Dong rem kin (Servo 1: 0 do  | Servo 2: 70 do)");
  Serial.println("  Nhap '3' : Mo nua rem   (Servo 1: 35 do | Servo 2: 35 do)");
  Serial.println("  Nhap '7' : Mo toi da    (Servo 1: 70 do | Servo 2: 0 do)");
  Serial.println("========================================================\n");
}

void loop() {
  // Nhận lệnh từ Serial Monitor để kiểm tra hoạt động đồng bộ
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == '0') {
      Serial.println("\n[LENH] -> DONG REM");
      setLouverAngle(0);
    } 
    else if (cmd == '3') {
      Serial.println("\n[LENH] -> MO NUA (35 DO)");
      setLouverAngle(35);
    } 
    else if (cmd == '7') {
      Serial.println("\n[LENH] -> MO TOI DA (70 DO)");
      setLouverAngle(70);
    }
  }

  delay(20);
}
