#include "stdafx.h"
#include "crypto.h"
#include "utils.h"


int balloc(buffer_t* ptr, size_t capacity) {
	SecureZeroMemory(ptr, sizeof(buffer_t));
	ptr->data = (char*)ss_malloc(capacity);
	ptr->capacity = capacity;
	return capacity;
}

void bfree(buffer_t* ptr) {
	if (ptr == NULL)
		return;
	ptr->idx = 0;
	ptr->len = 0;
	ptr->capacity = 0;
	if (ptr->data != NULL) {
		ss_free(ptr->data);
	}
}

int bprepend(buffer_t* dst, buffer_t* src, size_t capacity) {
	brealloc(dst, dst->len + src->len, capacity);
	memmove(dst->data + src->len, dst->data, dst->len);
	memcpy(dst->data, src->data, src->len);
	dst->len = dst->len + src->len;
	return dst->len;
}

int brealloc(buffer_t* ptr, size_t len, size_t capacity) {
	if (ptr == nullptr)
		return -1;

	size_t real_capacity = std::max<size_t>(len, capacity);
	if (ptr->capacity < real_capacity) {
		ptr->data = (char*)ss_realloc(ptr->data, real_capacity);
		ptr->capacity = real_capacity;
	}

	return real_capacity;
}