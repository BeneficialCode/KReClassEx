#include "stdafx.h"
#include "NodePtr.h"

CNodePtr::CNodePtr() {
	m_NodeType = NodeType::Pointer;
}

void CNodePtr::Update(const PHOTSPOT spot)
{
	StandardUpdate(spot);
}

NODESIZE CNodePtr::Draw(const PVIEWINFO view, int x, int y)
{
    NODESIZE drawSize;
    NODESIZE childDrawSize;
    ULONG_PTR* pData;
    int tx;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    pData = (ULONG_PTR*)(view->Data + m_Offset);

    //printf( "read ptr: %p\n", View->Data );
    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);

    x = AddOpenClose(view, x, y);
    x = AddIcon(view, x, y, ICON_POINTER, -1, -1);

    tx = AddAddressOffset(view, x, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("Ptr "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(view, tx, y, g_clrValue, HS_NONE, _T(" <%s>"), m_pClassNode->GetName());
    tx = AddIcon(view, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_A);

    tx += g_FontWidth;
    tx = AddComment(view, tx, y);

    y += g_FontHeight;
    drawSize.x = tx;

    if (m_LevelsOpen[view->Level])
    {
        DWORD needSize = m_pClassNode->GetMemorySize();
        m_Memory.SetSize(needSize);

        VIEWINFO newView;
        memcpy(&newView, view, sizeof(newView));
        newView.Data = m_Memory.Data();
        newView.Address = *pData;

        ReClassReadMemory(newView.Address, (LPVOID)newView.Data, needSize);

        childDrawSize = m_pClassNode->Draw(&newView, x, y);

        y = childDrawSize.y;
        if (childDrawSize.x > drawSize.x)
            drawSize.x = childDrawSize.x;
    }

    drawSize.y = y;
    return drawSize;
}