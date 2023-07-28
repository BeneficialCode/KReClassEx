#pragma once
#include "NodeTypes.h"
#include "HotSpot.h"

#include <vector>

#define TXOFFSET 16

typedef struct tagVIEWINFO {
	CDCHandle* Dc;
	CRect* ClientRect;
	std::vector<HOTSPOT>* Hotspots;
	ULONG_PTR Address;
	UCHAR* Data;
	UINT Level;
	BOOL MultiSelected;
}VIEWINFO,*PVIEWINFO;

typedef struct _TYPE_INFO {
	ULONG_PTR VirtualTable;	// type info class vftable
	ULONG_PTR Data;			// NULL until loaded at runtime
	CHAR Name[1];			// Mangled name (prefix: .?AV=classes, .?AU=structs)
}TYPE_INFO,*PTYPE_INFO;

typedef struct _TYPE_INFO TYPE_DESCRIPTOR;
typedef TYPE_DESCRIPTOR* PTYPE_DESCRIPTOR;

typedef struct _TYPE_INFO_DISP_INFO {
	LONG MemberDisp;				// 0x00 Member displacement
	LONG VirtualTableDisp;			// 0x04 Vftable displacement
	LONG InternalVirtualTableDisp;	// 0x08 Displacement inside vftable
} TYPE_INFO_DISP_INFO, * PTYPE_INFO_DISP_INFO;

typedef struct _RTTI_BASE_CLASS_DESCRIPTOR {
#if defined(_M_AMD64)
	ULONG TypeDescriptor;				// 0x00 Offset to TYPE_DESCRIPTOR of the class
#else
	PTYPE_DESCRIPTOR TypeDescriptor;	// 0x00 TYPE_DESCRIPTOR of the class
#endif
	ULONG ContainedBasesCount;			// 0x04 Number of nested classes in the RTTI_BASE_CLASS_ARRAY
	TYPE_INFO_DISP_INFO DispInfo;		// 0x08 Pointer-to-member displacement info
	ULONG Attributes;					// 0x14 Flags
} RTTI_BASE_CLASS_DESCRIPTOR, * PRTTI_BASE_CLASS_DESCRIPTOR;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4200)
#endif

typedef struct _RTTI_BASE_CLASS_ARRAY {
	PRTTI_BASE_CLASS_DESCRIPTOR* BaseClassDescriptors;
} RTTI_BASE_CLASS_ARRAY, * PRTTI_BASE_CLASS_ARRAY;

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

typedef struct _RTTI_CLASS_HIERARCHY_DESCRIPTOR {
	ULONG Signature;			// 0x00 Zero until loaded
	ULONG Attributes;			// 0x04 Flags
	ULONG BaseClassesCount;		// 0x08 Number of classes in RTTI_BASE_CLASS_ARRAY
#if defined(_M_AMD64)
	ULONG BaseClassArrayOffset; // 0x0C Offset to PRTTI_BASE_CLASS_ARRAY
#else
	PRTTI_BASE_CLASS_ARRAY BaseClassArray; // 0x0C
#endif
} RTTI_CLASS_HIERARCHY_DESCRIPTOR, * PRTTI_CLASS_HIERARCHY_DESCRIPTOR;

typedef struct _RTTI_COMPLETE_OBJECT_LOCATOR {
	ULONG Signature;				// 0x00 '0' in x86, '1' in x86_64
	ULONG Offset;					// 0x04 Offset of this vftable in the complete class
	ULONG ConstructorDispOffset;	// 0x08 Constructor displacement offset
#if defined(_M_AMD64)
	ULONG TypeDescriptorOffset;		// 0x0C Offset to PTYPE_INFO
	ULONG ClassDescriptorOffset;	// 0x10 Offset to PRTTI_CLASS_HIERARCHY_DESCRIPTOR which describes inheritance hierarchy
	ULONG ObjectBase;				// 0x14 Object base offset (base = ptr col - objectBase)
#else
	PTYPE_DESCRIPTOR TypeDescriptor; // 0x0C PTYPE_INFO of the complete class
	PRTTI_CLASS_HIERARCHY_DESCRIPTOR ClassDescriptor; // 0x10
#endif
} RTTI_COMPLETE_OBJECT_LOCATOR, * PRTTI_COMPLETE_OBJECT_LOCATOR;

typedef struct _NODESIZE {
	INT x;
	INT y;
} NODESIZE, * PNODESIZE;

class CNodeBase {
public:
	CNodeBase();

	virtual NODESIZE Draw(const PVIEWINFO View, int x, int y) = 0;
	virtual ULONG GetMemorySize() = 0;
	virtual void Update(const PHOTSPOT Spot) = 0;

