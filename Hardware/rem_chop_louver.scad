// =====================================================================
//  MÔ HÌNH 3D: CƠ CẤU MÁI CHỚP CHE NẮNG TỰ ĐỘNG (LOUVER MECHANISM)
//  Dự án: Nhà Màng Thông Minh - ĐATN
//  Thông số: 5 tấm mái 5x25cm, Dầm giằng 30cm
//  Phần mềm: OpenSCAD (miễn phí)
// =====================================================================

// ─────────────────────────────────────────────────────────────────────
//  THÔNG SỐ TOÀN HỆ THỐNG (Chỉnh số ở đây, toàn bộ mô hình tự cập nhật)
// ─────────────────────────────────────────────────────────────────────

// --- Thông số tấm mái chớp ---
blade_length    = 250;  // mm - Chiều dài tấm lá chớp (chiều trước-sau)
blade_width     = 50;   // mm - Chiều rộng tấm lá chớp (chiều ngang che phủ)
blade_thick     = 2.5;  // mm - Độ dày tấm lá chớp (mica/nhựa PVC)
num_blades      = 5;    // Số lượng tấm lá chớp

// --- Thông số khoảng cách trục xoay ---
pitch           = 44;   // mm - Khoảng cách tâm-tâm giữa 2 trục xoay liên tiếp
arm_height      = 18;   // mm - Chiều cao tay đòn (Lỗ gốc -> Lỗ chốt thanh giằng)

// --- Thông số khung đỡ tĩnh ---
beam_length     = 300;  // mm - Chiều dài thanh dầm giằng (30cm)
beam_width      = 15;   // mm - Chiều ngang thanh dầm
beam_thick      = 8;    // mm - Độ dày thanh dầm
col_height      = 60;   // mm - Chiều cao 4 cột trụ đứng
col_w           = 15;   // mm - Chiều ngang mặt cắt cột trụ

// --- Thông số tai xoay / bản lề (Pivot Bracket) ---
bracket_h       = 22;   // mm - Chiều cao tai xoay (tính từ đáy lá chớp xuống)
bracket_w       = 10;   // mm - Chiều rộng tai xoay
bracket_thick   = 2;    // mm - Độ dày tai xoay
bolt_r          = 1.6;  // mm - Bán kính lỗ ốc M3 (đường kính 3.2mm, xoay trơn)

// --- Thông số thanh giằng di động (Linkage Rod) ---
link_thick      = 4;    // mm - Độ dày thanh giằng động
link_height     = 6;    // mm - Chiều cao mặt cắt thanh giằng

// --- Góc mở hiện tại (Thay đổi để xem mô hình ở trạng thái khác nhau) ---
// 0   = Đóng hoàn toàn (các lá nằm ngang che kín)
// 90  = Mở tối đa (các lá dựng đứng thông gió)
open_angle      = 45;   // <<< CHỈNH GIÁ TRỊ NÀY để xem các trạng thái khác nhau

// --- Màu sắc hiển thị ---
COLOR_FRAME     = [0.9, 0.9, 0.92, 1.0];   // Xám sáng - Khung & dầm
COLOR_BLADE     = [0.75, 0.92, 1.0, 0.85]; // Xanh nhạt trong - Tấm lá chớp (mica)
COLOR_BRACKET   = [0.6, 0.6, 0.6, 1.0];    // Xám tối - Tai xoay (kim loại)
COLOR_LINKAGE   = [0.85, 0.45, 0.1, 1.0];  // Cam - Thanh giằng động (nổi bật)
COLOR_SERVO     = [0.15, 0.45, 0.85, 1.0]; // Xanh dương - Khối Servo

// ─────────────────────────────────────────────────────────────────────
//  TÍNH TOÁN DẪN XUẤT (Tự động)
// ─────────────────────────────────────────────────────────────────────
total_span   = (num_blades - 1) * pitch;  // Tổng chiều dài vùng giàn mái chớp
margin       = (beam_length - total_span - blade_width) / 2; // Khoảng dư 2 đầu dầm
start_x      = margin + blade_width/2; // Tọa độ X của trục tấm lá đầu tiên

linkage_y_offset = arm_height; // Thanh giằng động nằm phía trên dầm tĩnh 1 đoạn arm_height
linkage_x_shift  = arm_height * sin(open_angle); // Độ dịch ngang tương ứng góc mở

// ─────────────────────────────────────────────────────────────────────
//  CÁC MODULE VẼ TỪNG KHỐI
// ─────────────────────────────────────────────────────────────────────

// --- Module: Thanh dầm tĩnh (ngang, có 5 lỗ) ---
module static_beam(xpos_list) {
    color(COLOR_FRAME)
    difference() {
        // Thân thanh dầm
        cube([beam_length, beam_thick, beam_width], center=false);
        // Khoét 5 lỗ ốc M3
        for (x = xpos_list) {
            translate([x, beam_thick/2, beam_width/2])
                rotate([90,0,0])
                    cylinder(r=bolt_r, h=beam_thick+2, center=true, $fn=20);
        }
    }
}

