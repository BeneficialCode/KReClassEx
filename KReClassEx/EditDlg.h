#pragma once
#include "resource.h"
#include "ScintillaView.h"

class CEditDlg : public CDialogImpl<CEditDlg> 
{
public:
	enum { IDD = IDD_EDIT };

	CEditDlg(IMainFrame* frame, CString title, CString text) :m_pFrame(frame), _title(title), _text(text) {
	}

	BEGIN_MSG_MAP(CEditDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE,OnSize)
		MESSAGE_HANDLER(WM_CLOSE,OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CString _title;
	CString _text;

	CScintillaView* _view = nullptr;

private:
	IMainFrame* m_pFrame;
};
