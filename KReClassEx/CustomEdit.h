#pragma once
#include "HotSpot.h"

class CClassView;
class CCustomEdit : public CWindowImpl<CCustomEdit,CEdit> {
public:

	void OnChar(TCHAR chChar, UINT nRepCnt, UINT nFlags);
	HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit);

	void SetClassView(CClassView* pView) {
		m_pClassView = pView;
	}

	BEGIN_MSG_MAP_EX(CCustomEdit)
		MSG_WM_CHAR(OnChar)
		MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
		REFLECTED_COMMAND_CODE_HANDLER(EN_CHANGE,OnTextChanged) // This is important.
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	LRESULT OnTextChanged(WORD /*wNotifyCode*/, WORD, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
	HOTSPOT m_Hotspot;
	LONG m_MinWidth;
	HBRUSH m_hBackBrush;
	CClassView* m_pClassView;
};