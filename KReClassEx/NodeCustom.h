#pragma once

#include "NodeBase.h"

class CNodeCustom : public CNodeBase {
public:
    CNodeCustom();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize();

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);

    inline void SetSize(ULONG size) { m_MemorySize = size; }
    inline ULONG GetSize(void) { return m_MemorySize; }

private:
    ULONG m_MemorySize;
};