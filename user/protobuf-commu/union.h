#ifndef SUB_NAVI_USER_PROTOBUF_UNION_H
#define SUB_NAVI_USER_PROTOBUF_UNION_H

#include "pb_common.h"
#include "wiringSerial.h"

// main
int protobuf_commu_init(void);
int protobuf_commu_send_cmd(const pb_msgdesc_t *messagetype, void *message);

// tools
#define protobuf_commu_intf_init_cust(uart_path, baudrate) serialOpen(uart_path, baudrate)

int protobuf_commu_send_cmd_cust(int uart_fd, const pb_msgdesc_t *messagetype, void *message);

#endif
