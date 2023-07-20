// stdafx.cpp : source file that includes just the standard includes
//	KReClassEx.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

std::vector<HICON> g_Icons;

COLORREF g_clrBackground = RGB(255, 255, 255);
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

BOOL ReClassReadMemory(ULONG_PTR address, LPVOID buffer, SIZE_T size, PSIZE_T bytesRead) {
	return FALSE;
}