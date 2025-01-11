#include <string.h>

#include "control.h"

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

void dir2output_get(double output[FRAME_FACTOR_ROW_MAX],
                    frame_factor_t *frame_factor,
                    thrusters_params *thruster_config,
                    dir_t dir)
{
    output[MOTOR0_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_0], dir),
        thruster_config->thruster_0);
    output[MOTOR1_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_1], dir),
        thruster_config->thruster_1);
    output[MOTOR2_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_2], dir),
        thruster_config->thruster_2);
    output[MOTOR3_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_3], dir),
        thruster_config->thruster_3);
    output[MOTOR4_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_4], dir),
        thruster_config->thruster_4);
    output[MOTOR5_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_5], dir),
        thruster_config->thruster_5);
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    output[MOTOR6_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_6], dir),
        thruster_config->thruster_6);
    output[MOTOR7_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_7], dir),
        thruster_config->thruster_7);
#endif
}

void input2dir_get(dir_t *dir, lock_flags_t flags, input_data_t data,
                        PIDController *pid[MAX_PID_NUM])
{
    static float roll_ex = 0, pitch_ex = 0, yaw_ex = 0, throttle_ex = 0;
    bzero(dir, sizeof(dir_t));

    dir->forward += data.y;
    dir->lateral += data.x;
    dir->throttle += data.z;
    dir->yaw += data.r;

    if (flags.roll_locked == true)
        dir->roll += PIDController_Update(pid[ROLL_PID], roll_ex, data.roll);
    else
        roll_ex = data.roll;

    if (flags.pitch_locked == true)
        dir->pitch += PIDController_Update(pid[PITCH_PID], pitch_ex, data.pitch);
    else
        pitch_ex = data.pitch;

    if (flags.yaw_locked == true)
        dir->yaw += PIDController_Update(pid[YAW_PID], yaw_ex, data.yaw);
    else
        yaw_ex = data.yaw;

    if (flags.throttle_locked == true)
        dir->throttle += PIDController_Update(pid[THROTTLE_PID], throttle_ex, data.depth);
    else
        throttle_ex = data.depth;
}

void output2send_get(ThrusterCommand *msg, double output[MOTOR_NUM_MAX])
{
    if (output[0])
    {
        msg->has_throttle0 = true;
        msg->throttle0 = output[0];
    }
    if (output[1])
    {
        msg->has_throttle1 = true;
        msg->throttle1 = output[1];
    }
    if (output[2])
    {
        msg->has_throttle2 = true;
        msg->throttle2 = output[2];
    }
    if (output[3])
    {
        msg->has_throttle3 = true;
        msg->throttle3 = output[3];
    }
    if (output[4])
    {
        msg->has_throttle4 = true;
        msg->throttle4 = output[4];
    }
    if (output[5])
    {
        msg->has_throttle5 = true;
        msg->throttle5 = output[5];
    }
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    if (output[6])
    {
        msg->has_throttle6 = true;
        msg->throttle6 = output[6];
    }
    if (output[7])
    {
        msg->has_throttle7 = true;
        msg->throttle7 = output[7];
    }
#endif
}
