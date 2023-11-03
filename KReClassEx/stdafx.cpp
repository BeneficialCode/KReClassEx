// stdafx.cpp : source file that includes just the standard includes
//	KReClassEx.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "local.h"

std::vector<HICON> g_Icons;

#ifdef _DEBUG
COLORREF g_clrBackground = RGB(255, 255, 255);
#else
COLORREF g_clrBackground = RGB(255, 255, 255);
#endif
COLORREF g_clrSelect = RGB(240, 240, 240);
COLORREF g_clrHidden = RGB(240, 240, 240);

COLORREF g_clrOffset = RGB(255, 0, 0);
COLORREF g_clrAddress = RGB(0, 200, 0);
COLORREF g_clrType = RGB(0, 0, 255);
COLORREF g_clrName = RGB(32, 32, 128);
COLORREF g_clrIndex = RGB(32, 200, 200);
COLORREF g_clrValue = RGB(255, 128, 0);
COLORREF g_clrComment = RGB(0, 200, 0);

COLORREF g_clrVTable = RGB(0, 255, 0);
COLORREF g_clrFunction = RGB(255, 0, 255);
COLORREF g_clrChar = RGB(0, 0, 255);
COLORREF g_clrBits = RGB(0, 0, 255);
COLORREF g_clrCustom = RGB(64, 128, 64);
COLORREF g_clrHex = RGB(0, 0, 0);

CString g_ViewFontName;
CFont g_ViewFont;

int g_FontWidth = FONT_DEFAULT_WIDTH;
int g_FontHeight = FONT_DEFAULT_HEIGHT;

bool g_bAddress = true;
bool g_bOffset = true;
bool g_bText = true;
bool g_bRTTI = true;
bool g_bRandomName = true;
bool g_bResizingFont = true;

bool g_bFloat = true;
bool g_bInt = true;
bool g_bString = true;
bool g_bPointers = true;
bool g_bUnsignedHex = true;

DWORD g_NodeCreateIndex = 0;
int g_socket = 0;
extern HANDLE g_hSem;

RCTYPEDEFS g_Typedefs;

void WritePacket(void* pPacket, ULONG length) {
	size_t idx = 0;
	do
	{
		int s = send(g_socket, reinterpret_cast<const char*>((PBYTE)pPacket + idx),
			length, 0);
		if (s == -1) {
			if (GETSOCKETERRNO() == EAGAIN || GETSOCKETERRNO() == EWOULDBLOCK) {
				// no data ,wait for send
				WaitForSingleObject(g_hSem, INFINITE);
			}
			else {
				// error
				return;
			}
		}
		else if (s < length) {
			length -= s;
			idx = s;
		}
		else {
			// ·¢ËÍÍê±Ï
			return;
		}
	} while (length);
}

BOOL ReClassReadMemory(ULONG_PTR address, LPVOID buffer, SIZE_T size, PSIZE_T bytesRead) {
	size_t len = sizeof(PACKET_HEADER) + sizeof(READ_MEMORY_INFO);
	void* pData = malloc(len);
	if (pData != NULL) {
		PPACKET_HEADER pHeader = (PPACKET_HEADER)pData;
		pHeader->Length = len;
		pHeader->Type = MsgType::ReadMemory;
		pHeader->Version = SVERSION;
		PREAD_MEMORY_INFO pInfo = (PREAD_MEMORY_INFO)((PBYTE)pData + sizeof(PACKET_HEADER));
		pInfo->Address = address;
		pInfo->Buffer = buffer;
		pInfo->ReadSize = size;
		pInfo->IsVirtual = true;
		WritePacket(pData, len);
		free(pData);
	}
	return FALSE;
}

BOOL ReClassWriteMemory(ULONG_PTR address, LPVOID buffer, SIZE_T size, PSIZE_T bytesWritten) {
	return FALSE;
}