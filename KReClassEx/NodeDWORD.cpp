#include "stdafx.h"
#include "NodeDWORD.h"

CNodeDword::CNodeDword()
{
    m_NodeType = NodeType::UINT32;
}

void CNodeDword::Update(const PHOTSPOT spot)
{
    DWORD value;

    StandardUpdate(spot);

    value = _tcstoul(spot->Text.GetString(), NULL, g_bUnsignedHex ? 16 : 10);
    if (spot->Id == 0)
        ReClassWriteMemory(spot->Address, &value, sizeof(DWORD));
}

NODESIZE CNodeDword::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    DWORD* pData;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    pData = (DWORD*)(view->Data + m_Offset);
    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_UNSIGNED, HS_NONE, HS_NONE);
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("DWORD "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(" = "));
    tx = AddText(view, tx, y, g_clrValue, HS_EDIT, g_bUnsignedHex ? _T("0x%X") : _T("%u"), *pData) + g_FontWidth;
    tx = AddComment(view, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}
