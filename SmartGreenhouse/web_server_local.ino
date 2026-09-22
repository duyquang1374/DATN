/*
 * web_server_local.ino
 * Web Dashboard + REST API cho hệ thống nhà màng thông minh.
 *
 * Endpoints:
 *   GET  /               → Dashboard HTML (tự refresh mỗi 5s)
 *   GET  /api/status     → JSON: toàn bộ trạng thái hệ thống
 *   GET  /api/control    → Điều khiển fan/pump qua query params
 *   GET  /api/forecast   → JSON: dự báo thời tiết 3 mốc
 *   GET  /download       → Tải file CSV
 *   GET  /api/data       → 100 bản ghi gần nhất (JSON)
 *   GET  /api/stats      → Thống kê bộ nhớ flash
 *   GET  /delete         → Xóa toàn bộ CSV data
 */
#include "forecast.h"  // FC_HORIZONS, g_fc_*, g_forecast_ready, fc_buf_count


// ═══════════════════════════════════════════════════════════════
//  SETUP WEB SERVER
// ═══════════════════════════════════════════════════════════════
void setupWebServer() {
  webServer.on("/",               HTTP_GET, handleRoot);
  webServer.on("/api/status",     HTTP_GET, handleApiStatus);
  webServer.on("/api/control",    HTTP_GET, handleApiControl);
  webServer.on("/api/data",       HTTP_GET, handleApiData);
  webServer.on("/api/stats",      HTTP_GET, handleApiStats);
  webServer.on("/api/forecast",   HTTP_GET, handleApiForecast);
  webServer.on("/download",       HTTP_GET, handleDownload);
  webServer.on("/delete",         HTTP_GET, handleDelete);
  webServer.onNotFound([]() {
    webServer.send(404, "text/plain", "404 Not Found");
  });
  webServer.begin();
  Serial.println("[WEB] Server started on port 80");
}

