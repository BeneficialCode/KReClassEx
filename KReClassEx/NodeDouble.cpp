#include "stdafx.h"
#include "NodeDouble.h"

CNodeDouble::CNodeDouble()
{
    m_NodeType = NodeType::Double;
}

void CNodeDouble::Update(const PHOTSPOT spot)
{
    double value;

    StandardUpdate(spot);

    value = _tcstod(spot->Text.GetString(), NULL);
    if (spot->Id == 0)
        ReClassWriteMemory(spot->Address, &value, sizeof(double));
}

NODESIZE CNodeDouble::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    double* pData;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    pData = (double*)(view->Data + m_Offset);

    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_FLOAT, HS_NONE, HS_NONE);
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("double "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(" = "));
    //tx = AddText(View, tx, y, g_clrValue, 0, "%.4lg", pMemory[0]) + FontWidth;
    tx = AddText(view, tx, y, g_clrValue, HS_EDIT, _T("%.4g"), *pData) + g_FontWidth;
    tx = AddComment(view, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}