	NodeType GetType() const { return m_NodeType; }

	size_t GetOffset() const { return m_Offset; }
	void SetOffset(const size_t offset) { m_Offset = offset; }

	const CString& GetOffsetText() const { return m_OffsetText; }
	void SetOffsetText(const CString& text) { m_OffsetText = text; }
	void SetOffsetText(WCHAR* text) { m_OffsetText = text; }

	const CString& GetName() const { return m_Name; }
	void SetName(const CString& name) { m_Name = name; }
	void SetName(WCHAR* name) { m_Name = name; }

	const CString& GetComment() const { return m_Comment; }
	void SetComment(const CString& comment) { m_Comment = comment; }
	void SetComment(WCHAR* comment) { m_Comment = comment; }

	CNodeBase* GetParent() const { return m_pParentNode; }
	void SetParent(CNodeBase* node) { m_pParentNode = node; }

	void AddNode(CNodeBase* node) { m_ChildNodes.push_back(node); }
	void InsertNode(size_t idx, CNodeBase* node) { 
		m_ChildNodes.insert(m_ChildNodes.begin() + idx, node);
	}
	CNodeBase* GetNode(size_t idx) { return m_ChildNodes[idx]; }
	int FindNode(CNodeBase* pNode) {
		auto found = std::find(m_ChildNodes.begin(), m_ChildNodes.end(), pNode);
		return (found != m_ChildNodes.end()) ? (int)(found - m_ChildNodes.begin()) : -1;
	}
	void SetNode(size_t idx, CNodeBase* node) { m_ChildNodes[idx] = node; }
	void RemoveNode(size_t idx) {
		m_ChildNodes.erase(m_ChildNodes.begin() + idx);
	}
	void DeleteNode(size_t idx) {
		if (m_ChildNodes[idx]) {
			delete m_ChildNodes[idx];
			RemoveNode(idx);
		}
	}
	size_t NodeCount() const { return m_ChildNodes.size(); }

	bool IsHidden() { return m_bHidden; }
	void Show() { m_bHidden = false; }
	void Hide() { m_bHidden = true; }
	void SetHidden(bool hidden) { m_bHidden = hidden; }
	void ToggleHidden() { m_bHidden = !m_bHidden; }

	bool IsSelected() { return m_bSelected; }
	void SetSelected(bool selected) { m_bSelected = selected; }
	void Select() { m_bSelected = true; }
	void Unselect() { m_bSelected = false; }
	void ToggleSelected() { m_bSelected = !m_bSelected; }

	bool IsLevelOpen(int idx) { return m_LevelsOpen[idx]; }
	void ToggleLevelOpen(int idx) { m_LevelsOpen[idx] = !m_LevelsOpen[idx]; }

	// Incorrect view.address
	void AddHotSpot(const PVIEWINFO view, const CRect& spot, CString text, int id, int type);

	int AddText(const PVIEWINFO view, int x, int y, DWORD color, int hitId, const wchar_t* fmt, ...);

	int AddText(const PVIEWINFO view, int x, int y, DWORD color, int hitId, const char* fmt, ...);

	int AddAddressOffset(const PVIEWINFO view, int x, int y);

	void AddSelection(const PVIEWINFO view, int x, int y, int height);

	int AddIcon(const PVIEWINFO view, int x, int y, int idx, int id, int type);

	int AddOpenClose(const PVIEWINFO view, int x, int y);

	void AddDelete(const PVIEWINFO view, int x, int y);

	void AddTypeDrop(const PVIEWINFO view, int x, int y);

	int ResolveRTTI(ULONG_PTR address, int x, const PVIEWINFO view, int y);

	int AddComment(const PVIEWINFO view, int x, int y);

	// 更新名字和注释
	void StandardUpdate(const PHOTSPOT spot);

	NODESIZE DrawHidden(const PVIEWINFO view, int x, int y);

	CStringA GetStringFromMemoryA(const char* pMemory, int length);
	CStringW GetStringFromMemoryW(const wchar_t* pMemory, int length);

	BOOLEAN IsMemory(ULONG_PTR address);


protected:
	NodeType m_NodeType{ NodeType::Base };

	// 地址
	size_t m_Offset{ 0 }; // Can also be an address
	CString m_OffsetText{ L"0" };

	// 节点名称
	CString m_Name;
	CString m_Comment;

	CNodeBase* m_pParentNode{ nullptr };
	std::vector<CNodeBase*> m_ChildNodes;

	bool m_bHidden{ false };
	bool m_bSelected{ false };

	std::vector<bool> m_LevelsOpen;
};