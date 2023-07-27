#pragma once

#include "NodeBase.h"

class CNodeInt64 :public CNodeBase {
public:
	CNodeInt64(){
		m_NodeType = NodeType::Int64;
	}

	virtual void Update(const PHOTSPOT spot);

	virtual ULONG GetMemorySize() { return sizeof(__int64); }

	virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};