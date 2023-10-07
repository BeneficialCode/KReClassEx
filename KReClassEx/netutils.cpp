#include "stdafx.h"
#include "netutils.h"

#pragma comment(lib,"Ws2_32.lib")

typedef struct in6_addr {
    union {
        UCHAR       Byte[16];
        USHORT      Word[8];
    } u;
} IN6_ADDR, * PIN6_ADDR, FAR* LPIN6_ADDR;

typedef struct sockaddr_in6 {
    ADDRESS_FAMILY sin6_family; // AF_INET6.
    USHORT sin6_port;           // Transport level port number.
    ULONG  sin6_flowinfo;       // IPv6 flow information.
    IN6_ADDR sin6_addr;         // IPv6 address.
    union {
        ULONG sin6_scope_id;     // Set of interfaces for a scope.
        SCOPE_ID sin6_scope_struct;
    };
} SOCKADDR_IN6_LH, * PSOCKADDR_IN6_LH, FAR* LPSOCKADDR_IN6_LH;

size_t get_sockaddr_len(struct sockaddr* addr) {
	if (addr->sa_family == AF_INET) {
		return sizeof(struct sockaddr_in);
	}
	else if (addr->sa_family == AF_INET6) {
		return sizeof(struct sockaddr_in6);
	}
	return 0;
}