// KReClassEx.cpp : main source file for KReClassEx.exe
//

#include "stdafx.h"
#include <atlribbon.h>

#include "Ribbon.h"

#include "resource.h"

#include "aboutdlg.h"
#include "MainFrm.h"

extern "C" int Scintilla_RegisterClasses(void* hInstance);

#pragma comment(lib, "imm32")

typedef enum PROCESS_DPI_AWARENESS {
	PROCESS_DPI_UNAWARE = 0,
	PROCESS_SYSTEM_DPI_AWARE = 1,
	PROCESS_PER_MONITOR_DPI_AWARE = 2
};

typedef enum MONITOR_DPI_TYPE {
	MDT_EFFECTIVE_DPI = 0,
	MDT_ANGULAR_DPI = 1,
	MDT_RAW_DPI = 2,
	MDT_DEFAULT
};

using PGetProcessDpiAwareness = HRESULT(WINAPI*) (
	_In_ HANDLE hProcess,
	_Out_ PROCESS_DPI_AWARENESS* value
	);

using PGetDpiForMonitor = HRESULT(WINAPI*)(
	_In_ HMONITOR hMonitor,
	_In_ MONITOR_DPI_TYPE dpiType,
	_Out_ UINT* dpiX,
	_Out_ UINT* dpiY);

CAppModule _Module;

BOOL InitApp(HINSTANCE hInst);
void ResizeMemoryFont(HWND hWnd, int width, int height);



int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	HWND hWnd = wndMain.CreateEx();
	if(hWnd == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	//
	// Fix for 4K monitors
	//
	ResizeMemoryFont(hWnd, g_FontWidth, g_FontHeight);

	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}



int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	InitApp(hInstance);

	//Scintilla_RegisterClasses(hInstance);

	int nRet = 0;
	if (RunTimeHelper::IsRibbonUIAvailable())
		nRet = Run(lpstrCmdLine, nCmdShow);
	else
		AtlMessageBox(NULL, L"Cannot run with this version of Windows", IDR_MAINFRAME);

	_Module.Term();
	::CoUninitialize();

	//Scintilla_ReleaseResources();

	return nRet;
}

void ResizeMemoryFont(HWND hWnd, int width, int height) {
	if(!g_ViewFont.IsNull())
		g_ViewFont.DeleteObject();
	HMODULE hShcoreBase = GetModuleHandle(L"shcore.dll");
	if (hShcoreBase) {
		PGetProcessDpiAwareness pGetProcessDpiAwareness = 
			reinterpret_cast<PGetProcessDpiAwareness>(
				GetProcAddress(hShcoreBase, "GetProcessDpiAwareness"));
		PGetDpiForMonitor pGetDpiForMonitor =
			reinterpret_cast<PGetDpiForMonitor>(GetProcAddress(hShcoreBase, "GetDpiForMonitor"));
		if (pGetProcessDpiAwareness && pGetDpiForMonitor) {
			PROCESS_DPI_AWARENESS dpiValue;
			UINT dpiX, dpiY;
			HMONITOR hMonitor;

			pGetProcessDpiAwareness(nullptr, &dpiValue);
			if (dpiValue == PROCESS_DPI_AWARENESS::PROCESS_PER_MONITOR_DPI_AWARE ||
				dpiValue == PROCESS_DPI_AWARENESS::PROCESS_SYSTEM_DPI_AWARE) {
				hMonitor = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
				pGetDpiForMonitor(hMonitor, MONITOR_DPI_TYPE::MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
				g_FontWidth = MulDiv(width, MulDiv(dpiX, 100, 96), 100);
				g_FontHeight = MulDiv(height, MulDiv(dpiY, 100, 96), 100);
			}
		}
	}

	g_ViewFont.CreateFont(g_FontHeight, g_FontWidth, 0, 0,
		FW_NORMAL, FALSE, FALSE, FALSE, 0,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, FIXED_PITCH, g_ViewFontName);
}

BOOL InitApp(HINSTANCE hInst) {
	g_ViewFontName = L"Terminal";

	UINT icons[] = { IDI_ICON_OPEN,IDI_ICON_CLOSED, IDI_ICON_CLASS,IDI_ICON_METHOD,
	IDI_ICON_VTABLE ,IDI_ICON_DELETE ,IDI_ICON_ADD ,IDI_ICON_RANDOM ,IDI_ICON_DROPARROW ,
	IDI_ICON_POINTER ,IDI_ICON_ARRAY ,IDI_ICON_CUSTOM ,IDI_ICON_ENUM ,
	IDI_ICON_FLOAT ,IDI_ICON_LEFT ,IDI_ICON_RIGHT ,IDI_ICON_MATRIX ,IDI_ICON_INTEGER ,
	IDI_ICON_TEXT ,IDI_ICON_UNSIGNED ,IDI_ICON_VECTOR ,IDI_ICON_CHANGE ,IDI_ICON_CAMERA };
	for (auto id : icons)
		g_Icons.emplace_back(::LoadIcon(hInst, MAKEINTRESOURCE(id)));

	g_Typedefs.Hex = L"char";
	g_Typedefs.Int64 = L"__int64";
	g_Typedefs.Int32 = L"__int32";
	g_Typedefs.Int16 = L"__int16";
	g_Typedefs.Int8 = L"__int8";
	g_Typedefs.Qword = L"DWORD64";
	g_Typedefs.Dword = L"DWORD";
	g_Typedefs.Word = L"WORD";
	g_Typedefs.Byte = L"unsigned char";
	g_Typedefs.Float = L"float";
	g_Typedefs.Double = L"double";
	g_Typedefs.Vec2 = L"Vector2"; // xy
	g_Typedefs.Vec3 = L"Vector3"; // xyz
	g_Typedefs.Quat = L"Vector4"; // xyzw
	// matrix3x4_t is a C++ class that represents a matrix.
	g_Typedefs.Matrix = L"matrix3x4_t"; // float m_matrix[3][4];
	g_Typedefs.PChar = L"char*";
	g_Typedefs.PWChar = L"wchar_t*";

	return TRUE;
}