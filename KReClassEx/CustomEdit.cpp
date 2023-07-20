#include "stdafx.h"
#include "CustomEdit.h"

void CCustomEdit::OnChar(TCHAR chChar, UINT nRepCnt, UINT nFlags)
{
	if (chChar == VK_RETURN) {
		ShowWindow(SW_HIDE);
		
	}
}

LRESULT CCustomEdit::OnTextChanged(WORD /*wNotifyCode*/, WORD, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CString text;
	int width;

	GetWindowText(text);

	width = text.GetLength() + 1;

	return 0;
}

HBRUSH CCustomEdit::OnCtlColorEdit(CDCHandle dc, CEdit edit) {
	
	return m_hBackBrush;
}
