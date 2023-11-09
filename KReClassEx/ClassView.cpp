#include "stdafx.h"
#include "ClassView.h"
#include "resource.h"

#define SB_WIDTH 14

void CClassView::DoPaint(CDCHandle dc, RECT& rect) {
	dc.FillSolidRect(&rect, g_clrBackground);

	VIEWINFO info;
	ULONG classSize;
	int xPos, yPos;
	NODESIZE drawMax;
	CRect clientRect = rect;

	if (m_pClass != nullptr) {
		dc.SelectFont(g_ViewFont);

		m_Hotspots.clear();

		classSize = m_pClass->GetMemorySize();
		m_Memory.SetSize(classSize);
		void* buffer = m_Memory.Data();
		if (classSize > 0 && buffer !=nullptr) {
			ReClassReadMemory(m_pClass->GetOffset(), buffer, classSize);
		}

		info.Address = m_pClass->GetOffset();
		info.Data = m_Memory.Data();

		info.ClientRect = &clientRect;
		info.Dc = &dc;
		info.Level = 0;
		info.Hotspots = &m_Hotspots;
		info.MultiSelected = m_Selected.size() > 1;
		

		if (m_VScroll.IsWindowVisible()) {
			info.ClientRect->right -= SB_WIDTH;
			info.ClientRect->bottom -= SB_WIDTH;
		}


		SCROLLINFO si;

		yPos = m_VScroll.GetScrollPos() * g_FontHeight;
		xPos = m_HScroll.GetScrollPos();


		// do the draw!
		drawMax = m_pClass->Draw(&info, 0 - xPos, -yPos);

		// dirty hack fix draw methods
		drawMax.x += xPos;
		drawMax.y += yPos;


		if (m_pClass->m_RequestPosition != -1) {

		}

		if (clientRect.Height() < drawMax.y) {
			ZeroMemory(&si, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_PAGE | SIF_RANGE;
			si.nMin = 0;
			si.nMax = drawMax.y / g_FontHeight;
			si.nPage = clientRect.Height() / g_FontHeight;
			m_VScroll.SetScrollInfo(&si);
			m_VScroll.ShowScrollBar();
		}
		else {
			m_VScroll.SetScrollPos(0);
			m_VScroll.ShowScrollBar(FALSE);
		}

		if (clientRect.Width() < drawMax.x) {
			ZeroMemory(&si, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_PAGE | SIF_RANGE;
			si.nMin = 0;
			si.nMax = drawMax.x;
			si.nPage = clientRect.Width();
			m_HScroll.SetScrollInfo(&si);
			m_HScroll.ShowScrollBar(TRUE);
		}
		else {
			m_HScroll.SetScrollPos(0);
			m_HScroll.ShowScrollBar(FALSE);
		}
	}
}

void CClassView::OnSize(UINT nType, CSize size) {
	CRect client;
	GetClientRect(&client);
	m_VScroll.SetWindowPos(nullptr, client.right - SB_WIDTH, 0, 
		SB_WIDTH, client.Height() - SB_WIDTH, SWP_NOZORDER);
	m_HScroll.SetWindowPos(nullptr, client.left, client.bottom - SB_WIDTH,
		client.Width() - SB_WIDTH, SB_WIDTH, SWP_NOZORDER);
	m_Edit.ShowWindow(SW_HIDE);
}

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CRect rect(0, 0, 100, 100);
	m_Edit.Create(m_hWnd, rect, nullptr, WS_CHILD | WS_TABSTOP, ES_AUTOHSCROLL);
	m_Edit.ShowWindow(SW_HIDE);
	m_Edit.SetFont(g_ViewFont);
	m_VScroll.Create(m_hWnd, rect, nullptr, SBS_VERT | WS_CHILD | WS_VISIBLE);
	m_VScroll.EnableScrollBar(ESB_ENABLE_BOTH);
	m_VScroll.ShowScrollBar();

	CRect hrect(5, 5, 100, 30);
	m_HScroll.Create(m_hWnd, hrect, nullptr, SBS_HORZ | WS_CHILD | WS_VISIBLE);
	m_HScroll.EnableScrollBar(ESB_ENABLE_BOTH);
	m_HScroll.ShowScrollBar();

	m_ToolTip.Create(m_hWnd, rect, nullptr, ES_MULTILINE | WS_BORDER);
	m_ToolTip.SetFont(g_ViewFont);
	m_ToolTip.EnableWindow(FALSE);

	SetTimer(1, 250, nullptr);
	return 0;
}

void CClassView::OnRButtonDown(UINT nFlags, CPoint point) {
	size_t i;
	CNodeBase* pNode;

	m_Edit.ShowWindow(SW_HIDE);

	for (i = 0; i < m_Hotspots.size(); i++) {
		if (m_Hotspots[i].Rect.PtInRect(point)) {
			pNode = m_Hotspots[i].Object;

			if (m_Hotspots[i].Type == HS_CLICK) {
				pNode->Update(&m_Hotspots[i]);
			}
			else if (m_Hotspots[i].Type == HS_SELECT) {
				if (nFlags == MK_RBUTTON) {
					ClearSelection();

					pNode->Select();
					m_Selected.push_back(m_Hotspots[i]);

					CRect rect;
					GetClientRect(&rect);
					ClientToScreen(&rect);

					CMenu menu;
					menu.LoadMenu(IDR_MENU_QUICKMODIFY);
					TrackPopupMenu(menu.GetSubMenu(0), TPM_LEFTALIGN | TPM_HORNEGANIMATION,
						rect.left + m_Hotspots[i].Rect.left + point.x,
						rect.top + point.y, 0, m_hWnd, nullptr);
				}
			}

			// Update
			Invalidate();
		}
	}
}

void CClassView::ClearSelection()
{
	for (size_t i = 0; i < m_Selected.size(); i++) {
		m_Selected[i].Object->Unselect();
	}
	m_Selected.clear();
}

void CClassView::OnTimer(UINT_PTR nIDEvent) {
	if (nIDEvent == 1)
		Invalidate(FALSE);
}

void CClassView::OnLButtonDown(UINT nFlags, CPoint point) {
	CNodeBase* pObjectHit;
	CNodeBase* pSelectedNode;
	CNodeClass* pSelectedParentClassNode;
	UINT idx1, idx2;
	size_t i, s, j, m;

	m_Edit.ShowWindow(SW_HIDE);

	for (i = 0; i < m_Hotspots.size(); i++) {
		if (m_Hotspots[i].Rect.PtInRect(point)) {
			pObjectHit = static_cast<CNodeBase*>(m_Hotspots[i].Object);
			if (m_Hotspots[i].Type == HS_OPENCLOSE) {
				pObjectHit->ToggleLevelOpen(m_Hotspots[i].Level);
			}

			if (m_Hotspots[i].Type == HS_CLICK) {
				pObjectHit->Update(&m_Hotspots[i]);
			}

			if (m_Hotspots[i].Type == HS_SELECT) {
				if (nFlags == MK_LBUTTON) {
					ClearSelection();
					pObjectHit->Select();
					m_Selected.push_back(m_Hotspots[i]);
				}
				if (nFlags == (MK_LBUTTON | MK_CONTROL)) {
					pObjectHit->ToggleSelected();
					if (pObjectHit->IsSelected()) {
						m_Selected.push_back(m_Hotspots[i]);
					}
					else {
						for (s = 0; s < m_Selected.size(); s++) {
							if (m_Selected[s].Object == pObjectHit) {
								m_Selected.erase(m_Selected.begin() + s);
								break;
							}
						}
					}
				}

				if (nFlags == (MK_LBUTTON | MK_SHIFT)) {
					if (m_Selected.size() > 0) {
						pSelectedNode = m_Selected[0].Object;
						if (pSelectedNode->GetParent() != pObjectHit->GetParent())
							continue;

						pSelectedParentClassNode = static_cast<CNodeClass*>(pSelectedNode->GetParent());
						if (pSelectedParentClassNode->GetType() != NodeType::Class)
							continue;

						idx1 = FindNodeIndex(pSelectedNode);
						if (idx1 == MAX_NODES)
							continue;

						idx2 = FindNodeIndex(pObjectHit);
						if (idx2 == MAX_NODES)
							continue;

						if (idx2 < idx1)
							std::swap(idx1, idx2);

						ClearSelection();

						for (s = idx1; s <= idx2; s++) {
							HOTSPOT hotspot;
							hotspot.Address = pSelectedParentClassNode->GetOffset() +
								pSelectedParentClassNode->GetNode(s)->GetOffset();
							hotspot.Object = pSelectedParentClassNode->GetNode(s);
							hotspot.View = this;

							pSelectedParentClassNode->GetNode(s)->Select();
							m_Selected.push_back(hotspot);
						}
					}
				}
			}

			if (m_Hotspots[i].Type == HS_DROP) {
				CMenu menu;
				CRect rect;

				GetClientRect(&rect);
				ClientToScreen(&rect);

				menu.LoadMenu(IDR_MENU_QUICKMODIFY);
				TrackPopupMenu(menu.GetSubMenu(0), TPM_LEFTALIGN | TPM_HORNEGANIMATION,
					rect.left + m_Hotspots[i].Rect.left,
					rect.top + m_Hotspots[i].Rect.bottom, 0, m_hWnd, nullptr);
			}

			if (m_Hotspots[i].Type == HS_DELETE) {
				for (s = 0; s < m_Selected.size(); s++) {
					pSelectedParentClassNode = static_cast<CNodeClass*>(m_Selected[s].Object->GetParent());
					idx1 = FindNodeIndex(m_Selected[s].Object);
					if (idx1 != MAX_NODES) {
						pSelectedParentClassNode->DeleteNode(idx1);
						m_pFrame->CalcAllOffsets();
					}
				}
				m_Selected.clear();
			}

			if ((m_Hotspots[i].Type == HS_CHANGE_A) || (m_Hotspots[i].Type == HS_CHANGE_X)) {

			}

			Invalidate();
		}
	}
}

UINT CClassView::FindNodeIndex(CNodeBase* pNode) {
	CNodeClass* pParentClass;
	int index;

	pParentClass = static_cast<CNodeClass*>(pNode->GetParent());
	if (!pParentClass)
		return MAX_NODES;

	index = pParentClass->FindNode(pNode);

	if (index != -1)
		return index;

	return MAX_NODES;
}

void CClassView::OnLButtonDblClk(UINT nFlags, CPoint point) {
	for (const auto& hotspot : m_Hotspots) {
		if (hotspot.Rect.PtInRect(point)) {
			if (hotspot.Type == HS_EDIT) {
				//
				// Sets the edit window to where to cursor was editing at
				//
				m_Edit.SetWindowPos(
					nullptr,
					hotspot.Rect.left,
					hotspot.Rect.top,
					hotspot.Rect.Width(),
					hotspot.Rect.Height(),
					SWP_NOZORDER);

				m_Edit.m_Hotspot = hotspot;
				m_Edit.m_MinWidth = m_Edit.m_Hotspot.Rect.Width();
				m_Edit.SetWindowText(hotspot.Text);
				m_Edit.ShowWindow(SW_NORMAL);
				m_Edit.SetFocus();
				m_Edit.SetSel(0, 1024);
				return;
			}
		}
	}
}

void CClassView::OnAdd(DWORD size) {
	if (m_Selected[0].Object->GetType() == NodeType::Class) {
		AddBytes((CNodeClass*)m_Selected[0].Object, size);
	}
	else {
		AddBytes((CNodeClass*)m_Selected[0].Object->GetParent(), size);
	}
	Invalidate(FALSE);
}

LRESULT CClassView::OnAdd4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// AtlMessageBox(m_hWnd, L"I'm comming!", L"Info", MB_OK);
	OnAdd(4);
	return 0;
}

