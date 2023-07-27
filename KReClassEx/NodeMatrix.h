#pragma once

#include "NodeBase.h"

class CNodeMatrix : public CNodeBase {
public:
    CNodeMatrix();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return 4 * 4 * sizeof(float); }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};