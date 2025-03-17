#ifndef SUB_NAVI_GLOBAL_CONFIG_H
#define SUB_NAVI_GLOBAL_CONFIG_H

#include "log.h"
#include <stdbool.h>

#define SUB_NAVI_CONFIG_LOG_LEVEL                   LOG_INFO
#define SUB_NAVI_CONFIG_LOG_OUTPUT_FILE_PATH        "./navi.log"

// 0 is for 6 thrusters, 1 is for 8 thrusters
#define SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS         true

// configuration file path
#define SUB_NAVI_CONFIG_PID_CONFIG_FILE_PATH        "./pid_config.json"
#define SUB_NAVI_CONFIG_PWM_DEV_CONFIG_FILE_PATH    "./pwmDev_config.json"
#define SUB_NAVI_CONFIG_THRUSTER_CONFIG_FILE_PATH   "./thruster_config.json"
#define SUB_NAVI_CONFIG_FRAME_FACTOR_FILE_PATH      "./frame_factor.csv"

// tcp server configs
#define SUB_NAVI_CONFIG_RPC_SERVER_PORT             8888

// pca9685 configs
#define SUB_NAVI_CONFIG_PCA9685_PINBASE             300
#define SUB_NAVI_CONFIG_PCA9685_IIC_PATH            "/dev/i2c-1"
#define SUB_NAVI_CONFIG_PCA9685_IIC_ADDR            0x40
#define SUB_NAVI_CONFIG_PCA9685_PIN_OE              19
#define SUB_NAVI_CONFIG_PCA9685_PWM_FREQ            50.f

// jy901 configs
#define SUB_NAVI_CONFIG_JY901_ENABLE                true
#define SUB_NAVI_CONFIG_JY901_USE_IIC               false        // 0 is for uart, 1 is for iic
#if SUB_NAVI_CONFIG_JY901_USE_IIC
#define SUB_NAVI_CONFIG_JY901_IIC_PATH              "/dev/i2c-0"
#define SUB_NAVI_CONFIG_JY901_IIC_ADDR              0x50
#else
#define SUB_NAVI_CONFIG_JY901_UART_PATH             "/dev/ttyS1"
#define SUB_NAVI_CONFIG_JY901_UART_BAUDRATE         115200
#endif

// control configs
#define SUB_NAVI_CONFIG_CONTROL_RUN_EVERY_SECOND    0.02        // 20ms
#define SUB_NAVI_CONFIG_CONTROL_THROTTLE_MID        1500.       // means stop
#define SUB_NAVI_CONFIG_CONTROL_THROTTLE_MAX_OFFSET 500.

#endif