void CClassView::AddBytes(CNodeClass* pClass, DWORD length) {
	if (!pClass)
		return;

	if (length == 4) {
		CNodeBase* pNode = nullptr;
		if (pClass->GetType() == NodeType::VTable) {
			
		}
		else {
			pNode = new CNodeHex32;
		}
		pNode->SetParent(pClass);
		pClass->AddNode(pNode);
		m_pFrame->CalcAllOffsets();
		return;
	}

	for (UINT i = 0; i < length / sizeof(ULONG_PTR); i++) {
		CNodeBase* pNode;
		if (pClass->GetType() == NodeType::VTable) {

		}
		else {
			pNode = new CNodeHex;
		}

		pNode->SetParent(pClass);
		pClass->AddNode(pNode);
	}

	m_pFrame->CalcAllOffsets();
}

CClassView::CClassView(IMainFrame* frame):m_pFrame(frame){
	m_Edit.SetClassView(this);
}

LRESULT CClassView::OnAdd8(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	OnAdd(8);
	return 0;
}

LRESULT CClassView::OnAdd64(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	OnAdd(64);
	return 0;
}

LRESULT CClassView::OnAdd128(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	OnAdd(128);
	return 0;
}

LRESULT CClassView::OnAdd256(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	OnAdd(256);
	return 0;
}

