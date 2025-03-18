#include "log.h"
#include "jsonrpc-c.h"

#include "control/control_jrpc_func.h"
#include "peripheral/peripheral_jrpc_func.h"
#include "csv-json-config-sys/config_sys_jrpc_func.h"

#include "navi-config.h"

#include "jrpc_server.h"

static struct jrpc_server server_handle;

static cJSON *empty_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    return cJSON_CreateNull();
}

navi_ret_t navi_jrpc_server_start(jrpc_server_arg_t *arg)
{
    if (jrpc_server_init(&server_handle, SUB_NAVI_CONFIG_RPC_SERVER_PORT) != 0)
        return NAVI_RET_FAIL;

    if (MJRPC_RET_OK != mjrpc_add_method(&server_handle.rpc_handle, axis_handler, "axis", arg->input_data))
        return NAVI_RET_FAIL;

    // ...

    log_info("json rpc server started on \"http://localhost:%d\".", SUB_NAVI_CONFIG_RPC_SERVER_PORT);
    return NAVI_RET_SUCCESS;
}

navi_ret_t navi_jrpc_server_stop(void)
{
    if (MJRPC_RET_OK == mjrpc_del_method(&server_handle.rpc_handle, NULL))
    {
        log_info("json rpc server stopped.");
        return NAVI_RET_SUCCESS;
    }
    log_warn("json rpc server stop failed.");
    return NAVI_RET_FAIL;
}
