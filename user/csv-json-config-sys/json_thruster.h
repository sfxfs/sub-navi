#ifndef SUB_NAVI_CFG_JSON_THRUSTER_H
#define SUB_NAVI_CFG_JSON_THRUSTER_H

#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#include "navi-config.h"

// all the parameters are > 0
// p stands for positive throttle (not about actual thruster forward, or the robot movement)
// n stands for negative
typedef struct
{
    bool enabled;
    bool reversed; // 先乘反转后再乘其他值
    int channel; // 0 ~ 7
    double deadzone_p; // 0 ~ 1.0
    double deadzone_n; // 0 ~ 1.0
    double power_pLimit; // 0 ~ 1.0
    double power_nLimit; // 0 ~ 1.0
} thruster_attr;

typedef struct
{
    thruster_attr thruster_0;
    thruster_attr thruster_1;
    thruster_attr thruster_2;
    thruster_attr thruster_3;
    thruster_attr thruster_4;
    thruster_attr thruster_5;
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    thruster_attr thruster_6;
    thruster_attr thruster_7;
#endif
} thrusters_params;

#define thruster_check_file_exist() (access(SUB_NAVI_CONFIG_THRUSTER_CONFIG_FILE_PATH, F_OK) == 0)

/// @brief get the default value of thruster parameters
/// @return thrusters_params (need to free)
thrusters_params *thruster_create_with_init_val(void);

/// @brief convert the struct to json
/// @param params struct
/// @return cJSON (need to free)
void *thruster_s2j(thrusters_params *params);

/// @brief convert the json to struct
/// @param json cJSON struct
/// @return thrusters_params (need to free)
thrusters_params *thruster_j2s(void *json);

/// @brief write config to file
/// @param params config struct
/// @return 0 if success
int thruster_write_to_file(thrusters_params *params);

/// @brief read config from file
/// @return config struct (need to free)
thrusters_params *thruster_read_from_file(void);

#endif
