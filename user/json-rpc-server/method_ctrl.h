#ifndef SUB_NAVI_USER_RPC_METHOD_CTRL_H
#define SUB_NAVI_USER_RPC_METHOD_CTRL_H

#include <stdbool.h>
#include "mjsonrpc.h"

struct method_move
{
    double x;
    double y;
    double z;
    double r;
    bool updated;
};
extern struct method_move move_data;
cJSON *move_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id);

#endif
