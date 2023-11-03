#pragma once
#include "NodeClass.h"

enum class DependencyType {
	Pointer,
	Instance
};

class DependencyNode;
struct DependencyEdge {
	const DependencyNode* _Dependency;
	const DependencyType _EdgeType;

	DependencyEdge(const DependencyNode* dependency, const DependencyType edgeType)
		:_Dependency(dependency), _EdgeType(edgeType) {

	}

	bool operator==(const DependencyEdge& other) const {
		return this->_Dependency == other._Dependency && this->_EdgeType == other._EdgeType;
	}
};


class DependencyNode {
	const CNodeClass* _NodeClass = nullptr;
	std::vector<DependencyEdge> _Dependencies;
	std::vector<DependencyEdge> _Edges;

public:
	DependencyNode() = default;
	DependencyNode(const CNodeClass* nodeClass) :_NodeClass(nodeClass) {};
	const std::vector<DependencyEdge>& GetDependencies() const;
	const std::vector<DependencyEdge>& GetEdges() const { return _Edges; }
	const CNodeClass* GetNodeClass() const { return _NodeClass; }
	bool AddDependency(const DependencyNode* dependency, const DependencyType edgeType);
	bool AddEdge(const DependencyNode* parent, const DependencyType edgeType);
};

class ClassDependencyGraph {
	std::map<const CNodeClass*, DependencyNode> _Nodes;
	std::vector<DependencyNode*> GetLeafNodes();
	std::vector<DependencyNode*> GetInstanceNodes();
	enum class NodeGenerationStatus {
		Unprocessed,
		Processing,
		Processed,
		Instanced
	};
	int ProcessDependencies(DependencyEdge* edge, 
		std::map<const DependencyNode*, NodeGenerationStatus>& nodeStatus,
		std::set<const CNodeClass*>& forwardDeclarations,
		std::vector<const CNodeClass*>& classDefinitions);

public:
	bool AddNode(const CNodeClass* node) {
		if (_Nodes.find(node) == _Nodes.end()) {
			_Nodes[node] = DependencyNode(node);
			return true;
		}
		return false;
	}

	bool AddEdge(const CNodeClass* dependingClass, const CNodeClass* dependency, DependencyType depType) {
		// Ignore simple recursion in the dependency graph.  It won't help in our analysis and will just muddy
		// up the graph.
		if (dependingClass == dependency)
			return false;
		DependencyNode* dependingNode = &_Nodes[dependingClass];
		DependencyNode* dependencyNode = &_Nodes[dependency];
		dependencyNode->AddEdge(dependingNode, depType);
		return dependingNode->AddDependency(dependencyNode, depType);
	}
	std::wstring ToDot();
	int OrderClassesForGeneration(std::set<const CNodeClass*>& forwardDeclarations,
		std::vector<const CNodeClass*>& classDefinitions);
};