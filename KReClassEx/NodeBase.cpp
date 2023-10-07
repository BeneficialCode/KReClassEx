#include "stdafx.h"
#include "NodeBase.h"

// Global node index
extern DWORD g_NodeCreateIndex;

CNodeBase::CNodeBase() {
	// Optimized
	m_LevelsOpen.resize(32, false);
	m_LevelsOpen[0] = true;

	// This is the class name
	m_Name.Format(L"N%0.8X", g_NodeCreateIndex++);
}

void CNodeBase::AddHotSpot(const PVIEWINFO view, const CRect& spot, CString text, 
	int id, int type) {
	if (spot.top > view->ClientRect->bottom || spot.bottom < 0)
		return;

	HOTSPOT hotspot;
	hotspot.Rect = spot;
	hotspot.Text = text;
	hotspot.Address = view->Address + m_Offset;
	hotspot.Id = id;
	hotspot.Type = type;
	hotspot.Object = this;
	hotspot.Level = view->Level;
	view->Hotspots->push_back(hotspot);
}

int CNodeBase::AddText(const PVIEWINFO view, int x, int y, DWORD color, int hitId, const wchar_t* fmt, ...) {
	if (fmt == nullptr)
		return x;

	WCHAR buf[1024] = { 0 };
	va_list va_alist;
	va_start(va_alist, fmt);
	_vsnwprintf_s(buf, ARRAYSIZE(buf), fmt, va_alist);
	va_end(va_alist);

	int width = wcslen(buf) * g_FontWidth;

	if ((y >= -(g_FontHeight)) && 
		(y + g_FontHeight <= view->ClientRect->bottom + g_FontHeight)){
		CRect pos;
		if (hitId != HS_NONE) {
			if (width >= g_FontWidth * 2)
				pos.SetRect(x, y, x + width, y + g_FontHeight);
			else
				pos.SetRect(x, y, x + g_FontWidth * 2, y + g_FontHeight);

			AddHotSpot(view, pos, buf, hitId, HS_EDIT);
		}

		pos.SetRect(x, y, 0, 0);

		view->Dc->SetTextColor(color);
		view->Dc->SetBkMode(TRANSPARENT);
		view->Dc->DrawText(buf, -1, pos, DT_LEFT | DT_NOCLIP | DT_NOPREFIX);
	}
	
	return x + width;
}

int CNodeBase::AddText(const PVIEWINFO view, int x, int y, DWORD color, int hitId, const char* fmt, ...) {
	if (fmt == nullptr)
		return x;

	char buf[1024] = { 0 };
	WCHAR finalBuf[1024] = { 0 };

	va_list va_alist;
	va_start(va_alist, fmt);
	_vsnprintf_s(buf, ARRAYSIZE(buf), fmt, va_alist);
	va_end(va_alist);

	size_t converted = 0;
	mbstowcs_s(&converted, finalBuf, buf, ARRAYSIZE(buf));

	int width = strlen(buf) * g_FontWidth;

	if ((y >= -g_FontHeight) &&
		(y + g_FontHeight <= view->ClientRect->bottom + g_FontHeight)) {
		CRect pos;
		if (hitId != HS_NONE) {
			if (width >= g_FontWidth * 2)
				pos.SetRect(x, y, x + width, y + g_FontHeight);
			else
				pos.SetRect(x, y, x + g_FontWidth * 2, y + g_FontHeight);

			AddHotSpot(view, pos, buf, hitId, HS_EDIT);
		}

		pos.SetRect(x, y, 0, 0);

		view->Dc->SetTextColor(color);
		view->Dc->SetBkMode(TRANSPARENT);
		view->Dc->DrawText(finalBuf, -1, pos, DT_LEFT | DT_NOCLIP | DT_NOPREFIX);
	}

	return x + width;
}

int CNodeBase::AddAddressOffset(const PVIEWINFO view, int x, int y) {
	if (g_bOffset) {
#ifdef _WIN64
		x = AddText(view, x, y, g_clrOffset, HS_NONE, L"%0.4X", m_Offset) + g_FontWidth;
#else
		x = AddText(view, x, y, g_clrOffset, HS_NONE, L"%0.4X", m_Offset) + g_FontWidth;
#endif
	}

	if (g_bAddress) {
#ifdef _WIN64
		x = AddText(view, x, y, g_clrAddress, HS_ADDRESS, 
			L"%0.9I64X", view->Address+ m_Offset) + g_FontWidth;
#else
		x = AddText(view, x, y, g_clrAddress, HS_ADDRESS,
			L"%0.8X", view->Address + m_Offset) + g_FontWidth;
#endif
	}

	return x;
}

void CNodeBase::AddSelection(const PVIEWINFO view, int x, int y, int height) {
	if ((y > view->ClientRect->bottom) || (y + height < 0))
		return;

	if (m_bSelected)
		view->Dc->FillSolidRect(0, y, view->ClientRect->right, height, g_clrSelect);

	CRect pos(0, y, INT_MAX, y + height);
	AddHotSpot(view, pos, CString(), 0, HS_SELECT);
}

int CNodeBase::AddIcon(const PVIEWINFO view, int x, int y, int idx, int id, int type) {
	if ((y > view->ClientRect->bottom) || (y + 16 < 0))
		return x + 16;

	DrawIconEx(view->Dc->m_hDC, x, y, g_Icons[idx], 16, 16, 0, NULL, DI_NORMAL);

	if (id != -1) {
		CRect pos(x, y, x + 16, y + 16);
		AddHotSpot(view, pos, CString(), id, type);
	}

	return x + 16;
}

