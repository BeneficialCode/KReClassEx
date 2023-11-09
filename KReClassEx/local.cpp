#include "stdafx.h"
#include <event2/event.h>
#include <event2/util.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/thread.h>

#include <algorithm>

#include "local.h"
#include "winsock.h"
#include "jconf.h"
#include "netutils.h"
#include "utils.h"
#include "crypto.h"

#pragma comment(lib,"Iphlpapi.lib")


static int ipv6first = 0;
struct timeval tv;
int no_delay = 0;
extern int g_socket;
HANDLE g_hSem = NULL;
struct evbuffer* g_buf;
uint64_t rx = 0;

int start_local(profile_t profile) {
	char* remote_host = profile.remote_host;
	int remote_port = profile.remote_port;
	int timeout = profile.timeout;

	char remote_port_str[16];
	sprintf_s(remote_port_str, "%d", remote_port);

	winsock_init();

    evthread_use_windows_threads();

	struct event_base* base = event_base_new();

    g_hSem = ::CreateSemaphore(nullptr, 0, 1, nullptr);
    if (g_hSem == NULL) {
        return -1;
    }
    
	struct sockaddr_storage* storage = (sockaddr_storage*)ss_malloc(sizeof(struct sockaddr_storage));
	memset(storage, 0, sizeof(struct sockaddr_storage));
	if (get_sockaddr(remote_host, remote_port_str, storage, 0, ipv6first) == -1) {
		return -1;
	}

    
    listen_ctx_t listen_ctx;
    listen_ctx.remote_addr = (struct sockaddr*)storage;
    listen_ctx.timeout = timeout;

    listen_ctx.base = base;

    remote_t* remote = create_remote(&listen_ctx, NULL, 0);

    if (remote == NULL) {
        printf("invalid remote\n");
        return -1;
    }

    if (!remote->send_ctx->connected) {
        remote->buf->idx = 0;
        int r = connect(remote->fd, (struct sockaddr*)&(remote->addr), remote->addr_len);
        if (r == -1 && GETSOCKETERRNO() != CONNECT_IN_PROGRESS) {
            // 连接失败
            close_and_free_remote(remote);
            return -1;
        }
        g_socket = remote->fd;
        event_add(remote->send_ctx->io, NULL);
        evtimer_add(remote->send_ctx->watcher, &tv);
    }

    g_buf = evbuffer_new();

    event_base_dispatch(base);

    if (storage != nullptr) {
        free(storage);
    }

    winsock_cleanup();

    if (g_hSem != NULL) {
        CloseHandle(g_hSem);
    }

    return 0;
}

size_t get_sockaddr(char* host, char* port,
    struct sockaddr_storage* storage, int block,
    int ipv6first) {
    struct addrinfo hints;
    struct addrinfo* result, * rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;/* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */

    int err = getaddrinfo(host, port, &hints, &result);

    if (err != 0) {
        return -1;
    }

    int prefer_af = ipv6first ? AF_INET6 : AF_INET;
    for (rp = result; rp != NULL; rp = rp->ai_next)
        if (rp->ai_family == prefer_af) {
            if (rp->ai_family == AF_INET)
                memcpy(storage, rp->ai_addr, sizeof(struct sockaddr_in));
            else if (rp->ai_family == AF_INET6)
                memcpy(storage, rp->ai_addr, sizeof(struct sockaddr_in6));
            break;
        }

    if (rp == NULL) {
        for (rp = result; rp != NULL; rp = rp->ai_next) {
            if (rp->ai_family == AF_INET)
                memcpy(storage, rp->ai_addr, sizeof(struct sockaddr_in));
            else if (rp->ai_family == AF_INET6)
                memcpy(storage, rp->ai_addr, sizeof(struct sockaddr_in6));
            break;
        }
    }

    if (rp == NULL) {
        return -1;
    }

    freeaddrinfo(result);
    return 0;
}

