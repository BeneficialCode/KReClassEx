#include "stdafx.h"
#include "NodeCustom.h"

CNodeCustom::CNodeCustom()
{
    m_NodeType = NodeType::Custom;
    m_Name = _T("Custom");
    m_MemorySize = sizeof(void*);
}

void CNodeCustom::Update(const PHOTSPOT spot)
{
    StandardUpdate(spot);
    if (spot->Id == 0)
        m_MemorySize = _ttoi(spot->Text.GetString());
}

ULONG CNodeCustom::GetMemorySize()
{
    return m_MemorySize;
}

NODESIZE CNodeCustom::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE drawSize;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_CUSTOM, HS_NONE, HS_NONE);
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("Custom "));
    tx = AddText(view, tx, y, g_clrIndex, HS_NONE, _T("["));
    tx = AddText(view, tx, y, g_clrIndex, HS_EDIT, _T("%i"), m_MemorySize);
    tx = AddText(view, tx, y, g_clrIndex, HS_NONE, _T("] "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name) + g_FontWidth;
    tx = AddComment(view, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}