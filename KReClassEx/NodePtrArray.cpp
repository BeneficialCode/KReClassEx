#include "stdafx.h"
#include "NodePtrArray.h"

CNodePtrArray::CNodePtrArray() :m_pNodePtr(new CNodePtr) {
	m_NodeType = NodeType::PtrArray;
}

NODESIZE CNodePtrArray::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    NODESIZE childDrawSize;
    ULONG classSize;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    drawSize.x = 0;
    drawSize.y = 0;

    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);

    x = AddOpenClose(view, x, y);
    x = AddIcon(view, x, y, ICON_ARRAY, -1, -1);

    tx = x;
    tx = AddAddressOffset(view, tx, y);

    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("ArrayOfPointers "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(view, tx, y, g_clrIndex, HS_NONE, _T("["));
    tx = AddText(view, tx, y, g_clrIndex, HS_EDIT, _T("%u"), m_PtrCount);
    tx = AddText(view, tx, y, g_clrIndex, HS_NONE, _T("]"));

    tx = AddIcon(view, tx, y, ICON_LEFT, HS_SELECT, HS_CLICK);
    tx = AddText(view, tx, y, g_clrIndex, HS_NONE, _T("("));
    tx = AddText(view, tx, y, g_clrIndex, 1, _T("%i"), m_CurrentIndex);
    tx = AddText(view, tx, y, g_clrIndex, HS_NONE, _T(")"));
    tx = AddIcon(view, tx, y, ICON_RIGHT, HS_DROP, HS_CLICK);

    tx = AddText(view, tx, y, g_clrValue, HS_NONE, _T("<%s* Size=%u>"), m_pNodePtr->GetClass()->GetName(), GetMemorySize());
    tx = AddIcon(view, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X);

    tx += g_FontWidth;
    tx = AddComment(view, tx, y);

    y += g_FontHeight;
    if (m_LevelsOpen[view->Level])
    {
        if (IsMemory(view->Address + m_Offset + (sizeof(ULONG_PTR) * m_CurrentIndex)))
        {
            classSize = m_pNodePtr->GetClass()->GetMemorySize();
            m_pNodePtr->GetMemory()->SetSize(classSize);

            VIEWINFO newView;
            memcpy(&newView, view, sizeof(newView));
            newView.Data = m_pNodePtr->GetMemory()->Data();
            newView.Address = *(ULONG_PTR*)(view->Data + m_Offset + (sizeof(ULONG_PTR) * m_CurrentIndex));

            ReClassReadMemory(newView.Address, (LPVOID)newView.Data, classSize);

            childDrawSize = m_pNodePtr->GetClass()->Draw(&newView, x, y);

            y = childDrawSize.y;
            if (childDrawSize.x > drawSize.x)
                drawSize.x = childDrawSize.x;
        }
    }

    drawSize.y = y;
    return drawSize;
}

ULONG CNodePtrArray::GetMemorySize()
{
    return m_pNodePtr->GetMemorySize() * m_PtrCount;
}

void CNodePtrArray::Update(const PHOTSPOT spot)
{
    LONG lValue;
    ULONG ulValue;

    StandardUpdate(spot);

    lValue = _ttol(spot->Text.GetString());
    if (lValue < 0)
        return;

    ulValue = (ULONG)lValue;

    if (spot->Id == 0)
    {
        if (ulValue == 0)
            return;
        m_PtrCount = ulValue;
    }
    else if (spot->Id == 1)
    {
        if (ulValue >= m_PtrCount)
            return;
        m_CurrentIndex = ulValue;
    }
    else if (spot->Id == 2)
    {
        if (m_CurrentIndex > 0)
            m_CurrentIndex--;
    }
    else if (spot->Id == 3)
    {
        if (m_CurrentIndex < (INT)m_PtrCount - 1)
            m_CurrentIndex++;
    }
}