#pragma once
#include "NodeClass.h"

struct IMainFrame abstract {
	virtual void CalcAllOffsets() = 0;
	virtual void CalcOffsets(CNodeClass* pClass) = 0;
	virtual void ClearHidden() = 0;
	virtual bool IsNodeValid(CNodeBase* pCheckNode) = 0;
	virtual CNodeBase* CreateNewNode(NodeType type) = 0;
};