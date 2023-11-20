#pragma once
#include "NodeBase.h"

class CNodeListEntry : public CNodeBase {
public:
	CNodeListEntry();

	virtual void Update(const PHOTSPOT spot);
	virtual ULONG GetMemorySize() {
		return sizeof(LIST_ENTRY);
	}

	virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};