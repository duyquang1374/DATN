"""
============================================================
 HỆ THỐNG NHÀ MÀNG THÔNG MINH - MQTT COLLECTOR
============================================================
 Subscribe MQTT topic, nhận dữ liệu sensor, lưu vào SQLite.
 Chạy: python mqtt_collector.py
============================================================
"""

import json
import logging
import signal
import sys
import os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import paho.mqtt.client as mqtt
from config import (
    MQTT_BROKER, MQTT_PORT, MQTT_TOPIC_SENSOR, MQTT_CLIENT_ID,
    TEMP_MIN, TEMP_MAX, HUMIDITY_MIN, HUMIDITY_MAX,
    LOG_FORMAT, LOG_DATE_FORMAT
)
from database import init_database, insert_sensor_data, get_record_count, get_stats

# Logging setup
logging.basicConfig(
    level=logging.INFO,
    format=LOG_FORMAT,
    datefmt=LOG_DATE_FORMAT,
    handlers=[
        logging.StreamHandler(),
        logging.FileHandler("collector.log", encoding="utf-8")
    ]
)
logger = logging.getLogger(__name__)

# Biến toàn cục
received_count = 0
saved_count = 0
error_count = 0


def validate_data(temperature, humidity):
    """Kiểm tra dữ liệu có hợp lệ không."""
    if temperature < TEMP_MIN or temperature > TEMP_MAX:
        logger.warning(f"Nhiệt độ ngoài khoảng: {temperature}°C")
        return False
    if humidity < HUMIDITY_MIN or humidity > HUMIDITY_MAX:
        logger.warning(f"Độ ẩm ngoài khoảng: {humidity}%")
        return False
    return True


def on_connect(client, userdata, flags, rc):
    """Callback khi kết nối MQTT broker."""
    if rc == 0:
        logger.info("═══ KẾT NỐI MQTT BROKER THÀNH CÔNG ═══")
        logger.info(f"Broker: {MQTT_BROKER}:{MQTT_PORT}")
        client.subscribe(MQTT_TOPIC_SENSOR)
        logger.info(f"Subscribed topic: {MQTT_TOPIC_SENSOR}")

        # In thống kê hiện tại
        stats = get_stats()
        if stats:
            logger.info(f"Database hiện có {stats['total_records']} bản ghi")
            logger.info(f"Dữ liệu từ {stats['first_record']} → {stats['last_record']}")
        else:
            logger.info("Database trống - Bắt đầu thu thập!")
    else:
        logger.error(f"Kết nối thất bại, return code: {rc}")


def on_disconnect(client, userdata, rc):
    """Callback khi mất kết nối."""
    if rc != 0:
        logger.warning(f"Mất kết nối (rc={rc})! Đang tự reconnect...")


def on_message(client, userdata, msg):
    """Callback khi nhận message từ MQTT."""
    global received_count, saved_count, error_count

    received_count += 1

    try:
        # Parse JSON
        payload = json.loads(msg.payload.decode("utf-8"))
        temperature = float(payload.get("temperature", 0))
        humidity = float(payload.get("humidity", 0))
        timestamp = payload.get("timestamp", None)
        device = payload.get("device", "ESP32_Greenhouse")

        # Validate
        if not validate_data(temperature, humidity):
            error_count += 1
            return

        # Lưu vào database
        record_id = insert_sensor_data(timestamp, temperature, humidity, device)

        if record_id:
            saved_count += 1
            logger.info(
                f"#{record_id} | {temperature:.1f}°C | {humidity:.1f}% | "
                f"[Tổng: {saved_count} saved / {received_count} received]"
            )
        else:
            error_count += 1
            logger.error("Lưu database thất bại!")

    except json.JSONDecodeError:
        error_count += 1
        logger.error(f"JSON không hợp lệ: {msg.payload}")
    except (KeyError, ValueError, TypeError) as e:
        error_count += 1
        logger.error(f"Dữ liệu lỗi: {e} | Payload: {msg.payload}")


def print_summary():
    """In tổng kết khi thoát."""
    logger.info("═══════════════════════════════════════")
    logger.info("           TỔNG KẾT PHIÊN THU THẬP")
    logger.info("═══════════════════════════════════════")
    logger.info(f"  Nhận được:  {received_count} messages")
    logger.info(f"  Đã lưu:    {saved_count} records")
    logger.info(f"  Lỗi:       {error_count}")
    total = get_record_count()
    logger.info(f"  Tổng DB:   {total} records")
    logger.info("═══════════════════════════════════════")


def signal_handler(sig, frame):
    """Xử lý Ctrl+C."""
    logger.info("\nĐang dừng collector...")
    print_summary()
    sys.exit(0)


def main():
    """Entry point."""
    signal.signal(signal.SIGINT, signal_handler)

    print("╔══════════════════════════════════════════╗")
    print("║  NHÀ MÀNG THÔNG MINH - DATA COLLECTOR   ║")
    print("╠══════════════════════════════════════════╣")
    print(f"║  Broker: {MQTT_BROKER}:{MQTT_PORT}")
    print(f"║  Topic:  {MQTT_TOPIC_SENSOR}")
    print("╚══════════════════════════════════════════╝")

    # Init database
    init_database()

    # MQTT client setup
    client = mqtt.Client(client_id=MQTT_CLIENT_ID)
    client.on_connect = on_connect
    client.on_disconnect = on_disconnect
    client.on_message = on_message

    # Auth nếu cần
    # client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)

    # Kết nối
    try:
        client.connect(MQTT_BROKER, MQTT_PORT, keepalive=60)
    except ConnectionRefusedError:
        logger.error(f"Không thể kết nối {MQTT_BROKER}:{MQTT_PORT}")
        logger.error("Kiểm tra Mosquitto broker đã chạy chưa!")
        logger.error("  → sudo apt install mosquitto")
        logger.error("  → sudo systemctl start mosquitto")
        sys.exit(1)
    except Exception as e:
        logger.error(f"Lỗi kết nối: {e}")
        sys.exit(1)

    # Loop forever
    logger.info("Đang lắng nghe dữ liệu... (Ctrl+C để dừng)")
    client.loop_forever()


if __name__ == "__main__":
    main()
