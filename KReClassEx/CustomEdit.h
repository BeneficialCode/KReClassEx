#pragma once

#include "HotSpot.h"

class CCustomEdit : public CWindowImpl<CCustomEdit,CEdit> {
public:

	void OnChar(TCHAR chChar, UINT nRepCnt, UINT nFlags);
	HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit);

	BEGIN_MSG_MAP_EX(CCustomEdit)
		MSG_WM_CHAR(OnChar)
		MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
		COMMAND_CODE_HANDLER(EN_CHANGE,OnTextChanged)
	END_MSG_MAP()

	LRESULT OnTextChanged(WORD /*wNotifyCode*/, WORD, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	HOTSPOT m_Hotspot;
	LONG m_MinWidth;
	HBRUSH m_hBackBrush;
};