#ifndef SUB_NAVI_CFG_JSON_THRUSTER_H
#define SUB_NAVI_CFG_JSON_THRUSTER_H

#include <stdint.h>
#include <stdbool.h>

#include "navi-config.h"

enum thruster_index
{
    THRUSTER_0 = 0,
    THRUSTER_1,
    THRUSTER_2,
    THRUSTER_3,
    THRUSTER_4,
    THRUSTER_5,
#if SUB_NAVI_CONFIG_USE_EIGHT_THRUSTERS
    THRUSTER_6,
    THRUSTER_7,
#endif
    THRUSTER_MAX
};

// all the parameters are > 0
// p stands for positive throttle (not about actual thruster forward, or the robot movement)
// n stands for negative
typedef struct
{
    bool enabled;
    bool reversed;  // 先乘反转后再乘其他值
    uint16_t channel;
    uint16_t deadzone_p;
    uint16_t deadzone_n;
    double power_pLimit; // 0 ~ 1.0
    double power_nLimit; // 0 ~ 1.0
} thruster_attr;

typedef struct
{
    thruster_attr thrusters[THRUSTER_MAX];
} thrusters_params;


thrusters_params *thruster_create_with_init_val (void);

void *thruster_s2j (thrusters_params *params);

thrusters_params *thruster_j2s (void *json);

#endif
