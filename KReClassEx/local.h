#pragma once
#include "../KDbgEngExt/common.h"
#include <event2/event.h>

typedef struct profile{
	char* remote_host; // hostname or ip of remote server
	int remote_port;      // port number of remote server
	int timeout;          // connection timeout
}profile_t;

// local����ʱ�ᴴ���������
typedef struct listen_ctx {
    struct event_base* base;
    struct event* io;       // ��������ĳ���׽����ϵ��¼�
    int remote_num;
    int timeout;
    evutil_socket_t fd;     // local�������ļ����׽���������,tcp��������ͨ������
    struct sockaddr* remote_addr; // ����Զ��ss-server�������ĵ�ַ
} listen_ctx_t;

typedef struct remote_ctx {
    struct event* io;
    struct event* watcher;

    int connected;
    struct remote* remote;
} remote_ctx_t;

typedef struct remote {
    evutil_socket_t fd;
    int direct;// ���������ֱ�����Ǵ���ֱ����ʾֱ�Ӷ�ӦĿ�ķ�����
    int addr_len;
    uint32_t counter;
    OVERLAPPED olap;
    int connect_ex_done;

    buffer_t* buf;

    struct remote_ctx* recv_ctx;
    struct remote_ctx* send_ctx;
    struct sockaddr_storage addr;
} remote_t;

size_t get_sockaddr(char* host, char* port,
	struct sockaddr_storage* storage, int block,
	int ipv6first);
int start_local(profile_t profile);
remote_t* create_remote(listen_ctx_t* listener, struct sockaddr* addr,
	int direct);
remote_t* new_remote(int fd, int timeout, listen_ctx_t* listener);
void remote_recv_cb(evutil_socket_t fd, short events, void* arg);
void remote_send_cb(evutil_socket_t fd, short events, void* arg);
void remote_timeout_cb(evutil_socket_t sock, short which, void* arg);
void close_and_free_remote(remote_t* remote);
void free_remote(remote_t* remote);
int parse_packet(evutil_socket_t fd, struct evbuffer* buf);
