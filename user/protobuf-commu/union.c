#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <pb_decode.h>
#include <pb_encode.h>

#include <ev.h>
#include <log.h>

#include "linux_uart.h"
#include "navi-config.h"
#include "union.h"

#include "proto_def/navi_master.pb.c"

HARDWARE_UART uart_protobuf;
struct ev_loop *loop;
ev_io uart_watcher;

static const pb_msgdesc_t *decode_response_unionmessage_type(pb_istream_t *stream)
{
    pb_wire_type_t wire_type;
    uint32_t tag;
    bool eof;

    while (pb_decode_tag(stream, &wire_type, &tag, &eof))
    {
        if (wire_type == PB_WT_STRING)
        {
            pb_field_iter_t iter;
            if (pb_field_iter_begin(&iter, Responses_fields, NULL) &&
                pb_field_iter_find(&iter, tag))
            {
                /* Found our field. */
                return iter.submsg_desc;
            }
        }

        /* Wasn't our field.. */
        pb_skip_field(stream, wire_type);
    }

    return NULL;
}

static bool decode_unionmessage_contents(pb_istream_t *stream, const pb_msgdesc_t *messagetype, void *dest_struct)
{
    pb_istream_t substream;
    bool status;
    if (!pb_make_string_substream(stream, &substream))
        return false;

    status = pb_decode(&substream, messagetype, dest_struct);
    pb_close_string_substream(stream, &substream);
    return status;
}

static int protobuf_response_rpc(uint8_t *data, size_t size)
{
    if (size < 0)
        return -1;
    pb_istream_t stream = pb_istream_from_buffer(data, size);

    const pb_msgdesc_t *type = decode_response_unionmessage_type(&stream);
    bool status = false;

    if (type == DepthResponse_fields)
    {
        ThrusterCommand msg = {};
        status = decode_unionmessage_contents(&stream, DepthResponse_fields, &msg);
        log_debug("Got MsgType1");
    }
    else if (type == CleanPressureResponse_fields)
    {
        ArmCommand msg = {};
        status = decode_unionmessage_contents(&stream, CleanPressureResponse_fields, &msg);
        log_debug("Got MsgType2");
    }

    if (!status)
    {
        log_warn("Decode failed: %s", PB_GET_ERROR(&stream));
        return 1;
    }

    return 0;
}

static int protobuf_commu_intf_init(void)
{
    if (navi_uart_begin(&uart_protobuf, SUB_NAVI_CONFIG_PROTOBUF_UART_PATH) == 0)
        if (navi_uart_setBaudrate(&uart_protobuf, SUB_NAVI_CONFIG_PROTOBUF_UART_BAUDRATE) == 0)
            return 0;
    return -1;
}

static void uart_read_cb(struct ev_loop *loop, ev_io *watcher, int revents)
{
    log_debug("uart EV_READ event");
    uint8_t data[NAVI_MASTER_PB_H_MAX_SIZE];
    int len = read(watcher->fd, data, sizeof(data));
    protobuf_response_rpc(data, len);
}

static void *protobuf_routine(void *arg)
{
    ev_run(loop, 0);
    return NULL;
}

int protobuf_commu_start_thread(void)
{
    if (protobuf_commu_intf_init() != 0)
    {
        log_error("protobuf uart interface init failed.");
        return -1;
    }

    loop = EV_DEFAULT;
    ev_io_init(&uart_watcher, uart_read_cb, uart_protobuf.fd, EV_READ);
    ev_io_start(loop, &uart_watcher);

    pthread_t server_tid;
    if (pthread_create(&server_tid, NULL, protobuf_routine, NULL) == 0)
        if (pthread_detach(server_tid) == 0)
            return 0;
    log_error("protobuf commu thread create failed.");
    return -1;
}

void protobuf_commu_stop(void)
{
    ev_break(EV_A_ EVBREAK_ALL);
}

static bool encode_unionmessage_cmd(pb_ostream_t *stream, const pb_msgdesc_t *messagetype, void *message)
{
    pb_field_iter_t iter;

    if (!pb_field_iter_begin(&iter, Commands_fields, message))
        return false;

    do
    {
        if (iter.submsg_desc == messagetype)
        {
            /* This is our field, encode the message using it. */
            if (!pb_encode_tag_for_field(stream, &iter))
                return false;

            return pb_encode_submessage(stream, messagetype, message);
        }
    } while (pb_field_iter_next(&iter));

    /* Didn't find the field for messagetype */
    return false;
}

int protobuf_commu_send_cmd(const pb_msgdesc_t *messagetype, void *message)
{
    uint8_t data[NAVI_MASTER_PB_H_MAX_SIZE];
    pb_ostream_t stream = pb_ostream_from_buffer(data, sizeof(data));

    bool status = encode_unionmessage_cmd(&stream, messagetype, &message);
    if (!status)
    {
        log_warn("Encoding failed!");
        return -1;
    }

    if (navi_uart_write(uart_protobuf, data, stream.bytes_written) < 0)
    {
        log_warn("protobuf uart write failed.");
        return -1;
    }
    log_debug("protobuf uart send cmd success.");
    return 0;
}
