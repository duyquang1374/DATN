// =====================================================================
//  MÔ HÌNH 3D: CƠ CẤU RÈM CHỚP TỰ ĐỘNG THÔNG MINH (PHIÊN BẢN CHUẨN THỰC TẾ)
//  Dự án: Nhà Màng Thông Minh - ĐATN
//  Cơ cấu: Khung hình bình hành song song nhiều khâu (Parallel Linkage)
//  Khớp nối: Dùng vít M3 bắt vào 4 thanh dầm ở 2 đầu tấm lá
//  Phần mềm: OpenSCAD
// =====================================================================
// HƯỚNG DẪN SỬ DỤNG:
//   - F5 = Xem trước (Preview nhanh có màu sắc)
//   - F6 = Render chất lượng cao (Render để xuất file in 3D)
//   - Thay đổi biến "ANGLE" (từ 0 đến 85) để xem cơ cấu đóng mở
//   - Thay đổi "RENDER_PART" để xuất từng chi tiết riêng ra file STL
// =====================================================================

// ─────────────────────────────────────────────────────────────────────
//  1. CHỌN CHI TIẾT XUẤT FILE IN 3D (RENDER_PART)
//     0 = Toàn bộ mô hình lắp ráp (Full Assembly)
//     1 = Tấm lá chớp (50 x 250 x 5 mm, có 4 lỗ M3 ở 2 đầu)
//     2 = Thanh dầm / Thanh giằng (5 lỗ M3 cách nhau 44 mm)
//     3 = Cụm Khung dầm tĩnh & 4 Cột đỡ
//     4 = Đế gá động cơ Servo SG90
// ─────────────────────────────────────────────────────────────────────
RENDER_PART = 0;

// ─────────────────────────────────────────────────────────────────────
//  2. GÓC MỞ CỦA RÈM CHỚP (ANGLE: 0 đến 85 độ)
//     0  = ĐÓNG KÍN (che mưa khi cảm biến mưa kích hoạt, các lá nghiêng gối mí 15 độ)
//     45 = MỞ NỬA (thông thoáng vừa phải)
//     85 = MỞ TỐI ĐA (các tấm lá dựng đứng 85-90 độ để đón gió và nắng tối đa)
// ─────────────────────────────────────────────────────────────────────
ANGLE = 85;

// Độ mịn đường cong tròn (càng cao render càng đẹp)
$fn = 40;

// ─────────────────────────────────────────────────────────────────────
//  3. CÁC THÔNG SỐ HÌNH HỌC CHÍNH
// ─────────────────────────────────────────────────────────────────────

// Tấm lá chớp (Louver Blade)
BL_L         = 250;   // mm - Chiều dài tấm lá (chiều sâu khoang nhà màng = 25 cm)
BL_W         = 50;    // mm - Bề rộng tấm lá (= 5 cm)
BL_T         = 5;     // mm - Độ dày tấm lá (= 0.5 cm, đủ dày để bắt vít M3 chắc chắn)
HOLE_1_OFFSET = 3;    // mm - Vị trí lỗ 1 cách mép trên = 0.3 cm
HOLE_2_OFFSET = 47;   // mm - Vị trí lỗ 2 cách mép trên = 4.7 cm (cách mép dưới 0.3 cm)
HOLE_SPAN    = HOLE_2_OFFSET - HOLE_1_OFFSET; // = 44 mm (khoảng cách tâm giữa 2 lỗ)

// Số lượng tấm lá
N            = 5;

// Bước giữa các tấm lá (Pitch)
PITCH        = 44;    // mm - Đúng bằng HOLE_SPAN để tạo thành các hình bình hành chuẩn

// 4 Thanh dầm / Thanh giằng liên kết (Linkage Bars - 2 thanh trước, 2 thanh sau)
BAR_W        = 12;    // mm - Bề rộng thanh giằng
BAR_T        = 4;     // mm - Độ dày thanh giằng
BAR_MARGIN   = 10;    // mm - Khoảng chìa ra ở 2 đầu thanh
BAR_L        = (N - 1) * PITCH + 2 * BAR_MARGIN; // mm - Chiều dài thanh giằng (~196 mm)

// Đường kính lỗ vít M3 (3.4 mm lắp trơn, xoay tự do không bị kẹt)
BOLT_D       = 3.4;

