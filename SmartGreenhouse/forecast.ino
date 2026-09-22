/*
 * forecast.ino
 * ============================================================
 *  DỰ BÁO THỜI TIẾT NHÀ MÀNG – ML INFERENCE TRÊN ESP32
 * ============================================================
 *  Mô hình: ExtraTreesRegressor (emlearn, dtype=float)
 *  Features: 27 đặc trưng (lag, diff, rolling stats, hour encoding)
 *  Horizon : 3 mốc – +30 phút / +60 phút / +90 phút
 *             (rolling forecast: mỗi bước gọi lại model)
 *
 *  Phụ thuộc:
 *    - forecast_temperature.h / forecast_humidity.h / forecast_log_light.h
 *    - g_temperature, g_humidity, g_lightLux (từ sensors.ino)
 *    - g_ntpSynced (từ SmartGreenhouse.ino)
 * ============================================================
 */

#include "forecast_temperature.h"
#include "forecast_humidity.h"
#include "forecast_log_light.h"
// forecast.h KHÔNG include ở đây — file này là nơi ĐỊNH NGHĨA các biến extern
// (forecast.h chỉ dùng cho SmartGreenhouse.ino và web_server_local.ino)

#include <math.h>

// ─────────────────────────────────────────────────────────────
//  RING BUFFER lịch sử (đọc mỗi FORECAST_INTERVAL = 5 phút)
// ─────────────────────────────────────────────────────────────
static float fc_buf_temp[FC_HISTORY];
static float fc_buf_humid[FC_HISTORY];
static float fc_buf_logL[FC_HISTORY];   // log10(lux + 1)
static int   fc_buf_head  = 0;          // Vị trí ghi tiếp theo
int          fc_buf_count = 0;          // Số mẫu hiện có (non-static: dùng trong web_server)

// ─────────────────────────────────────────────────────────────
//  KẾT QUẢ DỰ BÁO (global – dùng trong web_server + MQTT)
// ─────────────────────────────────────────────────────────────
float g_fc_temp[FC_HORIZONS]  = {NAN, NAN, NAN};  // °C
float g_fc_humid[FC_HORIZONS] = {NAN, NAN, NAN};  // %
float g_fc_lux[FC_HORIZONS]   = {NAN, NAN, NAN};  // lux (đã convert)
bool  g_forecast_ready        = false;
unsigned long g_lastForecastTime = 0;

// ─────────────────────────────────────────────────────────────
//  Lấy giá trị từ ring buffer: idx=0 → mẫu mới nhất
// ─────────────────────────────────────────────────────────────
static inline float fcGetTemp(int idx) {
  return fc_buf_temp[(fc_buf_head - 1 - idx + FC_HISTORY) % FC_HISTORY];
}
static inline float fcGetHumid(int idx) {
  return fc_buf_humid[(fc_buf_head - 1 - idx + FC_HISTORY) % FC_HISTORY];
}
static inline float fcGetLogL(int idx) {
  return fc_buf_logL[(fc_buf_head - 1 - idx + FC_HISTORY) % FC_HISTORY];
}

