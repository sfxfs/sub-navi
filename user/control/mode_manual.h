#ifndef SUB_NAVI_CONTROL_MODE_MANUAL_H
#define SUB_NAVI_CONTROL_MODE_MANUAL_H

#include "csv-json-config-sys/csv_frame_factor.h"
#include "csv-json-config-sys/json_thruster.h"

int control_manual_start_thread(frame_factor_t *frame_factor, thrusters_params *thruster_config);

#endif //SUB_NAVI_CONTROL_MODE_MANUAL_H
