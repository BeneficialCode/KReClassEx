#pragma once

#include "NodeBase.h"

class CNodeQword : public CNodeBase {
public:
    CNodeQword();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return sizeof(UINT64); }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};