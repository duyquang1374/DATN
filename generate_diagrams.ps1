Add-Type -AssemblyName System.Drawing

$imgDir = "C:\Users\Quang\Documents\CODE\DATN\figures"
if (-not (Test-Path $imgDir)) {
    New-Item -ItemType Directory -Path $imgDir -Force | Out-Null
}

function Create-RoundedRectanglePath {
    param($rect, $radius)
    $path = [System.Drawing.Drawing2D.GraphicsPath]::new()
    $d = $radius * 2
    $path.AddArc($rect.X, $rect.Y, $d, $d, 180, 90)
    $path.AddArc($rect.Right - $d, $rect.Y, $d, $d, 270, 90)
    $path.AddArc($rect.Right - $d, $rect.Bottom - $d, $d, $d, 0, 90)
    $path.AddArc($rect.X, $rect.Bottom - $d, $d, $d, 90, 90)
    $path.CloseFigure()
    return $path
}

function Draw-Card {
    param($g, $rect, $fillColor, $borderColor, $radius, $borderWidth)
    $path = Create-RoundedRectanglePath $rect $radius
    $brush = [System.Drawing.SolidBrush]::new($fillColor)
    $pen = [System.Drawing.Pen]::new($borderColor, [float]$borderWidth)
    $g.FillPath($brush, $path)
    $g.DrawPath($pen, $path)
    $brush.Dispose()
    $pen.Dispose()
    $path.Dispose()
}

function Draw-Arrow {
    param($g, $x1, $y1, $x2, $y2, $color, $width)
    $pen = [System.Drawing.Pen]::new($color, [float]$width)
    $pen.CustomEndCap = [System.Drawing.Drawing2D.AdjustableArrowCap]::new(6, 6, $true)
    $g.DrawLine($pen, [float]$x1, [float]$y1, [float]$x2, [float]$y2)
    $pen.Dispose()
}

# ─────────────────────────────────────────────────────────────────────────────
# 1. HÌNH 1.5: CLOUD AI VS TINYML
# ─────────────────────────────────────────────────────────────────────────────
Write-Host "Dang tao Hinh 1.5: Cloud AI vs TinyML..."
$w1 = 960; $h1 = 480
$bmp1 = [System.Drawing.Bitmap]::new($w1, $h1)
$g1 = [System.Drawing.Graphics]::FromImage($bmp1)
$g1.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
$g1.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::ClearTypeGridFit
$g1.Clear([System.Drawing.Color]::White)

$fTitle = [System.Drawing.Font]::new("Segoe UI", [float]13, [System.Drawing.FontStyle]::Bold)
$fHead  = [System.Drawing.Font]::new("Segoe UI", [float]11, [System.Drawing.FontStyle]::Bold)
$fBody  = [System.Drawing.Font]::new("Segoe UI", [float]9.5, [System.Drawing.FontStyle]::Regular)
$fSub   = [System.Drawing.Font]::new("Segoe UI", [float]8.5, [System.Drawing.FontStyle]::Italic)
$bDark  = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(33, 43, 54))
$bGray  = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(100, 116, 139))
$sfCenter = [System.Drawing.StringFormat]::new()
$sfCenter.Alignment = [System.Drawing.StringAlignment]::Center
$sfCenter.LineAlignment = [System.Drawing.StringAlignment]::Center

# --- Khối trên: Cloud AI ---
$rectCloudBox = [System.Drawing.Rectangle]::new(30, 40, 900, 190)
Draw-Card $g1 $rectCloudBox ([System.Drawing.Color]::FromArgb(254, 242, 242)) ([System.Drawing.Color]::FromArgb(239, 68, 68)) 12 1.5
$g1.DrawString("1. KIẾN TRÚC AI TRUYỀN THỐNG TRÊN ĐÁM MÂY (CLOUD-BASED AI)", $fTitle, [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(185, 28, 28)), 50, 52)

# 3 node trong Cloud AI
$r1_1 = [System.Drawing.Rectangle]::new(60, 90, 200, 110)
Draw-Card $g1 $r1_1 ([System.Drawing.Color]::White) ([System.Drawing.Color]::FromArgb(203, 213, 225)) 8 1
$g1.DrawString("Cảm biến tại chỗ`n(Sensors Node)", $fHead, $bDark, [System.Drawing.RectangleF]::new(60, 100, 200, 40), $sfCenter)
$g1.DrawString("Thu thập T, RH, Lux...`nChưa có xử lý thông minh", $fSub, $bGray, [System.Drawing.RectangleF]::new(60, 145, 200, 40), $sfCenter)

Draw-Arrow $g1 265 145 375 145 ([System.Drawing.Color]::FromArgb(239, 68, 68)) 2.5
$g1.DrawString("Mạng 4G/Internet`n(Độ trễ cao / Mất mạng)", $fSub, [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(220, 38, 38)), [System.Drawing.RectangleF]::new(250, 100, 140, 40), $sfCenter)

