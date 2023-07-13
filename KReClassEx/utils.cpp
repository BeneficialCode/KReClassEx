#include "stdafx.h"
#include "utils.h"

void* ss_malloc(size_t size) {
	void* tmp = malloc(size);
	if (tmp == NULL) {

	}
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