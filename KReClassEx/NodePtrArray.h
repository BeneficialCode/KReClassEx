#pragma once

#include "NodeBase.h"
#include "NodePtr.h"

class CNodePtrArray :public CNodeBase {
public:
    CNodePtrArray();

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);

    virtual ULONG GetMemorySize();

    virtual void Update(const PHOTSPOT Spot);

    inline ULONG Count(void) { return m_PtrCount; }
    inline void SetCount(ULONG Count) { m_PtrCount = Count; }

    void SetClass(CNodeClass* pNode) { m_pNodePtr->SetClass(pNode); }
    CNodeClass* GetClass(void) { return m_pNodePtr->GetClass(); }

protected:
	CNodePtr* m_pNodePtr;
    ULONG m_PtrCount{ 1 };
    INT m_CurrentIndex{ 0 };
};