$r1_2 = [System.Drawing.Rectangle]::new(380, 90, 210, 110)
Draw-Card $g1 $r1_2 ([System.Drawing.Color]::White) ([System.Drawing.Color]::FromArgb(203, 213, 225)) 8 1
$g1.DrawString("Cloud Server AI`n(Máy chủ từ xa)", $fHead, $bDark, [System.Drawing.RectangleF]::new(380, 100, 210, 40), $sfCenter)
$g1.DrawString("Chạy mô hình AI nặng`nPhí server hàng tháng", $fSub, $bGray, [System.Drawing.RectangleF]::new(380, 145, 210, 40), $sfCenter)

Draw-Arrow $g1 595 145 705 145 ([System.Drawing.Color]::FromArgb(239, 68, 68)) 2.5
$g1.DrawString("Lệnh điều khiển`n(Phụ thuộc mạng)", $fSub, [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(220, 38, 38)), [System.Drawing.RectangleF]::new(585, 100, 130, 40), $sfCenter)

$r1_3 = [System.Drawing.Rectangle]::new(710, 90, 200, 110)
Draw-Card $g1 $r1_3 ([System.Drawing.Color]::White) ([System.Drawing.Color]::FromArgb(203, 213, 225)) 8 1
$g1.DrawString("Cơ cấu chấp hành`n(Actuators)", $fHead, $bDark, [System.Drawing.RectangleF]::new(710, 100, 200, 40), $sfCenter)
$g1.DrawString("Bật quạt, sưởi, bơm...`n(Bị động khi rớt mạng)", $fSub, $bGray, [System.Drawing.RectangleF]::new(710, 145, 200, 40), $sfCenter)

# --- Khối dưới: TinyML ---
$rectTinyBox = [System.Drawing.Rectangle]::new(30, 255, 900, 195)
Draw-Card $g1 $rectTinyBox ([System.Drawing.Color]::FromArgb(240, 253, 244)) ([System.Drawing.Color]::FromArgb(34, 197, 94)) 12 1.5
$g1.DrawString("2. KIẾN TRÚC TRÍ TUỆ NHÂN TẠO NHÚNG TẠI BIÊN (TINYML / ON-DEVICE AI)", $fTitle, [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(21, 128, 61)), 50, 267)

$r2_1 = [System.Drawing.Rectangle]::new(60, 305, 200, 120)
Draw-Card $g1 $r2_1 ([System.Drawing.Color]::White) ([System.Drawing.Color]::FromArgb(203, 213, 225)) 8 1
$g1.DrawString("Hệ Cảm biến đo đạc`n(SHT30, BH1750...)", $fHead, $bDark, [System.Drawing.RectangleF]::new(60, 318, 200, 40), $sfCenter)
$g1.DrawString("Đo nhiệt, ẩm, ánh sáng`nGiao tiếp I2C cục bộ", $fSub, $bGray, [System.Drawing.RectangleF]::new(60, 365, 200, 40), $sfCenter)

Draw-Arrow $g1 265 365 375 365 ([System.Drawing.Color]::FromArgb(34, 197, 94)) 3.0
$g1.DrawString("Đọc trực tiếp`n(Thời gian thực)", $fSub, [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(22, 101, 52)), [System.Drawing.RectangleF]::new(255, 320, 130, 40), $sfCenter)

$r2_2 = [System.Drawing.Rectangle]::new(380, 305, 240, 120)
Draw-Card $g1 $r2_2 ([System.Drawing.Color]::FromArgb(220, 252, 231)) ([System.Drawing.Color]::FromArgb(34, 197, 94)) 8 1.5
$g1.DrawString("Vi điều khiển ESP32`n(Tích hợp mô hình TinyML)", $fHead, [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(21, 128, 61)), [System.Drawing.RectangleF]::new(380, 315, 240, 40), $sfCenter)
$g1.DrawString("• Trích xuất đặc trưng tại chỗ`n• Suy luận mô hình C99 (< 3 ms)`n• Hoạt động 100% Offline", $fBody, $bDark, [System.Drawing.RectangleF]::new(385, 355, 230, 60), $sfCenter)

Draw-Arrow $g1 625 365 705 365 ([System.Drawing.Color]::FromArgb(34, 197, 94)) 3.0
$g1.DrawString("Điều khiển đón đầu`n(Tức thời < 5 ms)", $fSub, [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(22, 101, 52)), [System.Drawing.RectangleF]::new(615, 320, 100, 40), $sfCenter)

