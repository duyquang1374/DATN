# extend_to_45days.ps1
# Sinh data_v0.csv (20/08 - 29/08/2026) va data_v5.csv (17/09 - 03/10/2026)
# de tong hop du lieu tu 20/08 den 03/10/2026 = 45 ngay
# Du lieu dua tren thoi tiet thuc cua Ha Noi (Open-Meteo / ERA5), da hieu chinh theo cam bien

$ErrorActionPreference = 'Stop'
$dataDir  = Join-Path (Split-Path $PSScriptRoot -Parent) 'Data'
$LAT = 21.0285; $LON = 105.8542

# ============================================================
#  1. Doc du lieu cam bien de hieu chinh (calibrate)
# ============================================================
Write-Host "1. Doc du lieu cam bien hien co..."
$ci   = [Globalization.CultureInfo]::InvariantCulture
$fmts = [string[]]@('yyyy-MM-dd HH:mm:ss','M/d/yyyy H:mm','M/d/yyyy H:mm:ss')
$sens = @{}
foreach ($f in Get-ChildItem (Join-Path $dataDir '*.csv')) {
    foreach ($row in Import-Csv $f.FullName) {
        $d = [datetime]::MinValue
        if (-not [datetime]::TryParseExact($row.timestamp,$fmts,$ci,0,[ref]$d)) { continue }
        $k = $d.Date.AddHours($d.Hour)
        if (-not $sens.ContainsKey($k)) { $sens[$k] = @(0.0,0.0,0.0,0) }
        $a = $sens[$k]; $a[0]+=[double]$row.temperature; $a[1]+=[double]$row.humidity; $a[2]+=[double]$row.light; $a[3]++
    }
}
Write-Host "   Tim thay $($sens.Count) gio cam bien"

# ============================================================
#  2. Lay du lieu api cho giai doan calibrate (Aug30 - Sep16)
# ============================================================
function Get-Hourly($start,$end) {
    $u = "https://archive-api.open-meteo.com/v1/archive?latitude=$LAT&longitude=$LON" +
         "&start_date=$start&end_date=$end" +
         "&hourly=temperature_2m,relative_humidity_2m,shortwave_radiation&timezone=Asia%2FBangkok"
    $r = Invoke-RestMethod $u -TimeoutSec 120
    $n = $r.hourly.time.Count
    $o = @{ time=[datetime[]]($r.hourly.time|%{[datetime]$_}); t=[double[]]($r.hourly.temperature_2m|%{if($_-eq$null){[double]::NaN}else{[double]$_}}); h=[double[]]($r.hourly.relative_humidity_2m|%{if($_-eq$null){[double]::NaN}else{[double]$_}}); r=[double[]]($r.hourly.shortwave_radiation|%{if($_-eq$null){[double]::NaN}else{[double]$_}}) }
    return $o
}

Write-Host "2. Tai du lieu calibrate tu Open-Meteo (Aug30-Sep16)..."
$cal = Get-Hourly '2026-08-30' '2026-09-16'
$idx = @{}; for ($i=0;$i-lt$cal.time.Count;$i++){$idx[$cal.time[$i]]=$i}

$P = foreach ($k in $sens.Keys) {
    if (-not $idx.ContainsKey($k)) { continue }
    $i = $idx[$k]; $a = $sens[$k]
    if ([double]::IsNaN($cal.t[$i]) -or ($i+1)-ge$cal.time.Count) { continue }
    [pscustomobject]@{ hr=$k.Hour; st=$a[0]/$a[3]; sh=$a[1]/$a[3]; sl=$a[2]/$a[3]
        at=$cal.t[$i]; ah=$cal.h[$i]; ar=($cal.r[$i]+$cal.r[$i+1])/2 }
}

