"""
============================================================
 HỆ THỐNG NHÀ MÀNG THÔNG MINH - CẤU HÌNH SERVER
============================================================
"""

import os

# ======================== PATHS ========================

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA_DIR = os.path.join(BASE_DIR, "Data")
DB_PATH = os.path.join(DATA_DIR, "greenhouse.db")
MODEL_DIR = os.path.join(BASE_DIR, "AI", "model")

# Tạo thư mục nếu chưa có
os.makedirs(DATA_DIR, exist_ok=True)
os.makedirs(MODEL_DIR, exist_ok=True)

# ======================== MQTT ========================

MQTT_BROKER = "localhost"       # Địa chỉ Mosquitto broker
MQTT_PORT = 1883
MQTT_TOPIC_SENSOR = "greenhouse/sensor"
MQTT_TOPIC_STATUS = "greenhouse/status"
MQTT_TOPIC_PREDICT = "greenhouse/predict"
MQTT_CLIENT_ID = "PythonServer_Greenhouse"

# Nếu broker có auth, uncomment:
# MQTT_USERNAME = "username"
# MQTT_PASSWORD = "password"

# ======================== SENSOR VALIDATION ========================

TEMP_MIN = -10.0    # °C - Nhiệt độ tối thiểu hợp lệ
TEMP_MAX = 60.0     # °C - Nhiệt độ tối đa hợp lệ
HUMIDITY_MIN = 0.0  # % - Độ ẩm tối thiểu
HUMIDITY_MAX = 100.0  # % - Độ ẩm tối đa

# ======================== AI MODEL ========================

# Sliding window
SEQUENCE_LENGTH = 2880    # 24h dữ liệu (30s/sample × 2 × 60 × 24)
PREDICTION_STEPS = 120    # Dự đoán 1h tới (30s/sample × 2 × 60)

# Khoảng thời gian thu thập tối thiểu trước khi train (giờ)
MIN_HOURS_FOR_TRAINING = 72  # 3 ngày

# ======================== LOGGING ========================

LOG_FORMAT = "%(asctime)s [%(levelname)s] %(message)s"
LOG_DATE_FORMAT = "%Y-%m-%d %H:%M:%S"
