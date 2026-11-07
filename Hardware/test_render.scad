// Test script for checking assembly
$fn = 30;

BL_L    = 250;   // Chiều dài lá (mm)
BL_W    = 50;    // Chiều rộng lá (mm)
BL_T    = 5;     // Độ dày lá (mm)
PITCH   = 44;    // Bước giữa các lá (mm)
N       = 5;     // Số tấm lá

BAR_W   = 12;    // Bản rộng thanh giằng (mm)
BAR_T   = 4;     // Độ dày thanh giằng (mm)
BOLT_D  = 3.4;   // Lỗ vít M3 (mm)

BEAM_L  = 300;   // Dầm dài 300mm
COL_H   = 70;    // Chiều cao cột

// Góc mở rèm chớp:
// 15 = Đóng che mưa (nghiêng gối mí)
// 85 = Mở đứng đón gió/nắng
ANGLE   = 85;

echo("Test OpenSCAD syntax");
