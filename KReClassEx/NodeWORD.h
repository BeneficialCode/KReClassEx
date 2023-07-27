#pragma once

#include "NodeBase.h"

class CNodeWord :public CNodeBase {
public:
	CNodeWord() {
		m_NodeType = NodeType::UINT16;
	}

	virtual void Update(const PHOTSPOT spot);

	virtual ULONG GetMemorySize() {
		return sizeof(WORD);
	}

	virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};