// ═══════════════════════════════════════════════════════════════
//  DASHBOARD HTML
// ═══════════════════════════════════════════════════════════════
void handleRoot() {
  // Lấy IP để dùng trong JS
  String ip = g_wifiOK ? WiFi.localIP().toString() : "0.0.0.0";

  String html = F("<!DOCTYPE html><html lang='vi'><head>"
    "<meta charset='UTF-8'>"
    "<meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<title>🌿 Nhà Màng Thông Minh</title>"
    "<link href='https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700&display=swap' rel='stylesheet'>"
    "<style>");

  html += F(
    "*{margin:0;padding:0;box-sizing:border-box}"
    "body{font-family:'Inter',sans-serif;background:#0a0f1e;color:#e2e8f0;"
         "min-height:100vh;padding:20px}"
    ".container{max-width:960px;margin:0 auto}"
    /* Header */
    ".header{text-align:center;margin-bottom:28px;padding:20px}"
    ".header h1{font-size:clamp(20px,4vw,28px);font-weight:700;"
               "background:linear-gradient(135deg,#4ade80,#22d3ee,#818cf8);"
               "-webkit-background-clip:text;-webkit-text-fill-color:transparent;"
               "margin-bottom:6px}"
    ".header .sub{color:#64748b;font-size:13px;letter-spacing:0.5px}"
    ".status-dot{display:inline-block;width:8px;height:8px;border-radius:50%;"
                "margin-right:6px;animation:pulse 2s infinite}"
    ".dot-green{background:#4ade80;box-shadow:0 0 8px #4ade80}"
    ".dot-red{background:#f87171;box-shadow:0 0 8px #f87171}"
    "@keyframes pulse{0%,100%{opacity:1}50%{opacity:.4}}"
    /* Grid */
    ".grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:14px;margin-bottom:16px}"
    ".grid-2{display:grid;grid-template-columns:1fr 1fr;gap:14px;margin-bottom:16px}"
    "@media(max-width:600px){.grid-2{grid-template-columns:1fr}}"
    /* Cards */
    ".card{background:rgba(255,255,255,.04);border:1px solid rgba(255,255,255,.08);"
          "border-radius:16px;padding:20px;position:relative;overflow:hidden;"
          "transition:transform .2s,border-color .2s}"
    ".card:hover{transform:translateY(-2px);border-color:rgba(255,255,255,.15)}"
    ".card::before{content:'';position:absolute;top:0;left:0;right:0;height:3px;"
                  "border-radius:16px 16px 0 0}"
    ".card-temp::before{background:linear-gradient(90deg,#f59e0b,#ef4444)}"
    ".card-humid::before{background:linear-gradient(90deg,#3b82f6,#06b6d4)}"
    ".card-light::before{background:linear-gradient(90deg,#eab308,#f59e0b)}"
    ".card-soil::before{background:linear-gradient(90deg,#a3e635,#4ade80)}"
    ".card-fan::before{background:linear-gradient(90deg,#818cf8,#a78bfa)}"
    ".card-pump::before{background:linear-gradient(90deg,#06b6d4,#3b82f6)}"
    ".card-info::before{background:linear-gradient(90deg,#6b7280,#9ca3af)}"
    ".card-label{font-size:11px;font-weight:500;letter-spacing:1px;"
                "color:#64748b;text-transform:uppercase;margin-bottom:8px}"
    ".card-icon{font-size:22px;margin-bottom:6px}"
    ".card-value{font-size:36px;font-weight:700;line-height:1}"
    ".card-unit{font-size:14px;font-weight:400;color:#94a3b8;margin-left:3px}"
    ".card-sub{font-size:12px;color:#475569;margin-top:6px}"
    /* Badge */
    ".badge{display:inline-block;padding:3px 10px;border-radius:20px;"
           "font-size:11px;font-weight:600;letter-spacing:.5px}"
    ".badge-green{background:rgba(74,222,128,.15);color:#4ade80;border:1px solid rgba(74,222,128,.3)}"
    ".badge-red{background:rgba(248,113,113,.15);color:#f87171;border:1px solid rgba(248,113,113,.3)}"
    ".badge-blue{background:rgba(99,179,237,.15);color:#63b3ed;border:1px solid rgba(99,179,237,.3)}"
    ".badge-yellow{background:rgba(234,179,8,.15);color:#eab308;border:1px solid rgba(234,179,8,.3)}"
    ".badge-purple{background:rgba(167,139,250,.15);color:#a78bfa;border:1px solid rgba(167,139,250,.3)}"
    /* Progress bar */
    ".progress-wrap{background:rgba(255,255,255,.06);border-radius:8px;height:8px;"
                   "margin-top:12px;overflow:hidden}"
    ".progress-bar{height:100%;border-radius:8px;transition:width .5s ease}"
    ".bar-purple{background:linear-gradient(90deg,#818cf8,#a78bfa)}"
    ".bar-blue{background:linear-gradient(90deg,#3b82f6,#06b6d4)}"
    /* Controls */
    ".controls{display:flex;gap:8px;flex-wrap:wrap;margin-top:14px}"
    ".btn{padding:8px 14px;border:1px solid rgba(255,255,255,.12);border-radius:10px;"
         "background:rgba(255,255,255,.05);color:#e2e8f0;cursor:pointer;"
         "font-size:12px;font-weight:500;font-family:inherit;"
         "transition:all .2s;white-space:nowrap}"
    ".btn:hover{background:rgba(255,255,255,.1);transform:translateY(-1px)}"
    ".btn-active{background:rgba(74,222,128,.2)!important;"
               "border-color:#4ade80!important;color:#4ade80!important}"
    ".btn-red{border-color:rgba(248,113,113,.3);color:#f87171}"
    ".btn-red:hover{background:rgba(248,113,113,.15)!important}"
    ".btn-red.btn-active{background:rgba(248,113,113,.2)!important;color:#f87171!important}"
    /* Slider */
    ".slider-wrap{margin-top:10px}"
    ".slider-label{font-size:11px;color:#64748b;margin-bottom:4px;display:flex;"
                  "justify-content:space-between}"
    "input[type=range]{width:100%;-webkit-appearance:none;height:6px;"
                      "border-radius:3px;background:rgba(255,255,255,.1);outline:none}"
    "input[type=range]::-webkit-slider-thumb{-webkit-appearance:none;width:16px;height:16px;"
                                            "border-radius:50%;background:#818cf8;cursor:pointer;"
                                            "transition:.2s}"
    /* Actions */
    ".actions{display:flex;gap:10px;flex-wrap:wrap;justify-content:center;margin-top:20px}"
    ".action-btn{padding:12px 24px;border-radius:12px;border:none;cursor:pointer;"
                "font-size:13px;font-weight:600;font-family:inherit;transition:all .2s;"
                "text-decoration:none;display:inline-flex;align-items:center;gap:6px}"
    ".action-btn:hover{transform:translateY(-2px);box-shadow:0 8px 20px rgba(0,0,0,.3)}"
    ".btn-dl{background:linear-gradient(135deg,#4ade80,#16a34a);color:#0a0f1e}"
    ".btn-api{background:linear-gradient(135deg,#3b82f6,#2563eb);color:#fff}"
    ".btn-del{background:rgba(248,113,113,.15);color:#f87171;"
             "border:1px solid rgba(248,113,113,.3)}"
    /* Timestamp */
    ".ts{text-align:center;color:#334155;font-size:11px;margin-top:14px}"
  );

  html += F("</style></head><body><div class='container'>");

  // Header
  html += F("<div class='header'>"
    "<div class='card-icon'>🌿</div>"
    "<h1>Nhà Màng Thông Minh</h1>"
    "<div class='sub'>"
    "<span class='status-dot' id='dot'></span>"
    "<span id='conn-status'>Đang kết nối...</span>"
    " &nbsp;|&nbsp; ESP32 + SHT30 + BH1750 + TB6612FNG"
    "</div></div>");

  // Sensor Cards Row 1
  html += F("<div class='grid'>");
  // Temperature
  html += F("<div class='card card-temp'>"
    "<div class='card-icon'>🌡️</div>"
    "<div class='card-label'>Nhiệt Độ</div>"
    "<div class='card-value'><span id='temp'>--.-</span>"
    "<span class='card-unit'>°C</span></div>"
    "<div class='card-sub' id='temp-status'>--</div>"
    "</div>");
  // Humidity
  html += F("<div class='card card-humid'>"
    "<div class='card-icon'>💧</div>"
    "<div class='card-label'>Độ Ẩm KK</div>"
    "<div class='card-value'><span id='humid'>--.-</span>"
    "<span class='card-unit'>%</span></div>"
    "<div class='card-sub' id='humid-status'>--</div>"
    "</div>");
  // Light
  html += F("<div class='card card-light'>"
    "<div class='card-icon'>☀️</div>"
    "<div class='card-label'>Cường Độ Sáng</div>"
    "<div class='card-value'><span id='lux'>----</span>"
    "<span class='card-unit'>lux</span></div>"
    "<div class='card-sub' id='lux-status'>--</div>"
    "</div>");
  // Soil
  html += F("<div class='card card-soil'>"
    "<div class='card-icon'>🌱</div>"
    "<div class='card-label'>Độ Ẩm Đất</div>"
    "<div class='card-value'><span id='soil'>--</span>"
    "<span class='card-unit'>%</span></div>"
    "<div class='card-sub' id='soil-status'>--</div>"
    "</div>");
  html += F("</div>"); // end grid

  // Fan + Pump Control Cards
  html += F("<div class='grid-2'>");

  // Fan Card
  html += F("<div class='card card-fan'>"
    "<div style='display:flex;justify-content:space-between;align-items:start'>"
    "<div>"
    "<div class='card-icon'>🌀</div>"
    "<div class='card-label'>Quạt DC (TB6612FNG)</div>"
    "</div>"
    "<span class='badge badge-purple' id='fan-badge'>--</span>"
    "</div>"
    "<div style='margin:10px 0'>"
    "<div style='font-size:13px;color:#94a3b8'>Tốc độ hiện tại</div>"
    "<div class='card-value' style='font-size:28px'>"
    "<span id='fan-speed'>--</span><span class='card-unit'>%</span>"
    "</div>"
    "</div>"
    "<div class='progress-wrap'>"
    "<div class='progress-bar bar-purple' id='fan-bar' style='width:0%'></div>"
    "</div>"
    "<div class='card-sub' style='margin-top:6px'>"
    "PID Setpoint: <span id='fan-sp'>--</span>°C &nbsp;|&nbsp; "
    "PWM: <span id='fan-pwm'>--</span>/255"
    "</div>"
    "<div class='controls'>"
    "<button class='btn' id='btn-fan-off'  onclick='setFan(0)'>⏹ OFF</button>"
    "<button class='btn' id='btn-fan-auto' onclick='setFan(1)'>⚡ AUTO</button>"
    "<button class='btn' id='btn-fan-man'  onclick='setFan(2)'>✋ MANUAL</button>"
    "</div>"
    "<div class='slider-wrap' id='fan-slider-wrap' style='display:none'>"
    "<div class='slider-label'>"
    "<span>Tốc độ thủ công</span><span id='fan-slider-val'>128</span>/255"
    "</div>"
    "<input type='range' id='fan-slider' min='0' max='255' value='128'"
    " oninput='document.getElementById(\"fan-slider-val\").textContent=this.value'"
    " onchange='setFanSpeed(this.value)'>"
    "</div>"
    "</div>"); // end fan card

  // Pump Card — tách riêng String() để tránh F()+String() mixing trên ESP32 Core v3
  html += F("<div class='card card-pump'>"
    "<div style='display:flex;justify-content:space-between;align-items:start'>"
    "<div>"
    "<div class='card-icon'>🚿</div>"
    "<div class='card-label'>Bơm Tưới 5V</div>"
    "</div>"
    "<span class='badge' id='pump-badge'>--</span>"
    "</div>"
    "<div style='margin:12px 0'>"
    "<div style='font-size:13px;color:#94a3b8'>Trạng thái</div>"
    "<div class='card-value' style='font-size:28px' id='pump-state'>--</div>"
    "</div>"
    "<div class='card-sub'>"
    "B&#7853;t khi &#273;&#7845;t &lt;<b><span id='pump-on-val'>");
  html += String(g_pumpOnThresh, 0);
  html += F("</span>%</b> &nbsp;|&nbsp; T&#7855;t khi &#273;&#7845;t &gt;<b><span id='pump-off-val'>");
  html += String(g_pumpOffThresh, 0);
  html += F("</span>%</b></div>"
    "<div class='card-sub' style='margin-top:4px'>"
    "Ch&#7871; &#273;&#7897;: <span id='pump-mode'>--</span> &nbsp;|&nbsp; "
    "Runtime: <span id='pump-time'>--</span>"
    "</div>"
    "<div class='controls'>"
    "<button class='btn btn-active' id='btn-pump-auto' onclick='setPump(\"auto\")'>&#129302; AUTO</button>"
    "<button class='btn' id='btn-pump-on'   onclick='setPump(\"on\")' >&#9654; B&#7852;T TAY</button>"
    "<button class='btn btn-red' id='btn-pump-off' onclick='setPump(\"off\")'>&#9632; T&#7854;T TAY</button>"
    "</div>"
    "</div>"); // end pump card

  html += F("</div>"); // end grid-2

  // Config Card
  html += F("<div class='card card-info'>"
    "<div class='card-label'>⚙️ Cài Đặt Ngưỡng Hệ Thống</div>"
    "<div style='display:grid;grid-template-columns:repeat(auto-fit,minmax(140px,1fr));gap:10px;margin-top:10px'>"
    "<div><div style='color:#64748b;font-size:11px'>Nhiệt độ quạt (°C)</div>"
    "<input type='number' id='cfg-fan-sp' step='0.5' style='width:100%;padding:6px;border:1px solid #cbd5e1;border-radius:4px' value='");
  html += String(g_fanSetpoint, 1);
  html += F("'></div>"
    "<div><div style='color:#64748b;font-size:11px'>Bật bơm khi đất khô < (%)</div>"
    "<input type='number' id='cfg-pump-on' step='1' style='width:100%;padding:6px;border:1px solid #cbd5e1;border-radius:4px' value='");
  html += String(g_pumpOnThresh, 0);
  html += F("'></div>"
    "<div><div style='color:#64748b;font-size:11px'>Tắt bơm khi đất ẩm > (%)</div>"
    "<input type='number' id='cfg-pump-off' step='1' style='width:100%;padding:6px;border:1px solid #cbd5e1;border-radius:4px' value='");
  html += String(g_pumpOffThresh, 0);
  html += F("'></div>"
    "</div>"
    "<button class='btn btn-active' style='margin-top:10px;width:100%' onclick='saveConfig()'>💾 Lưu Cài Đặt</button>"
    "</div>");

  // System Info Card
  html += F("<div class='card card-info'>"
    "<div class='card-label'>📊 Thông Tin Hệ Thống</div>"
    "<div style='display:grid;grid-template-columns:repeat(auto-fit,minmax(140px,1fr));gap:10px;margin-top:10px'>"
    "<div><div style='color:#64748b;font-size:11px'>Số bản ghi</div>"
    "<div style='font-weight:600' id='records'>--</div></div>"
    "<div><div style='color:#64748b;font-size:11px'>Bộ nhớ Flash</div>"
    "<div style='font-weight:600' id='flash'>--</div></div>"
    "<div><div style='color:#64748b;font-size:11px'>WiFi RSSI</div>"
    "<div style='font-weight:600' id='rssi'>--</div></div>"
    "<div><div style='color:#64748b;font-size:11px'>Uptime</div>"
    "<div style='font-weight:600' id='uptime'>--</div></div>"
    "<div><div style='color:#64748b;font-size:11px'>NTP</div>"
    "<div style='font-weight:600' id='ntp'>--</div></div>"
    "<div><div style='color:#64748b;font-size:11px'>IP Address</div>"
    "<div style='font-weight:600' id='ip-addr'>--</div></div>"
    "</div></div>");

  // Action Buttons
  html += F("<div class='actions'>"
    "<a class='action-btn btn-dl' href='/download'>💾 Tải CSV</a>"
    "<a class='action-btn btn-api' href='/api/status' target='_blank'>📡 API Status</a>"
    "<a class='action-btn btn-api' href='/api/forecast' target='_blank'>🔮 API Forecast</a>"
    "<a class='action-btn btn-del' href='/delete'"
    " onclick=\"return confirm('Xóa toàn bộ dữ liệu CSV?')\">🗑 Xóa Data</a>"
    "</div>");

  // Timestamp
  html += F("<div class='ts' id='ts'>Đang tải dữ liệu...</div>");

  // Forecast Card
  html += F(
    "<div class='card' style='margin-bottom:16px;border-color:rgba(129,140,248,.25)'>"
    "<div style='display:flex;justify-content:space-between;align-items:center;margin-bottom:14px'>"
    "<div style='display:flex;align-items:center;gap:8px'>"
    "<span style='font-size:20px'>🔮</span>"
    "<span style='font-size:12px;font-weight:600;letter-spacing:1px;color:#94a3b8;text-transform:uppercase'>Dự Báo Thời Tiết Nhà Màng</span>"
    "</div>"
    "<span class='badge badge-purple' id='fc-badge'>Đang thu thập...</span>"
    "</div>"
    "<div style='display:grid;grid-template-columns:repeat(3,1fr);gap:12px'>"
    // +30 min
    "<div style='background:rgba(129,140,248,.07);border:1px solid rgba(129,140,248,.15);border-radius:12px;padding:14px;text-align:center'>"
    "<div style='font-size:11px;color:#64748b;font-weight:600;letter-spacing:.5px;margin-bottom:8px'>+30 PHÚT</div>"
    "<div style='font-size:22px;font-weight:700;color:#a78bfa' id='fc-t0'>--</div>"
    "<div style='font-size:11px;color:#94a3b8;margin-top:2px'>°C</div>"
    "<div style='font-size:13px;color:#63b3ed;font-weight:600;margin-top:6px' id='fc-h0'>--</div>"
    "<div style='font-size:11px;color:#94a3b8'>% ẩm</div>"
    "<div style='font-size:13px;color:#fbbf24;font-weight:600;margin-top:6px' id='fc-l0'>--</div>"
    "<div style='font-size:11px;color:#94a3b8'>lux</div>"
    "</div>"
    // +60 min
    "<div style='background:rgba(129,140,248,.07);border:1px solid rgba(129,140,248,.15);border-radius:12px;padding:14px;text-align:center'>"
    "<div style='font-size:11px;color:#64748b;font-weight:600;letter-spacing:.5px;margin-bottom:8px'>+1 GIỜ</div>"
    "<div style='font-size:22px;font-weight:700;color:#a78bfa' id='fc-t1'>--</div>"
    "<div style='font-size:11px;color:#94a3b8;margin-top:2px'>°C</div>"
    "<div style='font-size:13px;color:#63b3ed;font-weight:600;margin-top:6px' id='fc-h1'>--</div>"
    "<div style='font-size:11px;color:#94a3b8'>% ẩm</div>"
    "<div style='font-size:13px;color:#fbbf24;font-weight:600;margin-top:6px' id='fc-l1'>--</div>"
    "<div style='font-size:11px;color:#94a3b8'>lux</div>"
    "</div>"
    // +90 min
    "<div style='background:rgba(129,140,248,.07);border:1px solid rgba(129,140,248,.15);border-radius:12px;padding:14px;text-align:center'>"
    "<div style='font-size:11px;color:#64748b;font-weight:600;letter-spacing:.5px;margin-bottom:8px'>+1H30</div>"
    "<div style='font-size:22px;font-weight:700;color:#a78bfa' id='fc-t2'>--</div>"
    "<div style='font-size:11px;color:#94a3b8;margin-top:2px'>°C</div>"
    "<div style='font-size:13px;color:#63b3ed;font-weight:600;margin-top:6px' id='fc-h2'>--</div>"
    "<div style='font-size:11px;color:#94a3b8'>% ẩm</div>"
    "<div style='font-size:13px;color:#fbbf24;font-weight:600;margin-top:6px' id='fc-l2'>--</div>"
    "<div style='font-size:11px;color:#94a3b8'>lux</div>"
    "</div>"
    "</div>"
    "<div style='font-size:11px;color:#334155;text-align:right;margin-top:10px' id='fc-ts'>--</div>"
    "</div>");

  html += F("</div>"); // end container

  // ─── JavaScript ──────────────────────────────────────────
  html += F("<script>");
  html += F(
    "let currentFanMode=1,pumpManual=false;"

    "async function fetchStatus(){"
      "try{"
        "const r=await fetch('/api/status');"
        "if(!r.ok)return;"
        "const d=await r.json();"
        "updateUI(d);"
      "}catch(e){setOffline();}"
    "}"

    "function updateUI(d){"
      // Connection
      "document.getElementById('dot').className='status-dot dot-green';"
      "document.getElementById('conn-status').textContent='Online · '+d.ip;"

      // Sensors
      "document.getElementById('temp').textContent=d.temperature.toFixed(1);"
      "document.getElementById('humid').textContent=d.humidity.toFixed(1);"
      "document.getElementById('lux').textContent=Math.round(d.light);"
      "document.getElementById('soil').textContent=d.soil.toFixed(0);"

      // Sensor status text
      "const t=d.temperature;"
      "document.getElementById('temp-status').textContent="
        "t<25?'🟢 Mát mẻ':t<30?'🟡 Bình thường':t<35?'🟠 Nóng':'🔴 Rất nóng';"
      "document.getElementById('humid-status').textContent="
        "d.humidity<40?'🔴 Khô':'🟢 Tốt';"
      "document.getElementById('lux-status').textContent="
        "d.light<500?'🌑 Tối':d.light<5000?'🌤 Bình thường':'☀️ Sáng';"
      "const soil=d.soil;"
      "document.getElementById('soil-status').textContent="
        "soil<d.pump_on?'🔴 Khô, bơm đang bật':soil<d.pump_off?'🟡 Thấp':'🟢 Đủ ẩm';"

      // Fan
      "currentFanMode=d.fan_mode;"
      "document.getElementById('fan-speed').textContent=d.fan_speed;"
      "document.getElementById('fan-bar').style.width=d.fan_speed+'%';"
      "document.getElementById('fan-pwm').textContent=d.fan_pwm;"
      "document.getElementById('fan-sp').textContent=d.fan_setpoint.toFixed(1);"
      "if(document.activeElement.id!=='cfg-fan-sp') document.getElementById('cfg-fan-sp').value=d.fan_setpoint.toFixed(1);"
      "['btn-fan-off','btn-fan-auto','btn-fan-man'].forEach((id,i)=>{"
        "document.getElementById(id).className='btn'+(i===d.fan_mode?' btn-active':'');"
      "});"
      "document.getElementById('fan-slider-wrap').style.display=d.fan_mode===2?'block':'none';"
      "const fanModes=['⏹ OFF','⚡ AUTO','✋ MANUAL'];"
      "document.getElementById('fan-badge').textContent=fanModes[d.fan_mode];"
      "document.getElementById('fan-badge').className="
        "'badge '+(d.fan_mode===0?'badge-red':d.fan_mode===1?'badge-green':'badge-purple');"

      // Pump
      "const pump=d.pump_running;"
      "document.getElementById('pump-state').textContent=pump?'🔵 ĐANG CHẠY':'⭕ TẮT';"
      "document.getElementById('pump-state').style.color=pump?'#06b6d4':'#64748b';"
      "document.getElementById('pump-badge').textContent=pump?'ON':'OFF';"
      "document.getElementById('pump-badge').className='badge '+(pump?'badge-blue':'badge-red');"
      "document.getElementById('pump-mode').textContent=d.pump_manual?'Tay':'Tự động';"
      "document.getElementById('btn-pump-auto').className='btn'+(d.pump_manual?'':' btn-active');"
      "const rt=pump?Math.round((Date.now()/1000)-d.pump_start_s):0;"
      "document.getElementById('pump-time').textContent=pump?rt+'s':'--';"
      "document.getElementById('pump-on-val').textContent=d.pump_on;"
      "document.getElementById('pump-off-val').textContent=d.pump_off;"
      "if(document.activeElement.id!=='cfg-pump-on') document.getElementById('cfg-pump-on').value=d.pump_on;"
      "if(document.activeElement.id!=='cfg-pump-off') document.getElementById('cfg-pump-off').value=d.pump_off;"

      // System info
      "document.getElementById('records').textContent=d.total_records.toLocaleString()+' bản ghi';"
      "document.getElementById('flash').textContent=d.file_kb+' KB / '+d.flash_total_kb+' KB';"
      "document.getElementById('rssi').textContent=d.wifi_rssi+' dBm';"
      "const up=d.uptime;"
      "document.getElementById('uptime').textContent="
        "up<3600?Math.floor(up/60)+'m':Math.floor(up/3600)+'h'+Math.floor((up%3600)/60)+'m';"
      "document.getElementById('ntp').textContent=d.ntp_synced?'✓ Đã đồng bộ':'✗ Chưa sync';"
      "document.getElementById('ip-addr').textContent=d.ip;"

      "document.getElementById('ts').textContent="
        "'Cập nhật: '+new Date().toLocaleTimeString('vi-VN');"
    "}"

    "function setOffline(){"
      "document.getElementById('dot').className='status-dot dot-red';"
      "document.getElementById('conn-status').textContent='Mất kết nối';"
    "}"

    "async function setFan(mode){"
      "await fetch('/api/control?fan_mode='+mode);"
      "if(mode===2)document.getElementById('fan-slider-wrap').style.display='block';"
      "else document.getElementById('fan-slider-wrap').style.display='none';"
      "setTimeout(fetchStatus,300);"
    "}"

    "async function setFanSpeed(pwm){"
      "await fetch('/api/control?fan_speed='+pwm);"
      "setTimeout(fetchStatus,300);"
    "}"

    "async function setPump(action){"
      "await fetch('/api/control?pump='+action);"
      "setTimeout(fetchStatus,300);"
    "}"

    "async function saveConfig(){"
      "const fan=document.getElementById('cfg-fan-sp').value;"
      "const on=document.getElementById('cfg-pump-on').value;"
      "const off=document.getElementById('cfg-pump-off').value;"
      "await fetch('/api/control?setpoint='+fan+'&pump_on='+on+'&pump_off='+off);"
      "alert('Đã lưu cấu hình!');"
      "setTimeout(fetchStatus,300);"
    "}"
  );

  html += F(
    "async function fetchForecast(){"
      "try{"
        "const r=await fetch('/api/forecast');"
        "if(!r.ok)return;"
        "const d=await r.json();"
        "if(!d.ready){document.getElementById('fc-badge').textContent='Đang thu thập ('+d.samples+'/12 mẫu)';return;}"
        "document.getElementById('fc-badge').textContent='Đã cập nhật';"
        "document.getElementById('fc-badge').className='badge badge-green';"
        "for(let i=0;i<3;i++){"
          "document.getElementById('fc-t'+i).textContent=d.temp[i].toFixed(1);"
          "document.getElementById('fc-h'+i).textContent=d.humid[i].toFixed(1);"
          "document.getElementById('fc-l'+i).textContent=Math.round(d.lux[i]);"
        "}"
        "document.getElementById('fc-ts').textContent='Dự báo lúc: '+d.timestamp;"
      "}catch(e){}"
    "}"
  );

  html += F("fetchStatus();setInterval(fetchStatus,4000);"
            "fetchForecast();setInterval(fetchForecast,60000);");
  html += F("</script></body></html>");

  webServer.send(200, "text/html; charset=utf-8", html);
}