// Khung dầm tĩnh & Cột đỡ nhà màng
BEAM_L       = 300;   // mm - Chiều dài tổng thể dầm đỡ (30 cm)
BEAM_W       = 14;    // mm - Bề ngang thanh dầm tĩnh
BEAM_T       = 8;     // mm - Độ dày thanh dầm tĩnh
COL_H        = 80;    // mm - Chiều cao cột đỡ (đủ không gian lắp đặt Servo)
COL_W        = 14;    // mm - Tiết diện cột vuông

// ─────────────────────────────────────────────────────────────────────
//  4. TÍNH TOÁN ĐỘNG HỌC (KINEMATICS)
// ─────────────────────────────────────────────────────────────────────

// Góc nghiêng thực tế của tấm lá (độ):
// Khi ANGLE = 0: các lá nghiêng 15 độ (đủ dốc để thoát nước mưa ra ngoài)
// Khi ANGLE = 85: các lá dựng đứng 85 độ (mở tối đa)
BLADE_ROT = 15 + (ANGLE / 85) * 70;

// Căn giữa cụm 5 lá chớp trên chiều dài dầm 300mm
LOUVER_SPAN = (N - 1) * PITCH;
MARGIN_X    = (BEAM_L - LOUVER_SPAN) / 2;

// Tọa độ trục xoay trên của từng lá chớp (i = 0 đến N-1):
// Trục xoay trên cố định với khung trên
function pivot_top_x(i) = MARGIN_X + i * PITCH;
PIVOT_TOP_Z             = COL_H + HOLE_SPAN; // Độ cao trục xoay trên

// Tọa độ trục xoay dưới của từng lá chớp (chuyển động theo góc nghiêng):
// X_bot dịch sang phải theo cosin, Z_bot hạ xuống theo sin
function pivot_bot_x(i) = pivot_top_x(i) + HOLE_SPAN * cos(BLADE_ROT);
function pivot_bot_z()  = PIVOT_TOP_Z - HOLE_SPAN * sin(BLADE_ROT);

// ─────────────────────────────────────────────────────────────────────
//  5. BẢNG MÀU SẮC TRỰC QUAN (GIỐNG VIDEO THỰC TẾ)
// ─────────────────────────────────────────────────────────────────────
C_BLADE  = [0.96, 0.96, 0.94];  // Trắng sữa (tấm lá mica/nhựa)
C_BAR    = [0.15, 0.15, 0.16];  // Đen mờ (4 thanh giằng in 3D màu đen)
C_FRAME  = [0.92, 0.93, 0.95];  // Trắng sáng (khung hộp mô hình nhà màng)
C_SERVO  = [0.10, 0.45, 0.90];  // Xanh dương trong suốt (Servo SG90)
C_LABEL  = [0.95, 0.25, 0.45];  // Hồng / Đỏ (tem dán servo SG90)
C_HORN   = [0.15, 0.15, 0.15];  // Đen (đĩa tròn servo horn)
C_BOLT   = [0.82, 0.84, 0.88];  // Ánh bạc inox (đầu vít M3 lục giác)
C_SENSOR = [0.18, 0.18, 0.18];  // Bo mạch đen cảm biến mưa
C_COPPER = [0.85, 0.55, 0.25];  // Vân mạch đồng cảm biến mưa

// ─────────────────────────────────────────────────────────────────────
//  6. CÁC MODULE CHI TIẾT
// ─────────────────────────────────────────────────────────────────────

