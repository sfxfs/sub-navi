#ifndef SUB_NAVI_USER_PROTOBUF_UNION_H
#define SUB_NAVI_USER_PROTOBUF_UNION_H

#include "pb_common.h"
#include "linux_uart.h"

// main
int protobuf_commu_init(void);
int protobuf_commu_send_cmd(const pb_msgdesc_t *messagetype, void *message);

// tools
int protobuf_commu_intf_init_cust(HARDWARE_UART *dev, const char *uart_path, uint32_t baudrate);
int protobuf_commu_send_cmd_cust(HARDWARE_UART *dev, const pb_msgdesc_t *messagetype, void *message);

#endif