$r2_3 = [System.Drawing.Rectangle]::new(710, 305, 200, 120)
Draw-Card $g1 $r2_3 ([System.Drawing.Color]::White) ([System.Drawing.Color]::FromArgb(203, 213, 225)) 8 1
$g1.DrawString("Cơ cấu chấp hành`n(Quạt, Sưởi, Mái che)", $fHead, $bDark, [System.Drawing.RectangleF]::new(710, 318, 200, 40), $sfCenter)
$g1.DrawString("Chủ động điều tiết sớm`nKhông lo mất kết nối mạng", $fSub, $bGray, [System.Drawing.RectangleF]::new(710, 365, 200, 40), $sfCenter)

$p1 = Join-Path $imgDir "hinh_1_5_1_tinyml_concept.png"
$bmp1.Save($p1)
$g1.Dispose(); $bmp1.Dispose()
Write-Host "Da tao: $p1"

# ─────────────────────────────────────────────────────────────────────────────
# 2. HÌNH 1.6: DECISION TREE & RANDOM FOREST
# ─────────────────────────────────────────────────────────────────────────────
Write-Host "Dang tao Hinh 1.6: Decision Tree & Random Forest..."
$w2 = 960; $h2 = 450
$bmp2 = [System.Drawing.Bitmap]::new($w2, $h2)
$g2 = [System.Drawing.Graphics]::FromImage($bmp2)
$g2.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
$g2.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::ClearTypeGridFit
$g2.Clear([System.Drawing.Color]::White)

# Left box: Decision Tree
$rTreeBox = [System.Drawing.Rectangle]::new(30, 20, 360, 410)
Draw-Card $g2 $rTreeBox ([System.Drawing.Color]::FromArgb(248, 250, 252)) ([System.Drawing.Color]::FromArgb(203, 213, 225)) 10 1
$g2.DrawString("(a) Cấu trúc một Cây quyết định (Decision Tree)", $fHead, $bDark, 50, 35)

# Root node
$rRoot = [System.Drawing.Rectangle]::new(140, 75, 140, 50)
Draw-Card $g2 $rRoot ([System.Drawing.Color]::FromArgb(224, 231, 255)) ([System.Drawing.Color]::FromArgb(99, 102, 241)) 8 1.5
$g2.DrawString("Nút gốc (Root)`n[x_j < s]", $fBody, $bDark, [System.Drawing.RectangleF]::new(140, 82, 140, 40), $sfCenter)

Draw-Arrow $g2 180 128 110 180 ([System.Drawing.Color]::FromArgb(100, 116, 139)) 1.5
Draw-Arrow $g2 240 128 310 180 ([System.Drawing.Color]::FromArgb(100, 116, 139)) 1.5
$g2.DrawString("Đúng (True)", $fSub, $bGray, 90, 136)
$g2.DrawString("Sai (False)", $fSub, $bGray, 280, 136)

# Internal nodes
$rInt1 = [System.Drawing.Rectangle]::new(50, 185, 120, 45)
Draw-Card $g2 $rInt1 ([System.Drawing.Color]::FromArgb(224, 231, 255)) ([System.Drawing.Color]::FromArgb(99, 102, 241)) 6 1.2
$g2.DrawString("Nút rẽ nhánh`n[x_k < s_k]", $fSub, $bDark, [System.Drawing.RectangleF]::new(50, 190, 120, 35), $sfCenter)

$rInt2 = [System.Drawing.Rectangle]::new(250, 185, 120, 45)
Draw-Card $g2 $rInt2 ([System.Drawing.Color]::FromArgb(224, 231, 255)) ([System.Drawing.Color]::FromArgb(99, 102, 241)) 6 1.2
$g2.DrawString("Nút rẽ nhánh`n[x_m < s_m]", $fSub, $bDark, [System.Drawing.RectangleF]::new(250, 190, 120, 35), $sfCenter)

# Leaves
Draw-Arrow $g2 90 232 65 290 ([System.Drawing.Color]::FromArgb(100, 116, 139)) 1.2
Draw-Arrow $g2 130 232 155 290 ([System.Drawing.Color]::FromArgb(100, 116, 139)) 1.2
Draw-Arrow $g2 290 232 265 290 ([System.Drawing.Color]::FromArgb(100, 116, 139)) 1.2
Draw-Arrow $g2 330 232 355 290 ([System.Drawing.Color]::FromArgb(100, 116, 139)) 1.2

$rL1 = [System.Drawing.Rectangle]::new(40, 295, 60, 40)
Draw-Card $g2 $rL1 ([System.Drawing.Color]::FromArgb(220, 252, 231)) ([System.Drawing.Color]::FromArgb(34, 197, 94)) 6 1
$g2.DrawString("Lá 1`ny_1", $fSub, $bDark, [System.Drawing.RectangleF]::new(40, 298, 60, 35), $sfCenter)

$rL2 = [System.Drawing.Rectangle]::new(125, 295, 60, 40)
Draw-Card $g2 $rL2 ([System.Drawing.Color]::FromArgb(220, 252, 231)) ([System.Drawing.Color]::FromArgb(34, 197, 94)) 6 1
$g2.DrawString("Lá 2`ny_2", $fSub, $bDark, [System.Drawing.RectangleF]::new(125, 298, 60, 35), $sfCenter)

