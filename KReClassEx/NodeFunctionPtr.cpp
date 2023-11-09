#include "stdafx.h"
#include "NodeFunctionPtr.h"


CNodeFunctionPtr::CNodeFunctionPtr() {
	m_NodeType = NodeType::FunctionPtr;
	m_Name = L"";
}

CNodeFunctionPtr::CNodeFunctionPtr(CWindow* pParentWindow, ULONG_PTR address,ULONG_PTR value) {
    Initialize(pParentWindow, address, value);
}

void CNodeFunctionPtr::Initialize(CWindow* pParentWindow, ULONG_PTR addr,ULONG_PTR value) {
    m_pParentWindow = pParentWindow;
   

    size_t len = sizeof(PACKET_HEADER) + sizeof(LOOKUP_BY_ADDRESS);
    void* pData = malloc(len);
    if (pData != NULL) {
        PPACKET_HEADER pHeader = (PPACKET_HEADER)pData;
        pHeader->Length = len;
        pHeader->Type = MsgType::LookupByAddress;
        pHeader->Version = SVERSION;
        PLOOKUP_BY_ADDRESS pAddInfo = (PLOOKUP_BY_ADDRESS)((PBYTE)pData + sizeof(PACKET_HEADER));
        pAddInfo->Address = value;
        pAddInfo->pNode = this;
        WritePacket(pData, len);
        free(pData);
    }

    m_bRedrawNeeded = TRUE;
}

void CNodeFunctionPtr::Update(const PHOTSPOT pSpot) {
	StandardUpdate(pSpot);

	if (pSpot->Id == 0)
	{
		// Re-read bytes at specified address
		// DisassembleBytes(Spot->Address);
	}
}

NODESIZE CNodeFunctionPtr::Draw(const PVIEWINFO pView, int x, int y) {
    NODESIZE drawSize;
    int tx, ax;

    if (IsHidden())
        return DrawHidden(pView, x, y);

    AddSelection(pView, 0, y, g_FontHeight);
    AddDelete(pView, x, y);
    AddTypeDrop(pView, x, y);
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon(pView, tx, y, ICON_METHOD, -1, -1);
    ax = tx;
    tx = AddAddressOffset(pView, tx, y);

    if (m_pParentNode->GetType() != NodeType::VTable)
    {
        tx = AddText(pView, tx, y, g_clrType, HS_NONE, _T("FunctionPtr"));
    }
    else
    {
        tx = AddText(pView, tx, y, g_clrFunction, HS_NONE, _T("(%i)"), m_Offset / sizeof(ULONG_PTR));
    }

    tx = AddIcon(pView, tx, y, ICON_CAMERA, HS_EDIT, HS_CLICK);
    tx += g_FontWidth;

    if (m_Name.IsEmpty())
    {
        tx = AddText(pView, tx, y, g_clrName, HS_NAME, _T("Function_%i"), m_Offset / sizeof(ULONG_PTR));
    }
    else
    {
        tx = AddText(pView, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    }

    tx += g_FontWidth;

    if (m_Lines > 0)
        tx = AddOpenClose(pView, tx, y);

    tx += g_FontWidth;
    tx = AddComment(pView, tx, y);

    if (m_LevelsOpen[pView->Level])
    {
        //for (size_t i = 0; i < m_Assembly.size( ); i++)
        //{
        //	y += g_FontHeight;
        //	AddText( View, ax, y, g_clrHex, HS_EDIT, "%s", m_Assembly[i].GetBuffer( ) );
        //}

        y += g_FontHeight;

    }
    else
    {

        y += g_FontHeight;
    }

    drawSize.x = tx;
    drawSize.y = y;
    return drawSize;
}