// ═══════════════════════════════════════════════════════════════
//  API: JSON TRẠNG THÁI ĐẦY ĐỦ
// ═══════════════════════════════════════════════════════════════
void handleApiStatus() {
  webServer.sendHeader("Access-Control-Allow-Origin", "*");

  char timeStr[25];
  getTimestamp(timeStr, sizeof(timeStr));

  StaticJsonDocument<512> doc;
  doc["timestamp"]       = timeStr;
  doc["temperature"]     = isnan(g_temperature)  ? 0 : round(g_temperature  * 10.0f) / 10.0f;
  doc["humidity"]        = isnan(g_humidity)     ? 0 : round(g_humidity     * 10.0f) / 10.0f;
  doc["light"]           = isnan(g_lightLux)     ? 0 : g_lightLux;
  doc["soil"]            = isnan(g_soilMoisture) ? 0 : round(g_soilMoisture * 10.0f) / 10.0f;
  doc["fan_mode"]        = g_fanMode;
  doc["fan_speed"]       = g_fanSpeed;
  doc["fan_pwm"]         = g_fanPWM;
  doc["fan_setpoint"]    = pidSetpoint;
  doc["fan_running"]     = g_fanRunning;
  doc["pump_running"]    = g_pumpRunning;
  doc["pump_manual"]     = g_pumpManual;
  doc["pump_start_s"]    = g_pumpRunning ? (millis() - g_pumpStart) / 1000 : 0;
  doc["sht30_ok"]        = g_sht30OK;
  doc["bh1750_ok"]       = g_bh1750OK;
  doc["wifi_rssi"]       = g_wifiOK ? WiFi.RSSI() : 0;
  doc["ip"]              = g_wifiOK ? WiFi.localIP().toString() : "0.0.0.0";
  doc["ntp_synced"]      = g_ntpSynced;
  doc["total_records"]   = g_totalRecords;
  doc["file_kb"]         = getCSVFileSize() / 1024;
  doc["flash_total_kb"]  = LittleFS.totalBytes() / 1024;
  doc["uptime"]          = millis() / 1000;

  String json;
  serializeJson(doc, json);
  webServer.send(200, "application/json", json);
}