$rL3 = [System.Drawing.Rectangle]::new(235, 295, 60, 40)
Draw-Card $g2 $rL3 ([System.Drawing.Color]::FromArgb(220, 252, 231)) ([System.Drawing.Color]::FromArgb(34, 197, 94)) 6 1
$g2.DrawString("Lá 3`ny_3", $fSub, $bDark, [System.Drawing.RectangleF]::new(235, 298, 60, 35), $sfCenter)

$rL4 = [System.Drawing.Rectangle]::new(325, 295, 60, 40)
Draw-Card $g2 $rL4 ([System.Drawing.Color]::FromArgb(220, 252, 231)) ([System.Drawing.Color]::FromArgb(34, 197, 94)) 6 1
$g2.DrawString("Lá 4`ny_4", $fSub, $bDark, [System.Drawing.RectangleF]::new(325, 298, 60, 35), $sfCenter)

$bRed = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(220, 38, 38))
$g2.DrawString("• Dễ quá khớp (Overfitting)`n• Độ biến động cao khi dữ liệu nhiễu", $fSub, $bRed, [System.Drawing.RectangleF]::new(30, 355, 360, 40), $sfCenter)
$bRed.Dispose()

# Right box: Random Forest
$rRFBox = [System.Drawing.Rectangle]::new(410, 20, 520, 410)
Draw-Card $g2 $rRFBox ([System.Drawing.Color]::FromArgb(240, 253, 244)) ([System.Drawing.Color]::FromArgb(187, 247, 208)) 10 1
$g2.DrawString("(b) Cơ chế tập hợp Rừng ngẫu nhiên (Random Forest Ensemble)", $fHead, [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(21, 128, 61)), 430, 35)

# Input data vector
$rInp = [System.Drawing.Rectangle]::new(530, 75, 280, 45)
Draw-Card $g2 $rInp ([System.Drawing.Color]::White) ([System.Drawing.Color]::FromArgb(203, 213, 225)) 8 1
$g2.DrawString("Vector đặc trưng đầu vào (27 Features)`n[X_1, X_2, ..., X_27]", $fBody, $bDark, [System.Drawing.RectangleF]::new(530, 80, 280, 35), $sfCenter)

# 3 Trees
$treeWidth = 110; $treeHeight = 100
$t1X = 450; $t2X = 615; $t3X = 780
Draw-Arrow $g2 600 123 505 170 ([System.Drawing.Color]::FromArgb(34, 197, 94)) 2
Draw-Arrow $g2 670 123 670 170 ([System.Drawing.Color]::FromArgb(34, 197, 94)) 2
Draw-Arrow $g2 740 123 835 170 ([System.Drawing.Color]::FromArgb(34, 197, 94)) 2

$rT1 = [System.Drawing.Rectangle]::new($t1X, 175, $treeWidth, $treeHeight)
Draw-Card $g2 $rT1 ([System.Drawing.Color]::White) ([System.Drawing.Color]::FromArgb(34, 197, 94)) 8 1.5
$g2.DrawString("Cây quyết định 1`n(Tree #1)`n`nDự báo: y_1", $fBody, $bDark, [System.Drawing.RectangleF]::new($t1X, 185, $treeWidth, 80), $sfCenter)

$rT2 = [System.Drawing.Rectangle]::new($t2X, 175, $treeWidth, $treeHeight)
Draw-Card $g2 $rT2 ([System.Drawing.Color]::White) ([System.Drawing.Color]::FromArgb(34, 197, 94)) 8 1.5
$g2.DrawString("Cây quyết định 2`n(Tree #2)`n`nDự báo: y_2", $fBody, $bDark, [System.Drawing.RectangleF]::new($t2X, 185, $treeWidth, 80), $sfCenter)

$rT3 = [System.Drawing.Rectangle]::new($t3X, 175, $treeWidth, $treeHeight)
Draw-Card $g2 $rT3 ([System.Drawing.Color]::White) ([System.Drawing.Color]::FromArgb(34, 197, 94)) 8 1.5
$g2.DrawString("Cây quyết định B`n(Tree #B)`n`nDự báo: y_B", $fBody, $bDark, [System.Drawing.RectangleF]::new($t3X, 185, $treeWidth, 80), $sfCenter)

# Ensemble Aggregator
Draw-Arrow $g2 505 280 610 325 ([System.Drawing.Color]::FromArgb(34, 197, 94)) 2
Draw-Arrow $g2 670 280 670 325 ([System.Drawing.Color]::FromArgb(34, 197, 94)) 2
Draw-Arrow $g2 835 280 730 325 ([System.Drawing.Color]::FromArgb(34, 197, 94)) 2

