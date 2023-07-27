#pragma once

#include "NodeBase.h"

class CNodeVec2 :public CNodeBase {
public:
	CNodeVec2();

	virtual void Update(const PHOTSPOT spot);
	virtual ULONG GetMemorySize() {
		return sizeof(float) * 2;
	}

	virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};