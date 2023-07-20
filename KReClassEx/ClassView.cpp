#include "stdafx.h"
#include "ClassView.h"

#define SB_WIDTH 14

void CClassView::OnPaint(CDCHandle dc) {
	// draw context for painting
	CPaintDC paintDc(m_hWnd);
	CRect clientRect;
	CMemoryDC memDc(paintDc.m_hDC, clientRect);

	GetClientRect(&clientRect);
	memDc.FillSolidRect(&clientRect, g_clrBackground);

	VIEWINFO info;
	ULONG classSize;
	int xPos, yPos;
	NODESIZE drawMax;

	if (m_pClass != nullptr) {
		memDc.SelectFont(g_ViewFont);

		m_Hotspots.clear();

		classSize = m_pClass->GetMemorySize();
		m_Memory.SetSize(classSize);

		ReClassReadMemory(m_pClass->GetOffset(), m_Memory.Data(), classSize);

		info.Address = m_pClass->GetOffset();
		info.Data = m_Memory.Data();
		
		info.ClientRect = &clientRect;
		info.Dc = &memDc;
		info.Level = 0;
		info.Hotspots = &m_Hotspots;
		info.MultiSelected = m_Selected.size() > 1;

		if (m_VScroll.IsWindowVisible()) {
			info.ClientRect->right -= SB_WIDTH;
			info.ClientRect->bottom -= SB_WIDTH;
		}

		xPos = m_VScroll.GetScrollPos() * g_FontHeight;
		yPos = m_HScroll.GetScrollPos();

		// do the draw!
		drawMax = m_pClass->Draw(&info, 0 - xPos, -yPos);

		// dirty hack fix draw methods
		drawMax.x += xPos;
		drawMax.y = yPos;

		if (m_pClass->m_RequestPosition != -1) {

		}

		if (clientRect.Height() < drawMax.y) {
			SCROLLINFO scrollInfo;
			ZeroMemory(&scrollInfo, sizeof(SCROLLINFO));
			scrollInfo.cbSize = sizeof(SCROLLINFO);
			scrollInfo.fMask = SIF_PAGE | SIF_RANGE;
			scrollInfo.nMin = 0;
			scrollInfo.nMax = drawMax.y / g_FontHeight;
			scrollInfo.nPage = clientRect.Height() / g_FontHeight;
			m_VScroll.SetScrollInfo(&scrollInfo);
			m_VScroll.ShowScrollBar();
		}
		else {
			m_VScroll.SetScrollPos(0);
			m_VScroll.ShowScrollBar(FALSE);
		}

		if (clientRect.Width() < drawMax.x) {
			SCROLLINFO scrollInfo;
			ZeroMemory(&scrollInfo, sizeof(SCROLLINFO));
			scrollInfo.cbSize = sizeof(SCROLLINFO);
			scrollInfo.fMask = SIF_PAGE | SIF_RANGE;
			scrollInfo.nMin = 0;
			scrollInfo.nMax = drawMax.x;
			scrollInfo.nPage = clientRect.Width();
			m_HScroll.SetScrollInfo(&scrollInfo);
			m_HScroll.ShowScrollBar();
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
	m_VScroll.SetWindowPos(NULL, client.right - SB_WIDTH, 0, SB_WIDTH, 
		client.Height() - SB_WIDTH, SWP_NOZORDER);
	m_HScroll.SetWindowPos(NULL, client.left, client.bottom - SB_WIDTH * 2, 
		client.Width() - SB_WIDTH, SB_WIDTH, SWP_NOZORDER);
	m_Edit.ShowWindow(SW_HIDE);
}

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	lpCreateStruct->cy += g_FontHeight;

	CRect rect(0, 0, 100, 100);
	CRect hrect(5, 5, 100, 30);

	m_Edit.Create(m_hWnd, rect, nullptr, WS_CHILD | WS_TABSTOP);
	m_Edit.ShowWindow(SW_HIDE);
	m_Edit.SetFont(g_ViewFont);

	m_VScroll.Create(m_hWnd, rect, nullptr, SBS_VERT);
	m_VScroll.EnableScrollBar(ESB_ENABLE_BOTH);
	m_VScroll.ShowScrollBar();

	m_HScroll.Create(m_hWnd, rect, nullptr, SBS_HORZ);
	m_HScroll.EnableScrollBar(ESB_ENABLE_BOTH);
	m_HScroll.ShowScrollBar();

	m_ToolTip.Create(m_hWnd, rect, nullptr, ES_MULTILINE | WS_BORDER);
	m_ToolTip.SetFont(g_ViewFont);
	m_ToolTip.EnableWindow(FALSE);

	return 0;
}
