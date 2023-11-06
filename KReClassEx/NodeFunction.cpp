#include "stdafx.h"
#include "NodeFunction.h"

CNodeFunction::~CNodeFunction() {
	
}

CNodeFunction::CNodeFunction() {
	m_NodeType = NodeType::Function;
	m_MemorySize = sizeof(ULONG_PTR);
}

void CNodeFunction::Update(const PHOTSPOT spot) {
	StandardUpdate(spot);

	if (spot->Id == 0) {

	}
}

NODESIZE CNodeFunction::Draw(const PVIEWINFO pView, int x, int y) {
    NODESIZE drawSize;

    if (m_bHidden)
        return DrawHidden(pView, x, y);

    AddSelection(pView, 0, y, g_FontHeight);
    AddDelete(pView, x, y);
    AddTypeDrop(pView, x, y);

    int tx = x + TXOFFSET;
    int ax = 0;

    tx = AddIcon(pView, tx, y, ICON_METHOD, -1, -1);
    ax = tx;
    tx = AddAddressOffset(pView, tx, y);

    tx = AddText(pView, tx, y, g_clrType, HS_NONE, _T("Function"));

    tx = AddIcon(pView, tx, y, ICON_CAMERA, HS_EDIT, HS_CLICK);
    tx += g_FontWidth;

    tx = AddText(pView, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx += g_FontWidth;

    if (m_Lines > 0)
        tx = AddOpenClose(pView, tx, y);

    tx += g_FontWidth;

    tx = AddComment(pView, tx, y);

    if (m_LevelsOpen[pView->Level])
    {
        y += g_FontHeight;

        if (m_bRedrawNeeded)
        {
            
            

            m_bRedrawNeeded = FALSE;
        }
        else
        {
           
        }

       
    }
    else
    {
       
        m_bRedrawNeeded = TRUE;

        y += g_FontHeight;
    }

    drawSize.x = tx;
    drawSize.y = y;
    return drawSize;
}