#include <stdlib.h>

#include "ev.h"
#include "log.h"

#include "navi-type.h"
#include "navi-config.h"

#include "arg_parse.h"
#include "jrpc_server.h"
#include "ctrl_routine.h"

#include "csv-json-config-sys/json_thruster.h"
#include "csv-json-config-sys/csv_frame_factor.h"

// init order
// 1. csv config
// 2. json config
// 3. peripherals
// 4. control algorithm
// 5. protobuf rpc p2p
// 6. json rpc server
static int init(void)
{
    // 1. csv config
    if (frame_factor_check_file_exist() != true)
    {
        log_error("frame factor file does not exist! path: %s",
                  SUB_NAVI_CONFIG_FRAME_FACTOR_FILE_PATH);
        return NAVI_RET_FAIL;
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
        if (thruster_write_to_file(default_config) == 0)
            log_info("default thruster config file generated successfully.");
        free(default_config);
    }
    else
        thruster_config = thruster_read_from_file();

    // ...

    if (json_file_error == true)
    {
        log_warn("this is your first time running the program. "
                 "please modify the config file and restart the program.");
        return NAVI_RET_FAIL;
    }

    // 3. peripherals

    // ...

    // 4. control algorithm

    // ...

    // 5. protobuf rpc p2p

    // ...

    // 6. json rpc server

    if (navi_jrpc_server_start() != NAVI_RET_SUCCESS)
    {
        log_error("navi jsonrpc server start failed!");
        return NAVI_RET_FAIL;
    }

    // end ...
    return NAVI_RET_SUCCESS;
}

static void sigint_cb(EV_P_ ev_signal *w, int revents)
{
    log_debug("catch SIGINT, signal number %d.\n", w->signum);
    // peripherals deinit ...
    exit(EXIT_SUCCESS);
}

int main(int argc, const char *argv[])
{
    // check whether the program is run as root
    if (geteuid() != 0)
    {
        printf("error: please run as root user...\n");
        exit(EXIT_FAILURE);
    }

    // log save to file
    log_set_level(SUB_NAVI_CONFIG_LOG_LEVEL);
    FILE *logfile = fopen(SUB_NAVI_CONFIG_LOG_OUTPUT_FILE_PATH, "a+");
    log_add_fp(logfile, LOG_WARN);

    // parse arguments (if have)
    if (argc > 1)
    {
        if (navi_parse_arguments(argc, argv) != NAVI_RET_SUCCESS)
        {
            log_error("parse arguments failed!");
            return 1;
        }
        log_info("cmd send successfully!");
        return 0;
    }

    // welcome info
    printf("  ___ _   _ ___     _  _   ___   _____ \n"
           " / __| | | | _ )___| \\| | /_\\ \\ / /_ _|\n"
           " \\__ \\ |_| | _ \\___| .` |/ _ \\ V / | | \n"
           " |___/\\___/|___/   |_|\\_/_/ \\_\\_/ |___|\n"
           "                                       \n");
    printf("SUB-NAVI log level set to: %s, log history save to: %s\n\n",
           log_level_string(SUB_NAVI_CONFIG_LOG_LEVEL),
           SUB_NAVI_CONFIG_LOG_OUTPUT_FILE_PATH);

    // init sub-navi
    if (NAVI_RET_SUCCESS != init())
    {
        log_error("sub-navi init failed!");
        return 1;
    }

    // signal ev handle
    EV_P = EV_DEFAULT;
    ev_signal wsig;
    ev_signal_init(&wsig, sigint_cb, SIGINT);
    ev_signal_start(EV_A_ & wsig);

    // event loop
    ev_loop(EV_A_ 0);

    return 0;
}