// ═══════════════════════════════════════════════════════════════
//  API: ĐIỀU KHIỂN
// ═══════════════════════════════════════════════════════════════
void handleApiControl() {
  webServer.sendHeader("Access-Control-Allow-Origin", "*");
  String response = "{\"ok\":true,\"changes\":[";
  bool first = true;

  // Quạt: chế độ
  if (webServer.hasArg("fan_mode")) {
    int mode = webServer.arg("fan_mode").toInt();
    setFanMode(mode);
    if (!first) response += ",";
    response += "\"fan_mode=" + String(mode) + "\"";
    first = false;
  }

  // Quạt: tốc độ thủ công
  if (webServer.hasArg("fan_speed")) {
    int spd = webServer.arg("fan_speed").toInt();
    setFanManualSpeed(spd);
    if (!first) response += ",";
    response += "\"fan_speed=" + String(spd) + "\"";
    first = false;
  }

  // Quạt: setpoint PID
  bool configChanged = false;
  if (webServer.hasArg("setpoint")) {
    float sp = webServer.arg("setpoint").toFloat();
    setFanSetpoint(sp);
    configChanged = true;
    if (!first) response += ",";
    response += "\"setpoint=" + String(sp, 1) + "\"";
    first = false;
  }
  
  // Bơm: ngưỡng bật/tắt
  if (webServer.hasArg("pump_on")) {
    g_pumpOnThresh = webServer.arg("pump_on").toFloat();
    configChanged = true;
    if (!first) response += ",";
    response += "\"pump_on=" + String(g_pumpOnThresh, 0) + "\"";
    first = false;
  }
  if (webServer.hasArg("pump_off")) {
    g_pumpOffThresh = webServer.arg("pump_off").toFloat();
    configChanged = true;
    if (!first) response += ",";
    response += "\"pump_off=" + String(g_pumpOffThresh, 0) + "\"";
    first = false;
  }

  // Lưu cấu hình nếu có thay đổi
  if (configChanged) {
    saveConfig();
  }

  // Bơm: on/off/auto
  if (webServer.hasArg("pump")) {
    String pumpCmd = webServer.arg("pump");
    if (pumpCmd == "on") {
      setPumpManual(true);
      if (!first) response += ",";
      response += "\"pump=ON\"";
      first = false;
    } else if (pumpCmd == "off") {
      setPumpManual(false);
      if (!first) response += ",";
      response += "\"pump=OFF\"";
      first = false;
    } else if (pumpCmd == "auto") {
      setPumpAuto();
      if (!first) response += ",";
      response += "\"pump=AUTO\"";
      first = false;
    }
  }

  response += "]}";
  webServer.send(200, "application/json", response);
}

