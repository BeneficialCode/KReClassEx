#include "stdafx.h"
#include "NodeHex32.h"


void CNodeHex32::Update(const PHOTSPOT spot) {
	StandardUpdate(spot);
	unsigned char v = (unsigned char)(wcstoul(spot->Text, nullptr, 16) & 0xFF);
	if (spot->Id >= 0 && spot->Id < 4)
		ReClassWriteMemory(spot->Address + spot->Id, &v, 1);
}

NODESIZE CNodeHex32::Draw(const PVIEWINFO view, int x, int y) {
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
		// TODO: these are the dots, do alignment instead of 4
		CStringA str = GetStringFromMemoryA((const char*)data, 4);
		str += "     ";
		tx = AddText(view, tx, y, g_clrChar, HS_NONE, "%s", str);
	}

	for (int i = 0; i < 4; i++) {
		tx = AddText(view, tx, y, g_clrHex, i, L"%0.2X", data[i]) + g_FontWidth;
	}
	tx = AddComment(view, tx, y);

	drawSize.x = tx;
	drawSize.y = y + g_FontHeight;
	return drawSize;
}