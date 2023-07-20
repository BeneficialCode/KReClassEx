#pragma once

#define MAX_NODES	(0x8000)

#include "Memory.h"
#include "CustomEdit.h"
#include "CustomToolTip.h"
#include "NodeClass.h"


class CClassView : public CWindowImpl<CClassView>
{
public:
	DECLARE_WND_CLASS_EX(nullptr, CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,NULL)

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}
	void OnPaint(CDCHandle dc);
	void OnSize(UINT nType, CSize size);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	BEGIN_MSG_MAP_EX(CClassView)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_SIZE(OnSize)
	END_MSG_MAP()

	


public:
	void SetClass(CNodeClass* pClass) { m_pClass = pClass; }

private:
	CNodeClass* m_pClass;

	BOOLEAN m_bTracking;
	// ��ͣ��
	CPoint m_HoverPoint;
	
public:
	Memory m_Memory;

	std::vector<HOTSPOT> m_Hotspots;
	std::vector<HOTSPOT> m_Selected;
	HOTSPOT ExchangeTarget;

	// Controls
	CCustomEdit m_Edit;
	CCustomToolTip m_ToolTip;

	CScrollBar m_VScroll;
	CScrollBar m_HScroll;

	bool isDeleting;
};