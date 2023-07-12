#include "pch.h"
#include "server.h"
#include "KDbgEngExt.h"
#include "winsock.h"
#include "netutils.h"
#include "utils.h"

std::list<server_t*> g_connections;

static int server_conn = 0;

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
		dprintf("failed to resolve server name %s", host);
		return -1;
	}

	if (result == NULL) {
		dprintf("Cannot bind");
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

	
}

server_t* new_server(int fd, listen_ctx_t* listener) {
	server_conn++;
	dprintf("new connection from client, %d opened client connections\n",
		server_conn);

	server_t* server;
	server = (server_t*)ss_malloc(sizeof(server_t));

	return server;
}