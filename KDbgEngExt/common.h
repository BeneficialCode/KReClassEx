#pragma once

#ifndef SOL_TCP
#define SOL_TCP IPPROTO_TCP
#endif


#define ISVALIDSOCKET(s) ((s)!=INVALID_SOCKET)
#define GETSOCKETERRNO() (WSAGetLastError())


typedef struct buffer {
    size_t idx;
    size_t len;
    size_t capacity;
    char* data;
} buffer_t;

