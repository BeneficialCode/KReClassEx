#include "stdafx.h"
#include "NodeVec2.h"

CNodeVec2::CNodeVec2() {
	m_NodeType = NodeType::Vec2;
	for (size_t i = 0; i < m_LevelsOpen.size(); i++)
		m_LevelsOpen[i] = true;
}


void CNodeVec2::Update(const PHOTSPOT spot) {
	float value;

	StandardUpdate(spot);

	value = (float)_ttof(spot->Text.GetString());
	if (spot->Id >= 0 && spot->Id < 2)
		ReClassWriteMemory(spot->Address + spot->Id * 4, &value, sizeof(float));
}

NODESIZE CNodeVec2::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    float* data;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    data = (float*)(view->Data + m_Offset);

    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_VECTOR, HS_NONE, HS_NONE);
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("Vec2 "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddOpenClose(view, tx, y);
    if (m_LevelsOpen[view->Level])
    {
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T("("));
        tx = AddText(view, tx, y, g_clrValue, HS_EDIT, _T("%0.3f"), data[0]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, HS_OPENCLOSE, _T("%0.3f"), data[1]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(")"));
    }
    tx += g_FontWidth;
    tx = AddComment(view, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}