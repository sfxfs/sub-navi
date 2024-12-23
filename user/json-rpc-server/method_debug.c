#include <stdlib.h>
#include <string.h>

#include "log.h"

#include "csv-json-config-sys/json_thruster.h"

#include "method_debug.h"

cJSON *get_thruster_config_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    thrusters_params *data = ctx->data;

    return thruster_s2j(data);
}

cJSON *set_thruster_config_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    thrusters_params *data = ctx->data;

    thrusters_params * thruster_config = thruster_j2s(params);
    if (thruster_config)
    {
        log_debug("thruster_config get.");
        memcpy(data, thruster_config, sizeof(thrusters_params));
        free(thruster_config);
    }
    return cJSON_CreateNull();
}

cJSON *save_thruster_config_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    thrusters_params *data = ctx->data;
    thruster_write_to_file(data);
    return cJSON_CreateNull();
}
