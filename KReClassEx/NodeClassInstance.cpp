#include "stdafx.h"
#include "NodeClassInstance.h"

CNodeClassInstance::CNodeClassInstance()
    : m_pNode(NULL)
{
    m_NodeType = NodeType::Instance;
}

void CNodeClassInstance::Update(const PHOTSPOT spot)
{
    StandardUpdate(spot);
}

ULONG CNodeClassInstance::GetMemorySize()
{
    return m_pNode->GetMemorySize();
}

NODESIZE CNodeClassInstance::Draw(const PVIEWINFO pView, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    NODESIZE childDrawSize;

    if (m_bHidden)
        return DrawHidden(pView, x, y);

    drawSize.x = 0;
    drawSize.y = 0;

    AddSelection(pView, 0, y, g_FontHeight);
    AddDelete(pView, x, y);
    AddTypeDrop(pView, x, y);

    x = AddOpenClose(pView, x, y);
    x = AddIcon(pView, x, y, ICON_CLASS, -1, -1);

    tx = x;
    tx = AddAddressOffset(pView, tx, y);

    tx = AddText(pView, tx, y, g_clrType, HS_NONE, _T("Instance "));
    tx = AddText(pView, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(pView, tx, y, g_clrValue, HS_NONE, _T("<%s>"), m_pNode->GetName());
    tx = AddIcon(pView, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X);

    tx += g_FontWidth;
    tx = AddComment(pView, tx, y);

    y += g_FontHeight;
    if (m_LevelsOpen[pView->Level])
    {
        VIEWINFO newView;
        memcpy(&newView, pView, sizeof(newView));
        newView.Address = pView->Address + m_Offset;
        newView.Data = (UCHAR*)((uintptr_t)newView.Data + m_Offset);

        childDrawSize = m_pNode->Draw(&newView, x, y);

        y = childDrawSize.y;
        if (childDrawSize.x > drawSize.x)
        {
            drawSize.x = childDrawSize.x;
        }
    }

    drawSize.x = tx;
    drawSize.y = y;
    return drawSize;
}