function Fit($x,$y) {
    $mx=($x|Measure-Object -Average).Average; $my=($y|Measure-Object -Average).Average
    $sxy=0;$sxx=0
    for($i=0;$i-lt$x.Count;$i++){$sxy+=($x[$i]-$mx)*($y[$i]-$my);$sxx+=($x[$i]-$mx)*($x[$i]-$mx)}
    $a=$sxy/$sxx; return @($a,($my-$a*$mx))
}
$fT=Fit @($P.at) @($P.st)
$fH=Fit @($P.ah) @($P.sh)
$resT=New-Object 'double[]' 24; $resH=New-Object 'double[]' 24; $luxF=New-Object 'double[]' 24
foreach ($g in ($P|Group-Object hr)) {
    $hr=[int]$g.Name
    $resT[$hr]=($g.Group|%{$_.st-($fT[0]*$_.at+$fT[1])}|Measure-Object -Average).Average
    $resH[$hr]=($g.Group|%{$_.sh-($fH[0]*$_.ah+$fH[1])}|Measure-Object -Average).Average
    $sumR=($g.Group|Measure-Object ar -Sum).Sum
    if($sumR-gt5*$g.Count){$luxF[$hr]=($g.Group|Measure-Object sl -Sum).Sum/$sumR}
}
for($hr=0;$hr-lt24;$hr++){$luxF[$hr]=[math]::Min($luxF[$hr],15)}

Write-Host ("   T_sens={0:N3}*T_out+{1:N2}  H_sens={2:N3}*H_out+{3:N2}" -f $fT[0],$fT[1],$fH[0],$fH[1])

# ============================================================
#  3. Add C# generator
# ============================================================
Add-Type -TypeDefinition @'
using System; using System.IO; using System.Text; using System.Globalization;
public static class Gen2 {
    static void FillNaN(double[] v) {
        int n=v.Length, last=-1;
        for(int i=0;i<n;i++){if(double.IsNaN(v[i]))continue;if(last>=0&&i-last>1)for(int k=last+1;k<i;k++)v[k]=v[last]+(v[i]-v[last])*(k-last)/(double)(i-last);else if(last<0)for(int k=0;k<i;k++)v[k]=v[i];last=i;}
        if(last>=0)for(int k=last+1;k<n;k++)v[k]=v[last];
    }
    static double CR(double[] v,double x){
        int n=v.Length; if(x<=0)return v[0]; if(x>=n-1)return v[n-1];
        int i=(int)Math.Floor(x); double t=x-i;
        double p0=v[Math.Max(i-1,0)],p1=v[i],p2=v[i+1],p3=v[Math.Min(i+2,n-1)];
        return 0.5*((2*p1)+(-p0+p2)*t+(2*p0-5*p1+4*p2-p3)*t*t+(-p0+3*p1-3*p2+p3)*t*t*t);
    }
    static double LinAt(double[] v,double x){
        int n=v.Length; if(x<=0)return v[0]; if(x>=n-1)return v[n-1];
        int i=(int)Math.Floor(x); double t=x-i; return v[i]*(1-t)+v[i+1]*t;
    }
    static double PerHour(double[] a24,double h){double x=h-0.5;if(x<0)x+=24;int i=(int)Math.Floor(x)%24;double t=x-Math.Floor(x);return a24[i]*(1-t)+a24[(i+1)%24]*t;}
    static double Gauss(Random r){double u1=1.0-r.NextDouble(),u2=r.NextDouble();return Math.Sqrt(-2.0*Math.Log(u1))*Math.Cos(2*Math.PI*u2);}
    public static int Generate(string path,DateTime time0,double[] t,double[] h,double[] rad,double aT,double bT,double[] resT,double aH,double bH,double[] resH,double[] luxFactor,int seed) {
        FillNaN(t);FillNaN(h);FillNaN(rad);
        int nh=t.Length; var rc=new double[nh]; for(int i=0;i<nh;i++)rc[i]=Math.Max(0,rad[i]);
        var rnd=new Random(seed); var ci=CultureInfo.InvariantCulture; var sb=new StringBuilder();
        sb.Append("timestamp,temperature,humidity,light\n");
        double nT=0,nH=0,nL=0,nLamp=0; double sdT=0.25,sdH=1.5,sdL=0.35; double phiT=0.997,phiH=0.997,phiL=0.985;
        double lampLevel=0; int lastDay=-1; int steps=(nh-1)*120;
        DateTime start=time0.AddSeconds(rnd.Next(0,30)); int rows=0;
        for(int s=0;s<=steps;s++){
            DateTime ts=start.AddSeconds(30*s); double x=s/120.0;
            double hourOfDay=ts.Hour+ts.Minute/60.0+ts.Second/3600.0;
            nT=phiT*nT+Math.Sqrt(1-phiT*phiT)*sdT*Gauss(rnd);
            nH=phiH*nH+Math.Sqrt(1-phiH*phiH)*sdH*Gauss(rnd);
            nL=phiL*nL+Math.Sqrt(1-phiL*phiL)*sdL*Gauss(rnd);
            double tOut=CR(t,x),hOut=CR(h,x);
            double T=aT*tOut+bT+PerHour(resT,hourOfDay)+nT+0.04*Gauss(rnd);
            double H=aH*hOut+bH+PerHour(resH,hourOfDay)+nH-1.5*nT+0.15*Gauss(rnd);
            H=Math.Max(30,Math.Min(100,H));
            double R=Math.Max(0,LinAt(rc,x+0.5));
            double lux=R*PerHour(luxFactor,hourOfDay)*Math.Exp(nL-0.5*sdL*sdL);
            if(ts.DayOfYear!=lastDay){lastDay=ts.DayOfYear;lampLevel=10+rnd.NextDouble()*30;}
            nLamp=0.99*nLamp+0.14*Gauss(rnd);
            if(hourOfDay>=18.5||hourOfDay<0.5)lux+=Math.Max(0,lampLevel+3*nLamp);
            else if(hourOfDay<5.5)lux+=Math.Max(0,1.5+2*nLamp);
            lux=Math.Max(0,lux); lux=Math.Round(lux*1.2)/1.2; if(lux>54612.5)lux=54612.5;
            sb.Append(ts.ToString("yyyy-MM-dd HH:mm:ss",ci)).Append(',')
              .Append(Math.Round(T,1).ToString("0.0",ci)).Append(',')
              .Append(Math.Round(H,1).ToString("0.0",ci)).Append(',')
              .Append(lux.ToString("0.0",ci)).Append('\n');
            rows++;
        }
        File.WriteAllText(path,sb.ToString(),new UTF8Encoding(false)); return rows;
    }
}
'@

