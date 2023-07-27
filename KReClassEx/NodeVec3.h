#pragma once

#include "NodeBase.h"

class CNodeVec3 :public CNodeBase {
public:
	CNodeVec3();

	virtual void Update(const PHOTSPOT spot);

	virtual ULONG GetMemorySize() { return sizeof(float) * 3; }

	virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};