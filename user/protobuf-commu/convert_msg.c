#include <stdlib.h>

#include "log.h"
#include "pb_encode.h"
#include "pb_decode.h"

#include "navi_master.pb.h"
#include "union_msg.h"

#include "convert_msg.h"

const size_t msg2byte(const pb_msgdesc_t *messagetype, void *message, uint8_t *buffer, size_t bf_size)
{
    // 创建输出流，用于将protobuf消息写入字节数组
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, bf_size);

    // 将protobuf消息编码为字节数组
    bool status = encode_cmd_unionmessage(&stream, messagetype, message);
    if (!status)
    {
        log_warn("Encoding failed!");
        return 0;
    }

    return stream.bytes_written;
}

void * byte2msg(const uint8_t *buffer, size_t size, const pb_msgdesc_t **messagetype)
{
    if (size <= 0)
        return NULL;
    pb_istream_t stream = pb_istream_from_buffer(buffer, size);

    const pb_msgdesc_t *type = decode_response_unionmessage_type(&stream);
    bool status = false;

    void * message = NULL;
    if (type == DepthSensorResponse_fields)
    {
        message = calloc(1, sizeof(ThrusterCommand));
        status = decode_unionmessage_contents(&stream, DepthSensorResponse_fields, message);
        log_debug("Got DepthSensorResponse");
    }
    else if (type == PressureSensorResponse_fields)
    {
        message = calloc(1, sizeof(PWMDevCommand));
        status = decode_unionmessage_contents(&stream, PressureSensorResponse_fields, message);
        log_debug("Got PressureSensorResponse");
    }
    // ... other message types

    if (!status)
    {
        log_warn("Decode failed: %s", PB_GET_ERROR(&stream));
        free(message);
        return NULL;
    }

    *messagetype = type;
    return message;
}
