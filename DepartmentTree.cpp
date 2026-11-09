// DepartmentTree.cpp
// Design: Implements loading and building of the department tree.
#include "DepartmentTree.h"
#include "Utils.h"
#include <fstream>
using namespace std;

// Readable reason for each outcome, used in warnings and errors
string GetDepartmentResultMessage(DepartmentResult result) {
    switch (result) {
        case DEP_RES_ACCEPTED:
            return "Accepted";
        case DEP_RES_BAD_FIELD_COUNT:
            return "Expected DepartmentName|ParentName)";
        case DEP_RES_EMPTY_FIELD:
            return "Missing a required fieldy";
        case DEP_RES_ROOT_NEEDS_NONE:
            return "The first department, the root, must have parent: none";
        case DEP_RES_ROOT_EXISTS:
            return "Only the root can have parent: none";
        case DEP_RES_UNKNOWN_PARENT:
            return "Parent department is not in the tree";
        case DEP_RES_DUPLICATE_NAME:
            return "Duplicate department name";
    }
    return "Unknown Error";
}

// Constructor, an empty tree
DepartmentTree::DepartmentTree() : root(nullptr), count(0) {}

// Copy constructor, own copy of every node
DepartmentTree::DepartmentTree(const DepartmentTree& other) : root(nullptr), count(0) {
    if (other.root != nullptr) {
        root = copySubtree(other.root, nullptr);
    }
    count = other.count;
}

// Destructor, frees every node
DepartmentTree::~DepartmentTree() {
    deleteSubtree(root);
}

// Assignment operator, frees the old nodes, then copies the other tree
DepartmentTree& DepartmentTree::operator =(const DepartmentTree& other) {
    if (this != &other) {
        deleteSubtree(root);
        root = nullptr;
        if (other.root != nullptr) {
            root = copySubtree(other.root, nullptr);
        }
        count = other.count;
    }
    return *this;
}

