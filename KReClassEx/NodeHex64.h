#pragma once
#include "NodeBase.h"

class CNodeHex64 :
    public CNodeBase
{
public:
    CNodeHex64() {
        m_NodeType = NodeType::Hex64;
    }

    virtual void Update(const PHOTSPOT spot);
    virtual ULONG GetMemorySize() {
        return sizeof(__int64);
    }
    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};