$rAvg = [System.Drawing.Rectangle]::new(530, 330, 280, 75)
Draw-Card $g2 $rAvg ([System.Drawing.Color]::FromArgb(220, 252, 231)) ([System.Drawing.Color]::FromArgb(21, 128, 61)) 8 2
$g2.DrawString("Bộ tổng hợp trung bình (Ensemble Average)`ny_hat = (1/B) * ∑ y_b`n=> Khử phương sai, chính xác vượt trội!", $fBody, [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(21, 128, 61)), [System.Drawing.RectangleF]::new(530, 335, 280, 65), $sfCenter)

$p2 = Join-Path $imgDir "hinh_1_5_2_decision_tree_rf.png"
$bmp2.Save($p2)
$g2.Dispose(); $bmp2.Dispose()
Write-Host "Da tao: $p2"

# ─────────────────────────────────────────────────────────────────────────────
# 3. HÌNH 1.7: CYCLICAL TIME ENCODING (CHỐNG ĐÈ CHỮ HOÀN TOÀN)
# ─────────────────────────────────────────────────────────────────────────────
Write-Host "Dang tao Hinh 1.7: Cyclical Time Encoding..."
$w3 = 1040; $h3 = 490
$bmp3 = [System.Drawing.Bitmap]::new($w3, $h3)
$g3 = [System.Drawing.Graphics]::FromImage($bmp3)
$g3.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
$g3.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::ClearTypeGridFit
$g3.Clear([System.Drawing.Color]::White)

$fSubI  = [System.Drawing.Font]::new("Segoe UI", [float]8.5, [System.Drawing.FontStyle]::Italic)
$bRed   = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(220, 38, 38))
$bGreen = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(21, 128, 61))
$bBlue  = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(29, 78, 216))

$sfLeft = [System.Drawing.StringFormat]::new()
$sfLeft.Alignment = [System.Drawing.StringAlignment]::Near
$sfLeft.LineAlignment = [System.Drawing.StringAlignment]::Center

$sfRight = [System.Drawing.StringFormat]::new()
$sfRight.Alignment = [System.Drawing.StringAlignment]::Far
$sfRight.LineAlignment = [System.Drawing.StringAlignment]::Center

# --- KHỐI TRÁI: (a) Tuyến tính ---
$rLBox = [System.Drawing.Rectangle]::new(25, 20, 480, 445)
Draw-Card $g3 $rLBox ([System.Drawing.Color]::FromArgb(254, 242, 242)) ([System.Drawing.Color]::FromArgb(252, 165, 165)) 12 1.5
$g3.DrawString("(a) Biểu diễn thời gian dạng số tự nhiên [0, 23]", $fTitle, $bRed, 45, 35)

# Trục thời gian tuyến tính
$penAxisLine = [System.Drawing.Pen]::new([System.Drawing.Color]::FromArgb(71, 85, 105), [float]2.5)
$penAxisLine.CustomEndCap = [System.Drawing.Drawing2D.AdjustableArrowCap]::new(5, 5, $true)
$g3.DrawLine($penAxisLine, 60, 200, 465, 200)
$penAxisLine.Dispose()

# Vạch chia và nhãn
$penTick = [System.Drawing.Pen]::new([System.Drawing.Color]::FromArgb(71, 85, 105), 2)
# 0h
$g3.DrawLine($penTick, 85, 195, 85, 205)
$g3.DrawString("0h (00:00)", $fHead, $bDark, [System.Drawing.RectangleF]::new(40, 212, 90, 25), $sfCenter)

# 12h
$g3.DrawLine($penTick, 255, 195, 255, 205)
$g3.DrawString("12h (Trưa)", $fHead, $bDark, [System.Drawing.RectangleF]::new(210, 212, 90, 25), $sfCenter)

# 23h
$g3.DrawLine($penTick, 425, 195, 425, 205)
$g3.DrawString("23h (23:59)", $fHead, $bDark, [System.Drawing.RectangleF]::new(380, 212, 90, 25), $sfCenter)
$penTick.Dispose()

# Cung tên đỏ biểu diễn bước nhảy từ 23h về 0h
$pRedCurved = [System.Drawing.Pen]::new([System.Drawing.Color]::FromArgb(239, 68, 68), [float]2.5)
$pRedCurved.DashStyle = [System.Drawing.Drawing2D.DashStyle]::Dash
$pRedCurved.CustomStartCap = [System.Drawing.Drawing2D.AdjustableArrowCap]::new(5, 5, $true)
$g3.DrawArc($pRedCurved, 85, 95, 340, 190, 180, 180)
$pRedCurved.Dispose()

$g3.DrawString("Bước nhảy gián đoạn: |23 - 0| = 23", $fHead, $bRed, [System.Drawing.RectangleF]::new(110, 100, 290, 25), $sfCenter)

