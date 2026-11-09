// DepartmentNode.h
// Design: One department in the general tree. A node can have multiple children, stored in a vector in insertion order. 
// The DepartmentTree manages the nodes and is responsible for their deletion; individual nodes does not delete their children.
#ifndef DEPARTMENT_NODE_H
#define DEPARTMENT_NODE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class DepartmentNode {
private:
    string name;                       // First accepted becomes set spelling
    DepartmentNode* parent;            // nullptr for the root
    vector<DepartmentNode*> children;  // insertion order
    int depth;                         // root is 0

public:
    // Constructor: depth is the parent's depth plus 1. Root is 0
    DepartmentNode(const string& departmentName, DepartmentNode* parentNode);

    // Accessors (Getters)
    string GetName() const;
    DepartmentNode* GetParent() const;
    int GetDepth() const;
    int GetChildCount() const;
    DepartmentNode* GetChildAt(int index) const;

    // Adds a child at the end of the children list
    void AddChild(DepartmentNode* child);
};

#endif // DEPARTMENT_NODE_H
