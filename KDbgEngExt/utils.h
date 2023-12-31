#pragma once

int ss_is_ipv6addr(const char* addr);
void* ss_malloc(size_t size);
void* ss_realloc(void* ptr, size_t new_size);

#define ss_free(ptr) \
    { \
        free(ptr); \
        ptr = NULL; \
    }
