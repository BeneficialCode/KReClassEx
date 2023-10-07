#pragma once

struct server;
typedef struct server_ctx {
	struct event* io;
	struct event* watcher;
	int connected;
	struct server* server;
}server_ctx_t;

typedef struct server {
	int fd;
	int stage;
	int frag;

	buffer_t* buf;

	struct server_ctx* recv_ctx;
	struct server_ctx* send_ctx;
	struct listen_ctx* listen_ctx;
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
void server_recv_cb(evutil_socket_t fd, short events, void* arg);
void server_send_cb(evutil_socket_t fd, short events, void* arg);
void server_timeout_cb(evutil_socket_t fd, short events, void* arg);
void close_and_free_server(server_t* server);
void free_server(server_t* server);

void free_connections();

int parse_packet(evutil_socket_t fd,struct evbuffer* buf);
int OnReadMemory(evutil_socket_t fd, PREAD_MEMORY_INFO pInfo);
void WritePacket(evutil_socket_t fd, void* pPacket, ULONG length);
int OnGetModuleBase(evutil_socket_t fd, PGET_MODULE_BASE pInfo);