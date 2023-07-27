#include "stdafx.h"
#include "NodeFunction.h"

CNodeFunction::~CNodeFunction() {
	
}

CNodeFunction::CNodeFunction() {
	m_NodeType = NodeType::Function;
	m_MemorySize = sizeof(ULONG_PTR);
}
