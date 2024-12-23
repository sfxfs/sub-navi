/*
 * jsonrpc-c.c
 *
 *  Created on: Oct 11, 2012
 *      Author: hmng
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "log.h"

#include "mjsonrpc.h"

#include "jsonrpc-c.h"

static char *_strstr(const char *p1, int len, const char *p2)
{
    if (p1 == NULL || p2 == NULL)
        return NULL;
    if (*p2 == '\0' || *p1 == '\0')
        return (char *)p1;

    char *s1 = NULL;
    char *s2 = NULL;
    char *current = (char *)p1;

    for (; *current != '\0'; current++)
    {
        if (len == 0)
            return NULL;

        s1 = current;
        s2 = (char *)p2;

        while ((*s1 != '\0') && (*s2 != '\0') && (*s1 == *s2))
        {
            s1++;
            s2++;
        }

        if (*s2 == '\0')
            return current;
        if (*s1 == '\0')
            return NULL;

        len--;
    }
    return NULL;
}

static int __jrpc_server_start(struct jrpc_server *server);

// get sockaddr, IPv4 or IPv6:
static void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

#define HTTP_HEADER_BUF_SIZE 128

static int send_response(struct jrpc_connection * conn, char *response) {
	ssize_t ret = 0;
	int fd = conn->fd;
	log_debug("JSON Response:\n%s", response);
	int jresp_len = strlen(response);
	char *resp = calloc(jresp_len + HTTP_HEADER_BUF_SIZE, sizeof(char));
	sprintf(resp,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json; charset=UTF-8\r\n"
            "Accept: application/json\r\n"
            "Content-Length: %d\r\n\r\n%s",
            jresp_len, response);
	ret += write(fd, resp, strlen(resp));
	free(resp);
	return ret;
}

#undef HTTP_HEADER_BUF_SIZE

static void close_connection(struct ev_loop *loop, ev_io *w) {
	ev_io_stop(loop, w);
	close(((struct jrpc_connection *) w)->fd);
	free(((struct jrpc_connection *) w)->buffer);
	free(((struct jrpc_connection *) w));
}

static void connection_cb(struct ev_loop *loop, ev_io *w, int revents) {
	struct jrpc_connection *conn;
	struct jrpc_server *server = (struct jrpc_server *) w->data;
	size_t bytes_read = 0;
	//get our 'subclassed' event watcher
	conn = (struct jrpc_connection *) w;
	int fd = conn->fd;
	if (conn->pos == (conn->buffer_size - 1)) {
		char * new_buffer = realloc(conn->buffer, conn->buffer_size *= 2);
		if (new_buffer == NULL) {
			perror("Memory error");
			return close_connection(loop, w);
		}
		conn->buffer = new_buffer;
		memset(conn->buffer + conn->pos, 0, conn->buffer_size - conn->pos);
	}
	// can not fill the entire buffer, string must be NULL terminated
	int max_read_size = conn->buffer_size - conn->pos - 1;
	if ((bytes_read = read(fd, conn->buffer + conn->pos, max_read_size))
			== -1) {
		perror("read");
		return close_connection(loop, w);
	}
	if (!bytes_read) {
		// client closed the sending half of the connection
		log_info("Client closed connection.");
		return close_connection(loop, w);
	} else {
		cJSON *root;
		const char *end_ptr = NULL;
		conn->pos += bytes_read;

		/* get body */
		char *body = _strstr(conn->buffer, conn->buffer_size, "\r\n\r\n");
		if (body != NULL)
			body += 4;    // "\r\n\r\n"

		if ((root = cJSON_ParseWithOpts(body, &end_ptr, false)) != NULL) {
			char * str_result = cJSON_PrintUnformatted(root);
			log_debug("Valid JSON Received:\n%s", str_result);
			free(str_result);

			int ret_code;
			cJSON *ret_json = mjrpc_process_cjson(&server->rpc_handle, root, &ret_code);
			log_debug("Return Code: %d, (ret json == NULL) == %s", ret_code,
											ret_json == NULL ? "true" : "false");
			if (ret_json) {
				char *ret_str = cJSON_PrintUnformatted(ret_json);
				if (ret_str) {
					send_response(conn, ret_str);
					free(ret_str);
				}
				cJSON_Delete(ret_json);
			}
			//shift processed request, discarding it
			memmove(conn->buffer, end_ptr, strlen(end_ptr) + 2);

			conn->pos = strlen(end_ptr);
			memset(conn->buffer + conn->pos, 0,
					conn->buffer_size - conn->pos - 1);

			cJSON_Delete(root);
		} else {
			// did we parse the entire buffer? If so, just wait for more.
			// else there was an error before the buffer's end
			if (end_ptr != (conn->buffer + conn->pos)) {
				log_warn("INVALID JSON Received:\n---\n%s\n---",
						conn->buffer);
				char *ret_str = cJSON_PrintUnformatted(
					mjrpc_response_error(JSON_RPC_CODE_INVALID_REQUEST,
                                    strdup("Valid request received."), cJSON_CreateNull()));
				if (ret_str) {
					send_response(conn, ret_str);
					free(ret_str);
				}
				return close_connection(loop, w);
			}
		}
	}

}

static void accept_cb(struct ev_loop *loop, ev_io *w, int revents) {
	char s[INET6_ADDRSTRLEN];
	struct jrpc_connection *connection_watcher;
	connection_watcher = malloc(sizeof(struct jrpc_connection));
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	sin_size = sizeof their_addr;
	connection_watcher->fd = accept(w->fd, (struct sockaddr *) &their_addr,
			&sin_size);
	if (connection_watcher->fd == -1) {
		perror("accept");
		free(connection_watcher);
	} else {
		inet_ntop(their_addr.ss_family,
				get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);
		log_info("server: got connection from %s", s);
		ev_io_init(&connection_watcher->io, connection_cb,
				connection_watcher->fd, EV_READ);
		//copy pointer to struct jrpc_server
		connection_watcher->io.data = w->data;
		connection_watcher->buffer_size = 1500;
		connection_watcher->buffer = malloc(1500);
		memset(connection_watcher->buffer, 0, 1500);
		connection_watcher->pos = 0;
		ev_io_start(loop, &connection_watcher->io);
	}
}

int jrpc_server_init(struct jrpc_server *server, int port_number) {
    memset(server, 0, sizeof(struct jrpc_server));
	server->loop = EV_DEFAULT;
	server->port_number = port_number;
	return __jrpc_server_start(server);
}

static int __jrpc_server_start(struct jrpc_server *server) {
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_in sockaddr;
	unsigned int len;
	int yes = 1;
	int rv;
	char PORT[6];
	sprintf(PORT, "%d", server->port_number);
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("setsockopt");
			return -1;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		len = sizeof(sockaddr);
		if (getsockname(sockfd, (struct sockaddr *) &sockaddr, &len) == -1) {
			close(sockfd);
			perror("server: getsockname");
			continue;
		}
		server->port_number = ntohs( sockaddr.sin_port );

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, 5) == -1) {
		perror("listen");
		return -1;
	}
	log_info("server: waiting for connections...");

	ev_io_init(&server->listen_watcher, accept_cb, sockfd, EV_READ);
	server->listen_watcher.data = server;
	ev_io_start(server->loop, &server->listen_watcher);
	return 0;
}
