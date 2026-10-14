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
  │ TB6612 (VM)  │─── Quạt DC 12V, Bóng sưởi 12V │ LM2596 Buck  │ (12V xuống 5V - 3A)
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
               GPIO 25 ───6│ (TB6612 AIN1 - Quạt) │25─── GPIO 19 (UART2 RX - Từ LoRa TX)
               GPIO 26 ───7│ (TB6612 AIN2 - Quạt) │24─── GPIO 18 (UART2 TX - Sang LoRa RX)
               GPIO 27 ───8│ (TB6612 PWMA - Quạt) │23─── GPIO 5  (I2C1_SDA - SHT30 Ngoài)
               GPIO 14 ───9│ (TB6612 PWMB - Sưởi) │22─── GPIO 17 (UART1 TX - Sang HMI RX)
(TB6612 BIN1)  GPIO 12 ──10│                      │21─── GPIO 16 (UART1 RX - Từ HMI TX)
(TB6612 BIN2)  GPIO 13 ──11│                      │20─── GPIO 4  (MOSFET Đèn quang hợp)
 (Servo Mái)   GPIO 0  ──12│                      │19─── GPIO 0  (Kéo lên bootloader)
 (Relay Van 2) GPIO 2  ──13│                      │18─── GPIO 2  (Trạng thái nội)
 (Relay Van 1) GPIO 15 ──14│                      │17─── GPIO 15 (Relay Van 1)
                  GND ──15│ (Mass chung)         │16─── 3V3   (Nguồn nuôi cảm biến)
                       └──────────────────────┘
