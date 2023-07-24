#include "stdafx.h"
#include "ClassView.h"

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

	return 0;
}
