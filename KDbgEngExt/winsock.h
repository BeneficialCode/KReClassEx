#pragma once

#ifdef CONNECT_IN_PROGRESS
#undef CONNECT_IN_PROGRESS
#endif
#define CONNECT_IN_PROGRESS WSAEWOULDBLOCK

// Override close function
#define close(fd) closesocket(fd)

int setnonblocking(int fd);

void winsock_init(void);

void winsock_cleanup(void);