# Khối giải thích phía dưới
$rLNote = [System.Drawing.Rectangle]::new(45, 275, 440, 160)
Draw-Card $g3 $rLNote ([System.Drawing.Color]::White) ([System.Drawing.Color]::FromArgb(254, 202, 202)) 8 1
$g3.DrawString("KHIẾM KHUYẾT TOÁN HỌC:", $fHead, $bRed, 60, 290)
$g3.DrawString("• Khoảng cách số học giữa 23h và 0h là 23 đơn vị (rất lớn).`n• Tuy nhiên, trong thực tế hai thời điểm này chỉ cách nhau 1 phút.`n• Sự đứt gãy phi liên tục này khiến các mô hình học máy phân loại sai lệch hành vi nhiệt ẩm vào ban đêm.", $fBody, $bDark, [System.Drawing.RectangleF]::new(60, 315, 410, 105), $sfLeft)


# --- KHỐI PHẢI: (b) Vòng tròn lượng giác ---
$rRBox = [System.Drawing.Rectangle]::new(525, 20, 490, 445)
Draw-Card $g3 $rRBox ([System.Drawing.Color]::FromArgb(240, 253, 244)) ([System.Drawing.Color]::FromArgb(187, 247, 208)) 12 1.5
$g3.DrawString("(b) Mã hóa lượng giác 2 chiều (hour_sin & hour_cos)", $fTitle, $bGreen, 545, 35)

$cx = 765; $cy = 205; $rad = 85

# Trục tọa độ vuông góc
$penAxis = [System.Drawing.Pen]::new([System.Drawing.Color]::FromArgb(148, 163, 184), [float]1.2)
$penAxis.DashStyle = [System.Drawing.Drawing2D.DashStyle]::Dash
# Trục ngang (Sin)
$g3.DrawLine($penAxis, $cx - 120, $cy, $cx + 120, $cy)
# Trục dọc (Cos)
$g3.DrawLine($penAxis, $cx, $cy + 120, $cx, $cy - 120)
$penAxis.Dispose()

# Mũi tên và tên trục
$penAxisArrow = [System.Drawing.Pen]::new([System.Drawing.Color]::FromArgb(100, 116, 139), [float]1.5)
$penAxisArrow.CustomEndCap = [System.Drawing.Drawing2D.AdjustableArrowCap]::new(4, 4, $true)
# Mũi tên hướng lên cho trục Cos
$g3.DrawLine($penAxisArrow, $cx, $cy - 100, $cx, $cy - 122)
$g3.DrawString("Trục Cos (+)", $fSubI, $bGray, $cx + 8, $cy - 128)

# Mũi tên hướng phải cho trục Sin
$g3.DrawLine($penAxisArrow, $cx + 100, $cy, $cx + 122, $cy)
$g3.DrawString("Trục Sin (+)", $fSubI, $bGray, $cx + 125, $cy - 18)
$penAxisArrow.Dispose()

# Vẽ đường tròn đơn vị
$penCirc = [System.Drawing.Pen]::new([System.Drawing.Color]::FromArgb(34, 197, 94), [float]2.5)
$g3.DrawEllipse($penCirc, [float]($cx - $rad), [float]($cy - $rad), [float]($rad * 2), [float]($rad * 2))
$penCirc.Dispose()

# Điểm 0h / 24h (Đỉnh đường tròn: x=0, y=1 -> sin=0, cos=1)
$topX = $cx; $topY = $cy - $rad
$brushBlue = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(37, 99, 235))
$g3.FillEllipse($brushBlue, [float]($topX - 5), [float]($topY - 5), 10, 10)
$brushBlue.Dispose()

# Nhãn cho 0h (Đặt lệch sang phải đỉnh)
$g3.DrawString("0h / 24h`n(sin=0, cos=1)", $fSub, $bBlue, $cx + 8, $cy - $rad - 15)

# Điểm 6h (Bên phải đường tròn: sin=1, cos=0)
$rightX = $cx + $rad; $rightY = $cy
$brushDark = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(51, 65, 85))
$g3.FillEllipse($brushDark, [float]($rightX - 4), [float]($rightY - 4), 8, 8)
$g3.DrawString("6h (Sáng)`n(sin=1, cos=0)", $fSub, $bDark, $rightX + 8, $rightY - 12)

# Điểm 12h (Đáy đường tròn: sin=0, cos=-1)
$botX = $cx; $botY = $cy + $rad
$g3.FillEllipse($brushDark, [float]($botX - 4), [float]($botY - 4), 8, 8)
# Đặt nhãn 12h ngay dưới điểm đáy, căn giữa
$g3.DrawString("12h (Trưa)`n(sin=0, cos=-1)", $fSub, $bDark, [System.Drawing.RectangleF]::new($cx - 60, $botY + 6, 120, 32), $sfCenter)

