#pragma once

#include "NodeBase.h"

class CNodeBits :public CNodeBase {
public:
	CNodeBits() {
		m_NodeType = NodeType::Bits;
	}

	virtual void Update(const PHOTSPOT spot);

	virtual ULONG GetMemorySize() {
		return 1;
	}

	virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};