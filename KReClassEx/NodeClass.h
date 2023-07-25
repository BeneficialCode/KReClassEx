#pragma once
#include "NodeBase.h"

class CNodeClass : public CNodeBase {
public:
	CNodeClass();

	virtual void Update(const PHOTSPOT spot);

	virtual ULONG GetMemorySize();

	virtual NODESIZE Draw(const PVIEWINFO view, int x, int y);
	
	void SetCode(WCHAR* code) { m_Code = code; }

	ULONG_PTR ConvertStrToAddress(CString str);

	size_t m_Idx;
	size_t m_RequestPosition;
	CString m_Code;
};