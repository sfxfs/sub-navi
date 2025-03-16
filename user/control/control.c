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

// 根据方向和帧因子计算每个电机的输出值
void dir2output_get(double output[FRAME_FACTOR_ROW_MAX],
                    frame_factor_t *frame_factor,
                    thrusters_params *thruster_config,
                    dir_t dir)
{
    // 计算电机0的输出值
    output[MOTOR0_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_0], dir),
        thruster_config->thruster_0);
    // 计算电机1的输出值
    output[MOTOR1_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_1], dir),
        thruster_config->thruster_1);
    // 计算电机2的输出值
    output[MOTOR2_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_2], dir),
        thruster_config->thruster_2);
    // 计算电机3的输出值
    output[MOTOR3_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_3], dir),
        thruster_config->thruster_3);
    // 计算电机4的输出值
    output[MOTOR4_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_4], dir),
        thruster_config->thruster_4);
    // 计算电机5的输出值
    output[MOTOR5_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_5], dir),
        thruster_config->thruster_5);
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    // 计算电机6的输出值
    output[MOTOR6_NUM] = per_motor_cal(per_row_cal(
        frame_factor->value[FRAME_FACTOR_ROW_MOTOR_6], dir),
        thruster_config->thruster_6);
    // 计算电机7的输出值
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

    dir->forward += data.ly;
    dir->lateral += data.lx;
    dir->throttle += data.ry;
    dir->yaw += data.rx;

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
