#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

#include "peripheral/nxp_pca9685.h"

#include "navi-type.h"
#include "navi-config.h"

#include "arg_parse.h"

// {channel} {value}
static navi_ret_t pwm_cmd_callback(const char *args[], int arg_count)
{
    if (arg_count != 2)
    {
        log_error("Invalid arguments, use the format: pwm { channel(0~15) } { value(us) }");
        return NAVI_RET_ARG_ERROR;
    }

    int channel = atoi(args[0]);
    if (channel < 0)
        channel = 0;
    int us = atoi(args[1]);

    int pca9685_fd = pca9685Setup(SUB_NAVI_CONFIG_PCA9685_PINBASE,
                                  SUB_NAVI_CONFIG_PCA9685_IIC_PATH,
                                  SUB_NAVI_CONFIG_PCA9685_IIC_ADDR,
                                  SUB_NAVI_CONFIG_PCA9685_PIN_OE,
                                  SUB_NAVI_CONFIG_PCA9685_PWM_FREQ);
    if (pca9685_fd < 0)
    {
        log_error("PCA9685 init failed!");
        return NAVI_RET_FAIL;
    }

    float percent = (float)us / (1000.f * 1000.f / SUB_NAVI_CONFIG_PCA9685_PWM_FREQ);
    log_info("PWM channel %d set to %f%%", channel, percent);

    pca9685PWMWrite(pca9685_fd, channel, 0, percent * 0xFFF);
    return NAVI_RET_SUCCESS;
}

navi_ret_t navi_parse_arguments(int argc, const char *argv[])
{
    // 提取命令和参数
    const char *cmd = argv[1];
    const char **args = NULL; // 参数数组起始位置
    int arg_count = 0;        // 参数个数
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

    log_error("Error: Unknown command '%s'", cmd);
    return NAVI_RET_FAIL;
}
