#include "stdafx.h"
#include "NodeUnicode.h"

CNodeUnicode::CNodeUnicode()
{
    m_NodeType = NodeType::Unicode;
    m_Name = _T("WCHAR");
    m_MemorySize = 8 * sizeof(wchar_t);
}

void CNodeUnicode::Update(const PHOTSPOT spot)
{
    LPCTSTR srcStr;
    wchar_t* dstStr;
    SIZE_T len;

    StandardUpdate(spot);

    if (spot->Id == 0)
    {
        m_MemorySize = _ttoi(spot->Text) * sizeof(wchar_t);
    }
    else if (spot->Id == 1)
    {
        len = spot->Text.GetLength();
        if (len > (m_MemorySize / sizeof(wchar_t)))
            len = (m_MemorySize / sizeof(wchar_t));

        // Has to be done this way in order to make it compatible in mbs and unicode mode (ghetto)
        srcStr = spot->Text.GetString();
        dstStr = new wchar_t[len + 1];
        for (size_t i = 0; i <= len; i++)
            dstStr[i] = (wchar_t)srcStr[i];

        ReClassWriteMemory(spot->Address, dstStr, len);

        delete[] dstStr;
    }
}

NODESIZE CNodeUnicode::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    wchar_t* data;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    data = (wchar_t*)(view->Data + m_Offset);

    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_TEXT, HS_NONE, HS_NONE);
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("WCHAR "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, _T("%s"), m_Name);
    tx = AddText(view, tx, y, g_clrIndex, HS_NONE, _T("["));
    tx = AddText(view, tx, y, g_clrIndex, HS_EDIT, _T("%i"), m_MemorySize / sizeof(wchar_t));
    tx = AddText(view, tx, y, g_clrIndex, HS_NONE, _T("]"));

    if (data!=nullptr)
    {
        CStringW memStr(GetStringFromMemoryW(data, m_MemorySize / sizeof(wchar_t), true));
        tx = AddText(view, tx, y, g_clrChar, HS_NONE, _T(" = '"));
        tx = AddText(view, tx, y, g_clrChar, HS_OPENCLOSE, _T("%.150ws"), memStr.GetString()); // ws cause its unicode
        tx = AddText(view, tx, y, g_clrChar, HS_NONE, _T("' ")) + g_FontWidth;
    }

    tx = AddComment(view, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}