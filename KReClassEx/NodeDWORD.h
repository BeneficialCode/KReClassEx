#pragma once
#include "NodeBase.h"

class CNodeDword : public CNodeBase {
public:
    CNodeDword();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return sizeof(DWORD); }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};