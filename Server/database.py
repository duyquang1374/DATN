"""
============================================================
 HỆ THỐNG NHÀ MÀNG THÔNG MINH - DATABASE MODULE
============================================================
"""

import sqlite3
import os
import csv
import logging
from datetime import datetime, timedelta
from contextlib import contextmanager

import sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from config import DB_PATH, DATA_DIR

logger = logging.getLogger(__name__)


@contextmanager
def get_connection():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    try:
        yield conn
        conn.commit()
    except Exception as e:
        conn.rollback()
        raise e
    finally:
        conn.close()


def init_database():
    """Khởi tạo database và tạo bảng."""
    with get_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS sensor_data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                timestamp DATETIME NOT NULL,
                temperature REAL NOT NULL,
                humidity REAL NOT NULL,
                device TEXT DEFAULT 'ESP32_Greenhouse',
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        """)
        cursor.execute("""
            CREATE INDEX IF NOT EXISTS idx_sensor_timestamp 
            ON sensor_data(timestamp)
        """)
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS predictions (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                predicted_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                target_time DATETIME NOT NULL,
                predicted_temp REAL NOT NULL,
                actual_temp REAL,
                model_version TEXT
            )
        """)
        conn.commit()
        logger.info(f"Database initialized at: {DB_PATH}")


def insert_sensor_data(timestamp, temperature, humidity, device="ESP32_Greenhouse"):
    """Thêm 1 bản ghi sensor data."""
    try:
        if isinstance(timestamp, (int, float)):
            dt = datetime.fromtimestamp(timestamp)
        elif isinstance(timestamp, str):
            dt = datetime.fromisoformat(timestamp)
        else:
            dt = timestamp

        with get_connection() as conn:
            cursor = conn.cursor()
            cursor.execute(
                "INSERT INTO sensor_data (timestamp, temperature, humidity, device) VALUES (?, ?, ?, ?)",
                (dt.strftime("%Y-%m-%d %H:%M:%S"), temperature, humidity, device)
            )
            record_id = cursor.lastrowid
            logger.debug(f"Inserted #{record_id}: temp={temperature}°C, humid={humidity}%")
            return record_id
    except Exception as e:
        logger.error(f"Insert error: {e}")
        return None


def get_latest_data(hours=24):
    """Lấy dữ liệu N giờ gần nhất."""
    since = datetime.now() - timedelta(hours=hours)
    with get_connection() as conn:
        cursor = conn.cursor()
        cursor.execute(
            "SELECT timestamp, temperature, humidity FROM sensor_data WHERE timestamp >= ? ORDER BY timestamp ASC",
            (since.strftime("%Y-%m-%d %H:%M:%S"),)
        )
        return [dict(row) for row in cursor.fetchall()]


def get_all_data():
    """Lấy toàn bộ dữ liệu."""
    with get_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("SELECT timestamp, temperature, humidity FROM sensor_data ORDER BY timestamp ASC")
        return [dict(row) for row in cursor.fetchall()]


def get_record_count():
    """Đếm tổng số bản ghi."""
    with get_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("SELECT COUNT(*) FROM sensor_data")
        return cursor.fetchone()[0]


def get_stats():
    """Lấy thống kê tổng quan."""
    with get_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("""
            SELECT COUNT(*) as total_records,
                   MIN(temperature) as min_temp, MAX(temperature) as max_temp, AVG(temperature) as avg_temp,
                   MIN(humidity) as min_humidity, MAX(humidity) as max_humidity, AVG(humidity) as avg_humidity,
                   MIN(timestamp) as first_record, MAX(timestamp) as last_record
            FROM sensor_data
        """)
        row = cursor.fetchone()
        return dict(row) if row and row["total_records"] > 0 else None


def export_to_csv(filepath=None):
    """Export toàn bộ dữ liệu ra CSV."""
    if filepath is None:
        filepath = os.path.join(DATA_DIR, "greenhouse_data.csv")
    data = get_all_data()
    if not data:
        logger.warning("Không có dữ liệu để export!")
        return None
    with open(filepath, "w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=["timestamp", "temperature", "humidity"])
        writer.writeheader()
        writer.writerows(data)
    logger.info(f"Exported {len(data)} records to {filepath}")
    return filepath


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format="%(asctime)s [%(levelname)s] %(message)s")
    print("=== Database Module Test ===")
    init_database()
    test_id = insert_sensor_data(datetime.now(), 28.5, 75.0)
    print(f"Inserted test record #{test_id}")
    stats = get_stats()
    if stats:
        print(f"Total: {stats['total_records']} | Temp: {stats['min_temp']}-{stats['max_temp']}°C")
    print("=== Test Complete ===")
