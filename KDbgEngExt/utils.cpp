#include "pch.h"
#include "utils.h"

int ss_is_ipv6addr(const char* addr) {
	return strcmp(addr, ":") > 0;
}

void* ss_malloc(size_t size) {
	void* tmp = malloc(size);
	return tmp;
}