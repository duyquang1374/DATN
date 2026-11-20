param([string]$DataDir, [string]$OutDir)
$ErrorActionPreference = "Stop"
Add-Type -AssemblyName System.Drawing
$inv = [System.Globalization.CultureInfo]::InvariantCulture

function New-Canvas($w, $h) {
    $bmp = New-Object System.Drawing.Bitmap $w, $h
    $bmp.SetResolution(200, 200)
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.SmoothingMode = 'AntiAlias'
    $g.TextRenderingHint = 'AntiAliasGridFit'
    $g.Clear([System.Drawing.Color]::White)
    return @($bmp, $g)
}

# ───────────── Hình 2.27: sơ đồ quy trình ─────────────
$W = 2400; $H = 1100
$bmp, $g = New-Canvas $W $H
$fT = New-Object System.Drawing.Font("Times New Roman", 11, [System.Drawing.FontStyle]::Bold)
$fS = New-Object System.Drawing.Font("Times New Roman", 9)
$pen = New-Object System.Drawing.Pen ([System.Drawing.Color]::FromArgb(40,40,40)), 4
$pen.CustomEndCap = New-Object System.Drawing.Drawing2D.AdjustableArrowCap 5, 5
$dash = New-Object System.Drawing.Pen ([System.Drawing.Color]::FromArgb(90,90,90)), 4
$dash.DashStyle = 'Dash'
$dash.CustomEndCap = New-Object System.Drawing.Drawing2D.AdjustableArrowCap 5, 5
$sf = New-Object System.Drawing.StringFormat
$sf.Alignment = 'Center'; $sf.LineAlignment = 'Center'

function Draw-Box($x, $y, $w, $h, $title, $sub, $color) {
    $b = New-Object System.Drawing.SolidBrush $color
    $path = New-Object System.Drawing.Drawing2D.GraphicsPath
    $r = 30
    $path.AddArc($x, $y, $r, $r, 180, 90); $path.AddArc($x + $w - $r, $y, $r, $r, 270, 90)
    $path.AddArc($x + $w - $r, $y + $h - $r, $r, $r, 0, 90); $path.AddArc($x, $y + $h - $r, $r, $r, 90, 90)
    $path.CloseFigure()
    $g.FillPath($b, $path)
    $g.DrawPath((New-Object System.Drawing.Pen ([System.Drawing.Color]::FromArgb(60,60,60)), 3), $path)
    $g.DrawString($title, $fT, [System.Drawing.Brushes]::Black, (New-Object System.Drawing.RectangleF($x, ($y + 15), $w, ($h * 0.38))), $sf)
    $g.DrawString($sub, $fS, [System.Drawing.Brushes]::Black, (New-Object System.Drawing.RectangleF(($x + 10), ($y + $h * 0.38), ($w - 20), ($h * 0.58))), $sf)
}

$L = Get-Content -LiteralPath (Join-Path $OutDir "labels.txt") -Encoding UTF8
$blue  = [System.Drawing.Color]::FromArgb(214, 230, 248)
$green = [System.Drawing.Color]::FromArgb(218, 240, 216)
$orange= [System.Drawing.Color]::FromArgb(252, 228, 200)
$gray  = [System.Drawing.Color]::FromArgb(235, 235, 235)