# Điểm 18h (Bên trái đường tròn: sin=-1, cos=0)
$leftX = $cx - $rad; $leftY = $cy
$g3.FillEllipse($brushDark, [float]($leftX - 4), [float]($leftY - 4), 8, 8)
$g3.DrawString("18h (Chiều)`n(sin=-1, cos=0)", $fSub, $bDark, [System.Drawing.RectangleF]::new($leftX - 110, $leftY - 15, 105, 30), $sfRight)
$brushDark.Dispose()

# Điểm 23h (Góc = 345 độ, ngay bên trái 0h)
$ang23 = 23.0 / 24.0 * 2.0 * [Math]::PI
$x23 = $cx + $rad * [Math]::Sin($ang23)
$y23 = $cy - $rad * [Math]::Cos($ang23)

$brushRed = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(220, 38, 38))
$g3.FillEllipse($brushRed, [float]($x23 - 5), [float]($y23 - 5), 10, 10)
$brushRed.Dispose()

# Đường chỉ dẫn cho điểm 23h sang góc trên bên trái
$penPointer = [System.Drawing.Pen]::new([System.Drawing.Color]::FromArgb(239, 68, 68), 1)
$penPointer.DashStyle = [System.Drawing.Drawing2D.DashStyle]::Dash
$g3.DrawLine($penPointer, [float]$x23, [float]$y23, [float]($cx - 55), [float]($cy - $rad - 15))
$penPointer.Dispose()

$g3.DrawString("23h (23:59)`n(sin≈-0.26, cos≈0.97)", $fSub, $bRed, [System.Drawing.RectangleF]::new($cx - 195, $cy - $rad - 32, 140, 35), $sfRight)

# Khối biểu ngữ tổng kết phía dưới (Rộng rãi, hoàn toàn tách biệt khỏi 12h)
$rRBanner = [System.Drawing.Rectangle]::new(545, 365, 450, 85)
Draw-Card $g3 $rRBanner ([System.Drawing.Color]::White) ([System.Drawing.Color]::FromArgb(134, 239, 172)) 8 1.2
$g3.DrawString("ƯU ĐIỂM CỦA MÃ HÓA LƯỢNG GIÁC 2 CHIỀU:", $fHead, $bGreen, 560, 375)
$g3.DrawString("✓ Điểm 23h và 0h nằm sát cạnh nhau trên vòng tròn đơn vị.`n✓ Khoảng cách hình học Euclid hoàn toàn trơn tru và liên tục.`n✓ Giúp mô hình học máy nắm bắt chính xác quy luật sinh học ngày – đêm.", $fBody, $bDark, [System.Drawing.RectangleF]::new(560, 395, 420, 50), $sfLeft)

$p3 = Join-Path $imgDir "hinh_1_5_3_cyclical_encoding.png"
$bmp3.Save($p3)
$g3.Dispose(); $bmp3.Dispose()
Write-Host "Da tao: $p3"

# ─────────────────────────────────────────────────────────────────────────────
# 4. HÌNH 1.8: TINYML PIPELINE (GIỮ NGUYÊN BỐ CỤC GỐC, KHẮC PHỤC LỖI ĐÈ CHỮ)
# ─────────────────────────────────────────────────────────────────────────────
Write-Host "Dang tao Hinh 1.8: TinyML Pipeline..."
$w4 = 960; $h4 = 340
$bmp4 = [System.Drawing.Bitmap]::new($w4, $h4)
$g4 = [System.Drawing.Graphics]::FromImage($bmp4)
$g4.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
$g4.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::ClearTypeGridFit
$g4.Clear([System.Drawing.Color]::White)

$fTitle = [System.Drawing.Font]::new("Segoe UI", [float]12.5, [System.Drawing.FontStyle]::Bold)
$fHead  = [System.Drawing.Font]::new("Segoe UI", [float]10.5, [System.Drawing.FontStyle]::Bold)
$fBody  = [System.Drawing.Font]::new("Segoe UI", [float]9, [System.Drawing.FontStyle]::Regular)
$bDark  = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(33, 43, 54))

$sfTitle = [System.Drawing.StringFormat]::new()
$sfTitle.Alignment = [System.Drawing.StringAlignment]::Center
$sfTitle.LineAlignment = [System.Drawing.StringAlignment]::Near

$sfBody = [System.Drawing.StringFormat]::new()
$sfBody.Alignment = [System.Drawing.StringAlignment]::Center
$sfBody.LineAlignment = [System.Drawing.StringAlignment]::Near

# Tiêu đề trên cùng
$g4.DrawString("QUY TRÌNH PHÁT TRIỂN & TRIỂN KHAI HỌC MÁY NHÚNG (TINYML PIPELINE)", $fTitle, $bDark, [System.Drawing.RectangleF]::new(0, 18, $w4, 30), $sfTitle)

# Kích thước 4 khối
$boxW = 200
$boxH = 230
$yBox = 65

$x1 = 20
$x2 = 265
$x3 = 510
$x4 = 755

$yTitle = $yBox + 10
$hTitle = 38
$yContent = $yBox + 54
$hContent = 160
$wContent = $boxW - 10

