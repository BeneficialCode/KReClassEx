#pragma once

#include "resource.h"

class CClassesDlg :
	public CDialogImpl<CClassesDlg>
{
public:
	enum {IDD=IDD_CLASSES};

	CClassesDlg(IMainFrame* frame) : m_pFrame(frame) {
	}

	BEGIN_MSG_MAP(CClassesDlg)
		MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
		COMMAND_ID_HANDLER(IDOK,OnOK)
		COMMAND_ID_HANDLER(IDCANCEL,OnCancel)
		NOTIFY_HANDLER(IDC_CLASS_LIST,NM_DBLCLK,OnDoubleClick)
	END_MSG_MAP()

public:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void BuildList();
	void AddData(size_t row, int col, CString& str);
	size_t FindClassByName(CString& name);

	CImageList m_ImageList;
	
	CListViewCtrl m_ListView;
	// 窗口子类化
	CContainedWindowT<CEdit> m_Edit;
	CString m_Filter;
	HICON m_hClassIcon;

private:
	IMainFrame* m_pFrame;
};