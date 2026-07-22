"""
============================================================
 HỆ THỐNG NHÀ MÀNG THÔNG MINH - TRAINING LSTM MODEL
============================================================
 Train mô hình LSTM dự đoán nhiệt độ nhà màng.
 Chạy: python train_model.py
============================================================
"""

import os
import sys
import json
import logging
import numpy as np
import matplotlib
matplotlib.use("Agg")  # Non-interactive backend
import matplotlib.pyplot as plt
from datetime import datetime

os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"  # Suppress TF warnings

import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense, Dropout, Input
from tensorflow.keras.callbacks import EarlyStopping, ModelCheckpoint, ReduceLROnPlateau
from sklearn.metrics import mean_squared_error, mean_absolute_error

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from data_preprocessing import prepare_training_data

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "Server"))
from config import MODEL_DIR

logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")
logger = logging.getLogger(__name__)

# ======================== HYPERPARAMETERS ========================

LOOKBACK = 720        # 6 giờ lookback (30s/sample × 120/h × 6h)
PRED_STEPS = 120      # Dự đoán 1 giờ tới
EPOCHS = 100          # Số epoch tối đa (EarlyStopping sẽ dừng sớm)
BATCH_SIZE = 32
LEARNING_RATE = 0.001
PATIENCE = 10         # Dừng nếu val_loss không cải thiện sau N epoch


def build_model(input_shape):
    """
    Xây dựng mô hình LSTM.
    
    Architecture:
        Input → LSTM(64) → Dropout(0.2) → LSTM(32) → Dropout(0.2) → Dense(16) → Dense(1)
    """
    model = Sequential([
        Input(shape=input_shape),
        LSTM(64, return_sequences=True),
        Dropout(0.2),
        LSTM(32, return_sequences=False),
        Dropout(0.2),
        Dense(16, activation="relu"),
        Dense(1)
    ])

    model.compile(
        optimizer=tf.keras.optimizers.Adam(learning_rate=LEARNING_RATE),
        loss="mse",
        metrics=["mae"]
    )

    return model


def plot_training_history(history, save_path):
    """Vẽ biểu đồ training loss."""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

    # Loss
    ax1.plot(history.history["loss"], label="Train Loss", color="#2196F3")
    ax1.plot(history.history["val_loss"], label="Val Loss", color="#FF5722")
    ax1.set_title("Model Loss (MSE)", fontsize=14)
    ax1.set_xlabel("Epoch")
    ax1.set_ylabel("Loss")
    ax1.legend()
    ax1.grid(True, alpha=0.3)

    # MAE
    ax2.plot(history.history["mae"], label="Train MAE", color="#4CAF50")
    ax2.plot(history.history["val_mae"], label="Val MAE", color="#FF9800")
    ax2.set_title("Model MAE", fontsize=14)
    ax2.set_xlabel("Epoch")
    ax2.set_ylabel("MAE")
    ax2.legend()
    ax2.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(save_path, dpi=150)
    plt.close()
    logger.info(f"Training history plot saved: {save_path}")


def plot_predictions(y_true, y_pred, save_path):
    """Vẽ biểu đồ so sánh dự đoán vs thực tế."""
    fig, ax = plt.subplots(figsize=(14, 6))

    samples = min(500, len(y_true))  # Chỉ plot 500 điểm cuối
    ax.plot(y_true[-samples:], label="Thực tế", color="#2196F3", linewidth=1.5)
    ax.plot(y_pred[-samples:], label="Dự đoán", color="#FF5722", linewidth=1.5, alpha=0.8)

    ax.set_title("Nhiệt Độ: Dự Đoán vs Thực Tế", fontsize=14)
    ax.set_xlabel("Sample")
    ax.set_ylabel("Nhiệt độ (chuẩn hóa)")
    ax.legend()
    ax.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(save_path, dpi=150)
    plt.close()
    logger.info(f"Predictions plot saved: {save_path}")


