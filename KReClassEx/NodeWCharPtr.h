#pragma once

#include "NodeBase.h"
#include "Memory.h"

class CNodeWCharPtr :public CNodeBase {
public:
	CNodeWCharPtr() {
		m_NodeType = NodeType::PWChar;
		m_Name = "PWCHAR";
	}

	virtual void Update(const PHOTSPOT spot);

	virtual ULONG GetMemorySize() { return sizeof(void*); }

	virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);

	CStringW ReadMemoryStringW(ULONG_PTR address, SIZE_T max);

public:
	CNodeBase* m_pNode;
	Memory m_Memory;
};