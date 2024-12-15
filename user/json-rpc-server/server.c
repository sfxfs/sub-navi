#include "method_info.h"
#include "method_ctrl.h"
#include "method_debug.h"
#include "jsonrpc-c.h"

#include "server.h"

#define SERVER_DEBUG_LEVEL 0

static struct jrpc_server server_handle;

static cJSON *empty_handler(mjrpc_ctx_t *ctx, cJSON *params, cJSON *id)
{
    return cJSON_CreateNull();
}

int navi_server_init(int port)
{
    if (jrpc_server_init(&server_handle, port, SERVER_DEBUG_LEVEL) != 0)
        return -1;

    int ret = 0;
    // add essential methods
    ret += mjrpc_add_method(&server_handle.rpc_handle, move_handler, "move", NULL);
    ret += mjrpc_add_method(&server_handle.rpc_handle, empty_handler, "catch", NULL);
    ret += mjrpc_add_method(&server_handle.rpc_handle, empty_handler, "light", NULL);
    ret += mjrpc_add_method(&server_handle.rpc_handle, get_info_handler, "get_info", NULL);
    ret += mjrpc_add_method(&server_handle.rpc_handle, empty_handler, "set_direction_locked", NULL);
    ret += mjrpc_add_method(&server_handle.rpc_handle, empty_handler, "set_depth_locked", NULL);

    // addional methods (configuration, data feedback, etc.)
    // ...
    return ret;
}

int navi_server_run(void)
{
    jrpc_server_run(&server_handle);
    return 0;
}

int navi_server_stop(void)
{
    jrpc_server_stop(&server_handle);
    return mjrpc_del_method(&server_handle.rpc_handle, NULL);
}
