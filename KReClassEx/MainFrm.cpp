// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ribbon.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainFrm.h"
#include "local.h"
#include <Psapi.h>

int g_socket = 0;
extern HANDLE g_hSem;

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	return CRibbonFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	UIAddMenu(GetMenu(), true);
	m_CmdBar.Create(m_hWnd, rcDefault, NULL, WS_CHILD);
	m_CmdBar.AttachMenu(GetMenu());
	m_CmdBar.LoadImages(IDR_MAINFRAME);


	CreateSimpleStatusBar();
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

		ShowRibbonUI(true);

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document

	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	/*CAboutDlg dlg;
	dlg.DoModal();*/
	size_t len = sizeof(PACKET_HEADER) + sizeof(READ_MEMORY_INFO);
	void* pData = malloc(len);
	if (pData != NULL) {
		PPACKET_HEADER pHeader = (PPACKET_HEADER)pData;
		pHeader->Length = len;
		pHeader->Type = MsgType::ReadMemory;
		pHeader->Version = SVERSION;
		PREAD_MEMORY_INFO pInfo = (PREAD_MEMORY_INFO)((PBYTE)pData + sizeof(PACKET_HEADER));
		pInfo->Address = 0x00aa0000;
		pInfo->ReadSize = 0x257000;
		pInfo->IsVirtual = true;
		WritePacket(pData, len);
	}
	return 0;
}

LRESULT CMainFrame::OnConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HANDLE hThread = ::CreateThread(nullptr, 0, TunnelThread,
		nullptr, 0, nullptr
	);
	if (hThread != NULL)
		::CloseHandle(hThread);

	return 0;
}

DWORD WINAPI CMainFrame::TunnelThread(void* params) {
	const profile_t profile = {
		.remote_host = const_cast<char*>("127.0.0.1"),
		.remote_port = 9000,
		.timeout = 5000
	};

	return start_local(profile);
}

void CMainFrame::WritePacket(void* pPacket, ULONG length) {
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
