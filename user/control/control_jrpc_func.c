#include "control.h"

#include "control_jrpc_func.h"

cJSON *axis_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    input_data_t *data = (input_data_t *)ctx->data;

    // update values
    double lx = cJSON_GetNumberValue(cJSON_GetObjectItem(params, "lx"));
    if (lx == lx)   // NaN check
        data->lx = lx;
    double ly = cJSON_GetNumberValue(cJSON_GetObjectItem(params, "ly"));
    if (ly == ly)
        data->ly = ly;
    double rx = cJSON_GetNumberValue(cJSON_GetObjectItem(params, "rx"));
    if (rx == rx)
        data->rx = rx;
    double ry = cJSON_GetNumberValue(cJSON_GetObjectItem(params, "ry"));
    if (ry == ry)
        data->ry = ry;
    double lt = cJSON_GetNumberValue(cJSON_GetObjectItem(params, "lt"));
    if (lt == lt)
        data->lt = lt;
    double rt = cJSON_GetNumberValue(cJSON_GetObjectItem(params, "rt"));
    if (rt == rt)
        data->rt = rt;

    return cJSON_CreateNull();
}
