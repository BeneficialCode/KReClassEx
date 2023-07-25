#include "stdafx.h"
#include "NodeHex8.h"


void CNodeHex8::Update(const PHOTSPOT spot) {
	StandardUpdate(spot);
	unsigned char v = (unsigned char)(_tcstoul(spot->Text, NULL, 16) & 0xFF);
	if (spot->Id == 0)
		ReClassWriteMemory(spot->Address, &v, 1);
}

NODESIZE CNodeHex8::Draw(const PVIEWINFO view, int x, int y) {
	int tx;
	NODESIZE drawSize;
	const UCHAR* data;

	if (m_bHidden)
		return DrawHidden(view, x, y);

	data = (const UCHAR*)(view->Data + m_Offset);
	AddSelection(view, 0, y, g_FontHeight);
	AddDelete(view, x, y);
	AddTypeDrop(view, x, y);

	tx = x + TXOFFSET + 16;
	tx = AddAddressOffset(view, tx, y);
	
	if (g_bText)
	{
		CStringA str = GetStringFromMemoryA((const char*)data, 1);
		str += "        ";
		tx = AddText(view, tx, y, g_clrChar, HS_NONE, "%s", str.GetBuffer());
	}

	tx = AddText(view, tx, y, g_clrHex, 0, L"%0.2X", data[0] & 0xFF) + g_FontWidth;
	tx = AddComment(view, tx, y);

	drawSize.x = tx;
	drawSize.y = y + g_FontHeight;
	return drawSize;
}