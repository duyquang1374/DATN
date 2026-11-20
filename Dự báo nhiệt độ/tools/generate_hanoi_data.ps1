# Tao them du lieu huan luyen tu thoi tiet THAT cua Ha Noi (Open-Meteo Historical API, nguon ERA5/ECMWF).
#
# Cach lam:
#  1. Tai du lieu theo gio (nhiet do 2m, do am 2m, buc xa mat troi) cho Ha Noi trong dung khoang
#     thoi gian ban da do bang cam bien (Data/*.csv), roi hieu chinh (calibrate):
#       - Nhiet do / do am: hoi quy tuyen tinh + sai lech theo tung gio trong ngay
#         (cam bien dat trong nha kinh nen am hon, dao dong ngay-dem nho hon ngoai troi).
#       - Anh sang: he so lux / (W/m2) theo tung gio (phan anh huong dat cam bien, bong che...).
#  2. Tai du lieu cac giai doan khac (1-1.5 thang moi file), ap dung hieu chinh tren,
#     noi suy tu 1 gio xuong 30 giay (giong chu ky lay mau cua cam bien), them nhieu
#     ngau nhien co tuong quan thoi gian, lam tron giong cam bien (0.1 C, 0.1 %, buoc 1/1.2 lux cua BH1750).
#
# Chay:  powershell -ExecutionPolicy Bypass -File tools\generate_hanoi_data.ps1

$ErrorActionPreference = 'Stop'
$root    = Split-Path $PSScriptRoot -Parent
$dataDir = Join-Path $root 'Data'
$outDir  = Join-Path $dataDir 'hanoi_openmeteo'
New-Item -ItemType Directory -Force $outDir | Out-Null

$LAT = 21.0285; $LON = 105.8542   # Ha Noi (Hoan Kiem)
$periods = @(
    @('2023-08-01','2023-09-14'),
    @('2024-08-15','2024-09-30'),   # co bao Yagi (07/09/2024)
    @('2025-08-01','2025-09-14'),
    @('2025-09-15','2025-10-31'),
    @('2026-09-17','2026-10-31')    # noi tiep ngay sau dot thu thap cua ban
)

function Get-Hourly($start, $end) {
    $u = "https://archive-api.open-meteo.com/v1/archive?latitude=$LAT&longitude=$LON&start_date=$start&end_date=$end" +
         "&hourly=temperature_2m,relative_humidity_2m,shortwave_radiation&timezone=Asia%2FBangkok"
    $r = Invoke-RestMethod $u -TimeoutSec 120
    $n = $r.hourly.time.Count
    $o = [pscustomobject]@{ time = New-Object 'datetime[]' $n; t = New-Object 'double[]' $n; h = New-Object 'double[]' $n; r = New-Object 'double[]' $n }
    for ($i = 0; $i -lt $n; $i++) {
        $o.time[$i] = [datetime]$r.hourly.time[$i]
        $o.t[$i] = if ($r.hourly.temperature_2m[$i] -ne $null) { [double]$r.hourly.temperature_2m[$i] } else { [double]::NaN }
        $o.h[$i] = if ($r.hourly.relative_humidity_2m[$i] -ne $null) { [double]$r.hourly.relative_humidity_2m[$i] } else { [double]::NaN }
        $o.r[$i] = if ($r.hourly.shortwave_radiation[$i] -ne $null) { [double]$r.hourly.shortwave_radiation[$i] } else { [double]::NaN }
    }
    return $o
}

Add-Type -TypeDefinition @'
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;

public static class HanoiGen {
    static double Lin(double[] v, int i) { return v[i]; }

    // Fill NaN gaps by linear interpolation
    public static void FillNaN(double[] v) {
        int n = v.Length, last = -1;
        for (int i = 0; i < n; i++) {
            if (double.IsNaN(v[i])) continue;
            if (last >= 0 && i - last > 1)
                for (int k = last + 1; k < i; k++) v[k] = v[last] + (v[i] - v[last]) * (k - last) / (double)(i - last);
            else if (last < 0) for (int k = 0; k < i; k++) v[k] = v[i];
            last = i;
        }
        if (last >= 0) for (int k = last + 1; k < n; k++) v[k] = v[last];
    }