// === CHI TIẾT 1: TẤM LÁ CHỚP (Louver Blade) ===
// Kích thước: 50 x 250 x 5 mm
// 4 lỗ vít M3: 2 lỗ ở đầu trước (Y=0) và 2 lỗ ở đầu sau (Y=BL_L)
// Lỗ 1 cách mép trên 3 mm, Lỗ 2 cách mép trên 47 mm
module blade() {
    color(C_BLADE)
    difference() {
        // Thân tấm lá (gốc tọa độ đặt tại tâm lỗ xoay trên)
        translate([-HOLE_1_OFFSET, 0, -BL_T/2])
            cube([BL_W, BL_L, BL_T]);

        // --- 2 LỖ BẮT VÍT ĐẦU TRƯỚC (Y = 0) ---
        // Lỗ trên: tại X = 0 (cách mép 3mm)
        translate([0, -1, 0])
            rotate([-90, 0, 0])
                cylinder(d=BOLT_D, h=25, $fn=24);

        // Lỗ dưới: tại X = HOLE_SPAN = 44mm (cách mép 47mm)
        translate([HOLE_SPAN, -1, 0])
            rotate([-90, 0, 0])
                cylinder(d=BOLT_D, h=25, $fn=24);

        // --- 2 LỖ BẮT VÍT ĐẦU SAU (Y = BL_L) ---
        // Lỗ trên: tại X = 0
        translate([0, BL_L + 1, 0])
            rotate([90, 0, 0])
                cylinder(d=BOLT_D, h=25, $fn=24);

        // Lỗ dưới: tại X = HOLE_SPAN = 44mm
        translate([HOLE_SPAN, BL_L + 1, 0])
            rotate([90, 0, 0])
                cylinder(d=BOLT_D, h=25, $fn=24);
    }
}

// === CHI TIẾT 2: THANH DẦM / THANH GIẰNG (Linkage Bar) ===
// Dùng chung cho cả 4 thanh: 2 thanh trên (cố định/treo) và 2 thanh dưới (động)
// 5 lỗ M3 cách nhau 44 mm, 2 đầu bo tròn R = BAR_W/2
module linkage_bar() {
    color(C_BAR)
    difference() {
        // Thân thanh dẹp bo tròn 2 đầu
        hull() {
            // Đầu trái bo tròn
            translate([-BAR_MARGIN + BAR_W/2, 0, 0])
                rotate([-90, 0, 0])
                    cylinder(d=BAR_W, h=BAR_T, center=false, $fn=30);

            // Đầu phải bo tròn
            translate([(N - 1) * PITCH + BAR_MARGIN - BAR_W/2, 0, 0])
                rotate([-90, 0, 0])
                    cylinder(d=BAR_W, h=BAR_T, center=false, $fn=30);
        }

        // 5 Lỗ M3 liên kết các trục xoay của lá chớp
        for (i = [0:N-1]) {
            translate([i * PITCH, -1, 0])
                rotate([-90, 0, 0])
                    cylinder(d=BOLT_D, h=BAR_T + 2, $fn=24);
        }
    }
}

// === BU LÔNG / VÍT M3 ĐẦU DÙ INOX (Mô phỏng khớp xoay) ===
module bolt_m3() {
    color(C_BOLT) {
        // Đầu mũ vít tròn nằm áp sát mặt ngoài thanh dầm
        translate([0, 0, -1.8])
            cylinder(d=5.8, h=1.8, $fn=24);
        // Lỗ lục giác chìm
        translate([0, 0, -1.9])
            cylinder(d=2.2, h=0.9, $fn=6);
        // Thân ren vít M3 đâm xuyên qua thanh dầm vào đầu tấm lá
        translate([0, 0, 0])
            cylinder(d=3.0, h=BAR_T + 12, $fn=20);
    }
}

// === ĐỘNG CƠ SERVO SG90 (Mô phỏng chân thực) ===
module servo_sg90() {
    // Thân servo xanh dương
    color(C_SERVO, 0.85) {
        // Khối thân chính
        cube([22.8, 12.2, 22.8]);
        // Tai bắt vít 2 bên
        translate([-4.8, 0, 16.0]) cube([32.4, 12.2, 2.5]);
        // Cổ trục nhô lên
        translate([11.4 - 5.7, 6.1, 22.8])
            cylinder(d=12.0, h=4.0, $fn=24);
        // Trục bánh răng ra
        translate([5.7, 6.1, 26.8])
            cylinder(d=4.8, h=3.0, $fn=20);
    }

    // Tem dán màu hồng nhãn hiệu TowerPro SG90
    color(C_LABEL)
        translate([-0.1, 1.5, 5])
            cube([0.3, 9.2, 11]);

    // Đĩa tròn Servo (Servo Horn) màu đen nối trực tiếp vào trục xoay lá chớp
    color(C_HORN) {
        translate([5.7, 6.1, 29.8]) {
            cylinder(d=16, h=2.5, $fn=30);
            // Ốc khóa tâm đĩa
            translate([0, 0, 2.0])
                cylinder(d=4, h=1.2, $fn=16);
            // Các lỗ bắt ốc phụ quanh đĩa
            for (a = [0:60:300]) {
                rotate([0, 0, a])
                    translate([5.5, 0, -0.5])
                        cylinder(d=1.5, h=3.5, $fn=12);
            }
        }
    }
}