remote_t* create_remote(listen_ctx_t* listener, struct sockaddr* addr,
    int direct) {
    struct sockaddr* remote_addr;

    if (addr == NULL) {
        remote_addr = listener->remote_addr;
    }
    else {
        remote_addr = addr;
    }

    int protocol = IPPROTO_TCP;
    int remotefd = socket(remote_addr->sa_family, SOCK_STREAM, protocol);
    if (remotefd == -1) {
        return NULL;
    }

    int opt = 1;
    setsockopt(remotefd, SOL_TCP, TCP_NODELAY, (char*)&opt, sizeof(opt));

    // Setup
    setnonblocking(remotefd);

    remote_t* remote = new_remote(remotefd, direct ? MAX_CONNECT_TIMEOUT : listener->timeout,
        listener);
    remote->addr_len = get_sockaddr_len(remote_addr);
    memcpy(&(remote->addr), remote_addr, remote->addr_len);
    remote->direct = direct;

    struct sockaddr_in* sockaddr = (struct sockaddr_in*)&remote->addr;
    printf("remote: %s:%hu", inet_ntoa(sockaddr->sin_addr), ntohs(sockaddr->sin_port));

    return remote;
}

remote_t* new_remote(int fd, int timeout, listen_ctx_t* listener) {
    remote_t* remote;
    remote = (remote_t*)ss_malloc(sizeof(remote_t));

    memset(remote, 0, sizeof(remote_t));

    remote->buf = (buffer_t*)ss_malloc(sizeof(buffer_t));
    remote->recv_ctx = (remote_ctx*)ss_malloc(sizeof(remote_ctx_t));
    remote->send_ctx = (remote_ctx*)ss_malloc(sizeof(remote_ctx_t));
    balloc(remote->buf, SOCKET_BUF_SIZE);
    memset(remote->recv_ctx, 0, sizeof(remote_ctx_t));
    memset(remote->send_ctx, 0, sizeof(remote_ctx_t));
    remote->recv_ctx->connected = 0;
    remote->send_ctx->connected = 0;
    remote->fd = fd;
    remote->recv_ctx->remote = remote;
    remote->send_ctx->remote = remote;

    remote->recv_ctx->io = event_new(listener->base, fd, EV_READ | EV_PERSIST,
        remote_recv_cb, remote->recv_ctx);
    remote->send_ctx->io = event_new(listener->base, fd, EV_WRITE | EV_PERSIST,
        remote_send_cb, remote->send_ctx);
    remote->send_ctx->watcher = evtimer_new(listener->base,
        remote_timeout_cb, remote->send_ctx);
    if (!remote->send_ctx->watcher) {
        tv.tv_sec = std::min<int>(MAX_CONNECT_TIMEOUT, timeout);
        tv.tv_usec = 50000;
        evtimer_add(remote->send_ctx->watcher, &tv);
    }

    return remote;
}

void remote_recv_cb(evutil_socket_t fd, short events, void* arg) {
    remote_ctx_t* remote_recv_ctx = (remote_ctx_t*)arg;
    remote_t* remote = remote_recv_ctx->remote;

    size_t r = recv(remote->fd, remote->buf->data, SOCKET_BUF_SIZE, 0);
    if (r == 0) {
        close_and_free_remote(remote);
    }
    else if (r == -1) {
        if (GETSOCKETERRNO() == EAGAIN || GETSOCKETERRNO() == EWOULDBLOCK) {
            // no data
            // continue to wait for recv
            return;
        }
        else {
            close_and_free_remote(remote);
            return;
        }
    }

    // Disable TCP_NODELAY after the first response are sent
    if (!remote->recv_ctx->connected && !no_delay) {
        int opt = 0;
        setsockopt(remote->fd, SOL_TCP, TCP_NODELAY, (char*)&opt, sizeof(opt));
    }
    remote->recv_ctx->connected = 1;
    
    remote->buf->len = r;

    // Todo: handle the data
    rx += r;

    printf("We received total bytes: %I64u\n", rx);
    buffer_t* buf = remote->buf;
    evbuffer_add(g_buf, buf->data, buf->len);
    size_t size = evbuffer_get_length(g_buf);
    if (size < sizeof(PACKET_HEADER)) {
        // continue receiving the data
        return;
    }
    else {
        PPACKET_HEADER pHeader = (PPACKET_HEADER)evbuffer_pullup(g_buf, sizeof(PACKET_HEADER));
        if (pHeader == NULL) {
            return;
        }
        if (pHeader->Version != SVERSION) {
            evbuffer_drain(g_buf, size);
            return;
        }
        size_t totalSize = pHeader->Length;
        if (size == totalSize) {
            parse_packet(fd, g_buf);
        }
    }
}

