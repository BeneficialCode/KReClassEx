#pragma once


typedef struct server {
	int fd;
	int stage;
	int frag;

	buffer_t* buf;

	struct server_ctx* recv_ctx;
	struct server_ctx* send_ctx;
	struct listen_ctx* listen_ctx;
	struct remote* remote;
}server_t;

typedef struct listen_ctx {
	struct event_base* base;
	struct event* io;
	int fd;
	int timeout;
}listen_ctx_t;

server_t* new_server(int fd, listen_ctx_t* listener);


int create_and_bind(const char* host, 
	const char* port);
void accept_cb(evutil_socket_t fd, short event, void* arg);
