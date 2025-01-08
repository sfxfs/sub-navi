#ifndef SUB_NAVI_USER_RPC_SERVER_H
#define SUB_NAVI_USER_RPC_SERVER_H

#include "navi-type.h"

#include "csv-json-config-sys/json_thruster.h"

navi_ret_t navi_server_init(thrusters_params *thruster_config);

navi_ret_t navi_server_deinit(void);

#endif