#pragma once

#define SOCKET_BUF_SIZE (16 * 1024 - 1) // 16383 Byte, equals to the max chunk size

size_t get_sockaddr_len(struct sockaddr* addr);
