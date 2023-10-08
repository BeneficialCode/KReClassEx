#include "stdafx.h"
#include "ClassesDlg.h"


LRESULT CClassesDlg::OnDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/) {
	CString className;
	LVITEM item;
	m_ListView.GetSelectedItem(&item);
	m_ListView.GetItemText(item.iItem, item.iSubItem, className);

	size_t id = FindClassByName(className);

	m_pFrame->SwitchPage(id);

	return 0;
}

LRESULT CClassesDlg::OnCancel(WORD, WORD wID, HWND, BOOL&) {
	EndDialog(wID);
	return 0;
}

LRESULT CClassesDlg::OnOK(WORD, WORD wID, HWND, BOOL&) {
	UINT count = m_ListView.GetSelectedCount();
	

	EndDialog(wID);
	return 0;
}

LRESULT CClassesDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	m_ListView.Attach(GetDlgItem(IDC_CLASS_LIST));
	m_ListView.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);
	m_Edit.Attach(GetDlgItem(IDC_CLASS_NAME));
	m_ImageList.Create(GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON), ILC_COLOR32, 1, 1);
	m_ImageList.SetBkColor(RGB(255, 255, 255));
	m_hClassIcon = AtlLoadIcon(IDI_ICON_CLASS);
	m_ImageList.AddIcon(m_hClassIcon);

	BuildList();

	
	return TRUE;
}

void CClassesDlg::BuildList() {
	RECT listRect;
	m_ListView.GetWindowRect(&listRect);
	m_ListView.SetImageList(m_ImageList, LVSIL_SMALL);
	m_ListView.InsertColumn(0, L"Class", LVCFMT_CENTER, listRect.right - listRect.left - 4);
	

	for (size_t i = 0; i < m_pFrame->m_Classes.size(); i++) {
		CString name = m_pFrame->m_Classes[i]->GetName();
		if (m_Filter.GetLength() != 0 &&
			name.MakeUpper().Find(m_Filter.MakeUpper()) == -1) {
			continue;
		}
		AddData(i, 0, name);
	}
}

void CClassesDlg::AddData(size_t row, int col, CString& str) {
	LVITEM lv;
	ZeroMemory(&lv, sizeof(LVITEM));
	lv.iItem = row;
	lv.iSubItem = col;
	lv.pszText = str.GetBuffer();

	if (col == 0) {
		lv.mask = LVIF_IMAGE | LVIF_TEXT;
		lv.iImage = 0;
		m_ListView.InsertItem(&lv);
	}
	else {
		lv.mask = LVIF_TEXT;
		m_ListView.SetItem(&lv);
	}
}

size_t CClassesDlg::FindClassByName(CString& name) {
	for (size_t id = 0; id < m_pFrame->m_Classes.size(); id++) {
		CNodeClass* pNodeClass = m_pFrame->m_Classes[id];
		if (!pNodeClass)
			continue;
		if (_wcsicmp(pNodeClass->GetName(), name) == 0)
			return id;
	}
	return -1;
}