// === MÔ-ĐUN CẢM BIẾN MƯA (Rain Sensor Module - như trong video) ===
module rain_sensor() {
    // Tấm mạch in FR4 màu đen (50 x 40 x 1.6 mm)
    color(C_SENSOR)
        cube([50, 40, 1.6]);

    // Các đường vân mạ đồng zíc-zắc phát hiện giọt nước
    color(C_COPPER) {
        for (k = [6:3:44]) {
            translate([k, 4, 1.6])
                cube([1.2, 32, 0.1]);
        }
        // Thanh đồng gom 2 cực trên - dưới
        translate([6, 3, 1.6])  cube([39, 1.5, 0.1]);
        translate([6, 35, 1.6]) cube([39, 1.5, 0.1]);
    }

    // Header cắm dây 2 chân
    color([0.2, 0.2, 0.2])
        translate([42, 17, 1.6])
            cube([6, 6, 8]);
}

// === CHI TIẾT 3: KHUNG DẦM TĨNH & CỘT ĐỠ NHÀ MÀNG ===
module frame_assembly() {
    color(C_FRAME) {
        // --- 4 CỘT TRỤ ĐỨNG ---
        // Cột Trước - Trái
        translate([0, -BAR_T - BEAM_W, 0])
            cube([COL_W, COL_W, COL_H + 4]);
        // Cột Trước - Phải
        translate([BEAM_L - COL_W, -BAR_T - BEAM_W, 0])
            cube([COL_W, COL_W, COL_H + 4]);
        // Cột Sau - Trái
        translate([0, BL_L + BAR_T, 0])
            cube([COL_W, COL_W, COL_H + 4]);
        // Cột Sau - Phải
        translate([BEAM_L - COL_W, BL_L + BAR_T, 0])
            cube([COL_W, COL_W, COL_H + 4]);

        // --- THANH DẦM TĨNH ĐỠ DƯỚI (PHÍA TRƯỚC VÀ SAU) ---
        translate([0, -BAR_T - BEAM_W, COL_H - BEAM_T])
            cube([BEAM_L, BEAM_W, BEAM_T]);
        translate([0, BL_L + BAR_T, COL_H - BEAM_T])
            cube([BEAM_L, BEAM_W, BEAM_T]);

        // --- VÁCH ĐỠ VÀ GÁ LẮP MÁI TRẮNG (như mô hình hộp trong video) ---
        // Vách bên phải đỡ cảm biến mưa
        translate([BEAM_L - COL_W, -BAR_T - BEAM_W, COL_H])
            cube([COL_W, BL_L + 2*BAR_T + 2*BEAM_W, 45]);

        // Bệ đỡ cảm biến mưa nhô ra ngoài
        translate([BEAM_L + 5, 40, COL_H + 20])
            cube([60, 50, 4]);

        // Vách sau gá Servo
        translate([pivot_top_x(N-1) - 15, BL_L + BAR_T + 5, COL_H])
            cube([45, 4, 55]);
    }
}

// ─────────────────────────────────────────────────────────────────────
//  7. LẮP RÁP TOÀN BỘ CƠ CẤU (FULL ASSEMBLY)
// ─────────────────────────────────────────────────────────────────────
module full_assembly() {

    // 1. KHUNG NHÀ MÀNG & CỘT ĐỠ
    frame_assembly();

    // 2. CẢM BIẾN MƯA (ĐẶT BÊN CẠNH RÈM CHỚP NHƯ TRONG VIDEO)
    translate([BEAM_L + 10, 45, COL_H + 24])
        rotate([0, 0, 0])
            rain_sensor();

    // 3. ĐỘNG CƠ SERVO SG90 (GẮN Ở VÁCH SAU, ĐỒNG TRỤC VỚI LÁ SỐ 5)
    // Trục xoay servo thẳng hàng với lỗ trên của lá thứ 5 tại đầu sau
    translate([pivot_top_x(N-1) - 5.7, BL_L + BAR_T + 6 + 12.2, PIVOT_TOP_Z - 6.1])
        rotate([90, 0, 0])
            servo_sg90();

