#pragma once

#include "NodeBase.h"

class CNodeFunctionPtr : public CNodeBase {
public:
	CNodeFunctionPtr();
	CNodeFunctionPtr(CWindow* pParentWindow, ULONG_PTR address);

	virtual void Update(const PHOTSPOT pSpot);

	virtual ULONG GetMemorySize() { return sizeof(void*); }

	virtual NODESIZE Draw(const PVIEWINFO pView, int x, int y);


	inline bool IsInitialized() {
		return false;
	}

	void Initialize(CWindow* pParentWindow, ULONG_PTR Address);

	inline void HideAssemblyWindow() {

	}

private:

	CWindow* m_pParentWindow{nullptr};

	ULONG m_Lines{ 0 };
	ULONG m_LongestLine{ 0 };

	int m_Width{ 0 };
	int m_Height{ 0 };

	BOOLEAN m_bRedrawNeeded{ FALSE };
};