void CClassView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar) {
	m_Edit.ShowWindow(SW_HIDE);

	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
	{
		pScrollBar.SetScrollPos(nPos);
		Invalidate();
	}
	else if (nSBCode == SB_LINEUP || nSBCode == SB_LINEDOWN) {
		pScrollBar.SetScrollPos(pScrollBar.GetScrollPos() + ((nSBCode == SB_LINEUP) ? -1 : 1));
		Invalidate();
	}
}

void CClassView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar) {
	m_Edit.ShowWindow(SW_HIDE);

	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
	{
		pScrollBar.SetScrollPos(nPos);
		Invalidate();
	}
	else if (nSBCode == SB_LINELEFT || nSBCode == SB_LINERIGHT)
	{
		pScrollBar.SetScrollPos(pScrollBar.GetScrollPos() + ((nSBCode == SB_LINEUP) ? -1 : 1));
		Invalidate();
	}
}

BOOL CClassView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	if (m_VScroll.IsWindowVisible()) {
		if (GetAsyncKeyState(VK_LCONTROL))
			m_VScroll.SetScrollPos(m_VScroll.GetScrollPos() + ((zDelta < 0 ? 1 : -1)));
		else
			m_VScroll.SetScrollPos(m_VScroll.GetScrollPos() - ((int)zDelta / g_FontHeight));
		m_Edit.ShowWindow(SW_HIDE);
		m_ToolTip.ShowWindow(SW_HIDE);
		Invalidate();
	}
	return TRUE;
}

