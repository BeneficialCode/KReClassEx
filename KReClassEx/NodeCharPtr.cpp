#include "stdafx.h"
#include "NodeCharPtr.h"

CNodeCharPtr::CNodeCharPtr()
{
    m_NodeType = NodeType::PChar;
    m_Name = "PChar";
}

void CNodeCharPtr::Update(const PHOTSPOT pSpot)
{
    uintptr_t value;

    StandardUpdate(pSpot);

#if defined(_M_AMD64)
    value = (uintptr_t)_ttoi64(pSpot->Text.GetString());
#else
    value = (uintptr_t)_ttoi(Spot->Text.GetString());
#endif
    if (pSpot->Id == 0)
        ReClassWriteMemory(pSpot->Address, &value, sizeof(uintptr_t));
}

NODESIZE CNodeCharPtr::Draw(const PVIEWINFO pView, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    uintptr_t* pData;

    if (m_bHidden)
        return DrawHidden(pView, x, y);

    pData = (uintptr_t*)(pView->Data + m_Offset);

    AddSelection(pView, 0, y, g_FontHeight);
    AddDelete(pView, x, y);
    AddTypeDrop(pView, x, y);
    //AddAdd(View, x, y);

    tx = x + TXOFFSET;
    tx = AddIcon(pView, tx, y, ICON_INTEGER, HS_NONE, HS_NONE);
    tx = AddAddressOffset(pView, tx, y);
    tx = AddText(pView, tx, y, g_clrType, HS_NONE, _T("PCHAR "));
    tx = AddText(pView, tx, y, g_clrName, HS_NAME, m_Name);

    //tx = AddText(View,tx,y,g_clrName,HS_NONE," = ");
    //tx = AddText(View,tx,y,g_clrValue,0,"%lli",pMemory[0]) + FontWidth;
    //tx = AddComment(View,tx,y);

    /*
    int tx = x + 16;
    tx = AddIcon(View,tx,y,ICON_TEXT,HS_NONE,HS_NONE);
    tx = AddAddressOffset(View,tx,y);
    tx = AddText(View,tx,y,g_clrType,HS_NONE,"Text ");
    tx = AddText(View,tx,y,g_clrName,69,"%s",m_strName);
    tx = AddText(View,tx,y,g_clrIndex,HS_NONE,"[");
    tx = AddText(View,tx,y,g_clrIndex,0,"%i",memsize);
    tx = AddText(View,tx,y,g_clrIndex,HS_NONE,"]");
    */

    tx = AddText(pView, tx, y, g_clrChar, HS_NONE, _T(" = '"));
    if (pData!=nullptr)
    {
        CStringA MemoryString(ReadMemoryStringA(*pData, 128));
        tx = AddText(pView, tx, y, g_clrChar, 1, "%s", MemoryString.GetBuffer());
    }

    tx = AddText(pView, tx, y, g_clrChar, HS_NONE, _T("' ")) + g_FontWidth;
    tx = AddComment(pView, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}

CStringA CNodeCharPtr::ReadMemoryStringA(ULONG_PTR address, SIZE_T max)
{
    SIZE_T bytesRead = 0;
    auto buffer = std::make_unique<char[]>(max + 1);

    if (ReClassReadMemory(address, (LPVOID)buffer.get(), max, &bytesRead) != 0)
    {
        for (size_t i = 0; i < bytesRead; i++)
        {
            if (!(buffer[i] > 0x1F && buffer[i] < 0xFF && buffer[i] != 0x7F) && buffer[i] != '\0')
                buffer[i] = '.';
        }
        buffer[bytesRead] = '\0';
        return CStringA(buffer.get());
    }
    else
    {
        return CStringA("....");
    }
}