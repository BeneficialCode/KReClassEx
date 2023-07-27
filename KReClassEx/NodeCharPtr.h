#pragma once

#include "NodeBase.h"
#include "Memory.h"

class CNodeCharPtr : public CNodeBase {
public:
    CNodeCharPtr();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return sizeof(void*); }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);

    CStringA ReadMemoryStringA(ULONG_PTR address, SIZE_T max = 40);

public:
    CNodeBase* m_pNode;
    Memory m_Memory;
};