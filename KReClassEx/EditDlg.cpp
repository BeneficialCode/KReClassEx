#include "stdafx.h"
#include "EditDlg.h"

LRESULT CEditDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	SetWindowText(_title);
	_view = new CScintillaView(m_pFrame, L"Struct");
	
	RECT rect;
	GetClientRect(&rect);
	_view->Create(m_hWnd, rect, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN);
	int width, height;
	height = rect.bottom - rect.top;
	width = rect.right - rect.left;
	::MoveWindow(_view->m_hWnd, 0, 0, width, height, false);

	_view->SetLanguage(LexLanguage::CPP);
	_view->GetCtrl().SetReadOnly(false);
	_view->SetText(_text);
	_view->GetCtrl().SetReadOnly(true);

	return TRUE;
}

LRESULT CEditDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	EndDialog(0);
	return TRUE;
}


LRESULT CEditDlg::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	RECT rect;
	GetClientRect(&rect);
	int iHorizontalUnit = LOWORD(GetDialogBaseUnits());
	int iVerticalUnit = HIWORD(GetDialogBaseUnits());
	int width, height;
	height = rect.bottom - rect.top - 3 * iHorizontalUnit;
	width = rect.right - rect.left - 2 * iHorizontalUnit;
	::MoveWindow(_view->m_hWnd, iHorizontalUnit, iVerticalUnit, width, height, false);
	RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
	return TRUE;
}
