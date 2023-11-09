#include "stdafx.h"
#include "NodeVTable.h"
#include "NodeFunctionPtr.h"

void CNodeVTable::Update(const PHOTSPOT spot) {
	StandardUpdate(spot);
}

void CNodeVTable::Initialize(CWindow* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}

NODESIZE CNodeVTable::Draw(const PVIEWINFO view, int x, int y)
{
    NODESIZE drawSize;
    NODESIZE childDrawSize;
    ULONG_PTR* data;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    data = (ULONG_PTR*)(view->Data + m_Offset);

    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);

    x = AddOpenClose(view, x, y);
    x = AddIcon(view, x, y, ICON_VTABLE, -1, -1);

    int tx = x;
    x = AddAddressOffset(view, x, y);
    x = AddText(view, x, y, g_clrVTable, HS_NONE, _T("VTable[%i]"), m_ChildNodes.size()) + g_FontWidth;

    //if (m_strName.IsEmpty())
    x = AddText(view, x, y, g_clrName, HS_NAME, _T("%s"), m_Name) + g_FontWidth;
    //else
    //	x = AddText(View, x, y, g_clrName, HS_NONE, _T("%s_vtable"), pParent->m_strName) + FontWidth;

    x = AddComment(view, x, y);

    y += g_FontHeight;
    drawSize.x = x;
    drawSize.y = y;

    if (m_LevelsOpen[view->Level])
    {
        VIEWINFO newView;

        DWORD size = (DWORD)m_ChildNodes.size() * sizeof(ULONG_PTR);
        m_Memory.SetSize(size);

        memcpy(&newView, view, sizeof(newView));
        newView.Data = m_Memory.Data();
        newView.Address = *data;

        ReClassReadMemory(newView.Address, newView.Data, size);

        for (size_t i = 0; i < m_ChildNodes.size(); i++)
        {
            CNodeFunctionPtr* pFunctionPtrNode = static_cast<CNodeFunctionPtr*>(m_ChildNodes[i]);

            pFunctionPtrNode->SetOffset(i * sizeof(ULONG_PTR));

            if (!pFunctionPtrNode->IsInitialized()) {
                PUCHAR pMem = m_Memory.Data();
                size_t offset = i * sizeof(ULONG_PTR);
                ULONG_PTR value = *(PULONG_PTR)(pMem + offset);
                pFunctionPtrNode->Initialize(m_pParentWindow, newView.Address + offset, value);
            }

            childDrawSize = pFunctionPtrNode->Draw(&newView, tx, y);

            drawSize.y = childDrawSize.y;
            if (childDrawSize.x > drawSize.x)
            {
                drawSize.x = childDrawSize.x;
            }

            y = drawSize.y;
        }
    }
    else
    {
        for (size_t i = 0; i < m_ChildNodes.size(); i++)
        {
            static_cast<CNodeFunctionPtr*>(m_ChildNodes[i])->HideAssemblyWindow();
        }
    }

    return drawSize;
}