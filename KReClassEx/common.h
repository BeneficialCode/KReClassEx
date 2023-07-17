#pragma once
#define NOMINMAX

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#else

#endif

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s)!=INVALID_SOCKET)
#define GETSOCKETERRNO() (WSAGetLastError())
#else

#endif

#ifndef SOL_TCP
#define SOL_TCP IPPROTO_TCP
#endif

typedef struct buffer {
    size_t idx;
    size_t len;
    size_t capacity;
    char* data;
} buffer_t;

#define SVERSION 0x01

enum class MsgType {
    ReadMemory,
    GetStatus,
    HeartBeat,
};

typedef struct _PACKET_HEADER {
    MsgType Type;
    ULONG Length;
}PACKET_HEADER,*PPACKET_HEADER;

typedef struct _READ_MEMORY_INFO {
    unsigned char Version;
    ULONG64 Address;
    bool IsVirtual;
    ULONG ReadSize;
}READ_MEMORY_INFO, * PREAD_MEMORY_INFO;

typedef struct _MEMORY_DATA {
    ULONG64 Address;
    ULONG TotalSize;
    UCHAR Data[1];
}MEMORY_DATA, * PMEMORY_DATA;