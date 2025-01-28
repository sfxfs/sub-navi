#include "string.h"

#include "log.h"

#include "navi-type.h"

#include "arg_parse.h"

static navi_ret_t pwm_cmd_callback(const char *cmd, const char *args[], int arg_count)
{
    return NAVI_RET_SUCCESS;
}

static navi_ret_t shot_cmd_callback(const char *cmd, const char *args[], int arg_count)
{
    return NAVI_RET_SUCCESS;
}

navi_ret_t navi_parse_arguments(int argc, const char *argv[])
{
    // 提取命令和参数
    char *cmd = argv[1];
    char **args = NULL; // 参数数组起始位置
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
        return pwm_cmd_callback(cmd, args, arg_count);
    }
    else if (strncmp(cmd, "shot", 4) == 0)
    {
        return shot_cmd_callback(cmd, args, arg_count);
    }

    log_error("Error: Unknown command '%s'", cmd);
    return NAVI_RET_FAIL;
}
