#include "stdafx.h"
#include "common.h"
#include "netutils.h"

size_t get_sockaddr_len(struct sockaddr* addr) {
	if (addr->sa_family == AF_INET) {
		return sizeof(struct sockaddr_in);
	}
	else if (addr->sa_family == AF_INET6) {
		return sizeof(struct sockaddr_in6);
	}
	return 0;
}