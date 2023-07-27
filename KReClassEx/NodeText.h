#pragma once

#include "NodeBase.h"

class CNodeText :public CNodeBase {
public:
    CNodeText();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return m_MemorySize; }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);

    inline void SetSize(DWORD size) { m_MemorySize = size; }
    inline DWORD GetSize(void) { return m_MemorySize; }

private:
	DWORD m_MemorySize;
};