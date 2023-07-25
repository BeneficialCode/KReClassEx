#pragma once
#include "NodeBase.h"

class CNodeHex16 :public CNodeBase {
public:
	CNodeHex16() {
		m_NodeType = NodeType::Hex16;
	}

	virtual void Update(const PHOTSPOT spot);

	virtual ULONG GetMemorySize() { return sizeof(__int16); }

	virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};