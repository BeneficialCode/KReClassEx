#pragma once

class CCustomToolTip : public CWindowImpl<CCustomToolTip, CEdit> {
public:


	HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit);

	BEGIN_MSG_MAP_EX(CCustomToolTip)
		MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
	END_MSG_MAP()



private:
	HBRUSH m_hBackBrush;
};