// Searches the subtree below current, parent before children, children in insertion order. upperName must already be trimmed and uppercase
DepartmentNode* DepartmentTree::findNode(DepartmentNode* current, const string& upperName) const {
    if (current == nullptr) {
        return nullptr;
    }
    if (toUpperCase(current->GetName()) == upperName) {
        return current;
    }
    for (int i = 0; i < current->GetChildCount(); i++) {
        DepartmentNode* found = findNode(current->GetChildAt(i), upperName);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}

// Deletes a node and everything below it, children first
void DepartmentTree::deleteSubtree(DepartmentNode* current) {
    if (current == nullptr) {
        return;
    }
    for (int i = 0; i < current->GetChildCount(); i++) {
        deleteSubtree(current->GetChildAt(i));
    }
    delete current;
}

// Makes a copy of a node and all of its descendants under parentCopy
DepartmentNode* DepartmentTree::copySubtree(const DepartmentNode* source, DepartmentNode* parentCopy) {
    DepartmentNode* created = new DepartmentNode(source->GetName(), parentCopy);
    if (parentCopy != nullptr) {
        parentCopy->AddChild(created);
    }
    for (int i = 0; i < source->GetChildCount(); i++) {
        copySubtree(source->GetChildAt(i), created);
    }
    return created;
}

// Checks run in this order: empty names, root rules, parent exists, duplicate name
DepartmentResult DepartmentTree::AddDepartment(const string& name, const string& parentName) {
    string cleanName = trim(name);
    string cleanParent = trim(parentName);
    if (cleanName.empty() || cleanParent.empty()) {
        return DEP_RES_EMPTY_FIELD;
    }

    bool parentIsNone = (toUpperCase(cleanParent) == "NONE");
    DepartmentNode* parentNode = nullptr;

    if (root == nullptr) {
        if (!parentIsNone) {
            return DEP_RES_ROOT_NEEDS_NONE;
        }
    } else {
        if (parentIsNone) {
            return DEP_RES_ROOT_EXISTS;
        }
        parentNode = findNode(root, toUpperCase(cleanParent));
        if (parentNode == nullptr) {
            return DEP_RES_UNKNOWN_PARENT;
        }
    }

    if (findNode(root, toUpperCase(cleanName)) != nullptr) {
        return DEP_RES_DUPLICATE_NAME;
    }

    DepartmentNode* created = new DepartmentNode(cleanName, parentNode);
    if (parentNode == nullptr) {
        root = created;
    } else {
        parentNode->AddChild(created);
    }
    count++;
    return DEP_RES_ACCEPTED;
}

// Reads departments.txt line by line. A parent must already be in the tree
int DepartmentTree::LoadFromFile(const string& directory, const string& filename) {
    string path = buildPath(directory, filename);
    int accepted = 0;
    int rejected = 0;

    ifstream file(path.c_str());
    if (!file.is_open()) {
        cout << "Error: cannot open " << path << " - treating it as empty." << endl;
        cout << filename << ": 0 accepted, 0 rejected" << endl;
        return 0;
    }

    string line;
    int lineNumber = 0;
    while (getline(file, line)) {
        lineNumber++;
        if (isSkippableLine(line)) {
            continue;
        }

        vector<string> batch;
        DepartmentResult result = DEP_RES_ACCEPTED;
        if (splitBatch(line, batch) != 2) {
            result = DEP_RES_BAD_FIELD_COUNT;
        } else {
            result = AddDepartment(batch.at(0), batch.at(1));
        }

        if (result != DEP_RES_ACCEPTED) {
            cout << "Warning: " << filename << " line " << lineNumber << ": "
                 << GetDepartmentResultMessage(result);
            if (result == DEP_RES_UNKNOWN_PARENT) {
                cout << ": " << trim(batch.at(1));
            }
            cout << endl;
            rejected++;
            continue;
        }
        accepted++;
    }
    file.close();

    cout << filename << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
    return accepted;
}

// True when no department has been added
bool DepartmentTree::IsEmpty() const {
    return root == nullptr;
}

// Number of departments
int DepartmentTree::GetSize() const {
    return count;
}

// The root node, or nullptr for an empty tree
DepartmentNode* DepartmentTree::GetRoot() const {
    return root;
}

// Name lookup: trimmed, internal spaces kept
DepartmentNode* DepartmentTree::FindDepartment(const string& name) const {
    string wanted = toUpperCase(trim(name));
    if (wanted.empty()) {
        return nullptr;
    }
    return findNode(root, wanted);
}

// Prints a node, then each child in insertion order, indenting four spaces per level of depth
void DepartmentTree::displayHierarchy(const DepartmentNode* current) const {
    cout << string(4 * current->GetDepth(), ' ') << current->GetName() << endl;
    for (int i = 0; i < current->GetChildCount(); i++) {
        displayHierarchy(current->GetChildAt(i));
    }
}

// Preorder, the node first, then each child's subtree in insertion order
void DepartmentTree::collectPreorder(const DepartmentNode* current, vector<string>& names) const {
    names.push_back(current->GetName());
    for (int i = 0; i < current->GetChildCount(); i++) {
        collectPreorder(current->GetChildAt(i), names);
    }
}

// Adds a department. Prints report
// Any error the tree is unchanged
void DepartmentTree::AddDepartmentWithReport(const string& name, const string& parentName) {
    DepartmentResult result = AddDepartment(name, parentName);

    if (result == DEP_RES_ACCEPTED) {
        DepartmentNode* added = FindDepartment(name);
        if (added->GetParent() == nullptr) {
            cout << "Added root department: " << added->GetName() << endl;
        } else {
            cout << "Added department: " << added->GetName() << " (parent: "
                 << added->GetParent()->GetName() << ")" << endl;
        }
        return;
    }

    cout << "Error: " << GetDepartmentResultMessage(result);
    if (result == DEP_RES_UNKNOWN_PARENT) {
        cout << ": " << trim(parentName);
    } else if (result == DEP_RES_DUPLICATE_NAME) {
        cout << ": " << trim(name);
    }
    cout << endl;
}

// Looks the department up by name and print details
void DepartmentTree::SearchByName(const string& name) const {
    string cleaned = trim(name);
    if (cleaned.empty()) {
        cout << "Error: the department name cannot be blank." << endl;
        return;
    }

    DepartmentNode* found = FindDepartment(cleaned);
    if (found == nullptr) {
        cout << "Not found: no department named " << cleaned << endl;
        return;
    }

    cout << "Department: " << found->GetName() << endl;
    if (found->GetParent() == nullptr) {
        cout << "Parent: None" << endl;
    } else {
        cout << "Parent: " << found->GetParent()->GetName() << endl;
    }
    cout << "Direct children: " << found->GetChildCount() << endl;
    cout << "Depth: " << found->GetDepth() << endl;
}

// Tree in preorder with indentation
void DepartmentTree::DisplayHierarchy() const {
    if (root == nullptr) {
        cout << "No departments are loaded." << endl;
        return;
    }
    displayHierarchy(root);
}

// Preorder sequence on one line, separated by commas
void DepartmentTree::DisplayPreorder() const {
    if (root == nullptr) {
        cout << "No departments are loaded." << endl;
        return;
    }

    vector<string> names;
    collectPreorder(root, names);
    cout << "Preorder traversal: ";
    for (size_t i = 0; i < names.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << names.at(i);
    }
    cout << endl;
}