LRESULT CClassView::OnAdd512(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	OnAdd(512);
	return 0;
}

LRESULT CClassView::OnAdd1024(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	OnAdd(1024);
	return 0;
}
	
LRESULT CClassView::OnAdd2048(WORD, WORD, HWND, BOOL&) {
	OnAdd(2048);
	return 0;
}

void CClassView::InsertBytes(CNodeClass* pClass, UINT index, DWORD length) {
	if (!pClass || index == MAX_NODES) {
		return;
	}

	if (length == 4) {
		CNodeBase* pNode = nullptr;
		if (pClass->GetType() == NodeType::VTable) {

		}
		else {
			pNode = new CNodeHex32;
		}

		pNode->SetParent(pClass);
		pClass->InsertNode(index, pNode);
		m_pFrame->CalcAllOffsets();
		return;
	}

	for (UINT i = 0; i < length / sizeof(ULONG_PTR); i++) {
		CNodeBase* pNode;
		if (pClass->GetType() == NodeType::VTable) {

		}
		else {
			pNode = new CNodeHex;
		}

		pNode->SetParent(pClass);
		pClass->InsertNode(index, pNode);
	}

	m_pFrame->CalcAllOffsets();
}

void CClassView::OnInsert(DWORD size) {
	InsertBytes((CNodeClass*)m_Selected[0].Object->GetParent(), FindNodeIndex(m_Selected[0].Object),
		size);
	Invalidate(FALSE);
}

LRESULT CClassView::OnInsert4(WORD, WORD, HWND, BOOL&) {
	OnInsert(4);
	return 0;
}

LRESULT CClassView::OnInsert8(WORD, WORD, HWND, BOOL&) {
	OnInsert(8);
	return 0;
}

