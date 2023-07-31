// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Interfaces.h"

class CMainFrame : 
	public CRibbonFrameWindowImpl<CMainFrame>, 
	public CMessageFilter, public CIdleHandler,
	public IMainFrame
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CCommandBarCtrl m_CmdBar;

	// Inherited via IMainFrame
	void CalcAllOffsets();
	void CalcOffsets(CNodeClass* pClass);
	void ClearHidden();
	bool IsNodeValid(CNodeBase* pCheckNode);
	CNodeBase* CreateNewNode(NodeType type);

	//TODO: Declare ribbon controls
	LRESULT OnForwardToActiveView(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	// Ribbon control map
	BEGIN_RIBBON_CONTROL_MAP(CMainFrame)
		
	END_RIBBON_CONTROL_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	void UpdateUI();
	void UIEnableAllAdd(BOOL bEnable);
	void StandardTypeUpdate(CClassView* pClassView);
	void InsertTypeUpdate(CClassView* pClassView);
	void UIEnableAllInsert(BOOL bEnable);

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_ADD_4,UPDUI_RIBBON)
		UPDATE_ELEMENT(ID_ADD_8,UPDUI_RIBBON)
		UPDATE_ELEMENT(ID_ADD_64,UPDUI_RIBBON)
		UPDATE_ELEMENT(ID_ADD_1024,UPDUI_RIBBON)
		UPDATE_ELEMENT(ID_ADD_2048,UPDUI_RIBBON)
	END_UPDATE_UI_MAP()




	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_BTN_CONNECT,OnConnect)
		COMMAND_ID_HANDLER(ID_BTN_NEW,OnNewClass)
		COMMAND_ID_HANDLER(ID_ADD_4,OnForwardToActiveView)
		CHAIN_MSG_MAP(CRibbonFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNewClass(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);


	static DWORD WINAPI TunnelThread(void* params);

	void WritePacket(void* pPacket, ULONG length);

private:

	CTabView m_view;
	
};
