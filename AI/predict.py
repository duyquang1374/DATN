"""
============================================================
 HỆ THỐNG NHÀ MÀNG THÔNG MINH - DỰ ĐOÁN NHIỆT ĐỘ
============================================================
 Load model đã train, dự đoán nhiệt độ tương lai.
 Chạy: python predict.py
============================================================
"""

import os
import sys
import json
import logging
import numpy as np
import pandas as pd
import joblib

os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"
from tensorflow.keras.models import load_model

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "Server"))
from config import MODEL_DIR, DB_PATH
from database import get_latest_data

logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")
logger = logging.getLogger(__name__)


def load_trained_model():
    """Load model và scaler đã train."""
    model_path = os.path.join(MODEL_DIR, "lstm_greenhouse.h5")
    scaler_path = os.path.join(MODEL_DIR, "scaler.pkl")
    meta_path = os.path.join(MODEL_DIR, "model_metadata.json")

    if not os.path.exists(model_path):
        raise FileNotFoundError(f"Chưa có model! Chạy train_model.py trước.\n  → {model_path}")

    model = load_model(model_path)
    scaler = joblib.load(scaler_path)

    with open(meta_path, "r") as f:
        metadata = json.load(f)

    logger.info(f"Model loaded (trained: {metadata['trained_at']})")
    logger.info(f"Metrics: RMSE={metadata['metrics']['rmse']:.4f}, MAE={metadata['metrics']['mae']:.4f}")

    return model, scaler, metadata


def prepare_input(scaler, metadata, hours=24):
    """
    Lấy dữ liệu gần nhất và chuẩn bị input cho model.
    """
    lookback = metadata["lookback"]
    feature_cols = metadata["features"]
    use_time = "hour_sin" in feature_cols

    # Lấy dữ liệu từ DB
    data = get_latest_data(hours=hours)
    if not data:
        raise ValueError("Không có dữ liệu trong database!")

    df = pd.DataFrame(data)
    df["timestamp"] = pd.to_datetime(df["timestamp"])
    df.set_index("timestamp", inplace=True)

    # Thêm time features nếu model yêu cầu
    if use_time:
        df["hour_sin"] = np.sin(2 * np.pi * df.index.hour / 24)
        df["hour_cos"] = np.cos(2 * np.pi * df.index.hour / 24)

    # Đảm bảo đúng thứ tự features
    base_cols = [c for c in feature_cols if c in df.columns]
    df = df[base_cols]

    if len(df) < lookback:
        raise ValueError(
            f"Cần ít nhất {lookback} samples ({lookback * 30 / 3600:.1f}h), "
            f"hiện chỉ có {len(df)} samples."
        )

    # Scale
    scaled = scaler.transform(df.values)

    # Lấy lookback samples cuối cùng
    sequence = scaled[-lookback:]
    X = np.array([sequence])  # shape: (1, lookback, n_features)

    return X, df


def predict_temperature():
    """
    Dự đoán nhiệt độ và trả về kết quả.

    Returns:
        dict: Kết quả dự đoán
    """
    # Load model
    model, scaler, metadata = load_trained_model()
    pred_steps = metadata["pred_steps"]

    # Chuẩn bị input
    X, df = prepare_input(scaler, metadata)

    # Dự đoán
    prediction_scaled = model.predict(X, verbose=0)[0][0]

    # Inverse scale (chỉ cột temperature)
    n_features = metadata["n_features"]
    dummy = np.zeros((1, n_features))
    dummy[0, 0] = prediction_scaled
    predicted_temp = scaler.inverse_transform(dummy)[0, 0]

    # Thông tin hiện tại
    current_temp = df["temperature"].iloc[-1]
    current_humidity = df["humidity"].iloc[-1]
    current_time = df.index[-1]

    # Thời gian dự đoán
    pred_minutes = pred_steps * 30 / 60  # 30s mỗi sample
    from datetime import timedelta
    target_time = current_time + timedelta(minutes=pred_minutes)

    result = {
        "current": {
            "temperature": round(float(current_temp), 1),
            "humidity": round(float(current_humidity), 1),
            "time": current_time.strftime("%Y-%m-%d %H:%M:%S")
        },
        "prediction": {
            "temperature": round(float(predicted_temp), 1),
            "target_time": target_time.strftime("%Y-%m-%d %H:%M:%S"),
            "prediction_horizon": f"{pred_minutes:.0f} phút"
        },
        "change": round(float(predicted_temp - current_temp), 1),
        "model_rmse": round(metadata["metrics"]["rmse"], 4)
    }

    return result


def main():
    print("╔══════════════════════════════════════════╗")
    print("║  NHÀ MÀNG THÔNG MINH - DỰ ĐOÁN NHIỆT ĐỘ ║")
    print("╚══════════════════════════════════════════╝")

    try:
        result = predict_temperature()

        print(f"\n  📍 Hiện tại ({result['current']['time']}):")
        print(f"     Nhiệt độ: {result['current']['temperature']}°C")
        print(f"     Độ ẩm:    {result['current']['humidity']}%")
        print()
        print(f"  🔮 Dự đoán ({result['prediction']['target_time']}):")
        print(f"     Nhiệt độ: {result['prediction']['temperature']}°C")
        print(f"     Thay đổi: {result['change']:+.1f}°C")
        print(f"     Horizon:  {result['prediction']['prediction_horizon']}")
        print()
        print(f"  📊 Model RMSE: {result['model_rmse']}")

        # Cảnh báo
        if result["prediction"]["temperature"] > 35:
            print("\n  ⚠️  CẢNH BÁO: Nhiệt độ dự đoán CAO! Cần bật quạt/tưới nước!")
        elif result["prediction"]["temperature"] < 15:
            print("\n  ⚠️  CẢNH BÁO: Nhiệt độ dự đoán THẤP! Cần đóng cửa nhà màng!")

    except FileNotFoundError as e:
        print(f"\n  ❌ {e}")
    except ValueError as e:
        print(f"\n  ❌ {e}")


if __name__ == "__main__":
    main()
