#include "pch.h"
#include "winsock.h"
#include "KDbgEngExt.h"
#include "server.h"
#include "utils.h"

#ifndef SSMAXCONN
#define SSMAXCONN 1024
#endif

using json = nlohmann::json;

#define DBGEXT_DEF extern "C" __declspec(dllexport) HRESULT __cdecl

PDEBUG_CLIENT g_DebugClient = NULL;
PDEBUG_CONTROL4 g_DebugControl = NULL;
PDEBUG_DATA_SPACES g_DebugDataSpaces = NULL;
PDEBUG_SYMBOLS3 g_DebugSymbols = NULL;
CHAR g_ConfigPath[MAX_PATH] = { 0 };
struct event_base* g_base;
bool g_IsSeverRun = false;

extern std::list<server_t*> g_connections;

EXTERN_C __declspec(dllexport) void __cdecl DebugExtensionUninitialize(void);
extern void __cdecl dprintf(PCSTR Format, ...);

DWORD WINAPI TunnelThread(void* params);

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
	hr = g_DebugClient->QueryInterface(__uuidof(IDebugSymbols3), (void**)&g_DebugSymbols);
	if (FAILED(hr)) {
		goto exit;
	}
	dprintf("Initialize success!\n");


exit:
	if (FAILED(hr)) {
		dprintf("Initialize failed\n");
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
	if (g_DebugSymbols != NULL) {
		g_DebugSymbols->Release();
		g_DebugSymbols = NULL;
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
#ifdef _DEBUG
	va_list Args;

	va_start(Args, Format);
	g_DebugControl->OutputVaList(DEBUG_OUTPUT_ERROR, Format, Args);
	va_end(Args);
#endif // DEBUG
}

DWORD WINAPI TunnelThread(void* params) {
	int ret = 0;
	dprintf("config path: %s\n", g_ConfigPath);
	std::ifstream f(g_ConfigPath);
	json data;
	try
	{
		data = json::parse(f);
	}
	catch (json::parse_error& ex)
	{
		dprintf("parser error %s\n", ex.what());
		return 1;
	}

	winsock_init();

	evthread_use_windows_threads();

	struct event_base* base = event_base_new();
	g_base = base;
	// initialize listen context
	listen_ctx listen_ctx;

	std::string host = data["server"].get<std::string>();
	std::string port = data["server_port"].get<std::string>();
	int timeout = data["timeout"].get<int>();

	if (ss_is_ipv6addr(host.c_str())) {
		dprintf("tcp server listening at [%s]:%s\n", host, port);
	}
	else
		dprintf("tcp server listening at %s:%s\n", host.c_str(), port.c_str());

	// Bind to port
	int listenfd;
	listenfd = create_and_bind(host.c_str(), port.c_str());
	if (listenfd == -1) {
		return 2;
	}
	if (listen(listenfd, SSMAXCONN) == -1) {
		dprintf("listen() error!\n");
		return 3;
	}
	setnonblocking(listenfd);
	
	// Setup context
	listen_ctx.timeout = timeout;
	listen_ctx.fd = listenfd;
	listen_ctx.base = base;

	listen_ctx.io = event_new(base, listenfd, EV_READ | EV_PERSIST,
		accept_cb, &listen_ctx);
	event_add(listen_ctx.io, nullptr);

	// start event loop
	event_base_dispatch(base);

	dprintf("closed gracefully\n");

	event_del(listen_ctx.io);

	free_connections();

	// avoid effecting the windbg
	// winsock_cleanup();

	return ret;
}

DBGEXT_DEF runserver(__in PDEBUG_CLIENT4 client, __in PCSTR args) {
	strcpy_s(g_ConfigPath, args);
	HANDLE hThread = ::CreateThread(nullptr, 0, TunnelThread, nullptr, 0, nullptr);
	if (hThread == NULL) {
		dprintf("CreateThread error: %d\n", GetLastError());
		return S_FALSE;
	}
	::CloseHandle(hThread);
	g_IsSeverRun = true;

	return S_OK;
}