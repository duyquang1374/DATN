"""
============================================================
 HỆ THỐNG NHÀ MÀNG THÔNG MINH - TIỀN XỬ LÝ DỮ LIỆU
============================================================
 Đọc dữ liệu từ SQLite, chuẩn hóa, tạo sequences cho LSTM.
============================================================
"""

import numpy as np
import pandas as pd
import os
import sys
import logging
import joblib
from sklearn.preprocessing import MinMaxScaler

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "Server"))
from config import DB_PATH, MODEL_DIR, SEQUENCE_LENGTH, PREDICTION_STEPS

logger = logging.getLogger(__name__)


def load_data_from_db():
    """Đọc dữ liệu từ SQLite database."""
    import sqlite3
    conn = sqlite3.connect(DB_PATH)
    df = pd.read_sql_query(
        "SELECT timestamp, temperature, humidity FROM sensor_data ORDER BY timestamp ASC",
        conn
    )
    conn.close()

    if df.empty:
        raise ValueError("Database trống! Cần thu thập dữ liệu trước.")

    df["timestamp"] = pd.to_datetime(df["timestamp"])
    df.set_index("timestamp", inplace=True)

    logger.info(f"Loaded {len(df)} records from database")
    logger.info(f"Time range: {df.index.min()} → {df.index.max()}")
    logger.info(f"Temp range: {df['temperature'].min():.1f} - {df['temperature'].max():.1f}°C")
    logger.info(f"Humidity range: {df['humidity'].min():.1f} - {df['humidity'].max():.1f}%")

    return df


def clean_data(df):
    """Xử lý dữ liệu thiếu và bất thường."""
    original_len = len(df)

    # Loại bỏ duplicates
    df = df[~df.index.duplicated(keep="first")]

    # Xử lý missing values bằng interpolation
    df = df.interpolate(method="time")
    df = df.dropna()

    cleaned = original_len - len(df)
    if cleaned > 0:
        logger.info(f"Cleaned {cleaned} records (duplicates/NaN)")

    return df


def add_time_features(df):
    """
    Thêm features thời gian để giúp model học pattern ngày/đêm.
    """
    df = df.copy()
    df["hour_sin"] = np.sin(2 * np.pi * df.index.hour / 24)
    df["hour_cos"] = np.cos(2 * np.pi * df.index.hour / 24)
    return df


def scale_data(df, fit_scaler=True, scaler_path=None):
    """
    Chuẩn hóa dữ liệu về khoảng [0, 1].

    Args:
        df: DataFrame với columns [temperature, humidity, ...]
        fit_scaler: True nếu đang training (fit + transform), False nếu predict (chỉ transform)
        scaler_path: Đường dẫn lưu/load scaler

    Returns:
        scaled_data: numpy array đã chuẩn hóa
        scaler: fitted MinMaxScaler
    """
    if scaler_path is None:
        scaler_path = os.path.join(MODEL_DIR, "scaler.pkl")

    features = df.values  # numpy array

    if fit_scaler:
        scaler = MinMaxScaler(feature_range=(0, 1))
        scaled_data = scaler.fit_transform(features)
        # Lưu scaler
        joblib.dump(scaler, scaler_path)
        logger.info(f"Scaler saved to {scaler_path}")
    else:
        scaler = joblib.load(scaler_path)
        scaled_data = scaler.transform(features)
        logger.info(f"Scaler loaded from {scaler_path}")

    return scaled_data, scaler


def create_sequences(data, seq_length, pred_steps=1):
    """
    Tạo sequences cho LSTM (sliding window).

    Args:
        data: numpy array đã chuẩn hóa, shape (n_samples, n_features)
        seq_length: Số bước thời gian đầu vào (lookback)
        pred_steps: Số bước dự đoán phía trước

    Returns:
        X: Input sequences, shape (n_sequences, seq_length, n_features)
        y: Target values, shape (n_sequences,)
    """
    X, y = [], []

    for i in range(len(data) - seq_length - pred_steps + 1):
        X.append(data[i : i + seq_length])
        # Dự đoán nhiệt độ (column 0) tại thời điểm seq_length + pred_steps
        y.append(data[i + seq_length + pred_steps - 1, 0])

    X = np.array(X)
    y = np.array(y)

    logger.info(f"Created {len(X)} sequences | X: {X.shape} | y: {y.shape}")
    return X, y


def split_data(X, y, train_ratio=0.8):
    """
    Chia train/test theo thời gian (KHÔNG shuffle).
    """
    split_idx = int(len(X) * train_ratio)

    X_train = X[:split_idx]
    X_test = X[split_idx:]
    y_train = y[:split_idx]
    y_test = y[split_idx:]

    logger.info(f"Train: {len(X_train)} | Test: {len(X_test)} | Ratio: {train_ratio}")
    return X_train, X_test, y_train, y_test


def prepare_training_data(seq_length=None, pred_steps=None, use_time_features=True):
    """
    Pipeline hoàn chỉnh: Load → Clean → Feature → Scale → Sequence → Split.

    Returns:
        X_train, X_test, y_train, y_test, scaler, feature_columns
    """
    if seq_length is None:
        seq_length = min(SEQUENCE_LENGTH, 720)  # Mặc định 6h nếu SEQUENCE_LENGTH quá lớn
    if pred_steps is None:
        pred_steps = min(PREDICTION_STEPS, 120)  # Mặc định 1h

    # Load & clean
    df = load_data_from_db()
    df = clean_data(df)

    # Thêm time features
    if use_time_features:
        df = add_time_features(df)

    feature_columns = df.columns.tolist()
    logger.info(f"Features: {feature_columns}")

    # Scale
    scaled_data, scaler = scale_data(df, fit_scaler=True)

    # Tạo sequences
    X, y = create_sequences(scaled_data, seq_length, pred_steps)

    # Split
    X_train, X_test, y_train, y_test = split_data(X, y)

    return X_train, X_test, y_train, y_test, scaler, feature_columns


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")

    print("=== Data Preprocessing Test ===")
    try:
        X_train, X_test, y_train, y_test, scaler, cols = prepare_training_data(
            seq_length=120,  # 1h lookback (cho test nhanh)
            pred_steps=60    # 30 phút dự đoán
        )
        print(f"Features: {cols}")
        print(f"X_train: {X_train.shape}")
        print(f"X_test:  {X_test.shape}")
        print(f"y_train: {y_train.shape}")
        print(f"y_test:  {y_test.shape}")
    except ValueError as e:
        print(f"Error: {e}")
    print("=== Test Complete ===")
