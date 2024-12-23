#ifndef SUB_NAVI_USER_RPC_SERVER_H
#define SUB_NAVI_USER_RPC_SERVER_H

#include "csv-json-config-sys/json_thruster.h"

int navi_server_init(thrusters_params **thruster_config);

int navi_server_deinit(void);

#endif