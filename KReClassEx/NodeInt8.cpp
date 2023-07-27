#include "stdafx.h"
#include "NodeInt8.h"

CNodeInt8::CNodeInt8()
{
    m_NodeType = NodeType::Int8;
}

void CNodeInt8::Update(const PHOTSPOT spot)
{
    __int8 value;

    StandardUpdate(spot);

    value = _ttoi(spot->Text.GetString());
    if (spot->Id == 0)
        ReClassWriteMemory(spot->Address, &value, sizeof(__int8));
}

NODESIZE CNodeInt8::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    __int8* pData;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    pData = (__int8*)(view->Data + m_Offset);
    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_INTEGER, HS_NONE, HS_NONE);
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("Int8  "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(" = "));
    tx = AddText(view, tx, y, g_clrValue, HS_EDIT, _T("%i"), *pData) + g_FontWidth;
    tx = AddComment(view, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}
