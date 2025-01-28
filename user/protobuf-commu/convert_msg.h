#ifndef SUB_NAVI_USER_PROTOBUF_CONVERT_H
#define SUB_NAVI_USER_PROTOBUF_CONVERT_H

#include "pb_common.h"

/**
 * @brief Protocol Buffer message to byte array
 * @param messagetype encode message type
 * @param message ptr to message
 * @return byte array
 */
const uint8_t * msg2byte(const pb_msgdesc_t *messagetype, void *message);

/**
 * @brief raw byte array to Protocol Buffer message
 * @param buffer bytes
 * @param size size of buffer
 * @param messagetype parsed message type
 * @return message
 */
void * byte2msg(const uint8_t *buffer, size_t size, const pb_msgdesc_t **messagetype);

#endif
