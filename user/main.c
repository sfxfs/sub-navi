#include <stdlib.h>
#include "log.h"

#include "json-rpc-server/server.h"
#include "csv-json-config-sys/json_thruster.h"
#include "csv-json-config-sys/csv_frame_factor.h"

// init order
// 1. csv config
// 2. json config
// 3. peripherals
// 4. control algorithm
// 5. json rpc server
static int init(void)
{
    // 1. csv config
    if (frame_factor_check_file_exist() != true)
    {
        log_error("frame factor file not exist! path: %s",
                    SUB_NAVI_CONFIG_FRAME_FACTOR_FILE_PATH);
        return -1;
    }

    frame_factor_t *frame_factor = frame_factor_get_from_file();

    // 2. json config
    bool json_file_error = false;

    thrusters_params *thruster_config = NULL;
    if (thruster_check_file_exist() != true)
    {
        log_warn("thruster file dose not exist! path: %s",
                    SUB_NAVI_CONFIG_THRUSTER_CONFIG_FILE_PATH);
        json_file_error = true;

        thrusters_params *default_config = thruster_create_with_init_val();
        thruster_write_to_file(default_config);
        free(default_config);
    }
    else
        thruster_config = thruster_read_from_file();
    
    // ...

    if (json_file_error == true)
    {
        log_error("this is your first time running the program. "
                    "please modify the config file and restart the program.");
        return -1;
    }

    // 3. peripherals

    // ...

    // 4. control algorithm

    // ...

    // 5. json rpc server
    if (0 != navi_server_init(&thruster_config))
    {
        log_error("jsonrpc server init failed!");
        // peripherals deinit ...
        return -1;
    }
    if (navi_server_start_thread() == 0)
        log_info("rpc server started on port %d.", SUB_NAVI_CONFIG_RPC_SERVER_PORT);
    else
    {
        log_error("rpc server start failed!");
        // peripherals deinit ...
        return -1;
    }

    // end ...
    return 0;
}

int main(int argc, const char *argv[])
{
    log_set_level(SUB_NAVI_CONFIG_LOG_LEVEL);
    if (0 != init())
        exit(EXIT_FAILURE);
    for(;;);
    return 0;
}