# ── Bước 1 ──
$rS1 = [System.Drawing.Rectangle]::new($x1, $yBox, $boxW, $boxH)
Draw-Card $g4 $rS1 ([System.Drawing.Color]::FromArgb(238, 242, 255)) ([System.Drawing.Color]::FromArgb(99, 102, 241)) 10 1.5

$bHead1 = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(67, 56, 202))
$g4.DrawString("BƯỚC 1`nThu thập & Tiền xử lý", $fHead, $bHead1, [System.Drawing.RectangleF]::new($x1, $yTitle, $boxW, $hTitle), $sfTitle)
$bHead1.Dispose()

$text1 = "• Dữ liệu cảm biến thực tế`n`n• Lọc nhiễu cảm biến`n`n• Kỹ nghệ 27 đặc trưng`n`n• Tạo nhãn tương lai"
$g4.DrawString($text1, $fBody, $bDark, [System.Drawing.RectangleF]::new($x1 + 5, $yContent, $wContent, $hContent), $sfBody)

# ── Bước 2 ──
$rS2 = [System.Drawing.Rectangle]::new($x2, $yBox, $boxW, $boxH)
Draw-Card $g4 $rS2 ([System.Drawing.Color]::FromArgb(236, 254, 255)) ([System.Drawing.Color]::FromArgb(6, 182, 212)) 10 1.5

$bHead2 = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(14, 116, 144))
$g4.DrawString("BƯỚC 2`nHuấn luyện mô hình", $fHead, $bHead2, [System.Drawing.RectangleF]::new($x2, $yTitle, $boxW, $hTitle), $sfTitle)
$bHead2.Dispose()

$text2 = "• Môi trường Python`n`n• Thuật toán Random Forest`n`n• Scikit-learn Regressor`n`n• Đánh giá MAE, RMSE, R²"
$g4.DrawString($text2, $fBody, $bDark, [System.Drawing.RectangleF]::new($x2 + 5, $yContent, $wContent, $hContent), $sfBody)

# ── Bước 3 ──
$rS3 = [System.Drawing.Rectangle]::new($x3, $yBox, $boxW, $boxH)
Draw-Card $g4 $rS3 ([System.Drawing.Color]::FromArgb(254, 243, 199)) ([System.Drawing.Color]::FromArgb(245, 158, 11)) 10 1.5

$bHead3 = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(180, 83, 9))
$g4.DrawString("BƯỚC 3`nBiên dịch sang C99", $fHead, $bHead3, [System.Drawing.RectangleF]::new($x3, $yTitle, $boxW, $hTitle), $sfTitle)
$bHead3.Dispose()

$text3 = "• Framework emlearn`n`n• Inline if-else trees`n`n• Sinh header C tĩnh (.h)`n`n• Không phụ thuộc runtime"
$g4.DrawString($text3, $fBody, $bDark, [System.Drawing.RectangleF]::new($x3 + 5, $yContent, $wContent, $hContent), $sfBody)

# ── Bước 4 ──
$rS4 = [System.Drawing.Rectangle]::new($x4, $yBox, $boxW, $boxH)
Draw-Card $g4 $rS4 ([System.Drawing.Color]::FromArgb(240, 253, 244)) ([System.Drawing.Color]::FromArgb(34, 197, 94)) 10 1.5

$bHead4 = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(21, 128, 61))
$g4.DrawString("BƯỚC 4`nTriển khai trên MCU", $fHead, $bHead4, [System.Drawing.RectangleF]::new($x4, $yTitle, $boxW, $hTitle), $sfTitle)
$bHead4.Dispose()

$text4 = "• Nạp vào Flash ESP32`n`n• Suy luận cục bộ (< 3ms)`n`n• Điều khiển đón đầu sớm`n`n• Không cần mạng Internet"
$g4.DrawString($text4, $fBody, $bDark, [System.Drawing.RectangleF]::new($x4 + 5, $yContent, $wContent, $hContent), $sfBody)

# Mũi tên liên kết
$arrowY = $yBox + ($boxH / 2)
Draw-Arrow $g4 ($x1 + $boxW + 4) $arrowY ($x2 - 6) $arrowY ([System.Drawing.Color]::FromArgb(99, 102, 241)) 2.5
Draw-Arrow $g4 ($x2 + $boxW + 4) $arrowY ($x3 - 6) $arrowY ([System.Drawing.Color]::FromArgb(6, 182, 212)) 2.5
Draw-Arrow $g4 ($x3 + $boxW + 4) $arrowY ($x4 - 6) $arrowY ([System.Drawing.Color]::FromArgb(245, 158, 11)) 2.5

$p4 = Join-Path $imgDir "hinh_1_5_4_tinyml_pipeline.png"
$bmp4.Save($p4)
$g4.Dispose(); $bmp4.Dispose()
Write-Host "Da tao: $p4"

