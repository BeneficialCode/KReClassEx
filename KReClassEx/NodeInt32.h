#pragma once

#include "NodeBase.h"

class CNodeInt32 :public CNodeBase {
public:
    CNodeInt32() {
        m_NodeType = NodeType::Int32;
    }

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return sizeof(__int32); }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
};