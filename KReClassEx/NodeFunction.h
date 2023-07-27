#pragma once

#include "NodeBase.h"
#include "ClassView.h"

class CNodeFunction :public CNodeBase {
public:
	CNodeFunction();
	~CNodeFunction();




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