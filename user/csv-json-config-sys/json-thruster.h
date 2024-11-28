#ifndef SUB_NAVI_CFG_JSON_THRUSTER_H
#define SUB_NAVI_CFG_JSON_THRUSTER_H

#include <stdint.h>
#include <stdbool.h>

#include "navi-config.h"

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


thrusters_params *thruster_create_with_init_val (void);

void *thruster_s2j (thrusters_params *params);

thrusters_params *thruster_j2s (void *json);

#endif
