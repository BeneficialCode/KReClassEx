#include "stdafx.h"
#include "NodeFloat.h"

CNodeFloat::CNodeFloat()
{
    m_NodeType = NodeType::Float;
}

void CNodeFloat::Update(const PHOTSPOT spot)
{
    float value;

    StandardUpdate(spot);

    value = (float)_ttof(spot->Text.GetString());
    if(spot->Id == HS_EDIT)
       ReClassWriteMemory(spot->Address, &value, sizeof(float));
}

NODESIZE CNodeFloat::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE DrawSize;
    float* pData;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    pData = (float*)(view->Data + m_Offset);
    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_FLOAT, HS_NONE, HS_NONE);
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("float "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(view, tx, y, g_clrName, HS_NONE, _T(" = "));
    //tx = AddText(View,tx,y,g_clrValue,0,"%.4f",pMemory[0]) + FontWidth;

    //if ( *pMemory > -99999.0f && *pMemory < 99999.0f )
    //	*pMemory = 0;

    tx = AddText(view, tx, y, g_clrValue, HS_EDIT, _T("%4.3f"), *pData) + g_FontWidth;
    tx = AddComment(view, tx, y);

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}