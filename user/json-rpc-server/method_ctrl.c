#include "method_ctrl.h"

struct method_move g_move_data;

static double cjson_value_analysis_double(cJSON *params,const char *str)
{
    cJSON* cjson_temp = NULL;
    double value;
    cjson_temp = cJSON_GetObjectItem(params, str);
    if (cjson_temp != NULL)
    {
        value = cjson_temp->valuedouble;
    }
    else value = 0;
    return value;
}

cJSON *move_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    g_move_data.x = cjson_value_analysis_double(params, "x");
    g_move_data.y = cjson_value_analysis_double(params, "y");
    g_move_data.z = cjson_value_analysis_double(params, "z");
    g_move_data.r = cjson_value_analysis_double(params, "rot");
    g_move_data.updated = true;
    return cJSON_CreateNull();
}