LRESULT CClassView::OnInsert64(WORD, WORD, HWND, BOOL&) {
	OnInsert(64);
	return 0;
}

LRESULT CClassView::OnInsert128(WORD, WORD, HWND, BOOL&) {
	OnInsert(128);
	return 0;
}

LRESULT CClassView::OnInsert256(WORD, WORD, HWND, BOOL&) {
	OnInsert(256);
	return 0;
}

LRESULT CClassView::OnInsert512(WORD, WORD, HWND, BOOL&) {
	OnInsert(512);
	return 0;
}

LRESULT CClassView::OnInsert1024(WORD, WORD, HWND, BOOL&) {
	OnInsert(1024);
	return 0;
}

LRESULT CClassView::OnInsert2048(WORD, WORD, HWND, BOOL&) {
	OnInsert(2048);
	return 0;
}

LRESULT CClassView::OnModifyHide(WORD, WORD, HWND, BOOL&) {
	for (UINT i = 0; i < m_Selected.size(); i++)
		m_Selected[i].Object->Hide();
	Invalidate(FALSE);
	return 0;
}

LRESULT CClassView::OnModifyShow(WORD, WORD, HWND, BOOL&) {
	m_pFrame->ClearHidden();
	return 0;
}

LRESULT CClassView::OnModifyDelete(WORD, WORD, HWND, BOOL&) {
	for (UINT i = 0; i < m_Selected.size(); i++) {
		CNodeBase* pClass = (CNodeClass*)m_Selected[i].Object->GetParent();
		UINT idx = FindNodeIndex(m_Selected[i].Object);
		if (idx != MAX_NODES) {
			pClass->DeleteNode(idx);
			m_pFrame->CalcAllOffsets();
		}
	}
	m_Selected.clear();
	return 0;
}

void CClassView::ReplaceSelectedWithType(NodeType type) {
	std::vector<CNodeBase*> newSelected;


	for (size_t i = 0; i < m_Selected.size(); i++) {
		if (!m_pFrame->IsNodeValid(m_Selected[i].Object))
			continue;

		if (m_Selected[i].Object->GetParent()->GetType() == NodeType::VTable) {
			type = NodeType::FunctionPtr;
		}

		CNodeBase* pNewNode = m_pFrame->CreateNewNode(type);

		if (type == NodeType::Class) {
			MakeBasicClass((CNodeClass*)pNewNode);
		}

		if (type == NodeType::Custom) {
			((CNodeCustom*)pNewNode)->SetSize(m_Selected[i].Object->GetMemorySize());
		}
		if (type == NodeType::Text) {
			((CNodeText*)pNewNode)->SetSize(m_Selected[i].Object->GetMemorySize());
		}
		if (type == NodeType::Unicode) {
			((CNodeUnicode*)pNewNode)->SetSize(m_Selected[i].Object->GetMemorySize());
		}
		if (type == NodeType::VTable) {
			for (int i = 0; i < 10; i++)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNewNode;
				pVTable->Initialize(this);

				PUCHAR pMem = m_Memory.Data();
				size_t offset = i * sizeof(ULONG_PTR);
				ULONG_PTR value = *(PULONG_PTR)(pMem + offset);
				CNodeFunctionPtr* pFunctionPtr = new CNodeFunctionPtr(this, pVTable->GetOffset() + offset, value);
				pFunctionPtr->SetOffset(pVTable->GetOffset() + (i * sizeof(size_t)));
				pFunctionPtr->SetParent(pVTable);

				pVTable->AddNode(pFunctionPtr);
			}
		}
		if (type == NodeType::Pointer) {
			CNodePtr* pPtr = (CNodePtr*)pNewNode;
			CNodeClass* pClass = (CNodeClass*)m_pFrame->CreateNewNode(NodeType::Class);
			MakeBasicClass(pClass);
			pPtr->SetClass(pClass);
		}
		if (type == NodeType::Array)
		{
			CNodeArray* pArray = (CNodeArray*)pNewNode;
			CNodeClass* pClass = (CNodeClass*)m_pFrame->CreateNewNode(NodeType::Class);
			MakeBasicClass(pClass);
			pArray->SetClass(pClass);
		}
		if (type == NodeType::PtrArray)
		{
			CNodePtrArray* pArray = (CNodePtrArray*)pNewNode;
			CNodeClass* pClass = (CNodeClass*)m_pFrame->CreateNewNode(NodeType::Class);
			MakeBasicClass(pClass);
			pArray->SetClass(pClass);
		}
		if (type == NodeType::Instance)
		{
			CNodeClassInstance* pInstance = (CNodeClassInstance*)pNewNode;
			CNodeClass* pClass = (CNodeClass*)m_pFrame->CreateNewNode(NodeType::Class);
			MakeBasicClass(pClass);
			pInstance->SetClass(pClass);
		}
		if (type == NodeType::Function)
		{
			CNodeFunction* pFunction = (CNodeFunction*)pNewNode;
			
		}
		if (type == NodeType::FunctionPtr)
		{
			CNodeFunctionPtr* pFunctionPtr = (CNodeFunctionPtr*)pNewNode;
			PUCHAR pMem = m_Memory.Data();
			size_t offset = m_Selected[i].Object->GetOffset();
			ULONG_PTR value = *(PULONG_PTR)(pMem + offset);
			pFunctionPtr->Initialize(this, m_Selected[i].Object->GetParent()->GetOffset()
				+ offset, value);
		}

		ReplaceNode((CNodeClass*)m_Selected[i].Object->GetParent(),
			FindNodeIndex(m_Selected[i].Object), pNewNode);

		newSelected.push_back(pNewNode);
	}

	m_Selected.clear();
	for (UINT i = 0; i < newSelected.size(); i++) {
		newSelected[i]->Select();
		CNodeClass* pClass = (CNodeClass*)newSelected[i]->GetParent();

		HOTSPOT hotspot;
		hotspot.Address = pClass->GetOffset() + newSelected[i]->GetOffset();
		hotspot.Object = newSelected[i];
		hotspot.View = this;
		m_Selected.push_back(hotspot);
	}

	Invalidate(FALSE);
}

