#include "stdafx.h"
#include "NodeQWORD.h"

CNodeQword::CNodeQword()
{
    m_NodeType = NodeType::UINT64;
}

void CNodeQword::Update(const PHOTSPOT spot)
{
    UINT64 value;

    StandardUpdate(spot);

    value = _tcstoull(spot->Text.GetString(), NULL, g_bUnsignedHex ? 16 : 10);
    if (spot->Id == 0)
        ReClassWriteMemory(spot->Address, &value, sizeof(UINT64));
}

NODESIZE CNodeQword::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    UINT64* pData;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    pData = (UINT64*)(view->Data + m_Offset);
    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_UNSIGNED, HS_NONE, HS_NONE);
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("QWORD "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(" = "));
    tx = AddText(view, tx, y, g_clrValue, HS_EDIT, g_bUnsignedHex ? _T("0x%I64X") : _T("%llu"), *pData) + g_FontWidth;
    tx = AddComment(view, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}