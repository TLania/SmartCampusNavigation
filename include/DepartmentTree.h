// DepartmentTree.h
// Design: Implement a hierarchical structure for departments using a general tree that allows for any number of children per node. Loads departments.txt and adds departments in memory.
// The first department becomes the root with no parent, while following departments reference an existing parent within the tree.
// Memory: the tree owns every node and deletes them in its destructor.

#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include <iostream>
#include <string>
#include "DepartmentNode.h"
using namespace std;

// Names for the outcome of adding one department
enum DepartmentResult {
    DEP_RES_ACCEPTED,
    DEP_RES_BAD_FIELD_COUNT,
    DEP_RES_EMPTY_FIELD,
    DEP_RES_ROOT_NEEDS_NONE,
    DEP_RES_ROOT_EXISTS,
    DEP_RES_UNKNOWN_PARENT,
    DEP_RES_DUPLICATE_NAME
};

// Turns a DepartmentResult into a readable reason for a warning
string GetDepartmentResultMessage(DepartmentResult result);

class DepartmentTree {
private:
    DepartmentNode* root; // nullptr when tree is empty
    int count;            // number of departments

    // Private helper functions
    DepartmentNode* findNode(DepartmentNode* current, const string& upperName) const;
    void deleteSubtree(DepartmentNode* current);
    DepartmentNode* copySubtree(const DepartmentNode* source, DepartmentNode* parentCopy);
    void displayHierarchy(const DepartmentNode* current) const;
    void collectPreorder(const DepartmentNode* current, vector<string>& names) const;

public:
    // Constructor
    DepartmentTree();
    DepartmentTree(const DepartmentTree& other);
    ~DepartmentTree();
    DepartmentTree& operator =(const DepartmentTree& other);

    // File input. Prints a warning for each rejected record and a summary line, then returns the number of accepted departments
    int LoadFromFile(const string& directory, const string& filename);

    // Adds a department under the named parent,use none for the root
    // Trims both names. Returns DEP_RES_ACCEPTED
    DepartmentResult AddDepartment(const string& name, const string& parentName);

    // Accessors
    bool IsEmpty() const;
    int GetSize() const;
    DepartmentNode* GetRoot() const;

    // Finds a department by name; nullptr if absent
    DepartmentNode* FindDepartment(const string& name) const;

    // Menu actions, exists in memory only; departments.txt is not changed
    // Adds a department and prints either a confirmation or reason for rejection
    void AddDepartmentWithReport(const string& name, const string& parentName);

    // Prints the name, parent, number of direct children and depth
    void SearchByName(const string& name) const;

    // Display the hierarchy in preorder, one department per line
    void DisplayHierarchy() const;

    // Display the preorder traversal as one sequence
    void DisplayPreorder() const;
};

#endif // DEPARTMENT_TREE_H
