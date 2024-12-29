#ifndef SUB_NAVI_USER_RPC_METHOD_INFO_H
#define SUB_NAVI_USER_RPC_METHOD_INFO_H

#include "mjsonrpc.h"

struct method_get_info
{
    int code_name;
    float yaw;
    float depth;
    float temp;
};
extern struct method_get_info g_get_info_data;
cJSON *get_info_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id);

#endif
