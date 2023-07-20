#include "stdafx.h"
#include "NodeClass.h"

CNodeClass::CNodeClass() {
	WCHAR offset[128];
	m_NodeType = NodeType::Class;
	m_Offset = 0x140000000;

#ifdef _M_AMD64
	_ui64tot_s(m_Offset, offset, 128, 16);
#else

#endif // _M_AMD64
	m_OffsetText.SetString(offset);

	m_RequestPosition = -1;
	m_Idx = 0;
}

void CNodeClass::Update(const PHOTSPOT spot) {
	StandardUpdate(spot);
	if (spot->Id == 0) {
		m_OffsetText.SetString(spot->Text);
		// TODO: m_Offset 
	}
	else if (spot->Id == 1) {
		m_RequestPosition = _tcstol(spot->Text, nullptr, 10);
	}
}

ULONG CNodeClass::GetMemorySize() {
	int size = 0;
	for (UINT i = 0; i < m_ChildNodes.size(); i++) {
		size += m_ChildNodes[i]->GetMemorySize();
	}
	return size;
}

NODESIZE CNodeClass::Draw(const PVIEWINFO view, int x, int y) {
	NODESIZE size;
	NODESIZE childSize;
	int tx;

	size.x = 0;
	size.y = 0;

	AddSelection(view, x, y, g_FontHeight);
	x = AddOpenClose(view, x, y);

	// save tx here
	tx = x;

	x = AddIcon(view, x, y, ICON_CLASS, -1, -1);
	x = AddText(view, x, y, g_clrOffset, 0, L"%s", m_OffsetText) + g_FontWidth;

	x = AddText(view, x, y, g_clrIndex, HS_NONE, L"(");
	x = AddText(view, x, y, g_clrIndex, HS_OPENCLOSE, L"%i", m_Idx);
	x = AddText(view, x, y, g_clrIndex, HS_NONE, L")");

	x = AddText(view, x, y, g_clrType, HS_NONE, L"Class ");
	x = AddText(view, x, y, g_clrName, HS_NAME, m_Name) + g_FontWidth;
	x = AddText(view, x, y, g_clrValue, HS_NONE, L"[%i]", GetMemorySize()) + g_FontWidth;
	x = AddComment(view, x, y);

	size.x = x;
	y += g_FontHeight;
	if (m_LevelsOpen[view->Level]) {
		VIEWINFO info;
		memcpy(&info, view, sizeof(info));
		info.Level++;
		for (UINT i = 0; i < m_ChildNodes.size(); i++) {
			CNodeBase* pNode = m_ChildNodes[i];
			if (!pNode) {
				// node is nullptr in this class
				continue;
			}

			if (pNode->GetType() == NodeType::Vtable) {

			}

			if (pNode->GetType() == NodeType::Function) {

			}

			childSize = pNode->Draw(&info, tx, y);
			y = childSize.y;
			if (childSize.x > size.x) {
				size.x = childSize.x;
			}
		}
	}

	size.y = y;
	return size;
}