#include "stdafx.h"
#include "CustomToolTip.h"

HBRUSH CCustomToolTip::OnCtlColorEdit(CDCHandle dc, CEdit edit) {

	return m_hBackBrush;
}
