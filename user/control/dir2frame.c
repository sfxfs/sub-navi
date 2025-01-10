#include "dir2frame.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

static float per_row_cal(double row[FRAME_FACTOR_COL_MAX], dir_t dir)
{
    float ret = 0;
    ret += row[FRAME_FACTOR_COL_ROLL] * dir.roll;
    ret += row[FRAME_FACTOR_COL_PITCH] * dir.pitch;
    ret += row[FRAME_FACTOR_COL_YAW] * dir.yaw;
    ret += row[FRAME_FACTOR_COL_THROTTLE] * dir.throttle;
    ret += row[FRAME_FACTOR_COL_FORWARD] * dir.forward;
    ret += row[FRAME_FACTOR_COL_LATERAL] * dir.lateral;
    return ret;
}

static float per_motor_cal(float raw, thruster_attr attr)
{
    if (raw == 0)
        return 0;
    raw *= attr.reversed == true ? -1. : 1.;
    raw += raw < 0 ? -attr.deadzone_n : attr.deadzone_p;
    raw = constrain(raw, -attr.power_nLimit, attr.power_pLimit);
    return raw;
}

void dir2frame_get(float throttle[FRAME_FACTOR_ROW_MAX],
                    frame_factor_t factor, thrusters_params *thruster_config,
                    dir_t dir)
{
    throttle[0] = per_motor_cal(per_row_cal(
        factor.value[FRAME_FACTOR_ROW_MOTOR_0], dir),
        thruster_config->thruster_0);
    throttle[1] = per_motor_cal(per_row_cal(
        factor.value[FRAME_FACTOR_ROW_MOTOR_1], dir),
        thruster_config->thruster_1);
    throttle[2] = per_motor_cal(per_row_cal(
        factor.value[FRAME_FACTOR_ROW_MOTOR_2], dir),
        thruster_config->thruster_2);
    throttle[3] = per_motor_cal(per_row_cal(
        factor.value[FRAME_FACTOR_ROW_MOTOR_3], dir),
        thruster_config->thruster_3);
    throttle[4] = per_motor_cal(per_row_cal(
        factor.value[FRAME_FACTOR_ROW_MOTOR_4], dir),
        thruster_config->thruster_4);
    throttle[5] = per_motor_cal(per_row_cal(
        factor.value[FRAME_FACTOR_ROW_MOTOR_5], dir),
        thruster_config->thruster_5);
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    throttle[6] = per_motor_cal(per_row_cal(
        factor.value[FRAME_FACTOR_ROW_MOTOR_6], dir),
        thruster_config->thruster_6);
    throttle[7] = per_motor_cal(per_row_cal(
        factor.value[FRAME_FACTOR_ROW_MOTOR_7], dir),
        thruster_config->thruster_7);
#endif
}