```

### 2.2. Bảng Đấu Nối Chi Tiết Khối Ngoại Vi Master

| Khối chức năng | Tên thiết bị | Chân thiết bị | Chân ESP32 | Mức áp | Linh kiện bổ sung khuyến nghị |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **I2C Bus 0** | **SHT30 (Trong)** | VCC / GND | 3.3V / GND | 3.3V | Tụ gốm 100nF đặt sát VCC-GND cảm biến. |
| *(Nhiệt/Ẩm trong & Ánh sáng)*| | SCL / SDA | **GPIO 22 / 21**| 3.3V | Trở pull-up 4.7kΩ nối lên 3.3V trên cả 2 đường SDA & SCL. |
| | **BH1750 (Lux)** | VCC / GND | 3.3V / GND | 3.3V | Chân ADDR nối cố định xuống GND (địa chỉ `0x23`). |
| | | SCL / SDA | **GPIO 22 / 21**| 3.3V | Đi chung bus I2C 0 với SHT30 Trong. |
| **I2C Bus 1** | **SHT30 (Ngoài)** | VCC / GND | 3.3V / GND | 3.3V | Tụ 100nF lọc nguồn; bọc dây chống nhiễu (Shielded Cable). |
| *(Nhiệt/Ẩm ngoài trời)* | | SCL / SDA | **GPIO 23 / 5** | 3.3V | Trở pull-up 4.7kΩ nối lên 3.3V trên chân 5 & 23. |
| **Độ ẩm đất** | **Capacitive v1.2**| VCC / GND | 3.3V / GND | 3.3V | Tránh cấp 5V vì điện áp ra analog sẽ vượt quá 3.3V của ADC ESP32. |
| *(Analog ADC)* | | AOUT | **GPIO 34** | 0-3.3V | Tụ gốm 100nF nối từ GPIO 34 xuống GND (lọc nhiễu răng cưa ADC). |
| **LoRa E32** | **E32-433T20D** | VCC / GND | 5V (Buck) / GND | 5V | **Tụ hóa 470µF 16V** hàn ngay tại chân VCC-GND của E32. |
| *(Truyền xa Gateway)* | *(hoặc T30D)* | RXD / TXD | **GPIO 18 / 19**| 3.3V | TXD module nối vào RX2 (GPIO 19); RXD nối TX2 (GPIO 18). |
| | | M0 / M1 | GND / GND | 0V | Nối cứng GND để module luôn chạy Mode 0 (Normal Transparent). |
| **Liên kết HMI** | **Cáp UART** | TXD | **GPIO 17** | 3.3V | Nối sang chân RX1 (GPIO 16) của ESP32 HMI. |
| *(Tủ điện điều khiển)* | | RXD | **GPIO 16** | 3.3V | Nối sang chân TX1 (GPIO 17) của ESP32 HMI. |
| | | GND | GND | 0V | Bắt buộc liên kết GND chung. |
| **Driver Quạt DC** | **TB6612FNG** | VM / VCC | +12V / +5V | 12V/5V | Tụ hóa 100µF 25V gắn tại cực nguồn VM-GND. |
| *(Kênh A - PID)* | | PWMA | **GPIO 27** | 3.3V PWM| Xung PWM điều khiển tốc độ (LEDC tần số 20kHz). |
| | | AIN1 / AIN2 | **GPIO 25 / 26**| 3.3V | Mức logic chiều quay (HIGH/LOW). |
| | | AO1, AO2 | Quạt DC 12V | 12V | Nối 2 dây quạt DC 12V. |
| | | STBY | +5V hoặc 3.3V | HIGH | Nối mức CAO để kích hoạt IC driver hoạt động. |
| **Driver Sưởi** | **TB6612FNG** | PWMB | **GPIO 14** | 3.3V PWM| Xung PWM điều khiển công suất nhiệt sưởi (LEDC). |
| *(Kênh B - PID)* | | BIN1 / BIN2 | **GPIO 12 / 13**| 3.3V | Mức logic kích mở sưởi (HIGH/LOW). |
| | | BO1, BO2 | Bóng sưởi 12V | 12V | Nối 2 cực bóng sưởi hoặc thanh nhiệt bán dẫn. |
| **Bơm tưới nước** | **Module Relay 5V**| IN | **GPIO 32** | 3.3V | Relay cách ly quang (Optocoupler), Active HIGH. |
| | | VCC / GND | +5V (Buck) / GND | 5V | Nguồn nuôi cuộn hút lấy từ LM2596. |
| **Phun sương** | **Module MOSFET** | SIG / PWM | **GPIO 33** | 3.3V | Kích mở van/bơm phun sương 12V DC. |
| **Đèn quang hợp** | **Module MOSFET** | SIG / PWM | **GPIO 4** | 3.3V | Kích mở dàn LED thanh 12V DC. |
| **Van tưới luống 1**| **Module Relay 5V**| IN | **GPIO 15** | 3.3V | Active HIGH. Tiếp điểm đóng mở van điện từ 12V DC. |
| **Van tưới luống 2**| **Module Relay 5V**| IN | **GPIO 2** | 3.3V | Active HIGH. Tiếp điểm đóng mở van điện từ 12V DC. |
| **Mái che Servo** | **MG996R / SG90** | Tín hiệu (Cam) | **GPIO 0** | 3.3V PWM| Điều khiển góc 0° (đóng mái) - 180° (mở mái). |
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

### 3.2. Bảng Đấu Nối Chi Tiết Nút Bấm & LED Chỉ Thị

* **Cơ chế Nút bấm**: Cấu hình `INPUT_PULLUP` nội. Một chân nút nối vào GPIO, chân đối diện nối trực tiếp về **GND** (Nhấn = `LOW`).
* **Cơ chế Đèn LED**: Chân Anode (+) nối tiếp qua **điện trở 220Ω - 330Ω** vào GPIO, chân Cathode (-) nối về **GND** (Xuất `HIGH` = Sáng).

| Thiết bị kiểm soát | Nút bấm thao tác | LED XANH (Báo Đang Bật) | LED ĐỎ (Báo Đang Tắt) | Trở hạn dòng LED |
| :--- | :--- | :--- | :--- | :--- |
| **1. Quạt thông gió (Fan)** | **GPIO 13** | **GPIO 32** | **GPIO 18** | 220Ω – 330Ω |
| **2. Đèn chiếu sáng (Light)**| **GPIO 14** | **GPIO 33** | **GPIO 5** | 220Ω – 330Ω |
| **3. Bóng sưởi ấm (Heater)** | **GPIO 27** | **GPIO 25** | **GPIO 4** | 220Ω – 330Ω |
| **4. Phun sương ẩm (Mist)** | **GPIO 26** | **GPIO 12** | **GPIO 2** | 220Ω – 330Ω |
| **5. Bơm tưới nước (Pump)** | **GPIO 15** | **GPIO 19** | **GPIO 23** | 220Ω – 330Ω |

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

### Bảng Đấu Nối Gateway

| Module / Ngoại vi | Chân Module | Chân ESP32 Gateway | Mức áp | Linh kiện bổ sung khuyến nghị |
| :--- | :--- | :--- | :--- | :--- |
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
| | Tụ hóa nhôm 100µF / 25V | 02 cái | Lắp tại ngõ nguồn VM (12V) của Driver TB6612FNG và ngõ ra LM2596. |
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
   * Tất cả các chân GND của nguồn 12V, mạch Buck 5V, 3 bo mạch ESP32, TB6612FNG, các module Relay và cảm biến phải đấu chập về một điểm nối trung tâm (Star Topology Grounding). Tuyệt đối không nối đất dạng chuỗi vòng tròn (Ground Loop) để tránh nhiễu tín hiệu ADC và I2C.
2. **Cách ly nguồn tải động cơ (Servo & Bơm)**:
   * Động cơ Servo MG996R khi tải nặng có thể ăn dòng tức thời **1.5A**. Nếu lấy nguồn 5V từ chân VIN/USB của ESP32, bộ hạ áp LDO trên bo ESP32 sẽ lập tức bị sụt áp dưới 2.7V, kích hoạt mạch `Brownout Detector` gây khởi động lại vi điều khiển liên tục. **Bắt buộc cấp 5V cho Servo trực tiếp từ ngõ ra của mạch LM2596**.
3. **Các chân đặc biệt (Strapping Pins) của ESP32**:
   * `GPIO 0`: Được nối với Servo Mái che. Lúc bootloader, chân 0 phải ở mức `HIGH`. Đảm bảo chân tín hiệu của Servo không kéo chân 0 xuống mass lúc vừa cắm nguồn.
   * `GPIO 34`: Cảm biến độ ẩm đất cắm vào chân này. Chân này là **Input-only**, không có điện trở kéo nội bộ, hoàn toàn tối ưu và chuẩn xác cho tín hiệu Analog từ cảm biến độ ẩm đất.
   * `GPIO 12` và `GPIO 15`: Khi nạp firmware cần đảm bảo điện áp không bị cưỡng bức sai mức logic quy định của ESP32.
4. **Chống nhiễu đường truyền I2C ngoài trời**:
   * Dây dẫn nối cảm biến SHT30 Ngoài trời nếu kéo dài trên 1 mét cần sử dụng dây cáp bọc giáp chống nhiễu, nối lớp lưới kim loại xuống GND ở một đầu phía tủ điện.
