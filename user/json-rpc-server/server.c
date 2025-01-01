#include "log.h"
#include "jsonrpc-c.h"

#include "method_info.h"
#include "method_ctrl.h"
#include "method_debug.h"

#include "navi-config.h"

#include "server.h"

static struct jrpc_server server_handle;

static cJSON *empty_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    return cJSON_CreateNull();
}

int navi_server_init(thrusters_params *thruster_config)
{
    if (jrpc_server_init(&server_handle, SUB_NAVI_CONFIG_RPC_SERVER_PORT) != 0)
        return NAVI_RET_FAIL;

    int ret = NAVI_RET_SUCCESS;
    // new methods
    ret += mjrpc_add_method(&server_handle.rpc_handle, get_thruster_config_handler, "get_thruster_config", thruster_config);
    ret += mjrpc_add_method(&server_handle.rpc_handle, set_thruster_config_handler, "set_thruster_config", thruster_config);
    ret += mjrpc_add_method(&server_handle.rpc_handle, save_thruster_config_handler, "save_thruster_config", thruster_config);

    // add old essential methods
    ret += mjrpc_add_method(&server_handle.rpc_handle, move_handler, "move", NULL);
    ret += mjrpc_add_method(&server_handle.rpc_handle, empty_handler, "catch", NULL);
    ret += mjrpc_add_method(&server_handle.rpc_handle, empty_handler, "light", NULL);
    ret += mjrpc_add_method(&server_handle.rpc_handle, get_info_handler, "get_info", NULL);
    ret += mjrpc_add_method(&server_handle.rpc_handle, empty_handler, "set_direction_locked", NULL);
    ret += mjrpc_add_method(&server_handle.rpc_handle, empty_handler, "set_depth_locked", NULL);

    // old addional methods (configuration, data feedback, etc.)
    // ...
    log_info("rpc server will start on port %d.", SUB_NAVI_CONFIG_RPC_SERVER_PORT);
    return ret;
}

int navi_server_deinit(void)
{
    return mjrpc_del_method(&server_handle.rpc_handle, NULL);
}
