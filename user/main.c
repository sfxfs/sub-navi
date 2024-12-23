#include <stdlib.h>
#include "log.h"

#include "protobuf-commu/union.h"
#include "protobuf-commu/proto_def/navi_master.pb.h"
#include "json-rpc-server/server.h"
#include "csv-json-config-sys/json_thruster.h"
#include "csv-json-config-sys/csv_frame_factor.h"

static int parseArguments(int argc, const char *argv[])
{
    if (argc != 5)
    {
        log_info("Usage: %s <serial_port_path> <communication_mode> <channel> <value>", argv[0]);
        return 1;
    }

    const char *serialPort = argv[1];
    const char *mode = argv[2];
    int channel = atoi(argv[3]);
    int value = atoi(argv[4]);

    log_info("Serial Port: %d", serialPort);
    HARDWARE_UART uart = {0};
    if (0 != protobuf_commu_intf_init_cust(&uart, serialPort, SUB_NAVI_CONFIG_PROTOBUF_UART_BAUDRATE))
    {
        log_error("Serial Port init failed!");
        return -1;
    }

    if (strncmp(mode, "dshot", 4) == 0)
    {
        log_info("Communication Mode: DShot");
        log_info("Channel: %d", channel);
        log_info("Value: %d", value);
        ThrusterCommand msg = {0};
        switch (channel)
        {
        case 0:
            msg.has_throttle0 = true;
            msg.throttle0 = value;
            break;
        case 1:
            msg.has_throttle1 = true;
            msg.throttle1 = value;
            break;
        case 2:
            msg.has_throttle2 = true;
            msg.throttle2 = value;
            break;
        case 3:
            msg.has_throttle3 = true;
            msg.throttle3 = value;
            break;
        case 4:
            msg.has_throttle4 = true;
            msg.throttle4 = value;
            break;
        case 5:
            msg.has_throttle5 = true;
            msg.throttle5 = value;
            break;
        case 6:
            msg.has_throttle6 = true;
            msg.throttle6 = value;
            break;
        case 7:
            msg.has_throttle7 = true;
            msg.throttle7 = value;
            break;
        default:
            break;
        }
        if (0 != protobuf_commu_send_cmd_cust(&uart, ThrusterCommand_fields, &msg))
        {
            log_error("protobuf_commu_send_cmd_cust failed!");
            close(uart.fd);
            return 1;
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
        case 0:
            msg.has_duty0 = true;
            msg.duty0 = value;
            break;
        case 1:
            msg.has_duty1 = true;
            msg.duty1 = value;
            break;
        case 2:
            msg.has_duty2 = true;
            msg.duty2 = value;
            break;
        case 3:
            msg.has_duty3 = true;
            msg.duty3 = value;
            break;
        case 4:
            msg.has_duty4 = true;
            msg.duty4 = value;
            break;
        case 5:
            msg.has_duty5 = true;
            msg.duty5 = value;
            break;
        case 6:
            msg.has_duty6 = true;
            msg.duty6 = value;
            break;
        case 7:
            msg.has_duty7 = true;
            msg.duty7 = value;
            break;
        case 8:
            msg.has_duty8 = true;
            msg.duty8 = value;
            break;
        case 9:
            msg.has_duty9 = true;
            msg.duty9 = value;
            break;
        case 10:
            msg.has_duty10 = true;
            msg.duty10 = value;
            break;
        case 11:
            msg.has_duty11 = true;
            msg.duty11 = value;
            break;
        case 12:
            msg.has_duty12 = true;
            msg.duty12 = value;
            break;
        case 13:
            msg.has_duty13 = true;
            msg.duty13 = value;
            break;
        case 14:
            msg.has_duty14 = true;
            msg.duty14 = value;
            break;
        case 15:
            msg.has_duty15 = true;
            msg.duty15 = value;
            break;
        default:
            break;
        }
        if (0 != protobuf_commu_send_cmd_cust(&uart, PWMDevCommand_fields, &msg))
        {
            log_error("protobuf_commu_send_cmd_cust failed!");
            close(uart.fd);
            return 1;
        }
    }
    else
    {
        log_error("Invalid communication mode. Use 'dshot' or 'pwm'.");
        close(uart.fd);
        return 1;
    }
    return 0;
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

    // 5. protobuf rpc p2p
    if (0 != protobuf_commu_start_thread())
    {
        log_error("protobuf rpc server init failed!");
        // peripherals deinit ...
        return -1;
    }

    // 6. json rpc server
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
    if (argc > 1)
        return parseArguments(argc, argv);

    log_set_level(SUB_NAVI_CONFIG_LOG_LEVEL);
    if (0 != init())
        return 1;
    for (;;)
        ;
    return 0;
}
