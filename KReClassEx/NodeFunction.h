#pragma once

#include "NodeBase.h"
#include "ClassView.h"

class CNodeFunction :public CNodeBase {
public:
	CNodeFunction();
	~CNodeFunction();

	virtual void Update(const PHOTSPOT spot);

	virtual ULONG GetMemorySize() { return m_MemorySize; }

	virtual NODESIZE Draw(const PVIEWINFO pView, int x, int y);

	inline void SetSize(ULONG Size) { m_MemorySize = Size; }

private:
	DWORD m_MemorySize;
	std::vector<CStringA> m_Assembly;

private:

	ULONG m_Lines{ 0 };
	ULONG m_LongestLine{ 0 };

	int m_Width{ 0 };
	int m_Height{ 0 };

	BOOLEAN m_bRedrawNeeded{ FALSE };
};