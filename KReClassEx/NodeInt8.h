#pragma once

#include "NodeBase.h"

class CNodeInt8 :public CNodeBase {
public:
	CNodeInt8();


    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return sizeof(__int8); }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};