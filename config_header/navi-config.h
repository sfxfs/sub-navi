#ifndef SUB_NAVI_GLOBAL_CONFIG_H
#define SUB_NAVI_GLOBAL_CONFIG_H

#define TRUE    1
#define FALSE   0

// 0=LOG_TRACE, 1=LOG_DEBUG, 2=LOG_INFO, 3=LOG_WARN, 4=LOG_ERROR, 5=LOG_FATAL, 6=LOG_OFF
#define SUB_NAVI_CONFIG_LOG_LEVEL                   2
#define SUB_NAVI_CONFIG_LOG_OUTPUT_FILE_PATH        "./navi.log"

// 0 is for 6 thrusters, 1 is for 8 thrusters
#define SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS         TRUE

// configuration file path
#define SUB_NAVI_CONFIG_PID_CONFIG_FILE_PATH        "./pid_config.json"
#define SUB_NAVI_CONFIG_PWM_DEV_CONFIG_FILE_PATH    "./pwmDev_config.json"
#define SUB_NAVI_CONFIG_THRUSTER_CONFIG_FILE_PATH   "./thruster_config.json"
#define SUB_NAVI_CONFIG_FRAME_FACTOR_FILE_PATH      "./frame_factor.csv"

// tcp server configs
#define SUB_NAVI_CONFIG_RPC_SERVER_PORT             8888

// protbuf configs
#define SUB_NAVI_CONFIG_PROTOBUF_UART_PATH          "/dev/ttyS5"
#define SUB_NAVI_CONFIG_PROTOBUF_UART_BAUDRATE      115200

// jy901 configs
#define SUB_NAVI_CONFIG_JY901_ENABLE                TRUE
#define SUB_NAVI_CONFIG_JY901_USE_IIC               TRUE        // 0 is for uart, 1 is for iic
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