    // 4. HAI THANH DẦM TRÊN (FRONT-TOP & REAR-TOP) - CỐ ĐỊNH TẠI TRỤC TRÊN
    // Thanh trên phía Trước (Y = -BAR_T)
    translate([MARGIN_X, -BAR_T, PIVOT_TOP_Z])
        linkage_bar();

    // Thanh trên phía Sau (Y = BL_L)
    translate([MARGIN_X, BL_L, PIVOT_TOP_Z])
        linkage_bar();

    // 5. NĂM TẤM LÁ CHỚP (QUAY THEO GÓC BLADE_ROT)
    for (i = [0:N-1]) {
        // Trục xoay trên đặt tại (pivot_top_x(i), 0, PIVOT_TOP_Z)
        // Khi quay quanh trục Y:
        // Góc BLADE_ROT = 85 (đứng thẳng), BLADE_ROT = 15 (nghiêng đóng kín)
        translate([pivot_top_x(i), 0, PIVOT_TOP_Z])
            rotate([0, BLADE_ROT, 0])
                blade();
    }

    // 6. HAI THANH GIẰNG DƯỚI (FRONT-BOTTOM & REAR-BOTTOM) - CHUYỂN ĐỘNG THEO LÁ
    // Vị trí thanh dưới tính theo tọa độ các trục xoay dưới của 5 lá
    x_bot = pivot_bot_x(0);
    z_bot = pivot_bot_z();

    // Thanh dưới phía Trước (Y = -BAR_T)
    translate([x_bot, -BAR_T, z_bot])
        linkage_bar();

    // Thanh dưới phía Sau (Y = BL_L)
    translate([x_bot, BL_L, z_bot])
        linkage_bar();

    // 7. BU LÔNG / VÍT M3 ĐẦU DÙ LIÊN KẾT TẠI CÁC KHỚP XOAY
    for (i = [0:N-1]) {
        // Vít ở trục trên - Phía trước
        translate([pivot_top_x(i), -BAR_T, PIVOT_TOP_Z])
            rotate([-90, 0, 0])
                bolt_m3();

        // Vít ở trục trên - Phía sau
        translate([pivot_top_x(i), BL_L + BAR_T, PIVOT_TOP_Z])
            rotate([90, 0, 0])
                bolt_m3();

        // Vít ở trục dưới - Phía trước
        translate([pivot_bot_x(i), -BAR_T, pivot_bot_z()])
            rotate([-90, 0, 0])
                bolt_m3();

        // Vít ở trục dưới - Phía sau
        translate([pivot_bot_x(i), BL_L + BAR_T, pivot_bot_z()])
            rotate([90, 0, 0])
                bolt_m3();
    }
}

// ─────────────────────────────────────────────────────────────────────
//  8. ĐIỀU PHỐI XUẤT HÌNH ẢNH / CHI TIẾT IN 3D
// ─────────────────────────────────────────────────────────────────────
if (RENDER_PART == 0) {
    // Xem toàn bộ mô hình hoàn chỉnh
    full_assembly();
}
else if (RENDER_PART == 1) {
    // Xuất riêng 1 Tấm lá chớp (in 5 tấm)
    // Đặt nằm phẳng trên bàn in 3D
    translate([0, 0, BL_T/2])
        blade();
}
else if (RENDER_PART == 2) {
    // Xuất riêng 1 Thanh dầm / Thanh giằng (in 4 thanh)
    // Đặt nằm phẳng trên bàn in 3D
    rotate([90, 0, 0])
        linkage_bar();
}
else if (RENDER_PART == 3) {
    // Xuất cụm khung đỡ tĩnh
    frame_assembly();
}
else if (RENDER_PART == 4) {
    // Xuất đế gá Servo SG90
    color(C_FRAME) {
        difference() {
            cube([35, 18, 30]);
            // Khe nhét thân servo SG90
            translate([6, -1, 4])
                cube([23.2, 20, 13]);
            // 2 Lỗ bắt vít gá tai servo
            translate([2.5, 9, -1])  cylinder(d=2.2, h=32, $fn=16);
            translate([32.5, 9, -1]) cylinder(d=2.2, h=32, $fn=16);
        }
    }
}