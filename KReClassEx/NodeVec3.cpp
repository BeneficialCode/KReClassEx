#include "stdafx.h"
#include "NodeVec3.h"

CNodeVec3::CNodeVec3() {
	m_NodeType = NodeType::Vec3;
	for (size_t i = 0; i < m_LevelsOpen.size(); i++)
		m_LevelsOpen[i] = true;
}

void CNodeVec3::Update(const PHOTSPOT spot) {
	float value;

	StandardUpdate(spot);

	value = (float)_ttof(spot->Text);
	if (spot->Id >= 0 && spot->Id < 3) {
		ReClassWriteMemory(spot->Address + spot->Id * sizeof(float),
			&value, sizeof(float));
	}
}

NODESIZE CNodeVec3::Draw(const PVIEWINFO view, int x, int y)
{
	NODESIZE drawSize;
	float* pData;

	if (m_bHidden)
		return DrawHidden(view, x, y);

	pData = (float*)(view->Data + m_Offset);

	AddSelection(view, 0, y, g_FontHeight);
	AddDelete(view, x, y);
	AddTypeDrop(view, x, y);

	int tx = x + TXOFFSET;
	tx = AddIcon(view, tx, y, ICON_VECTOR, HS_NONE, HS_NONE);
	tx = AddAddressOffset(view, tx, y);
	tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("Vec3 "));
	tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
	tx = AddOpenClose(view, tx, y);
	if (m_LevelsOpen[view->Level])
	{
		tx = AddText(view, tx, y, g_clrName, HS_NONE, _T("("));
		tx = AddText(view, tx, y, g_clrValue, 0, _T("%0.3f"), pData[0]);
		tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
		tx = AddText(view, tx, y, g_clrValue, 1, _T("%0.3f"), pData[1]);
		tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
		tx = AddText(view, tx, y, g_clrValue, 2, _T("%0.3f"), pData[2]);
		tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(")"));
	}
	tx += g_FontWidth;
	tx = AddComment(view, tx, y);

	drawSize.x = tx;
	drawSize.y = y + g_FontHeight;
	return drawSize;
}