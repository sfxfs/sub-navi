#include <stdlib.h>

#include "ev.h"
#include "log.h"

#include "protobuf-commu/union.h"
#include "protobuf-commu/proto_def/navi_master.pb.h"
#include "json-rpc-server/method_ctrl.h"

#include "navi-config.h"

#include "mode_manual.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

struct mode_manual_arg
{
    frame_factor_t *frame_factor;
    thrusters_params *thruster_config;
};

ev_timer routine_watcher;

static double per_motor_move_data_to_raw_throttle(double *factor,
                                                struct method_move move_data)
{
    double raw = 0.;
    raw += factor[FRAME_FACTOR_COL_FORWARD] * move_data.y;
    raw += factor[FRAME_FACTOR_COL_LATERAL] * move_data.x;
    raw += factor[FRAME_FACTOR_COL_THROTTLE] * move_data.z;
    raw += factor[FRAME_FACTOR_COL_YAW] * move_data.r;
    return raw;
}

static double *move_data_to_raw_throttle(frame_factor_t *frame_factor,
                                        struct method_move move_data)
{
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    double *array = malloc(sizeof(double) * 8);
#else
    double *array = malloc(sizeof(double) * 6);
#endif
    array[0] = per_motor_move_data_to_raw_throttle(
                        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_0], move_data);
    array[1] = per_motor_move_data_to_raw_throttle(
                        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_1], move_data);
    array[2] = per_motor_move_data_to_raw_throttle(
                        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_2], move_data);
    array[3] = per_motor_move_data_to_raw_throttle(
                        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_3], move_data);
    array[4] = per_motor_move_data_to_raw_throttle(
                        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_4], move_data);
    array[5] = per_motor_move_data_to_raw_throttle(
                        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_5], move_data);
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    array[6] = per_motor_move_data_to_raw_throttle(
                        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_6], move_data);
    array[7] = per_motor_move_data_to_raw_throttle(
                        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_7], move_data);
#endif
    return array;
}

static uint32_t throttle_double_to_uint32(double raw)
{
    return (uint32_t)(raw * 1000.); // TODO ... (== 0 means not to update)
}

static uint32_t per_motor_raw_to_throttle(double raw, thruster_attr attr)
{
    if (raw == 0.)
        return 0;
    raw *= attr.reversed == true ? -1. : 1.;
    raw += raw < 0 ? -attr.deadzone_n : attr.deadzone_p;
    raw = constrain(raw, -attr.power_nLimit, attr.power_pLimit);
    return throttle_double_to_uint32(raw);
}

static uint32_t *raw_to_throttle(double *raw, thrusters_params *thruster_config)
{
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    uint32_t *array = malloc(sizeof(uint32_t) * 8);
#else
    uint32_t *array = malloc(sizeof(uint32_t) * 6);
#endif
    array[0] = per_motor_raw_to_throttle(raw[0], thruster_config->thruster_0);
    array[1] = per_motor_raw_to_throttle(raw[1], thruster_config->thruster_1);
    array[2] = per_motor_raw_to_throttle(raw[2], thruster_config->thruster_2);
    array[3] = per_motor_raw_to_throttle(raw[3], thruster_config->thruster_3);
    array[4] = per_motor_raw_to_throttle(raw[4], thruster_config->thruster_4);
    array[5] = per_motor_raw_to_throttle(raw[5], thruster_config->thruster_5);
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    array[6] = per_motor_raw_to_throttle(raw[6], thruster_config->thruster_6);
    array[7] = per_motor_raw_to_throttle(raw[7], thruster_config->thruster_7);
#endif
    return array;
}

static int write_throttle_to_motor(uint32_t *throttle)
{
    ThrusterCommand msg = {0};
    if (throttle[0])
    {
        msg.has_throttle0 = true;
        msg.throttle0 = throttle[0];
    }
    if (throttle[1])
    {
        msg.has_throttle1 = true;
        msg.throttle1 = throttle[1];
    }
    if (throttle[2])
    {
        msg.has_throttle2 = true;
        msg.throttle2 = throttle[2];
    }
    if (throttle[3])
    {
        msg.has_throttle3 = true;
        msg.throttle3 = throttle[3];
    }
    if (throttle[4])
    {
        msg.has_throttle4 = true;
        msg.throttle4 = throttle[4];
    }
    if (throttle[5])
    {
        msg.has_throttle5 = true;
        msg.throttle5 = throttle[5];
    }
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    if (throttle[6])
    {
        msg.has_throttle6 = true;
        msg.throttle6 = throttle[6];
    }
    if (throttle[7])
    {
        msg.has_throttle7 = true;
        msg.throttle7 = throttle[7];
    }
#endif
    return protobuf_commu_send_cmd(ThrusterCommand_fields, &msg);
}

static void routine_cb(struct ev_loop *loop, ev_timer *w, int revents)
{
    if (g_move_data.updated)
    {
        log_debug("move_data updated.");
        g_move_data.updated = false;
        struct mode_manual_arg *arg = w->data;
        double *raw = move_data_to_raw_throttle(arg->frame_factor, g_move_data);
        uint32_t *throttle = raw_to_throttle(raw, arg->thruster_config);
        if (0 != write_throttle_to_motor(throttle))
            log_error("send command to sub-master failed.");
        free(throttle);
        free(raw);
    }
}

navi_ret_t control_manual_init(frame_factor_t *frame_factor,
                        thrusters_params *thruster_config)
{
    if (frame_factor == NULL || thruster_config == NULL)
        return NAVI_RET_ARG_ERROR;
    struct mode_manual_arg *arg = malloc(sizeof(struct mode_manual_arg));
    if (arg == NULL)
        return NAVI_RET_MEM_ALLOC_FAIL;
    arg->frame_factor = frame_factor;
    arg->thruster_config = thruster_config;

    EV_P = EV_DEFAULT;
    ev_timer_init(&routine_watcher, routine_cb,
                    SUB_NAVI_CONFIG_CONTROL_RUN_EVERY_SECOND,
                    SUB_NAVI_CONFIG_CONTROL_RUN_EVERY_SECOND);
    routine_watcher.data = arg;
    ev_timer_start(EV_A_ &routine_watcher);

    log_info("control routine watcher registered.");
    return NAVI_RET_SUCCESS;
}
