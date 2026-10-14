#pragma once

// ─────────────────────────────────────────────────────────────
//  LORA E32 (UART2)
// ─────────────────────────────────────────────────────────────
#define LORA_TX_PIN      18
#define LORA_RX_PIN      19
#define LORA_BAUD        115200

// ─────────────────────────────────────────────────────────────
//  UART HMI (UART1)
// ─────────────────────────────────────────────────────────────
#define HMI_TX_PIN       17
#define HMI_RX_PIN       16
#define HMI_BAUD         115200

// ─────────────────────────────────────────────────────────────
//  I2C BUS 0 (SHT30 Trong, BH1750)
// ─────────────────────────────────────────────────────────────
#define I2C_SDA   21
#define I2C_SCL   22

// ─────────────────────────────────────────────────────────────
//  I2C BUS 1 (SHT30 Ngoài trời - dùng các chân LoRa cũ)
// ─────────────────────────────────────────────────────────────
#define I2C1_SDA  5
#define I2C1_SCL  23

// ─────────────────────────────────────────────────────────────
//  QUẠT DC - TB6612FNG (Kênh A)
// ─────────────────────────────────────────────────────────────
#define FAN_AIN1         25    
#define FAN_AIN2         26    
#define FAN_PWMA         27    
#define FAN_PWM_FREQ     20000 
#define FAN_PWM_RES      8     

// ─────────────────────────────────────────────────────────────
//  BÓNG SƯỞI ẤM - TB6612FNG (Kênh B)
// ─────────────────────────────────────────────────────────────
#define HEATER_BIN1      12
#define HEATER_BIN2      13
#define HEATER_PWMB      14

// LƯU Ý: Chân STBY của TB6612FNG có thể nối chung vào 3.3V để tiết kiệm chân GPIO.

// ─────────────────────────────────────────────────────────────
//  THIẾT BỊ CHẤP HÀNH KHÁC
// ─────────────────────────────────────────────────────────────
#define PUMP_PIN         32    // Relay
#define PUMP_ACTIVE_HIGH true

#define MIST_PIN         33    // MOSFET
#define MIST_ACTIVE_HIGH true 

#define LIGHT_PIN           4      // MOSFET
#define LIGHT_ACTIVE_HIGH   true   
#define DEFAULT_LIGHT_THRESH 1000.0 

#define VALVE1_PIN       15    // Relay
#define VALVE2_PIN       2     // Relay

#define SERVO_PIN        0     // Servo mái che

// ─────────────────────────────────────────────────────────────
//  CẢM BIẾN ĐỘ ẨM ĐẤT
// ─────────────────────────────────────────────────────────────
#define SOIL_PIN        34   // GPIO34 (ADC1_CH6, input-only)
#define SOIL_SAMPLES    10   // Số mẫu trung bình (lọc nhiễu ADC)
#define SOIL_DRY_VALUE  1500 // ADC raw khi đất KHÔ
#define SOIL_WET_VALUE  3000 // ADC raw khi đất ƯỚT
#define SOIL_CALIBRATION_MODE 0 // =1: Serial in ADC raw để calibrate

// ─────────────────────────────────────────────────────────────
//  THỜI GIAN (milliseconds)
// ─────────────────────────────────────────────────────────────
#define SENSOR_INTERVAL     2000   
#define HMI_INTERVAL        1000   
#define LORA_INTERVAL       10000
#define FORECAST_INTERVAL   300000 // Dự báo mỗi 5 phút (= 1 horizon step)

// ─────────────────────────────────────────────────────────────
//  PID - ĐIỀU KHIỂN TỐC ĐỘ QUẠT
// ─────────────────────────────────────────────────────────────
#define PID_KP           8.0    
#define PID_KI           0.5    
#define PID_KD           2.0    
#define DEFAULT_FAN_SETPOINT 28.0 
#define FAN_DEADBAND     0.5    
#define FAN_PWM_MIN      50     
#define FAN_PWM_MAX      255    

#define FAN_MODE_OFF     0
#define FAN_MODE_AUTO    1
#define FAN_MODE_MANUAL  2

// ─────────────────────────────────────────────────────────────
//  BƠM - NGƯỠNG ĐỘ ẨM
// ─────────────────────────────────────────────────────────────
#define DEFAULT_PUMP_ON_THRESH   40.0   
#define DEFAULT_PUMP_OFF_THRESH  65.0   

// ─────────────────────────────────────────────────────────────
//  SENSOR VALIDATION
// ─────────────────────────────────────────────────────────────
#define TEMP_MIN         -10.0
#define TEMP_MAX          60.0
#define HUMID_MIN          0.0
#define HUMID_MAX        100.0
#define LUX_MAX          65535.0
#define MAX_SENSOR_ERRORS  5

