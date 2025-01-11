#ifndef SUB_NAVI_CONTROL_H
#define SUB_NAVI_CONTROL_H

#include <stdbool.h>

#include "pid.h"
#include "mjsonrpc.h"

#include "csv-json-config-sys/json_thruster.h"
#include "csv-json-config-sys/csv_frame_factor.h"
#include "protobuf-commu/proto_def/navi_master.pb.h"

typedef struct _dir_t
{
    double roll;
    double pitch;
    double yaw;
    double throttle; // depth
    double forward;
    double lateral;
} dir_t;

typedef struct _input_data_t
{
    float x, y, z, r;
    float roll, pitch, yaw, depth;
} input_data_t;

typedef struct _lock_flags_t
{
    bool roll_locked;
    bool pitch_locked;
    bool yaw_locked;
    bool throttle_locked;
} lock_flags_t;

enum pid_num
{
    ROLL_PID = 0,
    PITCH_PID,
    YAW_PID,
    THROTTLE_PID,
    MAX_PID_NUM, // must be the last one
};

enum motor_num
{
    MOTOR0_NUM = 0,
    MOTOR1_NUM,
    MOTOR2_NUM,
    MOTOR3_NUM,
    MOTOR4_NUM,
    MOTOR5_NUM,
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    MOTOR6_NUM,
    MOTOR7_NUM,
#endif
    MOTOR_NUM_MAX,
};

/**
 * @brief 
 * @param[out] dir 
 * @param[in] flags 
 * @param[in] data 
 * @param[in,out] pid 
 */
void input2dir_get(dir_t *dir, lock_flags_t flags, input_data_t data,
                        PIDController *pid[MAX_PID_NUM]);

/**
 * @brief 
 * @param[out] output 
 * @param[in] frame_factor 
 * @param[in] thruster_config 
 * @param[in] dir 
 */
void dir2output_get(double output[MOTOR_NUM_MAX],
                    frame_factor_t *frame_factor,
                    thrusters_params *thruster_config,
                    dir_t dir);

/**
 * @brief 
 * @param[out] msg 
 * @param[in] output 
 */
void output2send_get(ThrusterCommand *msg, double output[MOTOR_NUM_MAX]);

#endif /* SUB_NAVI_CONTROL_H */
