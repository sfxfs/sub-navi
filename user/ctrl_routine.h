#ifndef SUB_NAVI_CTRL_ROUTINE_H
#define SUB_NAVI_CTRL_ROUTINE_H

#include "navi-type.h"

#include "control/control.h"
#include "csv-json-config-sys/json_thruster.h"
#include "csv-json-config-sys/csv_frame_factor.h"

typedef struct _ctrl_routine_arg
{
    frame_factor_t *frame_factor;
    thrusters_params *thruster_config;
    input_data_t *input_data;
    lock_flags_t *lock_flags;
} ctrl_routine_arg_t;

navi_ret_t navi_ctrl_routine_start(ctrl_routine_arg_t *arg);

#endif // SUB_NAVI_CTRL_ROUTINE_H