// ─────────────────────────────────────────────────────────────
//  XÂY DỰNG VECTOR 27 FEATURES TỪ SNAPSHOT MẢNG
//  snapshot: bt[0]=hiện tại, bt[1]=5 phút trước, ...
// ─────────────────────────────────────────────────────────────
static void buildFeatures(const float* bt, const float* bh, const float* bl,
                           float feats[FC_N_FEATURES],
                           float hourSin, float hourCos)
{
  // Temperature
  feats[0]  = bt[0];
  feats[1]  = bt[0] - bt[1];
  feats[2]  = bt[0] - bt[2];
  feats[3]  = bt[0] - bt[3];
  feats[4]  = bt[0] - bt[6];
  feats[5]  = bt[0] - bt[11];
  float tM = 0;
  for (int i = 0; i < 6; i++) tM += bt[i];  tM /= 6.0f;
  float tS = 0;
  for (int i = 0; i < 6; i++) { float d = bt[i]-tM; tS += d*d; }
  feats[6]  = tM;
  feats[7]  = sqrtf(tS / 6.0f);

  // Humidity
  feats[8]  = bh[0];
  feats[9]  = bh[0] - bh[1];
  feats[10] = bh[0] - bh[2];
  feats[11] = bh[0] - bh[3];
  feats[12] = bh[0] - bh[6];
  feats[13] = bh[0] - bh[11];
  float hM = 0;
  for (int i = 0; i < 6; i++) hM += bh[i];  hM /= 6.0f;
  float hS = 0;
  for (int i = 0; i < 6; i++) { float d = bh[i]-hM; hS += d*d; }
  feats[14] = hM;
  feats[15] = sqrtf(hS / 6.0f);

  // Log-light
  feats[16] = bl[0];
  feats[17] = bl[0] - bl[1];
  feats[18] = bl[0] - bl[2];
  feats[19] = bl[0] - bl[3];
  feats[20] = bl[0] - bl[6];
  feats[21] = bl[0] - bl[11];
  float lM = 0;
  for (int i = 0; i < 6; i++) lM += bl[i];  lM /= 6.0f;
  float lS = 0;
  for (int i = 0; i < 6; i++) { float d = bl[i]-lM; lS += d*d; }
  feats[22] = lM;
  feats[23] = sqrtf(lS / 6.0f);

  // light_sat6: tỉ lệ mẫu có ánh sáng trong 6 bước gần nhất
  float sat = 0;
  for (int i = 0; i < 6; i++) sat += (bl[i] > 0.01f) ? 1.0f : 0.0f;
  feats[24] = sat / 6.0f;

  // Hour encoding
  feats[25] = hourSin;
  feats[26] = hourCos;
}

// ─────────────────────────────────────────────────────────────
//  ROLLING FORECAST: dự báo targetStep × 5 phút tới
//  Mỗi bước dùng đầu ra của bước trước làm input (shift snapshot)
// ─────────────────────────────────────────────────────────────
static void rollingForecast(int targetStep,
                             float* outTemp, float* outHumid, float* outLogL,
                             float initHourSin, float initHourCos)
{
  // Sao chép ring buffer vào mảng tạm (bt[0] = mới nhất)
  float bt[FC_HISTORY], bh[FC_HISTORY], bl[FC_HISTORY];
  for (int i = 0; i < FC_HISTORY; i++) {
    bt[i] = fcGetTemp(i);
    bh[i] = fcGetHumid(i);
    bl[i] = fcGetLogL(i);
  }

  float hourSin = initHourSin;
  float hourCos = initHourCos;
  // Bước tiến góc 5 phút trên vòng 24 giờ = 2π/288 rad
  float angleStep = 2.0f * (float)M_PI / 288.0f;
  float stepCos = cosf(angleStep);
  float stepSin = sinf(angleStep);

  for (int step = 0; step < targetStep; step++) {
    float feats[FC_N_FEATURES];
    buildFeatures(bt, bh, bl, feats, hourSin, hourCos);

    // Đầu ra model là delta so với giá trị hiện tại (bt[0])
    float dT = forecast_temperature_predict(feats, FC_N_FEATURES);
    float dH = forecast_humidity_predict(feats, FC_N_FEATURES);
    float dL = forecast_log_light_predict(feats, FC_N_FEATURES);

    float predT = constrain(bt[0] + dT, (float)TEMP_MIN,  (float)TEMP_MAX);
    float predH = constrain(bh[0] + dH, (float)HUMID_MIN, (float)HUMID_MAX);
    float predL = constrain(bl[0] + dL, 0.0f, 6.0f);

    // Shift snapshot: đẩy dự báo vào đầu
    for (int i = FC_HISTORY - 1; i > 0; i--) {
      bt[i] = bt[i-1];
      bh[i] = bh[i-1];
      bl[i] = bl[i-1];
    }
    bt[0] = predT;
    bh[0] = predH;
    bl[0] = predL;

    // Tiến giờ 5 phút (xoay vector sin/cos)
    float newSin = hourSin * stepCos + hourCos * stepSin;
    float newCos = hourCos * stepCos - hourSin * stepSin;
    hourSin = newSin;
    hourCos = newCos;
  }

  *outTemp  = bt[0];
  *outHumid = bh[0];
  *outLogL  = bl[0];
}