def train():
    """Main training pipeline."""
    print("╔══════════════════════════════════════════╗")
    print("║    NHÀ MÀNG THÔNG MINH - LSTM TRAINING   ║")
    print("╚══════════════════════════════════════════╝")

    # 1. Chuẩn bị dữ liệu
    logger.info("Step 1: Chuẩn bị dữ liệu...")
    X_train, X_test, y_train, y_test, scaler, feature_cols = prepare_training_data(
        seq_length=LOOKBACK,
        pred_steps=PRED_STEPS,
        use_time_features=True
    )

    n_features = X_train.shape[2]
    logger.info(f"Input shape: ({LOOKBACK}, {n_features})")
    logger.info(f"Features: {feature_cols}")

    # 2. Xây dựng model
    logger.info("Step 2: Xây dựng mô hình LSTM...")
    model = build_model(input_shape=(LOOKBACK, n_features))
    model.summary(print_fn=logger.info)

    # 3. Callbacks
    model_path = os.path.join(MODEL_DIR, "lstm_greenhouse.h5")
    callbacks = [
        EarlyStopping(
            monitor="val_loss",
            patience=PATIENCE,
            restore_best_weights=True,
            verbose=1
        ),
        ModelCheckpoint(
            model_path,
            monitor="val_loss",
            save_best_only=True,
            verbose=1
        ),
        ReduceLROnPlateau(
            monitor="val_loss",
            factor=0.5,
            patience=5,
            min_lr=1e-6,
            verbose=1
        )
    ]

    # 4. Training
    logger.info(f"Step 3: Training... (max {EPOCHS} epochs, patience={PATIENCE})")
    history = model.fit(
        X_train, y_train,
        epochs=EPOCHS,
        batch_size=BATCH_SIZE,
        validation_data=(X_test, y_test),
        callbacks=callbacks,
        verbose=1
    )

    # 5. Đánh giá
    logger.info("Step 4: Đánh giá model...")
    y_pred = model.predict(X_test, verbose=0).flatten()

    mse = mean_squared_error(y_test, y_pred)
    rmse = np.sqrt(mse)
    mae = mean_absolute_error(y_test, y_pred)

    logger.info("═══ KẾT QUẢ ĐÁNH GIÁ ═══")
    logger.info(f"  MSE:  {mse:.6f}")
    logger.info(f"  RMSE: {rmse:.6f}")
    logger.info(f"  MAE:  {mae:.6f}")

    # 6. Lưu plots
    plot_dir = os.path.join(MODEL_DIR, "plots")
    os.makedirs(plot_dir, exist_ok=True)

    plot_training_history(history, os.path.join(plot_dir, "training_history.png"))
    plot_predictions(y_test, y_pred, os.path.join(plot_dir, "predictions.png"))

    # 7. Lưu metadata
    metadata = {
        "trained_at": datetime.now().isoformat(),
        "lookback": LOOKBACK,
        "pred_steps": PRED_STEPS,
        "features": feature_cols,
        "n_features": n_features,
        "epochs_trained": len(history.history["loss"]),
        "train_samples": len(X_train),
        "test_samples": len(X_test),
        "metrics": {"mse": float(mse), "rmse": float(rmse), "mae": float(mae)},
        "hyperparameters": {
            "batch_size": BATCH_SIZE,
            "learning_rate": LEARNING_RATE,
            "lstm_units": [64, 32],
            "dropout": 0.2
        }
    }
    meta_path = os.path.join(MODEL_DIR, "model_metadata.json")
    with open(meta_path, "w") as f:
        json.dump(metadata, f, indent=2, ensure_ascii=False)
    logger.info(f"Metadata saved: {meta_path}")

    logger.info(f"Model saved: {model_path}")
    logger.info("═══ TRAINING HOÀN TẤT! ═══")

    return model, scaler, metadata


if __name__ == "__main__":
    train()