$bw = 620; $bh = 300; $gap = 150; $y1 = 110; $y2 = 690
$x = @(110, (110 + $bw + $gap), (110 + 2 * ($bw + $gap)))
# hàng 1 (trái -> phải)
Draw-Box $x[0] $y1 $bw $bh $L[0] $L[1] $blue
Draw-Box $x[1] $y1 $bw $bh $L[2] $L[3] $blue
Draw-Box $x[2] $y1 $bw $bh $L[4] $L[5] $blue
# hàng 2 (phải -> trái)
Draw-Box $x[2] $y2 $bw $bh $L[6] $L[7] $green
Draw-Box $x[1] $y2 $bw $bh $L[8] $L[9] $green
Draw-Box $x[0] $y2 $bw $bh $L[10] $L[11] $orange
$g.DrawLine($pen, ($x[0] + $bw), ($y1 + $bh / 2), $x[1], ($y1 + $bh / 2))
$g.DrawLine($pen, ($x[1] + $bw), ($y1 + $bh / 2), $x[2], ($y1 + $bh / 2))
$g.DrawLine($pen, ($x[2] + $bw / 2), ($y1 + $bh), ($x[2] + $bw / 2), $y2)
$g.DrawLine($pen, $x[2], ($y2 + $bh / 2), ($x[1] + $bw), ($y2 + $bh / 2))
$g.DrawLine($pen, $x[1], ($y2 + $bh / 2), ($x[0] + $bw), ($y2 + $bh / 2))
# nhóm "trên máy tính" / "trên ESP32"
$g.DrawString($L[12], $fS, [System.Drawing.Brushes]::DimGray, 110, 30)
$g.DrawString($L[13], $fS, [System.Drawing.Brushes]::DimGray, 110, ($y2 + $bh + 25))
$bmp.Save((Join-Path $OutDir "hinh_2_27_quy_trinh_ai.png"), [System.Drawing.Imaging.ImageFormat]::Png)
$g.Dispose(); $bmp.Dispose()

# ───────────── Hình 2.28: dữ liệu cảm biến thật ─────────────
$fmts = [string[]]@('yyyy-MM-dd HH:mm:ss', 'M/d/yyyy H:mm', 'M/d/yyyy HH:mm')
$bins = @{}
foreach ($f in (Get-ChildItem -LiteralPath $DataDir -Filter "*data_v*.csv" | Sort-Object Name)) {
    $first = $true
    foreach ($line in [System.IO.File]::ReadLines($f.FullName)) {
        if ($first) { $first = $false; continue }
        $p = $line.Split(',')
        if ($p.Count -lt 4) { continue }
        $t = [datetime]::MinValue
        if (-not [datetime]::TryParseExact($p[0].Trim(), $fmts, $inv, 'None', [ref]$t)) { continue }
        $key = [long]([math]::Floor(($t - [datetime]'2026-01-01').TotalMinutes / 30))
        $v = $bins[$key]
        if ($v -eq $null) { $v = @(0.0, 0.0, 0.0, 0); $bins[$key] = $v }
        $v[0] += [double]::Parse($p[1], $inv); $v[1] += [double]::Parse($p[2], $inv); $v[2] += [double]::Parse($p[3], $inv); $v[3] += 1
    }
}
$keys = $bins.Keys | Sort-Object
$ts = @(); $T = @(); $Hm = @(); $Lx = @()
foreach ($k in $keys) {
    $v = $bins[$k]
    $ts += [datetime]'2026-01-01' + [timespan]::FromMinutes($k * 30)
    $T += $v[0] / $v[3]; $Hm += $v[1] / $v[3]; $Lx += $v[2] / $v[3] / 1000.0
}
$t0 = $ts[0]; $t1 = $ts[-1]

