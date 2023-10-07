// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0601
#define _WIN32_WINNT	0x0601
#define _WIN32_IE	0x0700
#define _RICHEDIT_VER	0x0500

#include <atlbase.h>
#include <atlapp.h>
#include <atlstr.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atltypes.h>
#include <atlgdi.h>
#include <atltheme.h>
#include <strsafe.h>
#include <atlribbon.h>
#include <atlcrack.h>

#include <algorithm>
#include <vector>
#include <memory>

#include "../KDbgEngExt/common.h"

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

extern std::vector<HICON> g_Icons;

extern COLORREF g_clrBackground;
extern COLORREF g_clrSelect;
extern COLORREF g_clrHidden;
extern COLORREF g_clrOffset;
extern COLORREF g_clrAddress;
extern COLORREF g_clrType;
extern COLORREF g_clrName;
extern COLORREF g_clrIndex;
extern COLORREF g_clrValue;
extern COLORREF g_clrComment;
extern COLORREF g_clrVTable;
extern COLORREF g_clrFunction;
extern COLORREF g_clrChar;
extern COLORREF g_clrCustom;
extern COLORREF g_clrHex;

#define FONT_DEFAULT_WIDTH	8
#define FONT_DEFAULT_HEIGHT 16
#define FONT_DEFAULT_SIZE	10

extern CString g_ViewFontName;
extern CFont g_ViewFont;
extern int g_FontWidth;
extern int g_FontHeight;

extern bool g_bAddress;
extern bool g_bOffset;
extern bool g_bText;
extern bool g_bRTTI;
extern bool g_bRandomName;
extern bool g_bResizingFont;

#define ICON_OPEN 0
#define ICON_CLOSED 1
#define ICON_CLASS 2
#define ICON_METHOD 3
#define ICON_VTABLE 4
#define ICON_DELETE 5
#define ICON_ADD 6
#define ICON_RANDOM 7
#define ICON_DROPARROW 8
#define ICON_POINTER 9
#define ICON_ARRAY 10
#define ICON_CUSTOM 11
#define ICON_ENUM 12
#define ICON_FLOAT 13
#define ICON_LEFT 14
#define ICON_RIGHT 15
#define ICON_MATRIX 16
#define ICON_INTEGER 17
#define ICON_TEXT 18
#define ICON_UNSIGNED 19
#define ICON_VECTOR 20
#define ICON_CHANGE 21
#define ICON_CAMERA 22

BOOL ReClassReadMemory(ULONG_PTR address, LPVOID buffer, SIZE_T size, PSIZE_T bytesRead = nullptr);
BOOL ReClassWriteMemory(ULONG_PTR address, LPVOID buffer, SIZE_T size, PSIZE_T bytesWritten = nullptr);
void WritePacket(void* pPacket, ULONG length);

#include "Nodes.h"
#if defined(_M_AMD64)
#define CNodeHex CNodeHex64
#else
#define CNodeHex CNodeHex32
#endif

extern bool g_bFloat;
extern bool g_bInt;
extern bool g_bString;
extern bool g_bPointers;
extern bool g_bUnsignedHex;