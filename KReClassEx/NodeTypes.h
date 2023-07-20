#pragma once

enum class NodeType {
	Base,
	Instance,
	Struct,
	Hidden,
	Hex32,
	Hex64,
	Hex16,
	Hex8,
	Float,
	Double,
	UINT32,
	UINT16,
	UINT8,
	Text,
	Unicode,
	FunctionPtr,
	Custom,
	Vec2,
	Vec3,
	Quat,
	Matrix,
	Vtable,
	Array,
	Class,
	PChar,
	PWChar,
	Bits,
	UINT64, // qword
	Function,
	PTrArray
};



static const WCHAR* s_NodeTypes[] = {
	L"Base",
	L"Instance",
	L"Struct",
	L"Hidden",
	L"Hex32",
	L"Hex64"
	L"Hex16",
	L"Hex8",
	L"Pointer",
	L"Int64"
	L"Int32",
	L"Int16",
	L"Int8",
	L"Float",
	L"Double",
	L"Uint32",
	L"Uint16",
	L"Uint8",
	L"Text",
	L"Unicode",
	L"FunctionPtr",
	L"Custom",
	L"Vec2",
	L"Vec3",
	L"Quat",
	L"Matrix",
	L"Vtable",
	L"Array",
	L"Class",
	L"PChar",
	L"PWChar",
	L"Bits",
	L"Uint64",
	L"Function",
	L"PtArray",
};

const WCHAR* NodeTypeToString(NodeType type);
bool IsHexType(NodeType type);