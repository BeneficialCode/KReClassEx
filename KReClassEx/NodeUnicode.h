#pragma once

#include "NodeBase.h"

class CNodeUnicode : public CNodeBase {
public:
	CNodeUnicode();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize(void) { return m_MemorySize; }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);

    inline void SetSize(ULONG Size) { m_MemorySize = Size; }
    inline ULONG GetSize(void) { return m_MemorySize; }

private:
	ULONG m_MemorySize;
};