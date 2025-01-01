#include <stdlib.h>

#include "ev.h"
#include "log.h"

#include "control/mode_manual.h"
#include "protobuf-commu/union.h"
#include "protobuf-commu/proto_def/navi_master.pb.h"
#include "json-rpc-server/server.h"
#include "csv-json-config-sys/json_thruster.h"
#include "csv-json-config-sys/csv_frame_factor.h"

static navi_ret_t parseArguments(int argc, const char *argv[])
{
    if (argc != 5)
    {
        log_info("Usage: %s <serial_port_path> <communication_mode> <channel> <value>", argv[0]);
        return NAVI_RET_ARG_ERROR;
    }

    const char *serialPort = argv[1];
    const char *mode = argv[2];
    int channel = atoi(argv[3]);
    int value = atoi(argv[4]);

    log_info("Serial Port: %d", serialPort);
    int uart = protobuf_commu_intf_init_cust(serialPort, SUB_NAVI_CONFIG_PROTOBUF_UART_BAUDRATE);
    if (uart < 0)
    {
        log_error("Serial Port init failed!");
        return NAVI_RET_FAIL;
    }

    if (strncmp(mode, "dshot", 4) == 0)
    {
        log_info("Communication Mode: DShot");
        log_info("Channel: %d", channel);
        log_info("Value: %d", value);
        ThrusterCommand msg = {0};
        switch (channel)
        {
        case 0:msg.has_throttle0 = true; msg.throttle0 = value;break;
        case 1:msg.has_throttle1 = true; msg.throttle1 = value;break;
        case 2:msg.has_throttle2 = true; msg.throttle2 = value;break;
        case 3:msg.has_throttle3 = true; msg.throttle3 = value;break;
        case 4:msg.has_throttle4 = true; msg.throttle4 = value;break;
        case 5:msg.has_throttle5 = true; msg.throttle5 = value;break;
        case 6:msg.has_throttle6 = true; msg.throttle6 = value;break;
        case 7:msg.has_throttle7 = true; msg.throttle7 = value;break;
        default:log_error("Invalid channel number!");return 1;break;
        }
        if (0 != protobuf_commu_send_cmd_cust(uart, ThrusterCommand_fields, &msg))
        {
            log_error("protobuf_commu_send_cmd_cust failed!");
            close(uart);
            return NAVI_RET_FAIL;
        }
    }
    else if (strncmp(mode, "pwm", 3) == 0)
    {
        log_info("Communication Mode: PWM");
        log_info("Channel: %d", channel);
        log_info("Value: %d", value);
        PWMDevCommand msg = {0};
        switch (channel)
        {
        case 0:msg.has_duty0 = true; msg.duty0 = value;break;
        case 1:msg.has_duty1 = true; msg.duty1 = value;break;
        case 2:msg.has_duty2 = true; msg.duty2 = value;break;
        case 3:msg.has_duty3 = true; msg.duty3 = value;break;
        case 4:msg.has_duty4 = true; msg.duty4 = value;break;
        case 5:msg.has_duty5 = true; msg.duty5 = value;break;
        case 6:msg.has_duty6 = true; msg.duty6 = value;break;
        case 7:msg.has_duty7 = true; msg.duty7 = value;break;
        case 8:msg.has_duty8 = true; msg.duty8 = value;break;
        case 9:msg.has_duty9 = true; msg.duty9 = value;break;
        case 10:msg.has_duty10 = true; msg.duty10 = value;break;
        case 11:msg.has_duty11 = true; msg.duty11 = value;break;
        case 12:msg.has_duty12 = true; msg.duty12 = value;break;
        case 13:msg.has_duty13 = true; msg.duty13 = value;break;
        case 14:msg.has_duty14 = true; msg.duty14 = value;break;
        case 15:msg.has_duty15 = true; msg.duty15 = value;break;
        default:log_error("Invalid channel number!");return 1;break;
        }
        if (0 != protobuf_commu_send_cmd_cust(uart, PWMDevCommand_fields, &msg))
        {
            log_error("protobuf_commu_send_cmd_cust failed!");
            close(uart);
            return NAVI_RET_FAIL;
        }
    }
    else
    {
        log_error("Invalid communication mode. Use 'dshot' or 'pwm'.");
        close(uart);
        return NAVI_RET_FAIL;
    }
    return NAVI_RET_SUCCESS;
}

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
        log_error("frame factor file not exist! path: %s",
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
        thruster_write_to_file(default_config);
        free(default_config);
    }
    else
        thruster_config = thruster_read_from_file();

    // ...

    if (json_file_error == true)
    {
        log_info("this is your first time running the program. "
                  "please modify the config file and restart the program.");
        return NAVI_RET_FAIL;
    }

    // 3. peripherals

    // ...

    // 4. control algorithm

    if (0 != control_manual_init(frame_factor, thruster_config))
    {
        log_error("control manual init failed!");
        // peripherals deinit ...
        return NAVI_RET_FAIL;
    }

    // 5. protobuf rpc p2p
    if (0 != protobuf_commu_init())
    {
        log_error("protobuf rpc init failed!");
        // peripherals deinit ...
        return NAVI_RET_FAIL;
    }

    // 6. json rpc server
    if (0 != navi_server_init(thruster_config))
    {
        log_error("jsonrpc server init failed!");
        // peripherals deinit ...
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
    log_set_level(SUB_NAVI_CONFIG_LOG_LEVEL);
    // log save to file
    FILE *logfile = fopen(SUB_NAVI_CONFIG_LOG_OUTPUT_FILE_PATH, "a+");
    log_add_fp(logfile, LOG_WARN);

    if (argc > 1)
        return parseArguments(argc, argv);

    printf("  ___ _   _ ___     _  _   ___   _____ \n"
           " / __| | | | _ )___| \\| | /_\\ \\ / /_ _|\n"
           " \\__ \\ |_| | _ \\___| .` |/ _ \\ V / | | \n"
           " |___/\\___/|___/   |_|\\_/_/ \\_\\_/ |___|\n"
           "                                       \n");
    printf("SUB-NAVI log level set to: %s, log history save to: %s\n\n",
            log_level_string(SUB_NAVI_CONFIG_LOG_LEVEL),
            SUB_NAVI_CONFIG_LOG_OUTPUT_FILE_PATH);

    if (NAVI_RET_SUCCESS != init())
    {
        log_error("sub-navi init failed!");
        return 1;
    }

    EV_P = EV_DEFAULT;
    ev_signal wsig;
    ev_signal_init(&wsig, sigint_cb, SIGINT);
    ev_signal_start(EV_A_ &wsig);

    ev_loop(EV_A_ 0);

    return 0;
}
