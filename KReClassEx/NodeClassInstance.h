#pragma once

class CNodeClassInstance : public CNodeBase {
public:
    CNodeClassInstance();

    virtual void Update(const PHOTSPOT spot);

    virtual ULONG GetMemorySize();

    virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);

    inline void SetClass(CNodeClass* pNode) { m_pNode = pNode; }
    inline CNodeClass* GetClass(void) { return m_pNode; }

private:
    CNodeClass* m_pNode;
};