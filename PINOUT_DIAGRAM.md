# BẢN SƠ ĐỒ ĐẤU NỐI CHÂN & LINH KIỆN BỔ SUNG (PINOUT DIAGRAM)
## HỆ THỐNG NHÀ MÀNG THÔNG MINH (SMART GREENHOUSE IoT - TinyML)

Tài liệu này cung cấp chi tiết sơ đồ chân (Pinout Mapping), sơ đồ nguyên lý ghép nối, danh sách linh kiện bổ sung (điện trở, tụ lọc, bảo vệ, nguồn) và các lưu ý kỹ thuật khi thi công mạch cho **3 Node vi điều khiển ESP32** trong hệ thống.

---

## MỤC LỤC
1. [Sơ đồ Phân phối Nguồn điện Toàn hệ thống](#1-sơ-đồ-phân-phối-nguồn-điện-toàn-hệ-thống)
2. [Node 1: Master Node (ESP32 - Xử lý Trung tâm & AI)](#2-node-1-master-node-esp32---xử-lý-trung-tâm--ai)
3. [Node 2: HMI Node (ESP32 - Tủ điện Điều khiển & Giám sát tại chỗ)](#3-node-2-hmi-node-esp32---tủ-điện-điều-khiển--giám-sát-tại-chỗ)
4. [Node 3: Gateway Node (ESP32 - Trạm Thu LoRa & Web Server)](#4-node-3-gateway-node-esp32---trạm-thu-lora--web-server)
5. [Danh mục Linh kiện Bổ trợ & Phụ kiện Mạch (Bill of Materials)](#5-danh-mục-linh-kiện-bổ-trợ--phụ-kiện-mạch-bom)
6. [Quy chuẩn Thi công & Bảo vệ Mạch Thực tế](#6-quy-chuẩn-thi-công--bảo-vệ-mạch-thực-tế)

---

## 1. SƠ ĐỒ PHÂN PHỐI NGUỒN ĐIỆN TOÀN HỆ THỐNG

```
                   [ Nguồn Tổ Ong DC 12V - 10A ]
                                 │
         ┌───────────────────────┴───────────────────────┐
         │ (Tuyến 12V Tải công suất cao)                 │ (Tuyến hạ áp 5V)
         ▼                                               ▼
  ┌──────────────┐                                ┌──────────────┐
  │ MOSFET D4184 │─── Quạt DC 12V, Cụm 5 bóng sưởi│ LM2596 Buck  │ (12V xuống 5V - 3A)
  │ Relay Tiếp   │─── Máy bơm tưới 12V           └───────┬──────┘
  │   điểm NO/COM│─── Van điện từ 1 & 2 (12V)            │
  │ MOSFET Input │─── Máy tạo ẩm sương 12V               ├─► ESP32 Master (Chân VIN/5V)
  │   Power      │─── Đèn LED quang hợp 12V              ├─► ESP32 HMI (Chân VIN/5V)
  └──────────────┘                                       ├─► ESP32 Gateway (Chân VIN/5V)
                                                         ├─► LoRa E32 VCC (Cả Master & GW)
                                                         ├─► Servo MG996R VCC (Dây đỏ)
                                                         ├─► LCD 2004 VCC (+5V)
                                                         └─► Relay VCC (Cuộn hút 5V)
  ════════════════════════════════════════════════════════════════════════════════════════
  (*) ĐIỂM MASS CHUNG (COMMON GROUND): Tất cả cực (-) 12V, GND LM2596, GND 3 bo ESP32,
      GND các cảm biến, driver và relay BẮT BUỘC ĐẤU CHẬP VỀ 1 CẦU ĐẤU ĐỒNG CHUNG.
  ════════════════════════════════════════════════════════════════════════════════════════
```

---

## 2. NODE 1: MASTER NODE (ESP32 - XỬ LÝ TRUNG TÂM & AI)

Bo mạch sử dụng: **ESP32 DOIT DevKit V1 (30 chân)**.

### 2.1. Sơ đồ Pinout ESP32 Master

```
                       ┌──────────────────────┐
                       │     ESP32 MASTER     │
                EN ───1│                      │30─── GPIO 23 (I2C1_SCL - SHT30 Ngoài)
    (Soil ADC) GPIO 34 ───2│                      │29─── GPIO 22 (I2C0_SCL - SHT30 Trong, BH1750)
               GPIO 35 ───3│                      │28─── GPIO 1 (TX0 - Nạp code)
               GPIO 32 ───4│ (Relay Bơm)          │27─── GPIO 3 (RX0 - Nạp code)
               GPIO 33 ───5│ (MOSFET Phun sương)  │26─── GPIO 21 (I2C0_SDA - SHT30 Trong, BH1750)
(Servo Mái)    GPIO 25 ───6│                      │25─── GPIO 19 (UART2 RX - Từ LoRa TX)
(Relay Van 1)  GPIO 26 ───7│                      │24─── GPIO 18 (UART2 TX - Sang LoRa RX)
(MOSFET Quạt)  GPIO 27 ───8│                      │23─── GPIO 5  (I2C1_SDA - SHT30 Ngoài)
(MOSFET Sưởi)  GPIO 14 ───9│                      │22─── GPIO 17 (UART1 TX - Sang HMI RX)
               GPIO 12 ──10│ (Trống - Không nối)  │21─── GPIO 16 (UART1 RX - Từ HMI TX)
(Relay Van 2)  GPIO 13 ──11│                      │20─── GPIO 4  (MOSFET Đèn quang hợp)
               GPIO 0  ──12│ (Trống - Tránh boot) │19─── GPIO 0  (Chân Bootloader)
               GPIO 2  ──13│ (Trống - Tránh boot) │18─── GPIO 2  (Đèn LED nội)
               GPIO 15 ──14│ (Trống - Tránh boot) │17─── GPIO 15 (Chân MTDO)
                  GND ──15│ (Mass chung)         │16─── 3V3   (Nguồn nuôi cảm biến)
                       └──────────────────────┘
```

### 2.2. Bảng Đấu Nối Chi Tiết Master Node

*ESP32 này chịu trách nhiệm thu thập cảm biến, điều khiển các cơ cấu chấp hành và kết nối LoRa.*

| Thiết bị / Module | Chân trên Thiết bị | Chân trên Master ESP32 | Ghi chú |
| :--- | :--- | :--- | :--- |
| **Nguồn chung** | VCC / GND | **5V (hoặc 3.3V) / GND** | Các cảm biến dùng 3.3V; Relay, Servo, D4184 dùng 5V từ LM2596. Đấu chung mass GND. |
| **I2C Bus 0**<br>*(Trong nhà)* | SDA<br>SCL | **GPIO 21**<br>**GPIO 22** | Cho SHT30 Trong nhà (0x44) & BH1750 (0x23). Kèm 2 trở pull-up 4.7kΩ lên 3.3V. |
| **I2C Bus 1**<br>*(Ngoài trời)* | SDA<br>SCL | **GPIO 5**<br>**GPIO 23** | Cho SHT30 Ngoài trời (Địa chỉ mặc định 0x44). Kèm trở 4.7kΩ, bọc dây chống nhiễu. |
| **Độ ẩm đất 1**<br>*(Luống 1 / Van 1)* | AOUT<br>VCC / GND | **GPIO 34 (ADC1_CH6)**<br>3.3V / GND | Đọc Analog (0 - 3.3V). Lắp tụ 100nF từ GPIO 34 xuống GND lọc nhiễu răng cưa. |
| **Độ ẩm đất 2**<br>*(Luống 2 / Van 2)* | AOUT<br>VCC / GND | **GPIO 35 (ADC1_CH7)**<br>3.3V / GND | Đọc Analog (0 - 3.3V). Lắp tụ 100nF từ GPIO 35 xuống GND lọc nhiễu răng cưa. |
| **Giao tiếp HMI**<br>*(ESP32 số 2)* | TX (Của HMI ESP32)<br>RX (Của HMI ESP32)<br>GND | **GPIO 16 (RX1)**<br>**GPIO 17 (TX1)**<br>GND | Đấu chéo UART Serial1 (Nhận lệnh nút bấm & góc rèm từ HMI; gửi cảm biến sang HMI). |
| **LoRa E32**<br>*(Truyền thông Gateway)* | TXD<br>RXD<br>M0, M1<br>AUX<br>VCC / GND | **GPIO 19 (RX2)**<br>**GPIO 18 (TX2)**<br>GND<br>*(Không nối)*<br>5V (LM2596) / GND | Nhận dữ liệu từ LoRa.<br>Gửi dữ liệu ra LoRa.<br>Nối thẳng xuống âm nguồn (Chạy Normal Mode 0).<br>Để trống, không bắt buộc nối.<br>Hàn tụ hóa 470µF 16V sát chân VCC-GND chống sụt áp. |
| **Module MOSFET D4184 (1)**<br>*(Kênh Quạt làm mát)* | PWM / TRIG<br>GND<br>VIN+ / VIN-<br>OUT+ / OUT- | **GPIO 27**<br>GND chung<br>+12V / GND (Tổ ong)<br>Quạt DC 12V | Chân băm xung PWM 20kHz điều khiển tốc độ quạt êm ái (Thay thế TB6612).<br>Chung mass với ESP32.<br>Nguồn 12V cấp cho quạt.<br>Nối cực (+) và (-) của quạt. |
| **Module MOSFET D4184 (2)**<br>*(Kênh Bóng sưởi ấm)* | PWM / TRIG<br>GND<br>VIN+ / VIN-<br>OUT+ / OUT- | **GPIO 14**<br>GND chung<br>+12V (hoặc 5V) / GND<br>5 Bóng sợi đốt // | Chân băm xung PWM 1kHz điều khiển nhiệt lượng dứt khoát (Thay thế TB6612).<br>Chung mass với ESP32.<br>Nguồn cấp công suất chịu dòng 2 - 3A.<br>Mắc song song 5 bóng sợi đốt vào ngõ ra. |
| **Máy bơm tưới nước** | IN (Relay)<br>VCC / GND | **GPIO 32**<br>+5V (LM2596) / GND | Tích cực Mức CAO (Kích HIGH bơm chạy). Tiếp điểm NO/COM đóng ngắt bơm 12V. |
| **Phun sương ẩm** | IN (Relay / MOSFET) | **GPIO 33** | Tích cực Mức CAO (Kích HIGH bật tạo ẩm). |
| **Đèn quang hợp** | IN (Relay / MOSFET) | **GPIO 4** | Tích cực Mức CAO (Kích HIGH bật dàn LED thanh). |
| **Van điện từ 1** | IN (Relay) | **GPIO 26** | Tích cực Mức CAO (Đã chuyển từ chân nhạy cảm GPIO 15 sang GPIO 26 an toàn). |
| **Van điện từ 2** | IN (Relay) | **GPIO 13** | Tích cực Mức CAO (Đã chuyển từ chân nhạy cảm GPIO 2 sang GPIO 13 an toàn). |
| **Mái che (Rèm cửa)** | Signal (Dây Cam - Servo)<br>VCC (Dây Đỏ)<br>GND (Dây Nâu/Đen) | **GPIO 25**<br>+5V (LM2596)<br>GND chung | Điều khiển Servo MG996R bằng xung PWM (Chuyển từ GPIO 0 sang 25 an toàn tuyệt đối).<br>TUYỆT ĐỐI KHÔNG cắm 5V ESP32 (ăn dòng 1.5A sụt áp).<br>Tụ lọc 470µF 16V mắc song song VCC-GND của Servo. |

--- | :--- | :--- | :--- | :--- | :--- |
| **I2C Bus 0** | **SHT30 (Trong)** | VCC / GND | 3.3V / GND | 3.3V | Tụ gốm 100nF đặt sát VCC-GND cảm biến. |
| *(Nhiệt/Ẩm trong & Ánh sáng)*| | SCL / SDA | **GPIO 22 / 21**| 3.3V | Trở pull-up 4.7kΩ nối lên 3.3V trên cả 2 đường SDA & SCL. |
| | **BH1750 (Lux)** | VCC / GND | 3.3V / GND | 3.3V | Chân ADDR nối cố định xuống GND (địa chỉ `0x23`). |
| | | SCL / SDA | **GPIO 22 / 21**| 3.3V | Đi chung bus I2C 0 với SHT30 Trong. |
| **I2C Bus 1** | **SHT30 (Ngoài)** | VCC / GND | 3.3V / GND | 3.3V | Tụ 100nF lọc nguồn; bọc dây chống nhiễu (Shielded Cable). |
| *(Nhiệt/Ẩm ngoài trời)* | | SCL / SDA | **GPIO 23 / 5** | 3.3V | Trở pull-up 4.7kΩ nối lên 3.3V trên chân 5 & 23. |
| **Độ ẩm đất 1** | **Capacitive v1.2**| VCC / GND | 3.3V / GND | 3.3V | Cấp 3.3V từ AMS1117. Header H23 trên mạch PCB. |
| *(Luống 1 / Van 1)* | | AOUT | **GPIO 34** | 0-3.3V | ADC1_CH6. Có tụ gốm C2 (1µF) nối từ GPIO 34 xuống GND lọc nhiễu ADC. |
| **Độ ẩm đất 2** | **Capacitive v1.2**| VCC / GND | 3.3V / GND | 3.3V | Cấp 3.3V từ AMS1117. Header H28 trên mạch PCB. |
| *(Luống 2 / Van 2)* | | AOUT | **GPIO 35** | 0-3.3V | ADC1_CH7. Có tụ gốm C1 (1µF) nối từ GPIO 35 xuống GND lọc nhiễu ADC. |
| **LoRa E32** | **E32-433T20D** | VCC / GND | 3.3V (AMS1117) / GND | 3.3V | Header H19 (1x8) trên bo mạch Master. |
| *(Truyền xa Gateway)* | *(hoặc T30D)* | RXD / TXD | **GPIO 18 / 19**| 3.3V | Pin 4 (RXD) nối TX2 (GPIO 18); Pin 5 (TXD) nối RX2 (GPIO 19). |
| | | M0 / M1 | GND / GND | 0V | BẮT BUỘC: Pin 2 (M0) & Pin 3 (M1) nối cứng GND để chạy Mode 0 (Normal). |
| **Liên kết HMI** | **Cáp UART** | TXD | **GPIO 17** | 3.3V | Header UART: Chân hmi_tx (GPIO 17) sang hmi_rx (GPIO 16) bo HMI. |
| *(Tủ điện điều khiển)* | | RXD | **GPIO 16** | 3.3V | Nối sang chân TX1 (GPIO 17) của ESP32 HMI. |
| | | GND | GND | 0V | Bắt buộc liên kết GND chung. |
| **Quạt thông gió DC** | **Module MOSFET D4184 (Mạch 1)** | PWM / TRIG | **GPIO 27** | 3.3V PWM| Xung PWM 20kHz điều khiển tốc độ gió mượt mà. |
| *(PID làm mát)* | | GND | **GND chung** | 0V | Nối chung mass với ESP32. |
| | | VIN+ / VIN- | **+12V / GND (Nguồn)** | 12V | Nguồn cấp cho quạt DC. |
| | | OUT+ / OUT- | **Quạt DC 12V** | 12V | Cực (+) quạt vào OUT+, Cực (-) vào OUT-. |
| **Bóng sưởi ấm** | **Module MOSFET D4184 (Mạch 2)** | PWM / TRIG | **GPIO 14** | 3.3V PWM| Xung PWM 1kHz điều khiển nhiệt lượng dứt khoát. |
| *(Cụm 5 bóng sợi đốt //)*| | GND | **GND chung** | 0V | Nối chung mass với ESP32. |
| | | VIN+ / VIN- | **+5V (hoặc +12V) / GND**| 5V/12V | Cấp nguồn công suất cao từ LM2596/Tổ ong (chịu dòng >3A). |
| | | OUT+ / OUT- | **Cụm 5 bóng sợi đốt** | 5V/12V | Nối song song 5 bóng vào ngõ ra OUT+ và OUT-. |
| **Bơm tưới nước** | **Module Relay 5V**| IN | **GPIO 32** | 3.3V | Relay cách ly quang (Optocoupler), Active HIGH. |
| | | VCC / GND | +5V (Buck) / GND | 5V | Nguồn nuôi cuộn hút lấy từ LM2596. |
| **Phun sương** | **Module MOSFET** | SIG / PWM | **GPIO 33** | 3.3V | Kích mở van/bơm phun sương 12V DC. |
| **Đèn quang hợp** | **Module MOSFET** | SIG / PWM | **GPIO 4** | 3.3V | Kích mở dàn LED thanh 12V DC. |
| **Van tưới luống 1**| **Module Relay 5V**| IN | **GPIO 26** | 3.3V | Active HIGH. Chuyển từ GPIO 15 sang GPIO 26 an toàn tuyệt đối. |
| **Van tưới luống 2**| **Module Relay 5V**| IN | **GPIO 13** | 3.3V | Active HIGH. Chuyển từ GPIO 2 sang GPIO 13 an toàn tuyệt đối. |
| **Mái che Servo** | **MG996R / SG90** | Tín hiệu (Cam) | **GPIO 25** | 3.3V PWM| Chuyển từ GPIO 0 sang GPIO 25, triệt tiêu 100% lỗi kẹt bootloader. |
| | | VCC (Đỏ) | **+5V (LM2596)** | 5V-6V | **TUYỆT ĐỐI KHÔNG cắm vào chân 5V của ESP32!** |
| | | GND (Nâu/Đen) | GND chung | 0V | Tụ hóa 470µF 16V mắc song song chân VCC-GND của Servo. |

---

## 3. NODE 2: HMI NODE (ESP32 - TỦ ĐIỆN ĐIỀU KHIỂN & GIÁM SÁT TẠI CHỖ)

Bo mạch sử dụng: **ESP32 DOIT DevKit V1 (30 chân)**.

### 3.1. Sơ đồ Pinout ESP32 HMI

```
                       ┌──────────────────────┐
                       │      ESP32 HMI       │
                EN ───1│                      │30─── GPIO 23 (LED Đỏ 5 - Bơm Tắt)
               GPIO 36 ───2│                      │29─── GPIO 22 (LCD I2C SCL)
               GPIO 39 ───3│                      │28─── GPIO 1  (TX0)
(LED Xanh 1)   GPIO 32 ───4│                      │27─── GPIO 3  (RX0)
(LED Xanh 2)   GPIO 33 ───5│                      │26─── GPIO 21 (LCD I2C SDA)
(LED Xanh 3)   GPIO 25 ───6│                      │25─── GPIO 19 (LED Xanh 5 - Bơm Bật)
(Nút Bấm 4)    GPIO 26 ───7│                      │24─── GPIO 18 (LED Đỏ 1 - Quạt Tắt)
(Nút Bấm 3)    GPIO 27 ───8│                      │23─── GPIO 5  (LED Đỏ 2 - Đèn Tắt)
(Nút Bấm 2)    GPIO 14 ───9│                      │22─── GPIO 17 (UART1 TX - Sang Master RX16)
(LED Xanh 4)   GPIO 12 ──10│                      │21─── GPIO 16 (UART1 RX - Từ Master TX17)
(Nút Bấm 1)    GPIO 13 ──11│                      │20─── GPIO 4  (LED Đỏ 3 - Sưởi Tắt)
               GPIO 9  ──12│                      │19─── GPIO 0  (Boot)
(LED Đỏ 4)     GPIO 2  ──13│                      │18─── GPIO 2  (Trạng thái)
(Nút Bấm 5)    GPIO 15 ──14│                      │17─── GPIO 15 (Nút Bấm 5 - Bơm)
                  GND ──15│                      │16─── 3V3
                       └──────────────────────┘
```

### 3.2. Bảng Đấu Nối Chi Tiết HMI Node

*ESP32 này chịu trách nhiệm nhận nút bấm thủ công, điều khiển biến trở xoay rèm cửa, xuất đèn LED báo trạng thái và hiển thị thông số lên LCD 2004.*

| Thiết bị / Module | Chân trên Thiết bị | Chân trên HMI ESP32 | Ghi chú |
| :--- | :--- | :--- | :--- |
| **Nguồn chung** | VCC / GND | **5V (hoặc 3.3V) / GND** | LCD dùng 5V từ LM2596; LED và biến trở dùng 3.3V. Đấu chung mass GND. |
| **Màn hình LCD 2004**<br>*(I2C Backpack PCF8574)* | SDA<br>SCL<br>VCC / GND | **GPIO 21**<br>**GPIO 22**<br>+5V (LM2596) / GND | Bus I2C địa chỉ 0x27. Kèm 2 trở pull-up 4.7kΩ nếu dây dài.<br>Cấp 5V để màn hình có độ sáng đèn nền và tương phản rõ nét.<br>**Dòng 4 LCD hiển thị trực tiếp góc mở Rèm (0° - 180°).** |
| **Giao tiếp Master**<br>*(ESP32 số 1)* | TX (Của Master ESP32)<br>RX (Của Master ESP32)<br>GND | **GPIO 16 (RX2)**<br>**GPIO 17 (TX2)**<br>GND | Đấu chéo UART Serial2 (Nhận trạng thái và cảm biến từ Master; gửi lệnh nút & góc rèm).<br>Bắt buộc liên kết mass chung. |
| **Biến trở chỉnh rèm tay**<br>*(Chiết áp xoay 10kΩ)* | Chân giữa (Wiper)<br>Chân ngoài 1 & 2 | **GPIO 34 (ADC1)**<br>3.3V / GND | Chân Input-Only, đọc ADC ánh xạ mượt mà sang góc 0° – 180° (bộ lọc EMA + deadband 3°).<br>2 chân ngoài chiết áp nối 3.3V và GND. |
| **Quạt thông gió (Fan)** | Nút bấm (Nhấn nhả)<br>LED Xanh (Đang Bật)<br>LED Đỏ (Đang Tắt) | **GPIO 13**<br>**GPIO 32**<br>**GPIO 18** | Cấu hình INPUT_PULLUP nội. Chân đối diện nút nối GND.<br>Anode (+) qua trở 220Ω - 330Ω vào GPIO; Cathode (-) về GND.<br>Anode (+) qua trở 220Ω - 330Ω vào GPIO; Cathode (-) về GND. |
| **Đèn chiếu sáng (Light)**| Nút bấm (Nhấn nhả)<br>LED Xanh (Đang Bật)<br>LED Đỏ (Đang Tắt) | **GPIO 14**<br>**GPIO 33**<br>**GPIO 5** | Cấu hình INPUT_PULLUP nội. Chân đối diện nút nối GND.<br>Anode (+) qua trở 220Ω - 330Ω vào GPIO; Cathode (-) về GND.<br>Anode (+) qua trở 220Ω - 330Ω vào GPIO; Cathode (-) về GND. |
| **Bóng sưởi ấm (Heater)**| Nút bấm (Nhấn nhả)<br>LED Xanh (Đang Bật)<br>LED Đỏ (Đang Tắt) | **GPIO 27**<br>**GPIO 25**<br>**GPIO 4** | Cấu hình INPUT_PULLUP nội. Chân đối diện nút nối GND.<br>Anode (+) qua trở 220Ω - 330Ω vào GPIO; Cathode (-) về GND.<br>Anode (+) qua trở 220Ω - 330Ω vào GPIO; Cathode (-) về GND. |
| **Phun sương ẩm (Mist)** | Nút bấm (Nhấn nhả)<br>LED Xanh (Đang Bật)<br>LED Đỏ (Đang Tắt) | **GPIO 26**<br>**GPIO 12**<br>**GPIO 2** | Cấu hình INPUT_PULLUP nội. Chân đối diện nút nối GND.<br>Anode (+) qua trở 220Ω - 330Ω vào GPIO; Cathode (-) về GND.<br>Anode (+) qua trở 220Ω - 330Ω vào GPIO; Cathode (-) về GND. |
| **Bơm tưới nước (Pump)** | Nút bấm (Nhấn nhả)<br>LED Xanh (Đang Bật)<br>LED Đỏ (Đang Tắt) | **GPIO 15**<br>**GPIO 19**<br>**GPIO 23** | Cấu hình INPUT_PULLUP nội. Chân đối diện nút nối GND.<br>Anode (+) qua trở 220Ω - 330Ω vào GPIO; Cathode (-) về GND.<br>Anode (+) qua trở 220Ω - 330Ω vào GPIO; Cathode (-) về GND. |
| **Rèm cửa (Mái che)** | Nút bấm & Đèn LED | *(ĐÃ BỎ HOÀN TOÀN)* | **Không cần nút bấm & không cần LED** (Đã có biến trở GPIO 34 và màn hình LCD).<br>👉 **Giải phóng GPIO 0 (BOOT), GPIO 1 (TX0), GPIO 3 (RX0)** giúp nạp USB mượt mà 100%. |

--- | :--- | :--- | :--- | :--- |
| **1. Quạt thông gió (Fan)** | **GPIO 13** | **GPIO 32** | **GPIO 18** | 220Ω – 330Ω |
| **2. Đèn chiếu sáng (Light)**| **GPIO 14** | **GPIO 33** | **GPIO 5** | 220Ω – 330Ω |
| **3. Bóng sưởi ấm (Heater)** | **GPIO 27** | **GPIO 25** | **GPIO 4** | 220Ω – 330Ω |
| **4. Phun sương ẩm (Mist)** | **GPIO 26** | **GPIO 12** | **GPIO 2** | 220Ω – 330Ω |
| **5. Bơm tưới nước (Pump)** | **GPIO 15** | **GPIO 19** | **GPIO 23** | 220Ω – 330Ω |
| **6. Rèm cửa (Mái che - Roof)** | *(Dùng Biến trở GPIO 34 xoay 0°-180°)* | *(Không cần LED - Xem góc LCD dòng 4)* | *(Không cần LED - Xem góc LCD dòng 4)* | -- |

> [!NOTE]
> **Giải phóng 100% các chân nhạy cảm trên ESP32 HMI:**
> 1. **Bỏ LED rèm cửa:** Giải phóng hoàn toàn **GPIO 1 (TX0)** và **GPIO 3 (RX0)**, giúp nạp firmware qua cổng USB và xem Serial Monitor trên máy tính mượt mà 100%, không bị chớp nháy LED hay nghẽn đường truyền.
> 2. **Bỏ nút bấm rèm cửa (Dùng Biến trở xoay 10kΩ tại GPIO 34):** Giải phóng chân **GPIO 0 (BOOT)**, loại trừ hoàn toàn nguy cơ vô tình kích hoạt chế độ nạp flash lúc khởi động.

### 3.3. Đấu Nối Màn Hình LCD 2004 I2C & Giao Tiếp Master

| Module | Chân Module | Chân ESP32 HMI | Ghi chú & Linh kiện bổ sung |
| :--- | :--- | :--- | :--- |
| **LCD 2004 I2C** | VCC | **+5V (Buck LM2596)** | Cấp 5V để màn hình có độ sáng đèn nền và tương phản chuẩn. |
| *(Backpack PCF8574)*| GND | **GND chung** | |
| | SDA | **GPIO 21** | Kèm 2 trở kéo 4.7kΩ lên 3.3V nếu dây I2C dài quá 20cm. |
| | SCL | **GPIO 22** | Địa chỉ mặc định: `0x27`. |
| **UART liên kết Master**| RX1 | **GPIO 16** | Nối chéo sang **TX (GPIO 17)** của ESP32 Master. |
| | TX1 | **GPIO 17** | Nối chéo sang **RX (GPIO 16)** của ESP32 Master. |
| | GND | **GND chung** | Bắt buộc liên kết mass. |
| **Biến trở chỉnh rèm tay**| Chân giữa (Wiper) | **GPIO 34** | Chân ADC1 (Input Only). Điều chỉnh góc mở rèm mượt mà 0° - 180°. |
| *(Chiết áp xoay 10kΩ)* | Chân ngoài 1 & 3 | **GND / 3.3V** | 2 chân ngoài nối nguồn 3.3V và GND. |

---

## 4. NODE 3: GATEWAY NODE (ESP32 - TRẠM THU LORA & WEB SERVER)

Bo mạch sử dụng: **ESP32 DOIT DevKit V1 (30 chân)**.

```
                       ┌──────────────────────┐
                       │    ESP32 GATEWAY     │
               VIN/5V ───1│                      │30─── GPIO 23
                  GND ───2│                      │29─── GPIO 22
               GPIO 13 ───3│                      │28─── GPIO 1 (TX0)
               GPIO 12 ───4│                      │27─── GPIO 3 (RX0)
               GPIO 14 ───5│                      │26─── GPIO 21
               GPIO 27 ───6│                      │25─── GPIO 19 (UART2 RX - Từ LoRa TX)
               GPIO 26 ───7│                      │24─── GPIO 18 (UART2 TX - Sang LoRa RX)
               GPIO 25 ───8│                      │23─── GPIO 5
               GPIO 33 ───9│                      │22─── GPIO 17
               GPIO 32 ──10│                      │21─── GPIO 16
               GPIO 35 ──11│                      │20─── GPIO 4
               GPIO 34 ──12│                      │19─── GPIO 0
               GPIO 39 ──13│                      │18─── GPIO 2 (LED Status WiFi - Onboard)
               GPIO 36 ──14│                      │17─── GPIO 15
                  EN  ──15│                      │16─── 3V3
                       └──────────────────────┘
```

### 4.1. Bảng Đấu Nối Chi Tiết Gateway Node

*ESP32 này nhận dữ liệu cảm biến từ Master qua LoRa, phát WiFi / Web Server cục bộ và đồng bộ dữ liệu lên Dashboard.*

| Thiết bị / Module | Chân trên Thiết bị | Chân trên Gateway ESP32 | Ghi chú |
| :--- | :--- | :--- | :--- |
| **Nguồn chung** | VCC / GND | **5V (hoặc MicroUSB) / GND** | Cấp nguồn 5V - 2A ổn định. Đấu chung mass GND. |
| **LoRa E32**<br>*(Thu nhận từ Master)* | TXD<br>RXD<br>M0, M1<br>AUX<br>VCC / GND | **GPIO 19 (RX2)**<br>**GPIO 18 (TX2)**<br>GND<br>*(Không nối)*<br>+5V (LM2596) / GND | Nhận dữ liệu gói tin từ Master gửi sang.<br>Gửi lệnh điều khiển từ Web về Master.<br>Nối cố định xuống GND (Mode 0: Normal Transparent).<br>Để trống, không bắt buộc nối.<br>Lắp tụ lọc 470µF 16V sát chân VCC-GND chống sụt áp. Anten ngoài 433MHz. |
| **LED Báo WiFi** | Anode (+)<br>Cathode (-) | **GPIO 2**<br>GND | Có thể dùng LED Onboard chân 2 hoặc LED rời kèm trở hạn dòng 220Ω.<br>Sáng báo hiệu kết nối WiFi / Web Server thành công. |

--- | :--- | :--- | :--- | :--- |
| **Module LoRa E32** | VCC | **+5V (LM2596)** | 5V | **Tụ lọc 470µF 16V** đặt sát chân VCC-GND. |
| *(Thu nhận từ Master)*| GND | **GND chung** | 0V | Nối anten ngoài 433MHz (Gain 3dBi hoặc 5dBi). |
| | TXD | **GPIO 19** (RX2) | 3.3V | |
| | RXD | **GPIO 18** (TX2) | 3.3V | |
| | M0, M1 | **GND** | 0V | Nối cố định xuống mass (Mode 0: Normal). |
| **LED Báo WiFi** | Anode (+) | **GPIO 2** | 3.3V | Có thể dùng LED Onboard hoặc LED rời kèm trở 220Ω. |

---

## 5. DANH MỤC LINH KIỆN BỔ TRỢ & PHỤ KIỆN MẠCH (BOM)

Để hệ thống hoạt động ổn định 24/7 trong môi trường độ ẩm cao của nhà kính, danh sách linh kiện thụ động và phụ kiện đấu nối sau đây cần được trang bị đầy đủ:

| Nhóm linh kiện | Tên & Thông số kỹ thuật | Số lượng | Vị trí lắp đặt & Mục đích sử dụng |
| :--- | :--- | :--- | :--- |
| **Nguồn & Hạ áp** | Bộ nguồn tổ ong 12V - 10A DC | 01 cái | Cấp toàn bộ tải động cơ, sưởi, van, bơm và mạch hạ áp. |
| | Mạch hạ áp xung DC-DC LM2596 (Out 5V - 3A) | 01 - 02 cái | Hạ 12V xuống 5V sạch cấp cho 3 ESP32, Servo, LoRa, LCD. |
| **Tụ lọc nguồn** | Tụ hóa nhôm 470µF / 25V | 03 cái | Lắp sát ngõ cấp nguồn của 2 module LoRa E32 và 1 động cơ Servo MG996R (chống sụt áp tức thời khi phát xung dòng cao). |
| | Tụ hóa nhôm 100µF / 25V | 02 cái | Lắp tại ngõ nguồn VM (12V) của các Module MOSFET D4184 và ngõ ra LM2596. |
| | Tụ gốm nhiều lớp (Monolithic) 100nF (0.1µF) | 08 cái | Lắp sát chân VCC-GND các cảm biến (SHT30, BH1750) và song song tiếp điểm nút nhấn để dập xung nhiễu nảy phím. |
| **Điện trở** | Điện trở kim loại 220Ω hoặc 330Ω (0.25W) | 10 cái | Mắc nối tiếp hạn dòng cho 10 đèn LED trên tủ HMI (bảo vệ GPIO ESP32). |
| | Điện trở kim loại 4.7kΩ (0.25W) | 06 cái | Làm điện trở kéo (Pull-up) cho 2 bus I2C (Bus 0: chân 21, 22; Bus 1: chân 5, 23). |
| **Đèn LED & Nút**| LED báo phi 5mm (5 Xanh lá, 5 Đỏ) | 10 cái | Lắp mặt tủ điện HMI báo trạng thái thiết bị. |
| | Nút nhấn nhả có khóa/không khóa (Pushbutton phi 12mm hoặc nút tủ điện) | 05 cái | Lắp mặt tủ điện để thao tác bật/tắt thủ công. |
| **Bảo vệ & Cách ly**| Cầu chì ống thủy tinh 5A - 10A kèm hộp gắn | 01 cái | Đặt ngay sau đầu ra (+) 12V của nguồn tổ ong đề phòng chập tải. |
| | Diode 1N4007 (Chống ngược dòng cuộn cảm) | 03 cái | Đấu ngược cực song song với cuộn dây van điện từ và máy bơm (Flyback diode triệt tiêu xung điện áp tự cảm dội ngược). |
| **Cáp & Cầu đấu** | Cầu đấu chia nguồn (Terminal Block 12P/24P) | 02 thanh | Phân phối các nhánh 12V, 5V và cầu chập điểm mass chung (GND). |
| | Cáp bọc kim chống nhiễu (Shielded Cable 4 lõi) | 5 - 10m | Kéo dài tín hiệu cảm biến SHT30 ngoài trời và độ ẩm đất. |

---

## 6. QUY CHUẨN THI CÔNG & BẢO VỆ MẠCH THỰC TẾ

1. **Quy tắc Mass Chung (Common Ground)**:
   * Tất cả các chân GND của nguồn 12V, mạch Buck 5V, 3 bo mạch ESP32, Module MOSFET D4184, các module Relay và cảm biến phải đấu chập về một điểm nối trung tâm (Star Topology Grounding). Tuyệt đối không nối đất dạng chuỗi vòng tròn (Ground Loop) để tránh nhiễu tín hiệu ADC và I2C.
2. **Cách ly nguồn tải động cơ (Servo & Bơm)**:
   * Động cơ Servo MG996R khi tải nặng có thể ăn dòng tức thời **1.5A**. Nếu lấy nguồn 5V từ chân VIN/USB của ESP32, bộ hạ áp LDO trên bo ESP32 sẽ lập tức bị sụt áp dưới 2.7V, kích hoạt mạch `Brownout Detector` gây khởi động lại vi điều khiển liên tục. **Bắt buộc cấp 5V cho Servo trực tiếp từ ngõ ra của mạch LM2596**.
3. **Các chân đặc biệt (Strapping Pins) của ESP32**:
   * `GPIO 0, 2, 12, 15`: Đã được giải phóng hoàn toàn trên Master Node (Servo chuyển sang GPIO 25, Van 1 chuyển sang GPIO 26, Van 2 chuyển sang GPIO 13). Nhờ đó triệt tiêu 100% rủi ro treo bootloader hoặc kẹt nạp code.
   * `GPIO 34` & `GPIO 35`: Cảm biến độ ẩm đất 1 (Header H23 + tụ C2) và 2 (Header H28 + tụ C1) cắm vào 2 chân này. Cả hai chân đều là **Input-only (ADC1)**, không có điện trở kéo nội bộ, hoàn toàn tối ưu và chuẩn xác cho tín hiệu Analog từ cảm biến độ ẩm đất, không bị ảnh hưởng hay xung đột khi bật WiFi như các chân ADC2.
   * `GPIO 12` và `GPIO 15`: Khi nạp firmware cần đảm bảo điện áp không bị cưỡng bức sai mức logic quy định của ESP32.
4. **Chống nhiễu đường truyền I2C ngoài trời**:
   * Dây dẫn nối cảm biến SHT30 Ngoài trời nếu kéo dài trên 1 mét cần sử dụng dây cáp bọc giáp chống nhiễu, nối lớp lưới kim loại xuống GND ở một đầu phía tủ điện.
