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

	void ResizeNode(CNodeClass* pClass, UINT index, DWORD before, DWORD after);



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
	LRESULT OnTypeHex32(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeHex16(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeHex8(WORD, WORD, HWND, BOOL&);

	LRESULT OnTypeInt64(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeInt32(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeInt16(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeInt8(WORD, WORD, HWND, BOOL&);

	LRESULT OnTypeBits(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeQword(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeDword(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeWord(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeByte(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeVec2(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeVec3(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeQuat(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeFloat(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeDouble(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeMatrix(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeCustom(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeText(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypePChar(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypePWChar(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeUnicode(WORD, WORD, HWND, BOOL&);

	LRESULT OnTypeInstance(WORD, WORD, HWND, BOOL&);

	
	LRESULT OnTypeFunction(WORD, WORD, HWND, BOOL&);
	LRESULT OnTypeFunctionPtr(WORD, WORD, HWND, BOOL&);

	
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
		COMMAND_ID_HANDLER(ID_TYPE_HEX32,OnTypeHex32)
		COMMAND_ID_HANDLER(ID_TYPE_HEX16, OnTypeHex16)
		COMMAND_ID_HANDLER(ID_TYPE_HEX8, OnTypeHex8)
		COMMAND_ID_HANDLER(ID_TYPE_INT64,OnTypeInt64)
		COMMAND_ID_HANDLER(ID_TYPE_INT32, OnTypeInt32)
		COMMAND_ID_HANDLER(ID_TYPE_INT16, OnTypeInt16)
		COMMAND_ID_HANDLER(ID_TYPE_INT8, OnTypeInt8)
		COMMAND_ID_HANDLER(ID_TYPE_BITS, OnTypeBits)
		COMMAND_ID_HANDLER(ID_TYPE_QWORD, OnTypeQword)
		COMMAND_ID_HANDLER(ID_TYPE_DWORD, OnTypeDword)
		COMMAND_ID_HANDLER(ID_TYPE_WORD, OnTypeWord)
		COMMAND_ID_HANDLER(ID_TYPE_BYTE, OnTypeByte)
		COMMAND_ID_HANDLER(ID_TYPE_VEC2, OnTypeVec2)
		COMMAND_ID_HANDLER(ID_TYPE_VEC3, OnTypeVec3)
		COMMAND_ID_HANDLER(ID_TYPE_QUAT, OnTypeQuat)
		COMMAND_ID_HANDLER(ID_TYPE_FLOAT, OnTypeFloat)
		COMMAND_ID_HANDLER(ID_TYPE_DOUBLE, OnTypeDouble)
		COMMAND_ID_HANDLER(ID_TYPE_MATRIX, OnTypeMatrix)
		COMMAND_ID_HANDLER(ID_TYPE_CUSTOM, OnTypeCustom)
		COMMAND_ID_HANDLER(ID_TYPE_TEXT, OnTypeText)
		COMMAND_ID_HANDLER(ID_TYPE_PCHAR, OnTypePChar)
		COMMAND_ID_HANDLER(ID_TYPE_PWCHAR, OnTypePWChar)
		COMMAND_ID_HANDLER(ID_TYPE_UNICODE, OnTypeUnicode)
		COMMAND_ID_HANDLER(ID_TYPE_CLASS,OnTypeInstance)
		COMMAND_ID_HANDLER(ID_MODIFY_HIDE,OnModifyHide)
		COMMAND_ID_HANDLER(ID_MODIFY_SHOW,OnModifyShow)
		COMMAND_ID_HANDLER(ID_MODIFY_DELETE,OnModifyDelete)
		COMMAND_ID_HANDLER(ID_TYPE_FUNCTION,OnTypeFunction)
		CHAIN_MSG_MAP(CBufferedPaintWindowImpl<CClassView>)
		REFLECT_NOTIFICATIONS() // 为了能让消息反射到控件
	END_MSG_MAP()

	
public:
	void SetClass(CNodeClass* pClass) { m_pClass = pClass; }
	CNodeClass* GetClass() { return m_pClass; }
	

private:
	CNodeClass* m_pClass;

	BOOLEAN m_bTracking;
	// 悬停点
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