    // Catmull-Rom interpolation at fractional index x
    static double CR(double[] v, double x) {
        int n = v.Length;
        if (x <= 0) return v[0];
        if (x >= n - 1) return v[n - 1];
        int i = (int)Math.Floor(x); double t = x - i;
        double p0 = v[Math.Max(i - 1, 0)], p1 = v[i], p2 = v[i + 1], p3 = v[Math.Min(i + 2, n - 1)];
        return 0.5 * ((2 * p1) + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t + (-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);
    }
    static double LinAt(double[] v, double x) {
        int n = v.Length;
        if (x <= 0) return v[0];
        if (x >= n - 1) return v[n - 1];
        int i = (int)Math.Floor(x); double t = x - i;
        return v[i] * (1 - t) + v[i + 1] * t;
    }
    static double PerHour(double[] a24, double hourFrac) {
        // smooth periodic interpolation of a 24-value per-hour table (value centred at hh:30)
        double x = hourFrac - 0.5; if (x < 0) x += 24;
        int i = (int)Math.Floor(x) % 24; double t = x - Math.Floor(x);
        return a24[i] * (1 - t) + a24[(i + 1) % 24] * t;
    }

    static double Gauss(Random r) {
        double u1 = 1.0 - r.NextDouble(), u2 = r.NextDouble();
        return Math.Sqrt(-2.0 * Math.Log(u1)) * Math.Cos(2 * Math.PI * u2);
    }

    // time0: first hourly timestamp. t/h: instantaneous at each hour. rad: mean over previous hour.
    public static int Generate(string path, DateTime time0, double[] t, double[] h, double[] rad,
                               double aT, double bT, double[] resT, double aH, double bH, double[] resH,
                               double[] luxFactor, int seed) {
        FillNaN(t); FillNaN(h); FillNaN(rad);
        int nh = t.Length;
        // radiation value of hour i represents the interval centre (i - 0.5)
        var rc = new double[nh];
        for (int i = 0; i < nh; i++) rc[i] = Math.Max(0, rad[i]);

        var rnd = new Random(seed);
        var ci = CultureInfo.InvariantCulture;
        var sb = new StringBuilder();
        sb.Append("timestamp,temperature,humidity,light\n");

        double nT = 0, nH = 0, nL = 0, nLamp = 0;
        double sdT = 0.25, sdH = 1.5, sdL = 0.35;    // stationary std of slow (AR) noise
        double phiT = 0.997, phiH = 0.997, phiL = 0.985;
        double lampLevel = 0; int lastDay = -1;
        int steps = (nh - 1) * 120; // 30 s steps
        DateTime start = time0.AddSeconds(rnd.Next(0, 30));
        int rows = 0;
        for (int s = 0; s <= steps; s++) {
            DateTime ts = start.AddSeconds(30 * s);
            double x = s / 120.0;                     // fractional hour index
            double hourOfDay = ts.Hour + ts.Minute / 60.0 + ts.Second / 3600.0;

            nT = phiT * nT + Math.Sqrt(1 - phiT * phiT) * sdT * Gauss(rnd);
            nH = phiH * nH + Math.Sqrt(1 - phiH * phiH) * sdH * Gauss(rnd);
            nL = phiL * nL + Math.Sqrt(1 - phiL * phiL) * sdL * Gauss(rnd);

            double tOut = CR(t, x), hOut = CR(h, x);
            double T = aT * tOut + bT + PerHour(resT, hourOfDay) + nT + 0.04 * Gauss(rnd);
            double H = aH * hOut + bH + PerHour(resH, hourOfDay) + nH - 1.5 * nT + 0.15 * Gauss(rnd);
            H = Math.Max(30, Math.Min(100, H));

            // radiation: interpolate between interval centres (index i-0.5 -> shift +0.5)
            double R = Math.Max(0, LinAt(rc, x + 0.5));
            double lux = R * PerHour(luxFactor, hourOfDay) * Math.Exp(nL - 0.5 * sdL * sdL);

            // evening / night artificial light similar to the greenhouse sensor (0-50 lux)
            if (ts.DayOfYear != lastDay) { lastDay = ts.DayOfYear; lampLevel = 10 + rnd.NextDouble() * 30; }
            nLamp = 0.99 * nLamp + 0.14 * Gauss(rnd);
            if (hourOfDay >= 18.5 || hourOfDay < 0.5) lux += Math.Max(0, lampLevel + 3 * nLamp);
            else if (hourOfDay < 5.5) lux += Math.Max(0, 1.5 + 2 * nLamp);

            lux = Math.Max(0, lux);
            lux = Math.Round(lux * 1.2) / 1.2;        // BH1750 resolution
            if (lux > 54612.5) lux = 54612.5;          // BH1750 saturation

            sb.Append(ts.ToString("yyyy-MM-dd HH:mm:ss", ci)).Append(',')
              .Append(Math.Round(T, 1).ToString("0.0", ci)).Append(',')
              .Append(Math.Round(H, 1).ToString("0.0", ci)).Append(',')
              .Append(lux.ToString("0.0", ci)).Append('\n');
            rows++;
        }
        File.WriteAllText(path, sb.ToString(), new UTF8Encoding(false));
        return rows;
    }
}
'@

# ---------- 1. Hieu chinh tu du lieu cam bien cua ban ----------
Write-Host "Doc du lieu cam bien..."
$ci = [Globalization.CultureInfo]::InvariantCulture
$fmts = [string[]]@('yyyy-MM-dd HH:mm:ss', 'M/d/yyyy H:mm', 'M/d/yyyy H:mm:ss')
$sens = @{}
$minD = [datetime]::MaxValue; $maxD = [datetime]::MinValue
# chi hieu chinh theo du lieu cam bien that, bo qua cac file hanoi_* da sinh truoc do
foreach ($f in Get-ChildItem (Join-Path $dataDir '*.csv') | Where-Object { $_.Name -notlike 'hanoi_*' }) {
    foreach ($row in Import-Csv $f.FullName) {
        $d = [datetime]::MinValue
        if (-not [datetime]::TryParseExact($row.timestamp, $fmts, $ci, 0, [ref]$d)) { continue }
        $k = $d.Date.AddHours($d.Hour)
        if (-not $sens.ContainsKey($k)) { $sens[$k] = @(0.0, 0.0, 0.0, 0) }
        $a = $sens[$k]; $a[0] += [double]$row.temperature; $a[1] += [double]$row.humidity; $a[2] += [double]$row.light; $a[3]++
        if ($d -lt $minD) { $minD = $d }; if ($d -gt $maxD) { $maxD = $d }
    }
}
Write-Host ("  {0} gio du lieu, tu {1:yyyy-MM-dd} den {2:yyyy-MM-dd}" -f $sens.Count, $minD, $maxD)
$cal = Get-Hourly ($minD.ToString('yyyy-MM-dd')) ($maxD.ToString('yyyy-MM-dd'))
$idx = @{}; for ($i = 0; $i -lt $cal.time.Count; $i++) { $idx[$cal.time[$i]] = $i }

$P = foreach ($k in $sens.Keys) {
    if (-not $idx.ContainsKey($k)) { continue }
    $i = $idx[$k]; $a = $sens[$k]
    if ([double]::IsNaN($cal.t[$i]) -or ($i + 1) -ge $cal.time.Count) { continue }
    [pscustomobject]@{ hr = $k.Hour; st = $a[0] / $a[3]; sh = $a[1] / $a[3]; sl = $a[2] / $a[3]
        at = $cal.t[$i]; ah = $cal.h[$i]; ar = ($cal.r[$i] + $cal.r[$i + 1]) / 2 }
}
function Fit($x, $y) {
    $mx = ($x | Measure-Object -Average).Average; $my = ($y | Measure-Object -Average).Average
    $sxy = 0; $sxx = 0
    for ($i = 0; $i -lt $x.Count; $i++) { $sxy += ($x[$i] - $mx) * ($y[$i] - $my); $sxx += ($x[$i] - $mx) * ($x[$i] - $mx) }
    $a = $sxy / $sxx; return @($a, ($my - $a * $mx))
}
$fT = Fit @($P.at) @($P.st); $fH = Fit @($P.ah) @($P.sh)
$resT = New-Object 'double[]' 24; $resH = New-Object 'double[]' 24; $luxF = New-Object 'double[]' 24
foreach ($g in ($P | Group-Object hr)) {
    $hr = [int]$g.Name
    $resT[$hr] = ($g.Group | ForEach-Object { $_.st - ($fT[0] * $_.at + $fT[1]) } | Measure-Object -Average).Average
    $resH[$hr] = ($g.Group | ForEach-Object { $_.sh - ($fH[0] * $_.ah + $fH[1]) } | Measure-Object -Average).Average
    $sumR = ($g.Group | Measure-Object ar -Sum).Sum
    $luxF[$hr] = if ($sumR -gt 5 * $g.Count) { ($g.Group | Measure-Object sl -Sum).Sum / $sumR } else { 0 }
}
# don't let tiny-radiation hours (dawn/dusk) explode
for ($hr = 0; $hr -lt 24; $hr++) { $luxF[$hr] = [math]::Min($luxF[$hr], 15) }
Write-Host ("  T_cam_bien = {0:N3}*T_ngoai_troi + {1:N2};  H_cam_bien = {2:N3}*H_ngoai_troi + {3:N2}" -f $fT[0], $fT[1], $fH[0], $fH[1])
Write-Host ("  He so lux/(W/m2) theo gio: " + (($luxF | ForEach-Object { '{0:N1}' -f $_ }) -join ' '))

# ---------- 2. Sinh du lieu cho tung giai doan ----------
$seed = 2026
foreach ($p in $periods) {
    $name = "hanoi_{0}_{1}.csv" -f $p[0], $p[1]
    Write-Host "Tai $($p[0]) -> $($p[1]) ..."
    $d = Get-Hourly $p[0] $p[1]
    # drop trailing hours that are not yet available (NaN at end)
    $n = $d.time.Count; while ($n -gt 0 -and [double]::IsNaN($d.t[$n - 1])) { $n-- }
    $t = $d.t[0..($n - 1)]; $h = $d.h[0..($n - 1)]; $r = $d.r[0..($n - 1)]
    $rows = [HanoiGen]::Generate((Join-Path $outDir $name), $d.time[0], [double[]]$t, [double[]]$h, [double[]]$r,
        $fT[0], $fT[1], $resT, $fH[0], $fH[1], $resH, $luxF, $seed)
    Write-Host "  -> $name : $rows dong"
    $seed++
}
Write-Host "Xong. File o: $outDir"