void CClassView::ReplaceNode(CNodeClass* pClass, UINT index, CNodeBase* pNewNode) {
	if (!pClass || index == MAX_NODES)
		return;

	CNodeBase* pOldNode = pClass->GetNode(index);
	pNewNode->SetName(pOldNode->GetName());
	pNewNode->SetComment(pOldNode->GetComment());

	pNewNode->SetParent(pClass);
	pNewNode->Unselect();

	pClass->SetNode(index, pNewNode);

	DWORD oldSize = pOldNode->GetMemorySize();
	DWORD newSize = pNewNode->GetMemorySize();

	if (oldSize != newSize) {
		if (newSize < oldSize) {
			FillNodes(pClass, index + 1, oldSize - newSize);
		}
		else {
			RemoveNodes(pClass, index + 1, newSize - oldSize);
		}
	}

	delete pOldNode;

	m_pFrame->CalcAllOffsets();
}

void CClassView::FillNodes(CNodeClass* pClass, UINT index, DWORD length) {
	if (!pClass || index >= MAX_NODES)
		return;

	size_t offset = 0;

	if (index > 0) {
		CNodeBase* pNode = pClass->GetNode(index - 1);
		offset = pNode->GetOffset() + pNode->GetMemorySize();
	}

	while (length != 0) {
		// Assume 8 ?
		// TODO: to figure this out
		if (length >= 8) {
			CNodeHex64* pFill = new CNodeHex64;
			pFill->SetParent(pClass);
			pFill->SetOffset(offset);

			pClass->InsertNode(index, pFill);

			offset += 8;
			length -= 8;
			index++;
		}

		if (length >= 4) {
			CNodeHex32* pFill = new CNodeHex32;
			pFill->SetParent(pClass);
			pFill->SetOffset(offset);
			pClass->InsertNode(index, pFill);
			offset += 4;
			length -= 4;
			index++;
		}

		if (length >= 2 && length < 4) {
			CNodeHex16* pFill = new CNodeHex16;
			pFill->SetParent(pClass);
			pFill->SetOffset(offset);
			pClass->InsertNode(index, pFill);
			offset += 2;
			length -= 2;
			index++;
		}

		if (length == 1) {
			CNodeHex8* pFill = new CNodeHex8;
			pFill->SetParent(pClass);
			pFill->SetOffset(offset);
			pClass->InsertNode(index, pFill);
			offset += 1;
			length -= 1;
			index++;
		}
	}
}