$W = 2600; $H = 1900
$bmp, $g = New-Canvas $W $H
$fA = New-Object System.Drawing.Font("Times New Roman", 9)
$fY = New-Object System.Drawing.Font("Times New Roman", 10, [System.Drawing.FontStyle]::Bold)
$left = 300; $right = $W - 60; $ph = 470; $tops = @(80, 660, 1240)
$series = @(
    @{ d = $T;  lbl = $L[14]; c = [System.Drawing.Color]::FromArgb(214, 39, 40) },
    @{ d = $Hm; lbl = $L[15]; c = [System.Drawing.Color]::FromArgb(31, 119, 180) },
    @{ d = $Lx; lbl = $L[16]; c = [System.Drawing.Color]::FromArgb(230, 150, 0) }
)
$gridPen = New-Object System.Drawing.Pen ([System.Drawing.Color]::FromArgb(225, 225, 225)), 2
$axPen = New-Object System.Drawing.Pen ([System.Drawing.Color]::Black), 2
$span = ($t1 - $t0).TotalMinutes
for ($s = 0; $s -lt 3; $s++) {
    $d = $series[$s].d; $top = $tops[$s]; $bot = $top + $ph
    $mn = ($d | Measure-Object -Minimum).Minimum; $mx = ($d | Measure-Object -Maximum).Maximum
    $pad = ($mx - $mn) * 0.08; $mn -= $pad; $mx += $pad
    if ($s -eq 2) { $mn = 0 }
    if ($s -eq 1 -and $mx -gt 100) { $mx = 100 }
    # lưới & nhãn trục Y
    for ($k = 0; $k -le 4; $k++) {
        $val = $mn + ($mx - $mn) * $k / 4
        $yy = $bot - $ph * $k / 4
        $g.DrawLine($gridPen, $left, $yy, $right, $yy)
        $g.DrawString(($val.ToString("0.#", $inv)), $fA, [System.Drawing.Brushes]::Black, ($left - 120), ($yy - 25))
    }
    # lưới trục X theo ngày
    $day = $t0.Date.AddDays(1)
    while ($day -le $t1) {
        $xx = $left + ($right - $left) * (($day - $t0).TotalMinutes / $span)
        $g.DrawLine($gridPen, $xx, $top, $xx, $bot)
        if ($s -eq 2 -and ($day.Day % 2 -eq 0)) { $g.DrawString($day.ToString("dd/MM"), $fA, [System.Drawing.Brushes]::Black, ($xx - 45), ($bot + 10)) }
        $day = $day.AddDays(1)
    }
    $g.DrawRectangle($axPen, $left, $top, ($right - $left), $ph)
    # đường dữ liệu (ngắt tại khoảng trống > 1 giờ)
    $lp = New-Object System.Drawing.Pen $series[$s].c, 3
    for ($i = 1; $i -lt $d.Count; $i++) {
        if (($ts[$i] - $ts[$i - 1]).TotalMinutes -gt 60) { continue }
        $xa = $left + ($right - $left) * (($ts[$i - 1] - $t0).TotalMinutes / $span)
        $xb = $left + ($right - $left) * (($ts[$i] - $t0).TotalMinutes / $span)
        $ya = $bot - $ph * ($d[$i - 1] - $mn) / ($mx - $mn)
        $yb = $bot - $ph * ($d[$i] - $mn) / ($mx - $mn)
        $g.DrawLine($lp, [single]$xa, [single]$ya, [single]$xb, [single]$yb)
    }
    # nhãn trục Y (xoay dọc)
    $st = $g.Save()
    $g.TranslateTransform(40, ($top + $ph / 2))
    $g.RotateTransform(-90)
    $g.DrawString($series[$s].lbl, $fY, [System.Drawing.Brushes]::Black, (New-Object System.Drawing.RectangleF(-300, -10, 600, 80)), $sf)
    $g.Restore($st)
}
$g.DrawString($L[17], $fY, [System.Drawing.Brushes]::Black, (New-Object System.Drawing.RectangleF($left, ($H - 90), ($right - $left), 70)), $sf)
$bmp.Save((Join-Path $OutDir "hinh_2_28_du_lieu_cam_bien.png"), [System.Drawing.Imaging.ImageFormat]::Png)
$g.Dispose(); $bmp.Dispose()
"points=$($ts.Count) from $t0 to $t1; Tmin=$(($T|Measure-Object -Min).Minimum) Tmax=$(($T|Measure-Object -Max).Maximum) Hmin=$(($Hm|Measure-Object -Min).Minimum) Hmax=$(($Hm|Measure-Object -Max).Maximum) Lmax=$(($Lx|Measure-Object -Max).Maximum)"