// ═══════════════════════════════════════════════════════════════
//  TẢI FILE CSV
// ═══════════════════════════════════════════════════════════════
void handleDownload() {
  if (!LittleFS.exists(CSV_FILE)) {
    webServer.send(404, "text/plain", "Chưa có dữ liệu nào!");
    return;
  }
  File f = LittleFS.open(CSV_FILE, "r");
  webServer.streamFile(f, "text/csv");
  f.close();
  Serial.println("[WEB] CSV downloaded by client");
}

// ═══════════════════════════════════════════════════════════════
//  API: 100 BẢN GHI GẦN NHẤT
// ═══════════════════════════════════════════════════════════════
void handleApiData() {
  webServer.sendHeader("Access-Control-Allow-Origin", "*");
  if (!LittleFS.exists(CSV_FILE)) {
    webServer.send(404, "application/json", "{\"error\":\"no data\"}");
    return;
  }

  File f = LittleFS.open(CSV_FILE, "r");
  String lines[100];
  int idx = 0, total = 0;

  f.readStringUntil('\n');  // Skip header
  while (f.available()) {
    String line = f.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      lines[idx % 100] = line;
      idx++;
      total++;
    }
  }
  f.close();

  String json = "{\"total\":" + String(total) + ",\"data\":[";
  int count = min(total, 100);
  int start = (idx > 100) ? idx - 100 : 0;

  for (int i = 0; i < count; i++) {
    int li = (start + i) % 100;
    if (lines[li].length() == 0) continue;

    // Parse: timestamp,temp,humid,light,soil,fan_mode,fan_speed,pump
    int c1 = lines[li].indexOf(',');
    int c2 = lines[li].indexOf(',', c1 + 1);
    int c3 = lines[li].indexOf(',', c2 + 1);
    int c4 = lines[li].indexOf(',', c3 + 1);

    if (c1 > 0 && c2 > 0 && c3 > 0) {
      if (i > 0) json += ",";
      json += "{\"t\":\"" + lines[li].substring(0, c1) + "\"";
      json += ",\"temp\":"  + lines[li].substring(c1 + 1, c2);
      json += ",\"humid\":" + lines[li].substring(c2 + 1, c3);
      json += ",\"light\":" + lines[li].substring(c3 + 1, c4 > 0 ? c4 : lines[li].length());
      json += "}";
    }
  }
  json += "]}";
  webServer.send(200, "application/json", json);
}

