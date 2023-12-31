#include "pch.h"
#include "utils.h"

int ss_is_ipv6addr(const char* addr) {
	return strcmp(addr, ":") > 0;
}

void* ss_malloc(size_t size) {
	void* tmp = malloc(size);
	return tmp;
}

void* ss_realloc(void* ptr, size_t new_size) {
	void* new_buf = realloc(ptr, new_size);
	if (new_buf == NULL) {
		free(ptr);
		ptr = NULL;
	}
	return new_buf;
}