#include <unistd.h>
#include <stdlib.h>

#include "log.h"
#include "wiringSerial.h"

#include "navi-type.h"
#include "navi-config.h"

#include "protobuf-commu/convert_msg.h"
#include "protobuf-commu/navi_master.pb.h"

#include "arg_parse.h"

// {uart} {channel} {value}
static navi_ret_t pwm_cmd_callback(const char *args[], int arg_count)
{
    if (arg_count != 3)
        return NAVI_RET_ARG_ERROR;

    int channel = atoi(args[1]);
    int value = atoi(args[2]);
    PWMDevCommand msg = PWMDevCommand_init_default;
    switch (channel)
    {
    case 0:msg.has_duty0 = true; msg.duty0 = value;break;
    case 1:msg.has_duty1 = true; msg.duty1 = value;break;
    case 2:msg.has_duty2 = true; msg.duty2 = value;break;
    case 3:msg.has_duty3 = true; msg.duty3 = value;break;
    case 4:msg.has_duty4 = true; msg.duty4 = value;break;
    case 5:msg.has_duty5 = true; msg.duty5 = value;break;
    case 6:msg.has_duty6 = true; msg.duty6 = value;break;
    case 7:msg.has_duty7 = true; msg.duty7 = value;break;
    case 8:msg.has_duty8 = true; msg.duty8 = value;break;
    case 9:msg.has_duty9 = true; msg.duty9 = value;break;
    case 10:msg.has_duty10 = true; msg.duty10 = value;break;
    case 11:msg.has_duty11 = true; msg.duty11 = value;break;
    case 12:msg.has_duty12 = true; msg.duty12 = value;break;
    case 13:msg.has_duty13 = true; msg.duty13 = value;break;
    case 14:msg.has_duty14 = true; msg.duty14 = value;break;
    case 15:msg.has_duty15 = true; msg.duty15 = value;break;
    default:
        log_error("Invalid channel number!");
        return NAVI_RET_ARG_ERROR;
    }

    uint8_t data[Commands_size] = {0};
    size_t bytes = msg2byte(PWMDevCommand_fields, &msg, data, sizeof(data));
    if (bytes <= 0)
    {
        log_error("Error: Unable to convert message to byte array");
        return NAVI_RET_FAIL;
    }

    int uart_fd = serialOpen(args[0], SUB_NAVI_CONFIG_PROTOBUF_UART_BAUDRATE);
    if (uart_fd < 0)
    {
        log_error("Error: Unable to open UART device '%s'", args[0]);
        return NAVI_RET_FAIL;
    }

    int bytes_written = write(uart_fd, data, bytes);
    close(uart_fd);
    if (bytes_written != bytes)
    {
        log_error("Error: Unable to write all bytes to UART device");
        return NAVI_RET_FAIL;
    }

    return NAVI_RET_SUCCESS;
}

static navi_ret_t shot_cmd_callback(const char *args[], int arg_count)
{
    if (arg_count != 3)
        return NAVI_RET_ARG_ERROR;

    int channel = atoi(args[1]);
    int value = atoi(args[2]);
    ThrusterCommand msg = ThrusterCommand_init_default;
    switch (channel)
    {
    case 0:msg.has_throttle0 = true; msg.throttle0 = value;break;
    case 1:msg.has_throttle1 = true; msg.throttle1 = value;break;
    case 2:msg.has_throttle2 = true; msg.throttle2 = value;break;
    case 3:msg.has_throttle3 = true; msg.throttle3 = value;break;
    case 4:msg.has_throttle4 = true; msg.throttle4 = value;break;
    case 5:msg.has_throttle5 = true; msg.throttle5 = value;break;
    case 6:msg.has_throttle6 = true; msg.throttle6 = value;break;
    case 7:msg.has_throttle7 = true; msg.throttle7 = value;break;
    default:
        log_error("Invalid channel number!");
        return NAVI_RET_ARG_ERROR;
    }

    uint8_t data[Commands_size] = {0};
    size_t bytes = msg2byte(ThrusterCommand_fields, &msg, data, sizeof(data));
    if (bytes <= 0)
    {
        log_error("Error: Unable to convert message to byte array");
        return NAVI_RET_FAIL;
    }

    int uart_fd = serialOpen(args[0], SUB_NAVI_CONFIG_PROTOBUF_UART_BAUDRATE);
    if (uart_fd < 0)
    {
        log_error("Error: Unable to open UART device '%s'", args[0]);
        return NAVI_RET_FAIL;
    }

    int bytes_written = write(uart_fd, data, bytes);
    close(uart_fd);
    if (bytes_written != bytes)
    {
        log_error("Error: Unable to write all bytes to UART device");
        return NAVI_RET_FAIL;
    }

    return NAVI_RET_SUCCESS;
}

navi_ret_t navi_parse_arguments(int argc, const char *argv[])
{
    // 提取命令和参数
    const char *cmd = argv[1];
    const char **args = NULL; // 参数数组起始位置
    int arg_count = 0;  // 参数个数
    if (argc > 2)
    {
        args = &argv[2];
        arg_count = argc - 2;
    }

    // 打印解析结果
    log_debug("Command received: %s", cmd);
    log_debug("Number of arguments: %d", arg_count);

    // 遍历并打印所有参数
    for (int i = 0; i < arg_count; i++)
    {
        log_debug("Argument %d: %s", i, args[i]);
    }

    if (strncmp(cmd, "pwm", 3) == 0)
    {
        return pwm_cmd_callback(args, arg_count);
    }
    else if (strncmp(cmd, "shot", 4) == 0)
    {
        return shot_cmd_callback(args, arg_count);
    }

    log_error("Error: Unknown command '%s'", cmd);
    return NAVI_RET_FAIL;
}
