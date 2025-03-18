#ifndef SUB_NAVI_JRPC_SERVER_H
#define SUB_NAVI_JRPC_SERVER_H

#include <stdint.h>

#include "navi-type.h"
#include "control/control.h"

typedef struct _jrpc_server_arg
{
    input_data_t *input_data;
} jrpc_server_arg_t;

navi_ret_t navi_jrpc_server_start(jrpc_server_arg_t *arg);
navi_ret_t navi_jrpc_server_stop(void);

#endif  // SUB_NAVI_JRPC_SERVER_H
