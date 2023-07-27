#pragma once

#include "NodeBase.h"

class CNodeQuat : public CNodeBase {
public:
    CNodeQuat();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return sizeof(float) * 4; }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};