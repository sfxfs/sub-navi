#include <stdio.h>

#include "method_info.h"

struct method_get_info get_info_data;

cJSON *get_info_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    char temp_str[10] = {0};
    cJSON *cjson_info = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_info, "Model", "Navi");

    sprintf(temp_str, "%.02f", get_info_data.yaw);
    cJSON_AddStringToObject(cjson_info, "Yaw", temp_str);

    sprintf(temp_str, "%.02f", get_info_data.depth);
    cJSON_AddStringToObject(cjson_info, "Depth", temp_str);

    sprintf(temp_str, "%.02f", get_info_data.temp);
    cJSON_AddStringToObject(cjson_info, "Temp", temp_str);

    return cjson_info;
}
