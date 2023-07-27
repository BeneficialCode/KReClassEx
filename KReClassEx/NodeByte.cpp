#include "stdafx.h"
#include "NodeByte.h"

CNodeByte::CNodeByte()
{
    m_NodeType = NodeType::UINT8;
}

void CNodeByte::Update(const PHOTSPOT pSpot)
{
    unsigned __int8 value;

    StandardUpdate(pSpot);

    value = (unsigned __int8)_tcstoul(pSpot->Text, NULL, g_bUnsignedHex ? 16 : 10);
    if (pSpot->Id == 0)
        ReClassWriteMemory(pSpot->Address, &value, sizeof(unsigned __int8));
}

NODESIZE CNodeByte::Draw(const PVIEWINFO pView, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    unsigned __int8* Data;

    if (m_bHidden)
        return DrawHidden(pView, x, y);

    Data = (unsigned __int8*)(pView->Data + m_Offset);

    AddSelection(pView, 0, y, g_FontHeight);
    AddDelete(pView, x, y);
    AddTypeDrop(pView, x, y);
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon(pView, tx, y, ICON_UNSIGNED, HS_NONE, HS_NONE);
    tx = AddAddressOffset(pView, tx, y);
    tx = AddText(pView, tx, y, g_clrType, HS_NONE, _T("BYTE  "));
    tx = AddText(pView, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(pView, tx, y, g_clrName, HS_NONE, _T(" = "));
    tx = AddText(pView, tx, y, g_clrValue, HS_EDIT, g_bUnsignedHex ? _T("0x%02X") : _T("%u"), Data[0]) + g_FontWidth;
    tx = AddComment(pView, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}