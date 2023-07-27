#include "stdafx.h"
#include "NodeQuat.h"

CNodeQuat::CNodeQuat()
{
    m_NodeType = NodeType::Quat;
    for (UINT i = 0; i < m_LevelsOpen.size(); i++)
        m_LevelsOpen[i] = true;
}

void CNodeQuat::Update(const PHOTSPOT spot)
{
    float value;

    StandardUpdate(spot);

    value = (float)_ttof(spot->Text.GetString());
    if (spot->Id >= 0 && spot->Id < 4)
        ReClassWriteMemory(spot->Address + (spot->Id * sizeof(float)), &value, sizeof(float));
}

NODESIZE CNodeQuat::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    float* pData;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    pData = (float*)(view->Data + m_Offset);
    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_VECTOR, -1, -1);
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("Vec4 "));
    tx = AddText(view, tx, y, g_clrName, 69, _T("%s"), m_Name);
    tx = AddOpenClose(view, tx, y);
    if (m_LevelsOpen[view->Level])
    {
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T("("));
        tx = AddText(view, tx, y, g_clrValue, 0, _T("%0.3f"), pData[0]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 1, _T("%0.3f"), pData[1]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 2, _T("%0.3f"), pData[2]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(","));
        tx = AddText(view, tx, y, g_clrValue, 3, _T("%0.3f"), pData[3]);
        tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(")"));
    }
    tx += g_FontWidth;
    tx = AddComment(view, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}