#ifndef SUB_NAVI_GLOBAL_CONFIG_H
#define SUB_NAVI_GLOBAL_CONFIG_H

#define SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS         1   // 0 is for 6 thrusters

#define SUB_NAVI_CONFIG_PID_CONFIG_FILE_PATH        "./pid_config.json"
#define SUB_NAVI_CONFIG_PWM_DEV_CONFIG_FILE_PATH    "./pwmDev_config.json"
#define SUB_NAVI_CONFIG_THRUSTER_CONFIG_FILE_PATH   "./thruster_config.json"
#define SUB_NAVI_CONFIG_FRAME_FACTOR_FILE_PATH      "./frame_factor.csv"

#define SUB_NAVI_CONFIG_RPC_SERVER_PORT             8888

#define SUB_NAVI_CONFIG_PROTOBUF_UART_PATH          "/dev/ttyS5"
#define SUB_NAVI_CONFIG_PROTOBUF_UART_BAUDRATE      115200

#define SUB_NAVI_CONFIG_JY901_UART_PATH             "/dev/ttyS1"
#define SUB_NAVI_CONFIG_JY901_UART_BAUDRATE         115200

#endif
