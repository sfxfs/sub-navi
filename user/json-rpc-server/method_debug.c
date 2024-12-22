#include <stdlib.h>

#include "csv-json-config-sys/json_thruster.h"

#include "method_debug.h"

cJSON *get_thruster_config_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    thrusters_params * config = thruster_read_from_file();
    cJSON *json = thruster_s2j(config);
    free(config);
    return json;
}
