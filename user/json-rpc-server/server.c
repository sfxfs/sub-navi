#include "method_info.h"
#include "method_ctrl.h"
#include "method_debug.h"
#include "jsonrpc-c.h"

#include "server.h"

#define SERVER_DEBUG_LEVEL 0

static struct jrpc_server server_handle;

int navi_server_init(int port)
{
    if (jrpc_server_init(&server_handle, port, SERVER_DEBUG_LEVEL) != 0)
        return -1;
    // mjrpc_add_method ...
    return 0;
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
