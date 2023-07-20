// KReClassEx.cpp : main source file for KReClassEx.exe
//

#include "stdafx.h"
#include <atlribbon.h>

#include "Ribbon.h"

#include "resource.h"

#include "aboutdlg.h"
#include "MainFrm.h"

CAppModule _Module;

BOOL InitApp(HINSTANCE hInst);

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

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

	int nRet = 0;
	if (RunTimeHelper::IsRibbonUIAvailable())
		nRet = Run(lpstrCmdLine, nCmdShow);
	else
		AtlMessageBox(NULL, L"Cannot run with this version of Windows", IDR_MAINFRAME);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}

BOOL InitApp(HINSTANCE hInst) {
	UINT icons[] = { IDI_ICON_OPEN,IDI_ICON_CLOSED, IDI_ICON_CLASS,IDI_ICON_METHOD,
	IDI_ICON_VTABLE ,IDI_ICON_DELETE ,IDI_ICON_ADD ,IDI_ICON_RANDOM ,IDI_ICON_DROPARROW ,
	IDI_ICON_POINTER ,IDI_ICON_ARRAY ,IDI_ICON_CUSTOM ,IDI_ICON_ENUM ,
	IDI_ICON_FLOAT ,IDI_ICON_LEFT ,IDI_ICON_RIGHT ,IDI_ICON_MATRIX ,IDI_ICON_INTEGER ,
	IDI_ICON_TEXT ,IDI_ICON_UNSIGNED ,IDI_ICON_VECTOR ,IDI_ICON_CHANGE ,IDI_ICON_CAMERA };
	for (auto id : icons)
		g_Icons.emplace_back(::LoadIcon(hInst, MAKEINTRESOURCE(id)));

	return TRUE;
}