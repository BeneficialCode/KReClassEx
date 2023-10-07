#pragma once
#include "stdafx.h"

int balloc(buffer_t*, size_t);
void bfree(buffer_t* ptr);
int bprepend(buffer_t*, buffer_t*, size_t);
int brealloc(buffer_t*, size_t, size_t);