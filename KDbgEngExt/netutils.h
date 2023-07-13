#pragma once
#include "winsock.h"

#define SOCKET_BUF_SIZE (16 * 1024 - 1) // 16383 Byte, equals to the max chunk size

// Be compatible with older libc.
#ifndef IPPROTO_MPTCP
#define IPPROTO_MPTCP 262
#endif