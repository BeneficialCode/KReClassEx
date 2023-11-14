#include "stdafx.h"
#include "NodeText.h"

CNodeText::CNodeText()
{
    m_NodeType = NodeType::Text;
    m_Name = _T("Text");
    m_MemorySize = 16;
}

void CNodeText::Update(const PHOTSPOT spot)
{
    SIZE_T len;

    StandardUpdate(spot);

    if (spot->Id == 0)
    {
        m_MemorySize = _ttoi(spot->Text.GetString());
    }
    else if (spot->Id == 1)
    {
        len = spot->Text.GetLength() + 1;
        if (len > m_MemorySize)
            len = m_MemorySize;
        ReClassWriteMemory(spot->Address, (LPVOID)spot->Text.GetString(), len);
    }
}

NODESIZE CNodeText::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    char* pData;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    pData = (char*)(view->Data + m_Offset);

    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_TEXT, HS_NONE, HS_NONE);
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("char "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(view, tx, y, g_clrIndex, HS_NONE, _T("["));
    tx = AddText(view, tx, y, g_clrIndex, HS_EDIT, _T("%i"), GetMemorySize());
    tx = AddText(view, tx, y, g_clrIndex, HS_NONE, _T("]"));

    if (pData!=nullptr)
    {
        CStringA MemoryString(GetStringFromMemoryA(pData, GetMemorySize(), true));
        tx = AddText(view, tx, y, g_clrChar, HS_NONE, _T(" = '"));
        tx = AddText(view, tx, y, g_clrChar, 1, "%.150s", MemoryString.GetBuffer());
        tx = AddText(view, tx, y, g_clrChar, HS_NONE, _T("' ")) + g_FontWidth;
    }

    tx = AddComment(view, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}