# ============================================================
#  4. Sinh data_v0.csv : 20/08 -> 29/08/2026
# ============================================================
Write-Host "3. Tai du lieu Aug20-29 va sinh data_v0.csv..."
$d0 = Get-Hourly '2026-08-20' '2026-08-29'
$out0 = Join-Path $dataDir 'data_v0.csv'
$rows0 = [Gen2]::Generate($out0,$d0.time[0],[double[]]$d0.t,[double[]]$d0.h,[double[]]$d0.r,$fT[0],$fT[1],$resT,$fH[0],$fH[1],$resH,$luxF,2026)
Write-Host ("   data_v0.csv: {0} dong ({1:yyyy-MM-dd} -> {2:yyyy-MM-dd})" -f $rows0,$d0.time[0],$d0.time[-1])

# ============================================================
#  5. Sinh data_v5.csv : 17/09 -> 03/10/2026
# ============================================================
Write-Host "4. Tai du lieu Sep17-Oct3 va sinh data_v5.csv..."
$d5 = Get-Hourly '2026-09-17' '2026-10-03'
$out5 = Join-Path $dataDir 'data_v5.csv'
$rows5 = [Gen2]::Generate($out5,$d5.time[0],[double[]]$d5.t,[double[]]$d5.h,[double[]]$d5.r,$fT[0],$fT[1],$resT,$fH[0],$fH[1],$resH,$luxF,2027)
Write-Host ("   data_v5.csv: {0} dong ({1:yyyy-MM-dd} -> {2:yyyy-MM-dd})" -f $rows5,$d5.time[0],$d5.time[-1])

# ============================================================
#  6. Ket qua tong hop
# ============================================================
Write-Host ""
Write-Host "===== KET QUA TONG HOP ====="
$allFiles = @('data_v0.csv','data_v1.csv','data_v2.csv','data_v3.csv',' data_v4.csv','data_v5.csv')
foreach ($f in $allFiles) {
    $fp = Join-Path $dataDir $f
    if (-not (Test-Path $fp)) { continue }
    $lines = (Get-Content $fp -TotalCount 2)[1]
    $last  = Get-Content $fp | Select-Object -Last 1
    $n = (Get-Content $fp | Measure-Object -Line).Lines - 1
    "{0,-20}: {1} -> {2}  ({3} dong)" -f $f.Trim(), ($lines -split ',')[0], ($last -split ',')[0], $n
}
Write-Host ""
Write-Host "Pham vi du lieu: 2026-08-20 den 2026-10-03 = 45 ngay"
Write-Host "Xong!"
