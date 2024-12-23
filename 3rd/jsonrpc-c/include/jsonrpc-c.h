/*
 * jsonrpc-c.h
 *
 *  Created on: Oct 11, 2012
 *      Author: hmng
 */

#ifndef JSONRPCC_H_
#define JSONRPCC_H_

#include <ev.h>

#include "mjsonrpc.h"

struct jrpc_server {
	int port_number;
	struct ev_loop *loop;
	ev_io listen_watcher;
	mjrpc_handle_t rpc_handle;
	int debug_level;
};

struct jrpc_connection {
	struct ev_io io;
	int fd;
	int pos;
	unsigned int buffer_size;
	char * buffer;
	int debug_level;
};

#define GET_RPC_HANDLE(server_ptr) ((server_ptr)->rpc_handle)

int jrpc_server_init(struct jrpc_server *server, int port_number, int debug_level);

#endif
