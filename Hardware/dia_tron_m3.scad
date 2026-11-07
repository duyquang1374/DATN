// =====================================================================
//  CHI TIẾT: THANH ĐÒN HÌNH THOI BO GÓC (KẾT NỐI SERVO VÀ 2 DẦM)
//  Kiểu dáng: Hình thoi vuốt thon bo tròn 2 đầu (tối ưu cơ học & gọn nhẹ)
//  Kích thước: Dài 56mm, rộng giữa 20mm, rộng 2 đầu 12mm, dày 5mm
//  Lỗ vít: 1 lỗ tâm M3 + 2 lỗ M3 ở 2 đầu cách nhau 44mm
//  Phần mềm: OpenSCAD
// =====================================================================

// Khoảng cách giữa 2 lỗ bắt dầm (khớp chuẩn 44mm trên tấm lá chớp)
HOLE_SPAN = 44;   

// Độ dày thanh đòn
THICKNESS = 4;    

// Bề rộng ở phần tâm (chỗ trục servo chịu lực xoắn lớn nhất)
CENTER_W  = 20;   

// Bề rộng ở 2 đầu mút (ôm vừa lỗ ốc M3)
END_W     = 12;   

// Đường kính các lỗ vít M3 (3.4mm để luồn vừa ốc M3 trơn tru)
HOLE_D    = 3.4;  

// Độ mịn đường cong bo tròn
$fn = 80;

module diamond_servo_lever() {
    color([0.2, 0.6, 0.85]) // Màu xanh dương kỹ thuật
    difference() {
        // --- 1. THÂN THANH ĐÒN HÌNH THOI BO TRÒN ---
        // Dùng hull nối khối trụ tâm và 2 khối trụ ở 2 đầu
        hull() {
            // Khối tròn ở tâm (rộng 20mm)
            translate([0, 0, -THICKNESS/2])
                cylinder(d = CENTER_W, h = THICKNESS);

            // Đầu mút bên trái (rộng 12mm, tâm lỗ tại X = -22mm)
            translate([-HOLE_SPAN/2, 0, -THICKNESS/2])
                cylinder(d = END_W, h = THICKNESS);

            // Đầu mút bên phải (rộng 12mm, tâm lỗ tại X = +22mm)
            translate([HOLE_SPAN/2, 0, -THICKNESS/2])
                cylinder(d = END_W, h = THICKNESS);
        }

        // --- 2. BA LỖ BẮT VÍT M3 ---
        // Lỗ tròn ở tâm: bắt trục / ốc servo
        translate([0, 0, -THICKNESS/2 - 1])
            cylinder(d = HOLE_D, h = THICKNESS + 2);

        // Lỗ bắt dầm 1: tại X = -22mm
        translate([-HOLE_SPAN/2, 0, -THICKNESS/2 - 1])
            cylinder(d = HOLE_D, h = THICKNESS + 2);

        // Lỗ bắt dầm 2: tại X = +22mm
        translate([HOLE_SPAN/2, 0, -THICKNESS/2 - 1])
            cylinder(d = HOLE_D, h = THICKNESS + 2);
    }
}

// Hiển thị chi tiết
diamond_servo_lever();
