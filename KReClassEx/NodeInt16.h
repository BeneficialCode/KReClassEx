#pragma once

#include "NodeBase.h"

class CNodeInt16 : public CNodeBase {
public:
    CNodeInt16();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return sizeof(__int16); }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};