void remote_send_cb(evutil_socket_t fd, short events, void* arg) {
    remote_ctx_t* remote_send_ctx = (remote_ctx_t*)arg;
    remote_t* remote = remote_send_ctx->remote;

    if (!remote_send_ctx->connected) {
        // 尚未连接remote server时
        struct sockaddr_storage addr;
        socklen_t len = sizeof addr;
        // 这儿通过getpeername获取对端地址，即服务器地址
        int r = getpeername(remote->fd, (struct sockaddr*)&addr, &len);
        if (r == 0) { // 如果返回值为0表示获取成功，因此判断为连接成功
            remote_send_ctx->connected = 1;
            evtimer_del(remote_send_ctx->watcher);
            // 准备从remote接受返回的数据
            event_add(remote->recv_ctx->io, NULL);

            // no need to send any data
            if (remote->buf->len == 0) {
                // 如果为空则停止发送并开始监听读取客户端的数据
                event_del(remote->send_ctx->io);
                return;
            }
        }
        else {
            close_and_free_remote(remote);
            return;
        }
    }

    // We can continue sending data
    ReleaseSemaphore(g_hSem, 1, nullptr);
}

void remote_timeout_cb(evutil_socket_t sock, short which, void* arg) {
    remote_ctx_t* remote_ctx = (remote_ctx_t*)arg;
    remote_t* remote = remote_ctx->remote;

    printf("TCP connection timeout");

    close_and_free_remote(remote);
}

void close_and_free_remote(remote_t* remote) {
    if (remote != nullptr) {
        evtimer_del(remote->send_ctx->watcher);
        event_del(remote->send_ctx->io);
        event_del(remote->recv_ctx->io);
        close(remote->fd);
        free_remote(remote);
    }
}

void free_remote(remote_t* remote) {
    if (remote->buf != NULL) {
        bfree(remote->buf);
        ss_free(remote->buf);
    }
    ss_free(remote->recv_ctx);
    ss_free(remote->send_ctx);
    ss_free(remote);
}

int parse_packet(evutil_socket_t fd, struct evbuffer* buf) {
    PPACKET_HEADER pHeader = (PPACKET_HEADER)evbuffer_pullup(buf, sizeof(PACKET_HEADER));
    size_t len = pHeader->Length;
    size_t dataLen = len - sizeof(PACKET_HEADER);
    MsgType type = pHeader->Type;

    unsigned char* pBody = (unsigned char*)malloc(len);
    if (pBody == nullptr)
        return -1;

    evbuffer_drain(buf, sizeof(PACKET_HEADER));
    evbuffer_remove(buf, pBody, dataLen);

    switch (type)
    {
    case MsgType::MemoryData:
    {
        PMEMORY_DATA pMemData = (PMEMORY_DATA)pBody;
        memcpy(pMemData->Buffer, pMemData->Data, pMemData->TotalSize);
        break;
    }
    case MsgType::HeartBeatData:
        break;

    case MsgType::ModuleBaseData:
    {
        PMODULE_BASE_INFO pBaseData = (PMODULE_BASE_INFO)pBody;
        CNodeBase* pClass = (CNodeBase*)pBaseData->pClass;
        pClass->SetOffset(pBaseData->Base);
        break;
    }

    case MsgType::AddressData:
    {
        PADDRESS_INFO pData = (PADDRESS_INFO)pBody;
        CNodeBase* pNode = (CNodeBase*)pData->pNode;
        CString name = CString(pData->Name, pData->NameLen);
        int idx = name.Find(L"!");
        if (idx != -1) {
            CString functionName = L"p";
            functionName += name.Right(name.GetLength() - idx - 1);
            pNode->SetName(functionName);
        }
        pNode->SetComment(name);
        break;
    }

    default:

        return -1;
    }

    free(pBody);

    return 1;
}