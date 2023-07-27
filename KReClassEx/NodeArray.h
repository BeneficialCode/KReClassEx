#pragma once

#include "NodeBase.h"

class CNodeArray : public CNodeBase {
public:
    CNodeArray();

    virtual void Update(const PHOTSPOT pSpot);

    virtual ULONG GetMemorySize();

    virtual NODESIZE Draw(const PVIEWINFO pView, int x, int y);

    inline void SetTotal(ULONG total) { m_Total = total; }
    inline ULONG GetTotal(void) { return m_Total; }

    inline void SetClass(CNodeClass* pNode) { m_pNode = pNode; }
    inline CNodeClass* GetClass(void) { return m_pNode; }

protected:
    CNodeClass* m_pNode;
    ULONG m_Total;
    INT m_Current;
};