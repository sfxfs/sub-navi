#include <ev.h>
#include <stdlib.h>

#include "navi-config.h"

#include "ctrl_routine.h"

static void routine(EV_P_ ev_timer *w, int revents)
{
    ctrl_routine_arg_t *arg = w->data;
    dir_t dir = {0};
    double output[MOTOR_NUM_MAX] = {0};

    input2dir_get(&dir, *arg->lock_flags, *arg->input_data, NULL);
    dir2output_get(output, arg->frame_factor, arg->thruster_config, dir);
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    log_debug("output: %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f",
              output[0], output[1], output[2], output[3], output[4], output[5], output[6], output[7]);
#else
    log_debug("output: %.4f, %.4f, %.4f, %.4f, %.4f, %.4f",
              output[0], output[1], output[2], output[3], output[4], output[5]);
#endif
}

ev_timer routine_watcher;

navi_ret_t navi_ctrl_routine_start(ctrl_routine_arg_t *arg)
{
    EV_P = EV_DEFAULT;
    ev_timer_init(&routine_watcher, routine,
                  SUB_NAVI_CONFIG_CONTROL_RUN_EVERY_SECOND,
                  SUB_NAVI_CONFIG_CONTROL_RUN_EVERY_SECOND);
    routine_watcher.data = arg;
    ev_timer_start(EV_A_ & routine_watcher);

    log_info("control routine watcher registered.");
    return NAVI_RET_SUCCESS;
}
