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

		ReClassReadMemory(m_pClass->GetOffset(), m_Memory.Data(), classSize);

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
	m_Edit.Create(m_hWnd, rect, nullptr, WS_CHILD | WS_TABSTOP);
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
		if (pClass->GetType() == NodeType::Vtable) {
			
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
		if (pClass->GetType() == NodeType::Vtable) {

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
		if (pClass->GetType() == NodeType::Vtable) {

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
		if (pClass->GetType() == NodeType::Vtable) {

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

		if (m_Selected[i].Object->GetParent()->GetType() == NodeType::Vtable) {
			type = NodeType::FunctionPtr;
		}

		
	}

	m_Selected.clear();
	for (UINT i = 0; i < newSelected.size(); i++) {
		newSelected[i]->Select();

	}

	Invalidate(FALSE);
}