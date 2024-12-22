#include <stdlib.h>

#include "csv-json-config-sys/json_thruster.h"

#include "method_debug.h"

cJSON *get_thruster_config_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    thrusters_params *data = ctx->data;

    return thruster_s2j(data);
}

cJSON *set_thruster_config_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    thrusters_params **data = ctx->data;

    thrusters_params * thruster_config = thruster_j2s(params);
    if (thruster_config)
    {
        thrusters_params **copy = data;
        *data = thruster_config;
        free(*copy);
    }
    return cJSON_CreateNull();
}

cJSON *save_thruster_config_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    thrusters_params *data = ctx->data;
    thruster_write_to_file(data);
    return cJSON_CreateNull();
}
