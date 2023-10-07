// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ribbon.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainFrm.h"
#include "local.h"
#include "ClassView.h"




BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	return CRibbonFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UpdateUI();
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

	m_view.m_bDestroyImageList = false;
	
	m_hWndClient = m_view.Create(m_hWnd, rcDefault, nullptr,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	ShowRibbonUI(true);

	UIEnable(ID_ADD_4, false);


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

LRESULT CMainFrame::OnNewClass(WORD, WORD, HWND, BOOL&)
{
	// AtlMessageBox(m_hWnd, L"I'm comming!", L"Info", MB_OK);
	auto pView = new CClassView(this);
	auto hWnd = pView->Create(m_view, rcDefault, nullptr, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		WS_EX_STATICEDGE);
	if (!hWnd) {
		delete pView;
		return 0;
	}
	
	CNodeClass* pClass = new CNodeClass;
	pView->SetClass(pClass);
	m_Classes.push_back(pClass);

	m_view.AddPage(hWnd, L" N00000001     ", -1, pView);

	for (int i = 0; i < 64 / sizeof(size_t); i++) {
		CNodeHex* pNode = new CNodeHex;
		pNode->SetParent(pClass);
		pClass->AddNode(pNode);
	}
	
	CalcOffsets(pClass);
	return 0;
}

void CMainFrame::CalcOffsets(CNodeClass* pClass) {
	size_t offset = 0;
	for (UINT i = 0; i < pClass->NodeCount(); i++) {
		CNodeBase* pNode = pClass->GetNode(i);
		pNode->SetOffset(offset);
		offset += pNode->GetMemorySize();
	}
}

void CMainFrame::CalcAllOffsets() {
	for (UINT i = 0; i < m_Classes.size(); i++)
		CalcOffsets(m_Classes[i]);
}

void CMainFrame::ClearHidden() {
	for (size_t c = 0; c < m_Classes.size(); c++) {
		m_Classes[c]->Show();
		for (size_t n = 0; n < m_Classes[c]->NodeCount(); n++) {
			CNodeBase* pNode = m_Classes[c]->GetNode(n);
			pNode->Show();

			NodeType type = pNode->GetType();
			if (type == NodeType::VTable) {

			}
			else if (type == NodeType::Array) {

			}
			else if (type == NodeType::Pointer) {

			}
		}
	}
}

bool CMainFrame::IsNodeValid(CNodeBase* pCheckNode) {
	for (size_t c = 0; c < m_Classes.size(); c++) {
		for (size_t n = 0; n < m_Classes[c]->NodeCount(); n++) {
			CNodeBase* pNode = m_Classes[c]->GetNode(n);
			if (pNode == pCheckNode)
				return true;

			NodeType nodeType = pNode->GetType();
			if (nodeType == NodeType::VTable) {

			}
			if (nodeType == NodeType::Array) {

			}
			if (nodeType == NodeType::Pointer) {

			}
		}
	}

	return false;
}

CNodeBase* CMainFrame::CreateNewNode(NodeType type) {
	switch (type) {
	case NodeType::Class: return new CNodeClass;

	case NodeType::Hex64: return new CNodeHex64;
	case NodeType::Hex32: return new CNodeHex32;
	case NodeType::Hex16: return new CNodeHex16;
	case NodeType::Hex8: return new CNodeHex8;
	case NodeType::Bits: return new CNodeBits;

	case NodeType::Int64: return new CNodeInt64;
	case NodeType::Int32: return new CNodeInt32;
	case NodeType::Int16: return new CNodeInt16;
	case NodeType::Int8: return new CNodeInt8;

	case NodeType::UINT64: return new CNodeQword;
	case NodeType::UINT32: return new CNodeDword;
	case NodeType::UINT16: return new CNodeWord;
	case NodeType::UINT8: return new CNodeByte;

	case NodeType::Vec2:	return new CNodeVec2;
	case NodeType::Vec3:	return new CNodeVec3;
	case NodeType::Quat:	return new CNodeQuat;
	case NodeType::Matrix:	return new CNodeMatrix;

	case NodeType::Float:	return new CNodeFloat;
	case NodeType::Double:	return new CNodeDouble;

	case NodeType::Custom:	return new CNodeCustom;
	case NodeType::Text:	return new CNodeText;
	case NodeType::PChar:	return new CNodeCharPtr;
	case NodeType::PWChar:	return new CNodeWCharPtr;
	case NodeType::Unicode:	return new CNodeUnicode;

	case NodeType::VTable:		return new CNodeVTable;
	case NodeType::FunctionPtr:	return new CNodeFunctionPtr;
	case NodeType::Function:	return new CNodeFunction;

	case NodeType::Pointer:		return new CNodePtr;
	case NodeType::Array:		return new CNodeArray;
	case NodeType::PtrArray:	return new CNodePtrArray;

	case NodeType::Instance:	return new CNodeClassInstance;

	}

	return nullptr;
}

LRESULT CMainFrame::OnForwardToActiveView(WORD, WORD, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	int nPage = m_view.GetActivePage();
	auto msg = GetCurrentMessage();
	CClassView* pClassView = (CClassView * )m_view.GetPageData(nPage);
	switch (msg->wParam)
	{
	case ID_ADD_4:
		pClassView->SendMessage(msg->message, ID_ADD_ADD4, msg->lParam);
		break;
	case ID_ADD_8:
		pClassView->SendMessage(msg->message, ID_ADD_ADD8, msg->lParam);
		break;
	case ID_ADD_64:
		pClassView->SendMessage(msg->message, ID_ADD_ADD64, msg->lParam);
		break;
	case ID_ADD_1024:
		pClassView->SendMessage(msg->message, ID_ADD_ADD1024, msg->lParam);
		break;
	case ID_ADD_2048:
		pClassView->SendMessage(msg->message, ID_ADD_ADD2048, msg->lParam);
		break;
	case ID_INSERT_4:
		pClassView->SendMessage(msg->message, ID_INSERT_INSERT4, msg->lParam);
		break;
	case ID_INSERT_8:
		pClassView->SendMessage(msg->message, ID_INSERT_INSERT8, msg->lParam);
		break;
	case ID_INSERT_64:
		pClassView->SendMessage(msg->message, ID_INSERT_INSERT64, msg->lParam);
		break;
	case ID_INSERT_1024:
		pClassView->SendMessage(msg->message, ID_INSERT_INSERT1024, msg->lParam);
		break;
	case ID_INSERT_2048:
		pClassView->SendMessage(msg->message, ID_INSERT_INSERT2048, msg->lParam);
		break;
	case ID_HEX_64:
		pClassView->SendMessage(msg->message, ID_TYPE_HEX64, msg->lParam);
		break;
	case ID_HEX_32:
		pClassView->SendMessage(msg->message, ID_TYPE_HEX32, msg->lParam);
		break;
	case ID_HEX_16:
		pClassView->SendMessage(msg->message, ID_TYPE_HEX16, msg->lParam);
		break;
	case ID_HEX_8:
		pClassView->SendMessage(msg->message, ID_TYPE_HEX8, msg->lParam);
		break;
	case ID_HEX_BITS:
		pClassView->SendMessage(msg->message, ID_TYPE_BITS, msg->lParam);
		break;
	case ID_INT_64:
		pClassView->SendMessage(msg->message, ID_TYPE_INT64, msg->lParam);
		break;
	case ID_INT_32:
		pClassView->SendMessage(msg->message, ID_TYPE_INT32, msg->lParam);
		break;
	case ID_INT_16:
		pClassView->SendMessage(msg->message, ID_TYPE_INT16, msg->lParam);
		break;
	case ID_INT_8:
		pClassView->SendMessage(msg->message, ID_TYPE_INT8, msg->lParam);
		break;
	case ID_UINT_64:
		pClassView->SendMessage(msg->message, ID_TYPE_QWORD, msg->lParam);
		break;
	case ID_UINT_32:
		pClassView->SendMessage(msg->message, ID_TYPE_DWORD, msg->lParam);
		break;
	case ID_UINT_16:
		pClassView->SendMessage(msg->message, ID_TYPE_WORD, msg->lParam);
		break;
	case ID_UINT_8:
		pClassView->SendMessage(msg->message, ID_TYPE_BYTE, msg->lParam);
		break;
	case ID_SSE_DOUBLE:
		pClassView->SendMessage(msg->message, ID_TYPE_DOUBLE, msg->lParam);
		break;
	case ID_SSE_FLOAT:
		pClassView->SendMessage(msg->message, ID_TYPE_FLOAT, msg->lParam);
		break;
	case ID_VEC_2:
		pClassView->SendMessage(msg->message, ID_TYPE_VEC2, msg->lParam);
		break;
	case ID_VEC_3:
		pClassView->SendMessage(msg->message, ID_TYPE_VEC3, msg->lParam);
		break;
	case ID_VEC_4:
		pClassView->SendMessage(msg->message, ID_TYPE_VEC4, msg->lParam);
		break;
	case ID_MATRIX:
		pClassView->SendMessage(msg->message, ID_TYPE_MATRIX, msg->lParam);
		break;
	case ID_ARRAY:
		pClassView->SendMessage(msg->message, ID_TYPE_ARRAY, msg->lParam);
		break;
	case ID_CLASS:
		pClassView->SendMessage(msg->message, ID_TYPE_CLASS, msg->lParam);
		break;
	case ID_VTABLE:
		pClassView->SendMessage(msg->message, ID_TYPE_VTABLE, msg->lParam);
		break;
	case ID_FUNCTION:
		pClassView->SendMessage(msg->message, ID_TYPE_FUNCTION, msg->lParam);
		break;
	case ID_POINTER:
		pClassView->SendMessage(msg->message, ID_TYPE_POINTER, msg->lParam);
		break;
	case ID_ASCII:
		pClassView->SendMessage(msg->message, ID_TYPE_ASCII, msg->lParam);
		break;
	case ID_UNICODE:
		pClassView->SendMessage(msg->message, ID_TYPE_UNICODE, msg->lParam);
		break;
	case ID_PCHAR:
		pClassView->SendMessage(msg->message, ID_TYPE_PCHAR, msg->lParam);
		break;
	case ID_PWCHAR:
		pClassView->SendMessage(msg->message, ID_TYPE_PWCHAR, msg->lParam);
		break;
	default:
		break;
	}

	return 0;
}

void CMainFrame::UIEnableAllAdd(BOOL bEnable) {
	UIEnable(ID_ADD_4, bEnable);
	UIEnable(ID_ADD_8, bEnable);
	UIEnable(ID_ADD_64, bEnable);
	UIEnable(ID_ADD_1024, bEnable);
	UIEnable(ID_ADD_2048, bEnable);
}

void CMainFrame::StandardTypeUpdate(CClassView* pClassView) {
	if (pClassView->m_Selected.size() > 0) {
		if (pClassView->m_Selected[0].Object->GetType()
			== NodeType::Class) {
			UIEnableAllAdd(true);
			UIEnableAllType(false);
			UIEnable(ID_FUNCTION_PTR, true);
		}
		else {
			UIEnableAllAdd(true);
			UIEnableAllType(true);

		}
	}
	else {
		UIEnableAllAdd(false);
		UIEnableAllType(false);
	}
}

void CMainFrame::UpdateUI() {
	if (m_Classes.size() > 0) {
		UIEnable(ID_BTN_EDIT, TRUE);
		UIEnable(ID_BTN_DELETE, TRUE);
		UIEnable(ID_BTN_CLEAN_UP, TRUE);
		UIEnable(ID_BTN_CODE, TRUE);
	}
	else {
		UIEnable(ID_BTN_EDIT, FALSE);
		UIEnable(ID_BTN_DELETE, FALSE);
		UIEnable(ID_BTN_CLEAN_UP, FALSE);
		UIEnable(ID_BTN_CODE, FALSE);
	}
	int nPage = m_view.GetActivePage();
	if (nPage == -1) {
		UIEnableAllAdd(false);
		UIEnableAllInsert(false);
		UIEnableAllType(false);
		return;
	}
	CClassView* pClassView = (CClassView*)m_view.GetPageData(nPage);
	if (pClassView != nullptr) {
		StandardTypeUpdate(pClassView);
		InsertTypeUpdate(pClassView);
	}
}

void CMainFrame::UIEnableAllInsert(BOOL bEnable) {
	UIEnable(ID_INSERT_4, bEnable);
	UIEnable(ID_INSERT_8, bEnable);
	UIEnable(ID_INSERT_64, bEnable);
	UIEnable(ID_INSERT_1024, bEnable);
	UIEnable(ID_INSERT_2048, bEnable);
}

void CMainFrame::InsertTypeUpdate(CClassView* pClassView) {
	if (pClassView->m_Selected.size() == 1 &&
		pClassView->m_Selected[0].Object->GetParent()) {
		UIEnableAllInsert(true);
	}
	else {
		UIEnableAllInsert(false);
	}
}

void CMainFrame::UIEnableAllType(BOOL bEnable) {
	UIEnable(ID_HEX_64, bEnable);
	UIEnable(ID_HEX_32, bEnable);
	UIEnable(ID_HEX_16, bEnable);
	UIEnable(ID_HEX_8, bEnable);
	UIEnable(ID_HEX_BITS, bEnable);
	UIEnable(ID_INT_64, bEnable);
	UIEnable(ID_INT_32, bEnable);
	UIEnable(ID_INT_16, bEnable);
	UIEnable(ID_INT_8, bEnable);
	UIEnable(ID_UINT_64, bEnable);
	UIEnable(ID_UINT_32, bEnable);
	UIEnable(ID_UINT_16, bEnable);
	UIEnable(ID_UINT_8, bEnable);
	UIEnable(ID_SSE_DOUBLE, bEnable);
	UIEnable(ID_SSE_FLOAT, bEnable);
	UIEnable(ID_VEC_2, bEnable);
	UIEnable(ID_VEC_3, bEnable);
	UIEnable(ID_VEC_4, bEnable);
	UIEnable(ID_MATRIX, bEnable);
	UIEnable(ID_ARRAY, bEnable);
	UIEnable(ID_POINTER_ARRAY, bEnable);
	UIEnable(ID_CLASS, bEnable);
	UIEnable(ID_VTABLE, bEnable);
	UIEnable(ID_FUNCTION, bEnable);
	UIEnable(ID_FUNCTION_PTR, bEnable);
	UIEnable(ID_POINTER, bEnable);
	UIEnable(ID_ASCII, bEnable);
	UIEnable(ID_UNICODE, bEnable);
	UIEnable(ID_PCHAR, bEnable);
	UIEnable(ID_PWCHAR, bEnable);
}