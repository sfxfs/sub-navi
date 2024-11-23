/*
 * jsonrpc-c.h
 *
 *  Created on: Oct 11, 2012
 *      Author: hmng
 */

#ifndef JSONRPCC_H_
#define JSONRPCC_H_

#include "mjsonrpc.h"
#include <ev.h>

struct jrpc_server {
	int port_number;
	struct ev_loop *loop;
	ev_io listen_watcher;
	mjrpc_handler_t rpc_handler;
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

int jrpc_server_init(struct jrpc_server *server, int port_number);

void jrpc_server_run(struct jrpc_server *server);

int jrpc_server_stop(struct jrpc_server *server);

#endif
