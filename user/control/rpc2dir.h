#ifndef SUB_NAVI_CONTROL_RPC2DIR_H
#define SUB_NAVI_CONTROL_RPC2DIR_H

#include "pid.h"
#include "mjsonrpc.h"

typedef struct _dir_t
{
    double roll;
    double pitch;
    double yaw;
    double throttle; // depth
    double forward;
    double lateral;
} dir_t;

typedef struct _move_data_t
{
    double x, y, z, r;
    float roll, pitch, yaw, depth;
    bool updated;
} move_data_t;

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

/**
 * @brief 
 * @param dir 
 * @param flags 
 * @param move_data 
 * @param pid 
 * @return 
 */
void rpc2dir_get(dir_t *dir, lock_flags_t flags, move_data_t move_data,
                        PIDController *pid[MAX_PID_NUM]);

#endif // SUB_NAVI_CONTROL_RPC2DIR_H
