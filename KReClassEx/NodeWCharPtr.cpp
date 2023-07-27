#include "stdafx.h"
#include "NodeWCharPtr.h"

void CNodeWCharPtr::Update(const PHOTSPOT spot) {
    uintptr_t value;

    StandardUpdate(spot);

    value = (uintptr_t)_ttoi64(spot->Text);
    if (spot->Id == 0)
        ReClassWriteMemory(spot->Address, &value, sizeof(uintptr_t));
}

NODESIZE CNodeWCharPtr::Draw(const PVIEWINFO view, int x, int y)
{
    int tx;
    NODESIZE drawSize;
    uintptr_t* data;

    if (m_bHidden)
        return DrawHidden(view, x, y);

    data = (uintptr_t*)(view->Data + m_Offset);

    AddSelection(view, 0, y, g_FontHeight);
    AddDelete(view, x, y);
    AddTypeDrop(view, x, y);
    //AddAdd(View, x, y);

    tx = x + TXOFFSET;
    tx = AddIcon(view, tx, y, ICON_INTEGER, HS_NONE, HS_NONE);
    tx = AddAddressOffset(view, tx, y);
    tx = AddText(view, tx, y, g_clrType, HS_NONE, _T("PWCHAR "));
    tx = AddText(view, tx, y, g_clrName, HS_NAME, m_Name);


    tx = AddText(view, tx, y, g_clrChar, HS_NONE, _T(" = '"));
    if (data!=nullptr)
    {
        CStringW MemoryString = ReadMemoryStringW(*data, 128);
        tx = AddText(view, tx, y, g_clrChar, 1, "%ls", MemoryString.GetBuffer());
    }

    tx = AddText(view, tx, y, g_clrChar, HS_NONE, _T("' ")) + g_FontWidth;
    tx = AddComment(view, tx, y);

    drawSize.x = tx;
    drawSize.y = y + g_FontHeight;
    return drawSize;
}

CStringW CNodeWCharPtr::ReadMemoryStringW(ULONG_PTR address, SIZE_T max) {
    SIZE_T bytesRead = 0;
    auto buffer = std::make_unique<wchar_t[]>(max + 1);

    if (ReClassReadMemory(address, buffer.get(), 
        max * sizeof(wchar_t), &bytesRead) != 0) {

        for (size_t i = 0; i < bytesRead; i++) {
            if (!(buffer[i] > 0x1F && buffer[i] < 0xFF && buffer[i] != 0x7F) && buffer[i] != '\0')
                buffer[i] = '.';
        }
        buffer[bytesRead] = '\0';
        return CStringW(buffer.get());
    }
    else {
        return L"..";
    }
}