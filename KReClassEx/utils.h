#pragma once

void* ss_malloc(size_t size);
void* ss_realloc(void* ptr, size_t new_size);

#define ss_free(ptr) \
    { \
        free(ptr); \
        ptr = NULL; \
    }