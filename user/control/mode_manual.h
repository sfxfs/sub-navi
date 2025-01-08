#ifndef SUB_NAVI_CONTROL_MODE_MANUAL_H
#define SUB_NAVI_CONTROL_MODE_MANUAL_H

#include "navi-type.h"

#include "csv-json-config-sys/csv_frame_factor.h"
#include "csv-json-config-sys/json_thruster.h"

navi_ret_t control_manual_init(frame_factor_t *frame_factor, thrusters_params *thruster_config);

#endif //SUB_NAVI_CONTROL_MODE_MANUAL_H