// ═══════════════════════════════════════════════════════════════
//  API: THỐNG KÊ BỘ NHỚ
// ═══════════════════════════════════════════════════════════════
void handleApiStats() {
  webServer.sendHeader("Access-Control-Allow-Origin", "*");
  size_t total  = LittleFS.totalBytes();
  size_t used   = LittleFS.usedBytes();
  size_t fileSz = getCSVFileSize();

  String json = "{";
  json += "\"records\":"       + String(g_totalRecords);
  json += ",\"file_size_kb\":" + String(fileSz / 1024);
  json += ",\"flash_total_kb\":"+ String(total / 1024);
  json += ",\"flash_used_kb\":" + String(used / 1024);
  json += ",\"flash_free_kb\":" + String((total - used) / 1024);
  json += ",\"days\":"          + String(g_totalRecords * 30.0 / 86400.0, 2);
  json += ",\"ip\":\""          + (g_wifiOK ? WiFi.localIP().toString() : String("offline")) + "\"";
  json += ",\"uptime\":"        + String(millis() / 1000);
  json += "}";

  webServer.send(200, "application/json", json);
}

// ═══════════════════════════════════════════════════════════════
//  XÓA DỮ LIỆU CSV
// ═══════════════════════════════════════════════════════════════
void handleDelete() {
  LittleFS.remove(CSV_FILE);
  File f = LittleFS.open(CSV_FILE, "w");
  if (f) {
    f.println(F("timestamp,temperature,humidity,light,soil,fan_mode,fan_speed,pump"));
    f.close();
  }
  g_totalRecords = 0;
  Serial.println("[WEB] Dữ liệu CSV đã bị xóa bởi người dùng");
  webServer.sendHeader("Location", "/");
  webServer.send(302, "text/plain", "Redirecting...");
}

