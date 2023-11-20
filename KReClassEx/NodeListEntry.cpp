#include "stdafx.h"
#include "NodeListEntry.h"

CNodeListEntry::CNodeListEntry() {
	m_NodeType = NodeType::ListEntry;
	for (size_t i = 0; i < m_LevelsOpen.size(); i++)
		m_LevelsOpen[i] = true;
}

void CNodeListEntry::Update(const PHOTSPOT spot) {
	ULONG_PTR value;

	StandardUpdate(spot);

	value = (ULONG_PTR)_ttoll(spot->Text.GetString());
	if (spot->Id >= 0 && spot->Id < 2)
		ReClassWriteMemory(spot->Address + spot->Id * 8, &value, sizeof(value));
}

NODESIZE CNodeListEntry::Draw(const PVIEWINFO view, int x, int y) {
	int tx;
	NODESIZE drawSize;
	ULONG_PTR* pData;

	if (m_bHidden)
		return DrawHidden(view, x, y);

	pData = (ULONG_PTR*)(view->Data + m_Offset);

	AddSelection(view, 0, y, g_FontHeight);
	AddDelete(view, x, y);
	AddTypeDrop(view, x, y);
	tx = x + TXOFFSET;
	tx = AddIcon(view, tx, y, ICON_VECTOR, HS_NONE, HS_NONE);
	tx = AddAddressOffset(view, tx, y);
	tx = AddText(view, tx, y, g_clrType, HS_NONE, L"LIST_ENTRY ");
	tx = AddText(view, tx, y, g_clrName, HS_NAME, L"%s", m_Name);
	if (m_LevelsOpen[view->Level]) {
		tx = AddText(view, tx, y, g_clrName, HS_NONE, L"<");
		tx = AddText(view, tx, y, g_clrValue, HS_EDIT, L"%p", pData[0]);
		tx = AddText(view, tx, y, g_clrName, HS_NONE, L",");
		tx = AddText(view, tx, y, g_clrValue, HS_OPENCLOSE, L"%p", pData[1]);
		tx = AddText(view, tx, y, g_clrName, HS_NONE, L">");
	}
	tx += g_FontWidth;
	tx = AddComment(view, tx, y);

	drawSize.x = tx;
	drawSize.y = y + g_FontHeight;
	return drawSize;
}