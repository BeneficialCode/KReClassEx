#pragma once

#include "NodeBase.h"
#include "Memory.h"

class CNodePtr :public CNodeBase {
public:
    CNodePtr();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize() { return sizeof(void*); }

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);

    inline void SetClass(CNodeClass* pClassNode) { m_pClassNode = pClassNode; }
    inline CNodeClass* GetClass(void) { return m_pClassNode; }

    inline Memory* GetMemory() { return &m_Memory; }

private:
    CNodeClass* m_pClassNode{ nullptr };
	Memory m_Memory;
};