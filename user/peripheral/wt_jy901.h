#ifndef SUB_NAVI_USER_PERIPH_JY901_H
#define SUB_NAVI_USER_PERIPH_JY901_H

#include "navi-type.h"

typedef struct wt_jy901_data
{
    float Acc[3];
    float Gyro[3];
    float Angle[3];
} wt_jy901_data_t;
extern wt_jy901_data_t g_jy901_data;

navi_ret_t navi_jy901_init(void);

#endif