// ─────────────────────────────────────────────────────────────
//  INIT: Gọi từ setup()
// ─────────────────────────────────────────────────────────────
void initForecast() {
  fc_buf_head  = 0;
  fc_buf_count = 0;
  g_forecast_ready = false;
  for (int h = 0; h < FC_HORIZONS; h++) {
    g_fc_temp[h]  = NAN;
    g_fc_humid[h] = NAN;
    g_fc_lux[h]   = NAN;
  }
  Serial.println(F("[FORECAST] Init OK — cần 60 phút thu thập dữ liệu"));
}

// ─────────────────────────────────────────────────────────────
//  UPDATE: Gọi từ loop() mỗi FORECAST_INTERVAL (5 phút)
// ─────────────────────────────────────────────────────────────
void updateForecast() {
  // Bỏ qua nếu cảm biến lỗi
  if (!g_sht30OK || !g_bh1750OK ||
      isnan(g_temperature) || isnan(g_humidity) || isnan(g_lightLux)) {
    Serial.println(F("[FORECAST] Bỏ qua: cảm biến chưa sẵn sàng"));
    return;
  }

  // Đẩy mẫu mới vào ring buffer
  float logL = log10f(g_lightLux + 1.0f);
  fc_buf_temp[fc_buf_head]  = g_temperature;
  fc_buf_humid[fc_buf_head] = g_humidity;
  fc_buf_logL[fc_buf_head]  = logL;
  fc_buf_head = (fc_buf_head + 1) % FC_HISTORY;
  if (fc_buf_count < FC_HISTORY) fc_buf_count++;

  // Chưa đủ lịch sử
  if (fc_buf_count < FC_HISTORY) {
    Serial.printf("[FORECAST] Thu thập: %d/%d mẫu (còn %d phút)\n",
                  fc_buf_count, FC_HISTORY, (FC_HISTORY - fc_buf_count) * 5);
    return;
  }

  // Tính hour_sin/cos
  float hourSin = 0.0f, hourCos = 1.0f;
  struct tm tmNow;
  if (g_ntpSynced && getLocalTime(&tmNow)) {
    float hour  = tmNow.tm_hour + tmNow.tm_min / 60.0f;
    float angle = hour * 2.0f * (float)M_PI / 24.0f;
    hourSin = sinf(angle);
    hourCos = cosf(angle);
  }

  // Rolling forecast cho 3 horizons: 6 / 12 / 18 bước × 5 phút
  static const int steps[FC_HORIZONS]  = {6, 12, 18};
  static const int labels[FC_HORIZONS] = {30, 60, 90};

  Serial.println(F("[FORECAST] ──── Dự báo ────"));
  for (int h = 0; h < FC_HORIZONS; h++) {
    float pT, pH, pL;
    rollingForecast(steps[h], &pT, &pH, &pL, hourSin, hourCos);
    g_fc_temp[h]  = pT;
    g_fc_humid[h] = pH;
    g_fc_lux[h]   = powf(10.0f, pL) - 1.0f;
    Serial.printf("[FORECAST] +%d min → T=%.1f°C  H=%.1f%%  Lux=%.0f\n",
                  labels[h], pT, pH, g_fc_lux[h]);
  }
  g_forecast_ready = true;
  Serial.println(F("[FORECAST] ────────────────"));
}
