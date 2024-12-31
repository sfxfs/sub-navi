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
	ev_io listen_watcher;		// ev.h
	mjrpc_handle_t rpc_handle;	// mjsonrpc.h
};

#define jrpc_get_rpc_handle(server_ptr) ((server_ptr)->rpc_handle)

int jrpc_server_init(struct jrpc_server *server, int port_number);

void jrpc_server_start(void);

void jrpc_server_stop(void);

#endif
