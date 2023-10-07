#include "stdafx.h"
#include "CustomEdit.h"

void CCustomEdit::OnChar(TCHAR chChar, UINT nRepCnt, UINT nFlags)
{
	CNodeBase* pNode;
	UINT index;
	DWORD before, after;

	if (chChar == VK_RETURN) {
		ShowWindow(SW_HIDE);
		GetWindowText(m_Hotspot.Text);
		
		pNode = m_Hotspot.Object;
		before = pNode->GetMemorySize();
		pNode->Update(&m_Hotspot);
		after = pNode->GetMemorySize();

		index = m_pClassView->FindNodeIndex(pNode);
		
		m_pClassView->ResizeNode(static_cast<CNodeClass*>(pNode->GetParent()),
			index, before, after);
		m_pClassView->Invalidate();
	}

	SetMsgHandled(FALSE);
}

LRESULT CCustomEdit::OnTextChanged(WORD /*wNotifyCode*/, WORD, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CString text;
	int width;

	GetWindowText(text);

	width = (text.GetLength() + 6) * g_FontWidth;
	if (width > m_MinWidth)
		SetWindowPos(nullptr, 0, 0, width, g_FontHeight, SWP_NOMOVE);

	return 0;
}

HBRUSH CCustomEdit::OnCtlColorEdit(CDCHandle dc, CEdit edit) {
	
	return m_hBackBrush;
}
