// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <sdkddkver.h>

// Windows Header Files
#include <Windows.h>
#include <stdio.h>

//
// Define KDEXT_64BIT to make all wdbgexts APIs recognize 64 bit addresses
// It is recommended for extensions to use 64 bit headers from wdbgexts so
// the extensions could support 64 bit targets.
//
#define KDEXT_64BIT
#include <DbgEng.h>

#include <tchar.h>
#include <strsafe.h>
#include <DbgHelp.h>

#include <event2/event-config.h>
#include <event2/event.h>
#include <event2/thread.h>

#include <fstream>
#include "nlohmann/json.hpp"
#include "common.h"

#pragma comment(lib, "dbgeng.lib")
#pragma comment(lib,"ws2_32.lib") 
#pragma comment(lib,"Iphlpapi.lib")

#endif //PCH_H
