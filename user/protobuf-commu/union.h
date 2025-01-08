#ifndef SUB_NAVI_USER_PROTOBUF_UNION_H
#define SUB_NAVI_USER_PROTOBUF_UNION_H

#include "pb_common.h"
#include "wiringSerial.h"

#include "navi-type.h"

// main
navi_ret_t protobuf_commu_init(void);
navi_ret_t protobuf_commu_send_cmd(const pb_msgdesc_t *messagetype, void *message);

// tools
#define protobuf_commu_intf_init_cust(uart_path, baudrate) serialOpen(uart_path, baudrate)

navi_ret_t protobuf_commu_send_cmd_cust(int uart_fd, const pb_msgdesc_t *messagetype, void *message);

#endif