// --- Module: Cột trụ đứng góc ---
module corner_column(is_back) {
    color(COLOR_FRAME)
    difference() {
        cube([col_w, blade_length, col_w]);
        // Khe thông gió nhỏ trang trí (tuỳ chọn)
    }
}

// --- Module: Một tấm lá chớp (xoay theo góc mở) ---
module louver_blade(angle) {
    color(COLOR_BLADE)
    rotate([0, -angle, 0])  // Xoay quanh trục Y tại tâm chiều rộng
        translate([-blade_width/2, 0, 0])
            cube([blade_width, blade_length, blade_thick]);
}

// --- Module: Tai xoay / Bản lề (1 bên) ---
module pivot_bracket(angle) {
    color(COLOR_BRACKET)
    // Phần đứng của tai xoay
    cube([bracket_thick, blade_length * 0.12, bracket_h], center=false);
    // Phần ngang bắt vào tấm lá (xoay cùng lá chớp)
    translate([bracket_thick, 0, 0])
    rotate([0, -angle, 0])
        translate([0, 0, 0])
            cube([blade_thick + 1, blade_length * 0.12, bracket_w]);
}

// --- Module: Thanh giằng di động ---
module linkage_rod(xpos_list, x_shift) {
    color(COLOR_LINKAGE)
    // Thân thanh giằng
    translate([xpos_list[0] - 8, blade_length/2 - 3, 0])
        cube([xpos_list[num_blades-1] - xpos_list[0] + 16, link_height, link_thick]);
    // 5 lỗ ốc + tai bắt chốt
    for (x = xpos_list) {
        translate([x + x_shift - bolt_r, blade_length/2 - 3, -4])
            color(COLOR_BRACKET)
            difference() {
                cube([bracket_w, link_height, 8]);
                translate([bracket_w/2, link_height/2, -1])
                    cylinder(r=bolt_r, h=10, $fn=20);
            }
    }
}

// --- Module: Khối Servo MG996R (đơn giản hóa) ---
module servo_block() {
    color(COLOR_SERVO) {
        // Thân Servo
        cube([40, 20, 37]);
        // Tai bắt ốc 2 bên
        translate([-5, 3, 28])   cube([5, 14, 4]);
        translate([40, 3, 28])  cube([5, 14, 4]);
        // Trục Servo (nhô lên trên)
        translate([20, 10, 37])  cylinder(r=3, h=4, $fn=20);
    }
    // Cánh tay đòn Servo
    color([0.1,0.1,0.1])
    translate([20, 10, 41])
    rotate([0, 0, -(90 - open_angle)*0.7])  // Cánh tay quay tương ứng góc mở
        translate([-2, -2, 0])
            cube([arm_height + 4, 4, 3]);
}

// ─────────────────────────────────────────────────────────────────────
//  LẮP RÁP TOÀN BỘ CƠ CẤU (Assembly)
// ─────────────────────────────────────────────────────────────────────

// Tạo danh sách tọa độ X của từng trục xoay
x_positions = [for (i = [0:num_blades-1]) start_x + i * pitch];

// === KHỐI 1: KHUNG ĐỠ TĨNH ===

// 4 Cột trụ góc (2 trước, 2 sau)
translate([0, 0, 0])                          corner_column(false);
translate([beam_length - col_w, 0, 0])        corner_column(false);
translate([0, blade_length - col_w, 0])       corner_column(true);
translate([beam_length - col_w, blade_length - col_w, 0]) corner_column(true);

// Thanh dầm tĩnh PHÍA TRƯỚC (gắn trục xoay gốc)
translate([0, col_w - beam_thick, col_height - beam_width/2])
    static_beam(x_positions);

// Thanh dầm tĩnh PHÍA SAU (đối xứng)
translate([0, blade_length - col_w, col_height - beam_width/2])
    static_beam(x_positions);

// === KHỐI 2: 5 TẤM LÁ CHỚP (xoay theo open_angle) ===
for (i = [0:num_blades-1]) {
    x = x_positions[i];
    translate([x, col_w, col_height])
        louver_blade(open_angle);
}

// === KHỐI 3: THANH GIẰNG DI ĐỘNG (dịch chuyển theo open_angle) ===
translate([0, col_w, col_height + arm_height])
    linkage_rod(x_positions, linkage_x_shift);

// === KHỐI 4: SERVO MG996R ===
translate([beam_length - col_w - 50, col_w/2, col_height - 10])
    servo_block();

// ─────────────────────────────────────────────────────────────────────
//  CHÚ THÍCH KÍCH THƯỚC (Text 3D - chỉ hiển thị ở góc nhìn trên)
// ─────────────────────────────────────────────────────────────────────
// Bỏ comment dòng dưới nếu muốn hiển thị text kích thước:
// translate([10, -20, 0]) color("black") text("Dầm 300mm | 5 lá 50x250mm | Pitch 44mm", size=8);

