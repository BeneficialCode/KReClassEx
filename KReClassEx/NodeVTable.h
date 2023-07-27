#pragma once
#include "NodeBase.h"
#include "Memory.h"

class CNodeVTable :public CNodeBase {
public:

	CNodeVTable() {
		m_NodeType = NodeType::VTable;
	}
	CNodeVTable(CWindow* pParentWindow) :m_pParentWindow(pParentWindow) {
	}

	virtual void Update(const PHOTSPOT spot);

	virtual ULONG GetMemorySize() { return sizeof(void*); }

	virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);

	inline bool IsInitialized() { return m_pParentWindow != nullptr; }

	void Initialize(CWindow* pParentWindow);

private:
	Memory m_Memory;

	CWindow* m_pParentWindow{ nullptr };
};