int CNodeBase::AddOpenClose(const PVIEWINFO view, int x, int y) {
	if ((y > view->ClientRect->bottom) || (y + 16 < 0))
		return x + 16;

	return m_LevelsOpen[view->Level] ? AddIcon(view, x, y, ICON_OPEN, 0, HS_OPENCLOSE)
		: AddIcon(view, x, y, ICON_CLOSED, 0, HS_OPENCLOSE);
}

void CNodeBase::AddDelete(const PVIEWINFO view, int x, int y) {
	if ((y > view->ClientRect->bottom) || (y + 16 < 0))
		return;

	if (m_bSelected)
		AddIcon(view, view->ClientRect->right - 16, y, ICON_DELETE, 0, HS_DELETE);
}

void CNodeBase::AddTypeDrop(const PVIEWINFO view, int x, int y) {
	if (view->MultiSelected || ((y > view->ClientRect->bottom) || (y + 16) < 0))
		return;

	if (m_bSelected)
		AddIcon(view, 0, y, ICON_DROPARROW, 0, HS_DROP);
}

int CNodeBase::ResolveRTTI(ULONG_PTR address, int x, const PVIEWINFO view, int y) {
#ifdef _M_AMD64
	ULONG_PTR moduleBase = 0;

	ULONG_PTR RTTIObjectLocatorPtr = 0;
	ULONG_PTR RTTIObjectLocator = 0;

	ULONG ClassHierarchyDescriptorOffset = 0;
	ULONG_PTR ClassHierarchyDescriptor = 0;

	ULONG NumBaseClasses = 0;

	DWORD BaseClassArrayOffset = 0;
	ULONG_PTR BaseClassArray = 0;
	CString rttiName;
#endif // _M_AMD64

	x = AddText(view, x, y, g_clrOffset, HS_RTTI, L"%s", rttiName);

	return x;
}

int CNodeBase::AddComment(const PVIEWINFO view, int x, int y) {
	x = AddText(view, x, y, g_clrComment, HS_NONE, L"//");
	x = AddText(view, x, y, g_clrComment, HS_COMMENT, L"%s ", m_Comment);

	if (m_NodeType == NodeType::Hex64) {
		float fVal = *(float*)&view->Data[m_Offset];
		LONG_PTR iVal = *(LONG_PTR*)&view->Data[m_Offset];

		// 显示注释中的数值
		if (g_bInt) {
			// 此处地址判断有误，如果是内核地址的话
			if (iVal > 0x6FFFFFFF && iVal < 0x7FFFFFFFFFFF) {
				x = AddText(view, x, y, g_clrValue, HS_NONE, L"(%I64d|0x%IX)", iVal, iVal);
			}
			else if (iVal == 0) {
				x = AddText(view, x, y, g_clrValue, HS_NONE, L"(%I64d)", iVal);
			}
			else
				x = AddText(view, x, y, g_clrValue, HS_NONE, L"(%I64d|0x%X)", iVal, iVal);
		}
		

	}
	else if (m_NodeType == NodeType::Hex32) {
		float fVal = *(float*)&view->Data[m_Offset];
		LONG_PTR iVal = *(LONG_PTR*)&view->Data[m_Offset];

		if (g_bInt) {
#ifdef _M_AMD64
			if (iVal > 0x140000000) // in 64 bit address range
				x = AddText(view, x, y, g_clrValue, HS_NONE, _T("(%i|0x%IX)"), iVal, iVal);
			else if (iVal > 0x400000 && iVal < 0x140000000)
				x = AddText(view, x, y, g_clrValue, HS_NONE, _T("(%i|0x%X)"), iVal, iVal);
			else if (iVal == 0)
				x = AddText(view, x, y, g_clrValue, HS_NONE, _T("(%i)"), iVal);
			else
				x = AddText(view, x, y, g_clrValue, HS_NONE, _T("(%i|0x%X)"), iVal, iVal);
#else

#endif // _M_AMD64

		}
	}
	else if (m_NodeType == NodeType::Hex16) {

	}

	return x;
}

void CNodeBase::StandardUpdate(const PHOTSPOT spot) {
	if (spot->Id == HS_NAME) {
		m_Name = spot->Text;
	}
	else if (spot->Id == HS_COMMENT) {
		m_Comment = spot->Text;
	}
}

NODESIZE CNodeBase::DrawHidden(const PVIEWINFO view, int x, int y) {
	NODESIZE size;

	view->Dc->FillSolidRect(0, y, view->ClientRect->right, 1, m_bSelected ? g_clrSelect : g_clrHidden);

	size.x = 0;
	size.y = y;

	return size;
}

CStringA CNodeBase::GetStringFromMemoryA(const char* pMemory, int length) {
	CStringA str;
	for (int i = 0; i < length; i++) {
		if (pMemory[i] > 0x1F && pMemory[i] < 0xFF && pMemory[i] != 0x7F) {
			str += pMemory[i];
		}
		else
			str += '.';
	}
	return str;
}

CStringW CNodeBase::GetStringFromMemoryW(const wchar_t* pMemory, int length) {
	CStringW str;
	for (int i = 0; i < length; i++)
	{
		str += (pMemory[i] > 0x1F && pMemory[i] < 0xFF && pMemory[i] != 0x7F) ? (wchar_t)pMemory[i] : (wchar_t)(L'.');
	}
	return str;
}

BOOLEAN CNodeBase::IsMemory(ULONG_PTR address) {
	return false;
}