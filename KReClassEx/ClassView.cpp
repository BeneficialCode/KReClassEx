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

		int min, max;
		GetScrollRange(SB_VERT, &min, &max);
		if (min > 0 && max > 0) {
			info.ClientRect->right -= SB_WIDTH;
			info.ClientRect->bottom -= SB_WIDTH;
		}


		xPos = GetScrollPos(SB_HORZ) * g_FontHeight;
		yPos = GetScrollPos(SB_VERT);

		// do the draw!
		drawMax = m_pClass->Draw(&info, 0 - xPos, -yPos);

		// dirty hack fix draw methods
		drawMax.x += xPos;
		drawMax.y = yPos;


		if (m_pClass->m_RequestPosition != -1) {

		}

		if (clientRect.Height() < drawMax.y) {
			ZeroMemory(&si, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_PAGE | SIF_RANGE;
			si.nMin = 0;
			si.nMax = drawMax.y / g_FontHeight;
			si.nPage = clientRect.Height() / g_FontHeight;
			SetScrollInfo(SB_VERT, &si);
			ShowScrollBar(SB_VERT);
		}
		else {
			SetScrollPos(SB_VERT, 0);
			ShowScrollBar(SB_VERT, FALSE);
		}

		if (clientRect.Width() < drawMax.x) {
			ZeroMemory(&si, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_PAGE | SIF_RANGE;
			si.nMin = 0;
			si.nMax = drawMax.x;
			si.nPage = clientRect.Width();
			SetScrollInfo(SB_HORZ, &si);
			ShowScrollBar(SB_HORZ);
		}
		else {
			SetScrollPos(SB_HORZ, 0);
			ShowScrollBar(SB_HORZ, FALSE);
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


}