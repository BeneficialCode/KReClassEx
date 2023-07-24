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

		SCROLLINFO si;

		xPos = GetScrollPos(SB_HORZ) * g_FontHeight;
		yPos = GetScrollPos(SB_VERT);

		// do the draw!
		drawMax = m_pClass->Draw(&info, 0 - xPos, -yPos);

		// dirty hack fix draw methods
		drawMax.x += xPos;
		drawMax.y = yPos;


		if (m_pClass->m_RequestPosition != -1) {

		}
	}
}

void CClassView::OnSize(UINT nType, CSize size) {
	m_Edit.ShowWindow(SW_HIDE);
}

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CRect rect(0, 0, 100, 100);
	m_Edit.Create(m_hWnd, rect, nullptr, WS_CHILD | WS_TABSTOP);
	m_Edit.ShowWindow(SW_HIDE);
	m_Edit.SetFont(g_ViewFont);

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
						// TODO: Calculate all offsets
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