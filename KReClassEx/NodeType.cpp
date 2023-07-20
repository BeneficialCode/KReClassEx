#include "stdafx.h"
#include "NodeTypes.h"

const WCHAR* NodeTypeToString(NodeType type) {
	return s_NodeTypes[static_cast<int>(type)];
}

bool IsHexType(NodeType type) {
	if (type == NodeType::Hex64 ||
		type == NodeType::Hex32 ||
		type == NodeType::Hex16 ||
		type == NodeType::Hex8 ||
		type == NodeType::Bits) {
		return true;
	}
	return false;
}