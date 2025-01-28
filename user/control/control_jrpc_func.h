#ifndef SUB_NAVI_CONTROL_JRPC_FUNC_H
#define SUB_NAVI_CONTROL_JRPC_FUNC_H

#include "mjsonrpc.h"

// method name 'axis'
// param 'input_data_t'
// return 'cJSON_Null'
cJSON *axis_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id);

#endif //SUB_NAVI_CONTROL_JRPC_FUNC_H
