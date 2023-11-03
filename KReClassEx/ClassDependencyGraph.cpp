#include "stdafx.h"
#include "ClassDependencyGraph.h"

const std::vector<DependencyEdge>& DependencyNode::GetDependencies() const {
	return _Dependencies;
}

bool DependencyNode::AddDependency(const DependencyNode* dependency, const DependencyType edgeType) {
	DependencyEdge newEdge(dependency, edgeType);
	// We don't allow parallel edges.
	for (auto edge : _Dependencies) {
		if (edge == newEdge)
			return false;
	}

	_Dependencies.push_back(newEdge);
	return true;
}

bool DependencyNode::AddEdge(const DependencyNode* parent, const DependencyType edgeType) {
	DependencyEdge newEdge(parent, edgeType);

	for (auto edge : _Edges) {
		if (edge == newEdge)
			return false;
	}

	_Edges.push_back(newEdge);
	return true;
}

int ClassDependencyGraph::ProcessDependencies(DependencyEdge* edge, 
	std::map<const DependencyNode*, NodeGenerationStatus>& nodeStatus,
	std::set<const CNodeClass*>& forwardDeclarations,
	std::vector<const CNodeClass*>& classDefinitions) {
	int classesAdded = 0;
	NodeGenerationStatus nodeGenStatus;
	const DependencyNode* node = edge->_Dependency;
	if (nodeStatus.find(node) == nodeStatus.end()) {
		nodeStatus[node] = NodeGenerationStatus::Unprocessed;
	}
	nodeGenStatus = nodeStatus[node];
	// We have to treat instanced nodes differently, since they have 'hard' dependencies
	// on their parent classes.  If we come across an instanced node through a pointer, 
	// we will forward declare it because if we try to process it like normal we may end
	// up with a back edge to its parent that will break everything.
	//
	// If we come upon an instanced node througn an instance, we process it like normal.
	if (nodeGenStatus == NodeGenerationStatus::Instanced) {
		if (edge->_EdgeType == DependencyType::Pointer) {
			forwardDeclarations.insert(node->GetNodeClass());
			return 0;
		}
		else if (edge->_EdgeType == DependencyType::Instance) {
			// Change status to trigger the actual processing of the node
			nodeGenStatus = NodeGenerationStatus::Unprocessed;
		}
	}

	if (nodeGenStatus == NodeGenerationStatus::Processed) {
		// The node is already written to the output, no further processing needed
		return classesAdded;
	}
	else if (nodeGenStatus == NodeGenerationStatus::Processing) {
		// We've hit a back edge.  Since we start from leaf nodes, this dependency cannot
		// possibly be an instance dependency, it must be a pointer dependency.  Thus, a 
		// forward declaration is enough to handle this case.
		forwardDeclarations.insert(node->GetNodeClass());
		nodeStatus[node] = NodeGenerationStatus::Processed;
	}
	else {
		nodeStatus[node] = NodeGenerationStatus::Processing;
		for (auto dep : node->GetDependencies()) {
			if (dep._EdgeType == DependencyType::Instance)
				classesAdded += ProcessDependencies(&dep, nodeStatus, forwardDeclarations, classDefinitions);
		}
		for (auto dep : node->GetDependencies()) {
			if (dep._EdgeType == DependencyType::Pointer)
				classesAdded += ProcessDependencies(&dep, nodeStatus, forwardDeclarations, classDefinitions);
		}
		classDefinitions.push_back(node->GetNodeClass());
		nodeStatus[node] = NodeGenerationStatus::Processed;
		classesAdded += 1;
	}

	return classesAdded;
}

std::wstring ClassDependencyGraph::ToDot() {
	std::wstringstream stream;
	stream << L"digraph class_depency {";
	for (auto node : _Nodes) {
		for (auto edge : node.second.GetDependencies()) {
			stream << L"\"";
			stream << node.first->GetName();
			stream << L"\"";
			stream << L" -> ";
			stream << L"\"";
			stream << edge._Dependency->GetNodeClass()->GetName();
			stream << L"\"";
			if (edge._EdgeType == DependencyType::Pointer) {
				stream << L" [Style=dotted]";
			}
			stream << L":";
			stream << "\n";
		}
	}
	stream << "}";

	return stream.str();
}

std::vector<DependencyNode*> ClassDependencyGraph::GetLeafNodes() {
	std::vector<DependencyNode*> result;
	for (auto it = _Nodes.begin(); it != _Nodes.end(); it++) {
		if (it->second.GetEdges().size()==0) {
			result.push_back(&it->second);
		}
	}
	return result;
}

std::vector<DependencyNode*> ClassDependencyGraph::GetInstanceNodes() {
	std::vector<DependencyNode*> result;
	for (auto it = _Nodes.begin(); it != _Nodes.end(); it++) {
		bool hasOutInstanceEdge = false;

		for (auto edge : it->second.GetEdges()) {
			if (edge._EdgeType == DependencyType::Instance) {
				hasOutInstanceEdge = true;
				break;
			}
		}
		if (hasOutInstanceEdge) {
			result.push_back(&it->second);
		}
	}
	return result;
}


int ClassDependencyGraph::OrderClassesForGeneration(std::set<const CNodeClass*>& forwardDeclarations,
	std::vector<const CNodeClass*>& classDefinitions) {
	int classesAdded = 0;
	std::map<const DependencyNode*, NodeGenerationStatus> nodeStatus;
	std::vector<DependencyNode*> leafNodes = GetLeafNodes();
	std::vector<DependencyNode*> instancedNodes = GetInstanceNodes();

	for (auto node : instancedNodes) {
		nodeStatus[node] = NodeGenerationStatus::Instanced;
	}

	for (auto leaf : leafNodes) {
		for (auto dep : leaf->GetDependencies()) {
			classesAdded += ProcessDependencies(&dep, nodeStatus, forwardDeclarations, classDefinitions);
		}
		classDefinitions.push_back(leaf->GetNodeClass());
		classesAdded += 1;
	}

	return classesAdded;
}