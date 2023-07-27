#pragma once
#include "NodeBase.h"

class CNodeByte : public CNodeBase {
public:
    CNodeByte();

    virtual void Update(const PHOTSPOT pSpot);

    virtual ULONG GetMemorySize() { return sizeof(unsigned __int8); }

    virtual NODESIZE Draw(const PVIEWINFO pView, int x, int y);
};