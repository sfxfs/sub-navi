#include <stdio.h>
#include <stdlib.h>

#include "s2j.h"
#include "json_file.h"

#include "json_thruster.h"

static const thrusters_params default_params = {
    .thruster_0 = {
        .channel = 0,
        .enabled = true,
        .reversed = false,
        .deadzone_p = 0.0,
        .deadzone_n = 0.0,
        .power_pLimit = 0.5,
        .power_nLimit = 0.5,
    },
    .thruster_1 = {
        .channel = 1,
        .enabled = true,
        .reversed = false,
        .deadzone_p = 0.0,
        .deadzone_n = 0.0,
        .power_pLimit = 0.5,
        .power_nLimit = 0.5,
    },
    .thruster_2 = {
        .channel = 2,
        .enabled = true,
        .reversed = false,
        .deadzone_p = 0.0,
        .deadzone_n = 0.0,
        .power_pLimit = 0.5,
        .power_nLimit = 0.5,
    },
    .thruster_3 = {
        .channel = 3,
        .enabled = true,
        .reversed = false,
        .deadzone_p = 0.0,
        .deadzone_n = 0.0,
        .power_pLimit = 0.5,
        .power_nLimit = 0.5,
    },
    .thruster_4 = {
        .channel = 4,
        .enabled = true,
        .reversed = false,
        .deadzone_p = 0.0,
        .deadzone_n = 0.0,
        .power_pLimit = 0.5,
        .power_nLimit = 0.5,
    },
    .thruster_5 = {
        .channel = 5,
        .enabled = true,
        .reversed = false,
        .deadzone_p = 0.0,
        .deadzone_n = 0.0,
        .power_pLimit = 0.5,
        .power_nLimit = 0.5,
    },
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    .thruster_6 = {
        .channel = 6,
        .enabled = true,
        .reversed = false,
        .deadzone_p = 0.0,
        .deadzone_n = 0.0,
        .power_pLimit = 0.5,
        .power_nLimit = 0.5,
    },
    .thruster_7 = {
        .channel = 7,
        .enabled = true,
        .reversed = false,
        .deadzone_p = 0.0,
        .deadzone_n = 0.0,
        .power_pLimit = 0.5,
        .power_nLimit = 0.5,
    },
#endif
};

thrusters_params *thruster_create_with_init_val(void)
{
    thrusters_params *params_struct = calloc(1, sizeof(thrusters_params));

    if (params_struct)
        *params_struct = default_params;

    return params_struct;
}

static cJSON *struct_to_json_thruster_attr(thruster_attr *attr_struct)
{
    s2j_create_json_obj(attr_json);

    s2j_json_set_basic_element(attr_json, attr_struct, int, reversed);
    s2j_json_set_basic_element(attr_json, attr_struct, int, enabled);
    s2j_json_set_basic_element(attr_json, attr_struct, int, channel);
    s2j_json_set_basic_element(attr_json, attr_struct, double, deadzone_p);
    s2j_json_set_basic_element(attr_json, attr_struct, double, deadzone_n);
    s2j_json_set_basic_element(attr_json, attr_struct, double, power_pLimit);
    s2j_json_set_basic_element(attr_json, attr_struct, double, power_nLimit);

    return attr_json;
}

void *thruster_s2j(thrusters_params *params)
{
    if (params == NULL)
        return NULL;
    s2j_create_json_obj(json_params);

    s2j_json_set_struct_element_by_func(json_params, params,
                                        thruster_attr, thruster_0);
    s2j_json_set_struct_element_by_func(json_params, params,
                                        thruster_attr, thruster_1);
    s2j_json_set_struct_element_by_func(json_params, params,
                                        thruster_attr, thruster_2);
    s2j_json_set_struct_element_by_func(json_params, params,
                                        thruster_attr, thruster_3);

    s2j_json_set_struct_element_by_func(json_params, params,
                                        thruster_attr, thruster_4);
    s2j_json_set_struct_element_by_func(json_params, params,
                                        thruster_attr, thruster_5);
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    s2j_json_set_struct_element_by_func(json_params, params,
                                        thruster_attr, thruster_6);
    s2j_json_set_struct_element_by_func(json_params, params,
                                        thruster_attr, thruster_7);
#endif

    return json_params;
}

static thruster_attr *json_to_struct_thruster_attr(cJSON *attr_json)
{
    s2j_create_struct_obj(attr_struct, thruster_attr);

    s2j_struct_get_basic_element(attr_struct, attr_json, int, reversed);
    s2j_struct_get_basic_element(attr_struct, attr_json, int, enabled);
    s2j_struct_get_basic_element(attr_struct, attr_json, int, channel);
    s2j_struct_get_basic_element(attr_struct, attr_json, double, deadzone_p);
    s2j_struct_get_basic_element(attr_struct, attr_json, double, deadzone_n);
    s2j_struct_get_basic_element(attr_struct, attr_json, double, power_pLimit);
    s2j_struct_get_basic_element(attr_struct, attr_json, double, power_nLimit);

    return attr_struct;
}

thrusters_params *thruster_j2s(void *json)
{
    if (json == NULL)
        return NULL;
    s2j_create_struct_obj(params_struct, thrusters_params);
    (void)json_temp;

    s2j_struct_get_struct_element_by_func(params_struct, json,
                                          thruster_attr, thruster_0);
    s2j_struct_get_struct_element_by_func(params_struct, json,
                                          thruster_attr, thruster_1);
    s2j_struct_get_struct_element_by_func(params_struct, json,
                                          thruster_attr, thruster_2);
    s2j_struct_get_struct_element_by_func(params_struct, json,
                                          thruster_attr, thruster_3);

    s2j_struct_get_struct_element_by_func(params_struct, json,
                                          thruster_attr, thruster_4);
    s2j_struct_get_struct_element_by_func(params_struct, json,
                                          thruster_attr, thruster_5);
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    s2j_struct_get_struct_element_by_func(params_struct, json,
                                          thruster_attr, thruster_6);
    s2j_struct_get_struct_element_by_func(params_struct, json,
                                          thruster_attr, thruster_7);
#endif

    return params_struct;
}

int thruster_write_to_file(thrusters_params *params)
{
    if (params == NULL)
        return -1;
    cJSON *json = thruster_s2j(params);
    char *str = cJSON_Print(json);
    int ret = navi_write_to_file(SUB_NAVI_CONFIG_THRUSTER_CONFIG_FILE_PATH, str);
    free(str);
    cJSON_Delete(json);
    return ret;
}

thrusters_params *thruster_read_from_file()
{
    char *str = navi_read_from_file(SUB_NAVI_CONFIG_THRUSTER_CONFIG_FILE_PATH);
    cJSON *json = cJSON_Parse(str);
    thrusters_params *ret = thruster_j2s(json);
    free(str);
    cJSON_Delete(json);
    return ret;
}
