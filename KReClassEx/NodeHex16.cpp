#include "stdafx.h"
#include "NodeHex16.h"

void CNodeHex16::Update(const PHOTSPOT spot) {
	StandardUpdate(spot);
	unsigned char v = (unsigned char)(wcstoul(spot->Text, nullptr, 16) & 0xFF);
	if (spot->Id == 0)
		ReClassWriteMemory(spot->Address, &v, 1);
	if (spot->Id == 1)
		ReClassWriteMemory(spot->Address + 1, &v, 1);
}

NODESIZE CNodeHex16::Draw(const PVIEWINFO view, int x, int y) {
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

	if (g_bText) {
		CStringA str = GetStringFromMemoryA((const char*)data, 2);
		str+= "       ";
		tx = AddText(view, tx, y, g_clrChar, HS_NONE, "%s", str);
	}

	for (int i = 0; i < 2; i++) {
		tx = AddText(view, tx, y, g_clrHex, i, L"%0.2X", data[0] & 0xFF) + g_FontWidth;
	}
	tx = AddComment(view, tx, y);

	drawSize.x = tx;
	drawSize.y = y + g_FontHeight;
	return drawSize;
}