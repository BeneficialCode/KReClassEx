#include "stdafx.h"
#include "NodeArray.h"

CNodeArray::CNodeArray()
{
    m_NodeType = NodeType::Array;
    m_Total = 1;
    m_Current = 0;
}

void CNodeArray::Update(const PHOTSPOT pSpot)
{
    LONG lValue;
    ULONG ulValue;

    StandardUpdate(pSpot);

    lValue = _ttol(pSpot->Text.GetString());
    if (lValue < 0)
        return;

    ulValue = (ULONG)lValue;

    if (pSpot->Id == 0)
    {
        if (ulValue == 0)
            return;
        m_Total = ulValue;
    }
    else if (pSpot->Id == 1)
    {
        if (ulValue >= m_Total)
            return;
        m_Current = ulValue;
    }
    else if (pSpot->Id == 2)
    {
        if (m_Current > 0)
            m_Current--;
    }
    else if (pSpot->Id == 3)
    {
        if (m_Current < (INT)m_Total - 1)
            m_Current++;
    }
}

ULONG CNodeArray::GetMemorySize()
{
    return m_pNode->GetMemorySize() * m_Total;
}

NODESIZE CNodeArray::Draw(const PVIEWINFO pView, int x, int y)
{
    NODESIZE drawSize;
    NODESIZE childDrawSize;

    if (m_bHidden)
        return DrawHidden(pView, x, y);

    drawSize.x = 0;

    AddSelection(pView, 0, y, g_FontHeight);
    AddDelete(pView, x, y);
    AddTypeDrop(pView, x, y);

    x = AddOpenClose(pView, x, y);
    x = AddIcon(pView, x, y, ICON_ARRAY, -1, -1);

    int tx = x;
    tx = AddAddressOffset(pView, tx, y);

    tx = AddText(pView, tx, y, g_clrType, HS_NONE, _T("Array "));
    tx = AddText(pView, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(pView, tx, y, g_clrIndex, HS_NONE, _T("["));
    tx = AddText(pView, tx, y, g_clrIndex, HS_EDIT, _T("%u"), m_Total);
    tx = AddText(pView, tx, y, g_clrIndex, HS_NONE, _T("]"));

    tx = AddIcon(pView, tx, y, ICON_LEFT, HS_SELECT, HS_CLICK);
    tx = AddText(pView, tx, y, g_clrIndex, HS_NONE, _T("("));
    tx = AddText(pView, tx, y, g_clrIndex, 1, _T("%i"), m_Current);
    tx = AddText(pView, tx, y, g_clrIndex, HS_NONE, _T(")"));
    tx = AddIcon(pView, tx, y, ICON_RIGHT, HS_DROP, HS_CLICK);

    tx = AddText(pView, tx, y, g_clrValue, HS_NONE, _T("<%s Size=%u>"), 
        m_pNode->GetName(), GetMemorySize());
    tx = AddIcon(pView, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X);

    tx += g_FontWidth;
    tx = AddComment(pView, tx, y);

    y += g_FontHeight;
    if (m_LevelsOpen[pView->Level])
    {
        VIEWINFO newView;
        memcpy(&newView, pView, sizeof(newView));
        newView.Address = pView->Address + m_Offset + m_pNode->GetMemorySize() * m_Current;
        newView.Data = (UCHAR*)((uintptr_t)pView->Data + m_Offset 
            + m_pNode->GetMemorySize() * m_Current);

        childDrawSize = m_pNode->Draw(&newView, x, y);

        y = childDrawSize.y;
        if (childDrawSize.x > drawSize.x)
        {
            drawSize.x = childDrawSize.x;
        }
    }

    drawSize.y = y;
    return drawSize;
}
