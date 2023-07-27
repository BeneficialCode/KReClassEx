#include "stdafx.h"
#include "NodeBits.h"
#include "BitArray.h"

void CNodeBits::Update(const PHOTSPOT spot) {
	StandardUpdate(spot);
	unsigned char v = (unsigned char)(_tcstoul(spot->Text.GetString(), NULL, 16) & 0xFF);
	if (spot->Id == 0)
		ReClassWriteMemory(spot->Address, &v, 1);
}

NODESIZE CNodeBits::Draw(const PVIEWINFO view, int x, int y) {
	UCHAR* data;
	NODESIZE drawSize;

	if (m_bHidden)
		return DrawHidden(view, x, y);

	data = (UCHAR*)&view->Data[m_Offset];
	AddSelection(view, 0, y, g_FontHeight);
	AddDelete(view, x, y);
	AddTypeDrop(view, x, y);

	int tx = x + TXOFFSET + 16;
	tx = AddAddressOffset(view, tx, y);

	if (g_bText)
	{
		BitArray<UCHAR> bits;
		bits.SetValue(data[0]);

		tx = AddText(view, tx, y, g_clrChar, HS_EDIT, "%s ", bits.GetBitsReverseString());
	}

	tx = AddText(view, tx, y, g_clrHex, 0, _T("%0.2X"), data[0]) + g_FontWidth;
	tx = AddComment(view, tx, y);

	drawSize.x = tx;
	drawSize.y = y + g_FontHeight;

	return drawSize;
}