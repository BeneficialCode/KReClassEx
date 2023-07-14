#pragma once

#ifndef SOL_TCP
#define SOL_TCP IPPROTO_TCP
#endif

#define STAGE_ERROR     -1  /* Error detected                   */
#define STAGE_INIT       0  /* Initial stage                    */
#define STAGE_HANDSHAKE  1  /* Handshake with client            */
#define STAGE_RESOLVE    4  /* Resolve the hostname             */
#define STAGE_STREAM     5  /* Stream between client and server */
#define STAGE_STOP       6  /* Server stop to response          */

#define ISVALIDSOCKET(s) ((s)!=INVALID_SOCKET)
#define GETSOCKETERRNO() (WSAGetLastError())




typedef struct buffer {
    size_t idx;
    size_t len;
    size_t capacity;
    char* data;
} buffer_t;

#define SVERSION 0x01

typedef struct _READ_MEMORY_INFO {
    unsigned char Version;
    ULONG64 Address;
    bool IsVirtual;
    ULONG ReadSize;
}READ_MEMORY_INFO, * PREAD_MEMORY_INFO;

typedef struct _MEMORY_DATA_HEADER {
    ULONG64 Address;
    ULONG TotalSize;
}MEMORY_DATA_HEADER, * PMEMORY_DATA_HEADER;