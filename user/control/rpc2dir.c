#include <stdbool.h>
#include <string.h>

#include "rpc2dir.h"

void rpc2dir_get(dir_t *dir, lock_flags_t flags, move_data_t move_data,
                 PIDController *pid[MAX_PID_NUM])
{
    static float roll_ex = 0, pitch_ex = 0, yaw_ex = 0, throttle_ex = 0;
    bzero(dir, sizeof(dir_t));

    if (move_data.updated)
    {
        dir->forward += move_data.y;
        dir->lateral += move_data.x;
        dir->throttle += move_data.z;
        dir->yaw += move_data.r;
    }

    if (flags.roll_locked == true)
        dir->roll += PIDController_Update(pid[ROLL_PID], roll_ex, move_data.roll);
    else
        roll_ex = move_data.roll;

    if (flags.pitch_locked == true)
        dir->pitch += PIDController_Update(pid[PITCH_PID], pitch_ex, move_data.pitch);
    else
        pitch_ex = move_data.pitch;

    if (flags.yaw_locked == true)
        dir->yaw += PIDController_Update(pid[YAW_PID], yaw_ex, move_data.yaw);
    else
        yaw_ex = move_data.yaw;

    if (flags.throttle_locked == true)
        dir->throttle += PIDController_Update(pid[THROTTLE_PID], throttle_ex, move_data.depth);
    else
        throttle_ex = move_data.depth;
}
