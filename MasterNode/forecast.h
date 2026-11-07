#pragma once
/*
 * forecast.h
 * Defines + forward declarations cho forecast.ino
 * Include trong SmartGreenhouse.ino, web_server_local.ino
 * KHÔNG include trong forecast.ino (nơi định nghĩa biến)
 */

// ─── Cấu hình (cũng được dùng trong forecast.ino qua #define trực tiếp)
#define FC_HISTORY      12   // 12 mẫu × 5 phút = 60 phút lịch sử
#define FC_HORIZONS      3   // +30 min / +60 min / +90 min
#define FC_N_FEATURES   27   // Phải khớp với features.json

// ─── Forward declarations biến global (định nghĩa trong forecast.ino)
extern float g_fc_temp[FC_HORIZONS];
extern float g_fc_humid[FC_HORIZONS];
extern float g_fc_lux[FC_HORIZONS];
extern bool  g_forecast_ready;
extern unsigned long g_lastForecastTime;
extern int   fc_buf_count;

// ─── Forward declarations hàm
void initForecast();
void updateForecast();
