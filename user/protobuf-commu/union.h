#ifndef SUB_NAVI_USER_PROTOBUF_UNION_H
#define SUB_NAVI_USER_PROTOBUF_UNION_H

#include <pb_common.h>

int protobuf_commu_start_thread(void);

int protobuf_commu_send_cmd(const pb_msgdesc_t *messagetype, void *message);

#endif
