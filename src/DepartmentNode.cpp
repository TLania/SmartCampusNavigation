// DepartmentNode.cpp
// Design: Implements the department tree node declared in DepartmentNode.h.
#include "DepartmentNode.h"
using namespace std;

// Constructor
DepartmentNode::DepartmentNode(const string& departmentName, DepartmentNode* parentNode)
    : name(departmentName), parent(parentNode), children(), depth(0) {
    if (parentNode != nullptr) {
        depth = parentNode->GetDepth() + 1;
    }
}

// Accessors (Getters)
string DepartmentNode::GetName() const { return name; }
DepartmentNode* DepartmentNode::GetParent() const { return parent; }
int DepartmentNode::GetDepth() const { return depth; }
int DepartmentNode::GetChildCount() const { return (int)children.size(); }

// Child at a position in insertion order
DepartmentNode* DepartmentNode::GetChildAt(int index) const {
    return children.at(index);
}

// Adds a child at the end of the children list
void DepartmentNode::AddChild(DepartmentNode* child) {
    children.push_back(child);
}
