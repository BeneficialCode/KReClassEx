#pragma once

#include "NodeBase.h"

class CNodeDouble : public CNodeBase {
public:
    CNodeDouble();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize(void) { return sizeof(double); }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};