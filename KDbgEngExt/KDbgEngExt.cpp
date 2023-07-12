#include "pch.h"

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

#pragma comment (lib, "dbgeng.lib")

#define DBGEXT_DEF extern "C" __declspec(dllexport) HRESULT __cdecl

PDEBUG_CLIENT g_DebugClient = NULL;
PDEBUG_CONTROL4 g_DebugControl = NULL;
PDEBUG_DATA_SPACES g_DebugDataSpaces = NULL;

EXTERN_C __declspec(dllexport) void __cdecl DebugExtensionUninitialize(void);
extern void __cdecl dprintf(PCSTR Format, ...);

// DbgEng requires all extensions to implement this function.
EXTERN_C __declspec(dllexport) HRESULT __cdecl
DebugExtensionInitialize(
	PULONG version,
	PULONG flags
)
{
	HRESULT hr;

	*version = DEBUG_EXTENSION_VERSION(1, 0);
	*flags = 0;

	g_DebugClient = NULL;
	g_DebugControl = NULL;

	hr = DebugCreate(__uuidof(IDebugClient), (void**)&g_DebugClient);
	if (FAILED(hr)) {
		goto exit;
	}
	hr = g_DebugClient->QueryInterface(__uuidof(IDebugControl4), (void**)&g_DebugControl);
	if (FAILED(hr)) {
		goto exit;
	}
	hr = g_DebugClient->QueryInterface(__uuidof(IDebugDataSpaces), (void**)&g_DebugDataSpaces);
	if (FAILED(hr)) {
		goto exit;
	}
	dprintf("Initialize success!\n");


exit:
	if (FAILED(hr)) {
		dprintf("Initialize failed");
		DebugExtensionUninitialize();
	}
	return hr;
}

// WinDbg requires all extensions to implement this function.
EXTERN_C __declspec(dllexport) void __cdecl
DebugExtensionUninitialize(void)
{
	if (g_DebugControl != NULL) {
		g_DebugControl->Release();
		g_DebugControl = NULL;
	}
	if (g_DebugDataSpaces != NULL) {
		g_DebugDataSpaces->Release();
		g_DebugDataSpaces = NULL;
	}
	if (g_DebugClient != NULL) {
		g_DebugClient->Release();
		g_DebugClient = NULL;
	}
}

DBGEXT_DEF runcommand(__in PDEBUG_CLIENT4 client, __in PCSTR args)
{
	HRESULT hr = g_DebugControl->Execute(DEBUG_OUTCTL_ALL_CLIENTS, args, 0);
	if (hr == S_OK) {
		dprintf("success!\n");
	}
	else {
		dprintf("failed!\n");
	}
	return hr;
}

void __cdecl
dprintf(PCSTR Format, ...)
{
	va_list Args;

	va_start(Args, Format);
	g_DebugControl->OutputVaList(DEBUG_OUTPUT_ERROR, Format, Args);
	va_end(Args);
}