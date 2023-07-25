#pragma once
#include "NodeBase.h"
class CNodeHex32 :
    public CNodeBase
{
public:
    CNodeHex32() {
        m_NodeType = NodeType::Hex32;
    }

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return sizeof(__int32); }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};

