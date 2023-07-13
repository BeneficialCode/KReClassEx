#include "stdafx.h"
#include "winsock.h"

int setnonblocking(int fd) {
    u_long arg = 1;

    return ioctlsocket(fd, FIONBIO, &arg);
}

void winsock_init(void) {
    int ret;
    WSADATA wsa_data;
    ret = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (ret != 0) {

    }
}

void winsock_cleanup(void) {
    WSACleanup();
}