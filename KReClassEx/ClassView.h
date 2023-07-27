#pragma once

#define MAX_NODES	(0x8000)

#include "Memory.h"
#include "CustomEdit.h"
#include "CustomToolTip.h"
#include "NodeClass.h"
#include "resource.h"
#include "Interfaces.h"



class CClassView :
	public CBufferedPaintWindowImpl<CClassView>
{
public:
	DECLARE_WND_CLASS_EX(nullptr,CS_OWNDC| CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,NULL)

	CClassView(IMainFrame* frame);

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}
public:

	void ClearSelection();
	UINT FindNodeIndex(CNodeBase* pNode);

	void AddBytes(CNodeClass* pClass, DWORD length);
	void InsertBytes(CNodeClass* pClass, UINT index, DWORD length);

	void ReplaceSelectedWithType(NodeType type);
	void ReplaceNode(CNodeClass* pClass, UINT index, CNodeBase* pNewNode);
	void FillNodes(CNodeClass* pClass, UINT index, DWORD length);
	void RemoveNodes(CNodeClass* pClass, UINT index, DWORD length);

	void MakeBasicClass(CNodeClass* pClass);


public:
	void OnAdd(DWORD size);
	LRESULT OnAdd4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAdd8(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAdd64(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAdd128(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAdd256(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAdd512(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAdd1024(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAdd2048(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void OnInsert(DWORD size);
	LRESULT OnInsert4(WORD, WORD, HWND, BOOL&);
	LRESULT OnInsert8(WORD, WORD, HWND, BOOL&);
	LRESULT OnInsert64(WORD, WORD, HWND, BOOL&);
	LRESULT OnInsert128(WORD, WORD, HWND, BOOL&);
	LRESULT OnInsert256(WORD, WORD, HWND, BOOL&);
	LRESULT OnInsert512(WORD, WORD, HWND, BOOL&);
	LRESULT OnInsert1024(WORD, WORD, HWND, BOOL&);
	LRESULT OnInsert2048(WORD, WORD, HWND, BOOL&);

	LRESULT OnModifyHide(WORD, WORD, HWND, BOOL&);
	LRESULT OnModifyShow(WORD, WORD, HWND, BOOL&);
	LRESULT OnModifyDelete(WORD, WORD, HWND, BOOL&);

	LRESULT OnTypeHex64(WORD, WORD, HWND, BOOL&);


	
public:
	void OnSize(UINT nType, CSize size);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void DoPaint(CDCHandle dc, RECT& rect);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnTimer(UINT_PTR nIDEvent);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	BEGIN_MSG_MAP_EX(CClassView)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_SIZE(OnSize)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_VSCROLL(OnVScroll)
		MSG_WM_HSCROLL(OnHScroll)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)
		COMMAND_ID_HANDLER(ID_ADD_ADD4,OnAdd4)
		COMMAND_ID_HANDLER(ID_ADD_ADD8,OnAdd8)
		COMMAND_ID_HANDLER(ID_ADD_ADD64,OnAdd64)
		COMMAND_ID_HANDLER(ID_ADD_ADD128,OnAdd128)
		COMMAND_ID_HANDLER(ID_ADD_ADD256,OnAdd256)
		COMMAND_ID_HANDLER(ID_ADD_ADD512,OnAdd512)
		COMMAND_ID_HANDLER(ID_ADD_ADD1024,OnAdd1024)
		COMMAND_ID_HANDLER(ID_ADD_ADD2048,OnAdd2048)
		COMMAND_ID_HANDLER(ID_INSERT_INSERT4,OnInsert4)
		COMMAND_ID_HANDLER(ID_INSERT_INSERT8,OnInsert8)
		COMMAND_ID_HANDLER(ID_INSERT_INSERT64,OnInsert64)
		COMMAND_ID_HANDLER(ID_INSERT_INSERT128,OnInsert128)
		COMMAND_ID_HANDLER(ID_INSERT_INSERT256,OnInsert256)
		COMMAND_ID_HANDLER(ID_INSERT_INSERT512,OnInsert512)
		COMMAND_ID_HANDLER(ID_INSERT_INSERT1024,OnInsert1024)
		COMMAND_ID_HANDLER(ID_INSERT_INSERT2048,OnInsert2048)
		COMMAND_ID_HANDLER(ID_TYPE_HEX64,OnTypeHex64)
		COMMAND_ID_HANDLER(ID_MODIFY_HIDE,OnModifyHide)
		COMMAND_ID_HANDLER(ID_MODIFY_SHOW,OnModifyShow)
		COMMAND_ID_HANDLER(ID_MODIFY_DELETE,OnModifyDelete)
		CHAIN_MSG_MAP(CBufferedPaintWindowImpl<CClassView>)
	END_MSG_MAP()

	
public:
	void SetClass(CNodeClass* pClass) { m_pClass = pClass; }

	

private:
	CNodeClass* m_pClass;

	BOOLEAN m_bTracking;
	// ÐüÍ£µã
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

private:
	IMainFrame* m_pFrame;
};