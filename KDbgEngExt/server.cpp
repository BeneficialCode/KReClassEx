#include "pch.h"
#include "server.h"
#include "KDbgEngExt.h"
#include "winsock.h"
#include "netutils.h"
#include "utils.h"
#include "crypto.h"
#include "jconf.h"

std::list<server_t*> g_connections;

static int server_conn = 0;
uint64_t tx = 0;
uint64_t rx = 0;

int create_and_bind(const char* host,
	const char* port) {
	struct addrinfo hints;
	struct addrinfo* result, * rp, * ipv4v6bindall;
	int s, listen_sock = -1;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;               /* Return IPv4 and IPv6 choices */
	hints.ai_socktype = SOCK_STREAM;             /* We want a TCP socket */
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* For wildcard IP address */
	hints.ai_protocol = IPPROTO_TCP;

	result = NULL;

	s = getaddrinfo(host, port, &hints, &result);

	if (s != 0) {
		dprintf("failed to resolve server name %s\n", host);
		return -1;
	}

	if (result == NULL) {
		dprintf("Cannot bind\n");
		return -1;
	}

	rp = result;

	if (!host) {
		ipv4v6bindall = result;

		/* Loop over all address infos found until a IPV6 address is found. */
		while (ipv4v6bindall) {
			if (ipv4v6bindall->ai_family == AF_INET6) {
				rp = ipv4v6bindall; /* Take first IPV6 address available */
				break;
			}
			ipv4v6bindall = ipv4v6bindall->ai_next; /* Get next address info, if any */
		}
	}

    for (/*rp = result*/; rp != NULL; rp = rp->ai_next) {
        int protocol = rp->ai_protocol;
        listen_sock = socket(rp->ai_family, rp->ai_socktype, protocol);
        if (listen_sock == -1) {
            continue;
        }

        if (rp->ai_family == AF_INET6) {
            int opt = host ? 1 : 0;
            setsockopt(listen_sock, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&opt, sizeof(opt));
        }

        int opt = 1;
        setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

        s = bind(listen_sock, rp->ai_addr, rp->ai_addrlen);
        if (s == 0) {
			dprintf("We managed to bind successfully!\n");
            break;
        }
        else {
            dprintf("failed to bind address\n");
        }

        close(listen_sock);
        listen_sock = -1;
    }

    freeaddrinfo(result);

    return listen_sock;
}

void accept_cb(evutil_socket_t fd, short event, void* arg) {
	listen_ctx_t* listener = (listen_ctx_t*)arg;
	int serverfd = accept(listener->fd, NULL, NULL);
	if (serverfd == -1) {
		dprintf("accept() error\n");
		return;
	}

	int opt = 1;
	setsockopt(serverfd, SOL_TCP, TCP_NODELAY, (char*)&opt, sizeof(opt));

	setnonblocking(serverfd);

	server_t* server = new_server(serverfd, listener);
	event_add(server->recv_ctx->io, nullptr);
	evtimer_add(server->recv_ctx->watcher, nullptr);
}

server_t* new_server(int fd, listen_ctx_t* listener) {
	server_conn++;
	dprintf("new connection from client, %d opened client connections\n",
		server_conn);

	server_t* server;
	server = (server_t*)ss_malloc(sizeof(server_t));

	memset(server, 0, sizeof(server_t));

	server->recv_ctx = (server_ctx_t*)ss_malloc(sizeof(server_ctx_t));
	server->send_ctx = (server_ctx_t*)ss_malloc(sizeof(server_ctx_t));
	server->buf = (buffer_t*)ss_malloc(sizeof(buffer_t));
	memset(server->recv_ctx, 0, sizeof(server_ctx_t));
	memset(server->send_ctx, 0, sizeof(server_ctx_t));
	balloc(server->buf, SOCKET_BUF_SIZE);
	server->fd = fd;
	server->recv_ctx->server = server;
	server->recv_ctx->connected = 0;
	server->send_ctx->server = server;
	server->send_ctx->connected = 0;
	server->frag = 0;
	server->listen_ctx = listener;

	int timeout = std::max<int>(MIN_TCP_IDLE_TIMEOUT, server->listen_ctx->timeout);
	server->recv_ctx->io = event_new(listener->base, fd, EV_READ | EV_PERSIST,
		server_recv_cb, server->recv_ctx);
	server->send_ctx->io = event_new(listener->base, fd, EV_WRITE | EV_PERSIST,
		server_send_cb, server->send_ctx);
	server->recv_ctx->watcher = evtimer_new(listener->base,
		server_timeout_cb, server->recv_ctx);

	g_connections.push_back(server);

	return server;
}

void server_recv_cb(evutil_socket_t fd, short events, void* arg) {
	server_ctx_t* server_recv_ctx = (server_ctx_t*)arg;
	server_t* server = server_recv_ctx->server;

	buffer_t* buf = server->buf;

	if (server->stage == STAGE_STREAM) {
		// Only timer the watcher if a valid connection is established
		evtimer_add(server->recv_ctx->watcher, nullptr);
	}

	size_t r = recv(server->fd, buf->data, SOCKET_BUF_SIZE, 0);

	if (r == 0) {
		// connection closed
		close_and_free_server(server);
		return;
	}
	else if (r == -1) {
		if (GETSOCKETERRNO() == EAGAIN || GETSOCKETERRNO() == EWOULDBLOCK) {
			// no data
			// continue to wait for recv
			return;
		}
		else {
			close_and_free_server(server);
			return;
		}
	}

	// Ignore any new packet if the server is stopped
	if (server->stage == STAGE_STOP) {
		return;
	}

	tx += r;
	buf->len = r;

	// handle data
	dprintf("total received bytes: %ll\n", tx);
}

void server_send_cb(evutil_socket_t fd, short events, void* arg) {
	server_ctx_t* server_send_ctx = (server_ctx_t*)arg;
	server_t* server = server_send_ctx->server;

	if (server->buf->len == 0) {
		// close and free
		close_and_free_server(server);
		return;
	}
	else {
		// has data to send
		size_t s = send(server->fd, server->buf->data + server->buf->idx,
			server->buf->len, 0);
		if (s == -1) {
			if (GETSOCKETERRNO() != EAGAIN && GETSOCKETERRNO() != EWOULDBLOCK) {
				close_and_free_server(server);
			}
			return;
		}
		else if (s < server->buf->len) {
			// partly sent, move memory, wait for the next time to send
			server->buf->len -= s;
			server->buf->idx += s;
			return;
		}
		else {
			// all sent out, wait for reading
			server->buf->len = 0;
			server->buf->idx = 0;
			event_del(server_send_ctx->io);
		}
	}
}

void server_timeout_cb(evutil_socket_t fd, short events, void* arg) {
	server_ctx_t* server_ctx = (server_ctx_t*)arg;
	server_t* server = server_ctx->server;

	dprintf("TCP connection timeout\n");

	close_and_free_server(server);
}

void close_and_free_server(server_t* server) {
	if (server != NULL) {
		event_del(server->send_ctx->io);
		event_del(server->recv_ctx->io);
		evtimer_del(server->recv_ctx->watcher);
		close(server->fd);
		free_server(server);
		server_conn--;
		dprintf("close a connection from client, %d opened client connections\n", server_conn);
	}
}

void free_server(server_t* server) {
	g_connections.remove(server);

	if (server->buf != NULL) {
		bfree(server->buf);
		ss_free(server->buf);
	}

	ss_free(server->recv_ctx);
	ss_free(server->send_ctx);
	ss_free(server);
}

void free_connections() {
	for (auto& connection : g_connections) {
		close_and_free_server(connection);
	}
}