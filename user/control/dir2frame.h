#ifndef SUB_NAVI_CONTROL_DIR2FRAME_H
#define SUB_NAVI_CONTROL_DIR2FRAME_H

#include "csv-json-config-sys/json_thruster.h"
#include "csv-json-config-sys/csv_frame_factor.h"

#include "rpc2dir.h"

void dir2frame_get(float throttle[FRAME_FACTOR_ROW_MAX],
                    frame_factor_t factor, thrusters_params *thruster_config,
                    dir_t dir);

#endif // SUB_NAVI_CONTROL_DIR2FRAME_H