void CClassView::RemoveNodes(CNodeClass* pClass, UINT index, DWORD length) {
	if (!pClass || index == MAX_NODES)
		return;

	UINT t = 0;
	DWORD totalSize = 0;
	for (UINT i = index; i < pClass->NodeCount(); i++) {
		totalSize += pClass->GetNode(i)->GetMemorySize();
		t++;
		if (totalSize >= length)
			break;
	}

	for (UINT i = 0; i < t; i++) {
		pClass->DeleteNode(index);
	}

	if (totalSize > length) {
		FillNodes(pClass, index, totalSize - length);
	}

	m_pFrame->CalcAllOffsets();
}

LRESULT CClassView::OnTypeHex64(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Hex64);
	return 0;
}

void CClassView::MakeBasicClass(CNodeClass* pClass) {
	for (int i = 0; i < 1; i++) {
		CNodeHex* pNode = new CNodeHex();
		pNode->SetParent(pClass);
		pClass->AddNode(pNode);
	}
	m_pFrame->CalcOffsets(pClass);
	m_pFrame->m_Classes.push_back(pClass);
}

LRESULT CClassView::OnTypeHex32(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Hex32);
	return 0;
}

LRESULT CClassView::OnTypeHex16(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Hex16);
	return 0;
}

LRESULT CClassView::OnTypeHex8(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Hex8);
	return 0;
}

LRESULT CClassView::OnTypeInt64(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Int64);
	return 0;
}

LRESULT CClassView::OnTypeInt32(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Int32);
	return 0;
}

LRESULT CClassView::OnTypeInt16(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Int16);
	return 0;
}

LRESULT CClassView::OnTypeInt8(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Int8);
	return 0;
}

LRESULT CClassView::OnTypeBits(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Bits);
	return 0;
}

LRESULT CClassView::OnTypeQword(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::UINT64);
	return 0;
}

LRESULT CClassView::OnTypeDword(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::UINT32);
	return 0;
}

LRESULT CClassView::OnTypeWord(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::UINT16);
	return 0;
}

LRESULT CClassView::OnTypeByte(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::UINT8);
	return 0;
}

LRESULT CClassView::OnTypeVec2(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Vec2);
	return 0;
}

LRESULT CClassView::OnTypeVec3(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Vec3);
	return 0;
}

LRESULT CClassView::OnTypeQuat(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Quat);
	return 0;
}

LRESULT CClassView::OnTypeFloat(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Float);
	return 0;
}

LRESULT CClassView::OnTypeDouble(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Double);
	return 0;
}

LRESULT CClassView::OnTypeMatrix(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Matrix);
	return 0;
}

LRESULT CClassView::OnTypeCustom(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Custom);
	return 0;
}

LRESULT CClassView::OnTypeText(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Text);
	return 0;
}

LRESULT CClassView::OnTypePChar(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::PChar);
	return 0;
}

LRESULT CClassView::OnTypePWChar(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::PWChar);
	return 0;
}

LRESULT CClassView::OnTypeUnicode(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Unicode);
	return 0;
}

LRESULT CClassView::OnTypeInstance(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Class);
	return 0;
}

void CClassView::ResizeNode(CNodeClass* pClass, UINT index, DWORD before, DWORD after) {
	if (!pClass || index == MAX_NODES)
		return;

	if (before != after) {
		if (after < before) {
			FillNodes(pClass, index + 1, before - after);
		}
		else {
			RemoveNodes(pClass, index + 1, after - before);
		}
	}

	m_pFrame->CalcAllOffsets();
}

LRESULT CClassView::OnTypeFunction(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::Function);
	return TRUE;
}

LRESULT CClassView::OnTypeFunctionPtr(WORD, WORD, HWND, BOOL&) {
	ReplaceSelectedWithType(NodeType::FunctionPtr);
	return TRUE;

}