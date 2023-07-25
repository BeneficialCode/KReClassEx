#pragma once
#include "NodeBase.h"
class CNodeHex8 :
    public CNodeBase
{
public:
    CNodeHex8() {
        m_NodeType = NodeType::Hex8;
    }

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return sizeof(__int8); }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};