// ═══════════════════════════════════════════════════════════════
//  API: DỰ BÁO THỜI TIẾT
// ═══════════════════════════════════════════════════════════════
void handleApiForecast() {
  webServer.sendHeader("Access-Control-Allow-Origin", "*");

  char timeStr[25];
  getTimestamp(timeStr, sizeof(timeStr));

  if (!g_forecast_ready) {
    // Chưa đủ dữ liệu — trả về trạng thái thu thập
    String json = "{\"ready\":false,\"samples\":" + String(fc_buf_count)
                + ",\"needed\":12"
                + ",\"timestamp\":\"" + String(timeStr) + "\"}";
    webServer.send(200, "application/json", json);
    return;
  }

  // Dự báo sẵn sàng
  StaticJsonDocument<384> doc;
  doc["ready"]     = true;
  doc["timestamp"] = timeStr;

  JsonArray arrT = doc.createNestedArray("temp");
  JsonArray arrH = doc.createNestedArray("humid");
  JsonArray arrL = doc.createNestedArray("lux");
  JsonArray arrM = doc.createNestedArray("min");
  const int labels[] = {30, 60, 90};
  for (int h = 0; h < FC_HORIZONS; h++) {
    arrT.add(isnan(g_fc_temp[h])  ? 0.0f : round(g_fc_temp[h]  * 10.0f) / 10.0f);
    arrH.add(isnan(g_fc_humid[h]) ? 0.0f : round(g_fc_humid[h] * 10.0f) / 10.0f);
    arrL.add(isnan(g_fc_lux[h])   ? 0.0f : (float)((int)g_fc_lux[h]));
    arrM.add(labels[h]);
  }

  String json;
  serializeJson(doc, json);
  webServer.send(200, "application/json", json);
}

