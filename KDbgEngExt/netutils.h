#pragma once
#include "winsock.h"

// Be compatible with older libc.
#ifndef IPPROTO_MPTCP
#define IPPROTO_MPTCP 262
#endif