// main.cpp
// Purpose: Load campus records and coordinate the Milestone 1 menu.
// Author: Integration draft; team should confirm module ownership.
#include "CampusGraph.h"
#include "FacilityManager.h"
#include "DepartmentTree.h"
#include "Utils.h"
#include <iostream>
#include <string>
using namespace std;

// Read a nonempty name, preserving spaces. Return false for blank input.
bool readName(const string& prompt, string& value) {
    cout << prompt;
    if (!getline(cin,value)) return false;
    value=trim(value);
    if (value.empty()) { cout << "Error: name cannot be blank.\n"; return false; }
    return true;
}

// Display the exact Milestone 1 menu numbering.
void printMenu() {
    cout << "\n=== Smart Campus Navigation - Milestone 1 ===\n"
         << "1. View all facilities\n2. Search facility by ID\n"
         << "3. View campus graph\n4. Display graph counts and degrees\n"
         << "5. Find reachable buildings (BFS)\n6. Add department\n"
         << "7. Search department\n8. Display department hierarchy\n"
         << "9. Run preorder traversal\n10. Exit\nChoice: ";
}

int main(int argc, char* argv[]) {
    const string directory=(argc>1)?argv[1]:"data";
    CampusGraph graph;
    FacilityManager facilities;
    DepartmentTree departments;
    graph.LoadBuildings(directory,"buildings.txt");
    graph.LoadConnections(directory,"connections.txt");
    facilities.LoadFromFile(directory,"facilities.txt",graph.GetBuildings());
    departments.LoadFromFile(directory,"departments.txt");

    string input, name, parent;
    while (true) {
        printMenu();
        if (!getline(cin,input)) break;
        input=trim(input);
        // Only accept a single integer token in the valid menu range.
        int choice=0;
        try {
            size_t used=0;
            if (input.empty()) throw invalid_argument("empty");
            choice=stoi(input,&used);
            if (used!=input.size() || choice<1 || choice>10) throw invalid_argument("range");
        } catch (const exception&) { cout << "Error: invalid menu choice.\n"; continue; }
        if (choice==10) { cout << "Goodbye!\n"; break; }
        switch (choice) {
            case 1: facilities.DisplayAll(); break;
            case 2:
                if (readName("Facility ID: ",name)) facilities.SearchByID(name);
                break;
            case 3: graph.Display(); break;
            case 4: graph.DisplayStatistics(); break;
            case 5:
                if (readName("Starting building: ",name)) graph.BFS(name);
                break;
            case 6:
                if (!readName("Department name: ",name)) break;
                if (departments.IsEmpty()) parent="NONE";
                else if (!readName("Parent department: ",parent)) break;
                departments.AddDepartmentWithReport(name,parent);
                break;
            case 7:
                if (readName("Department name: ",name)) departments.SearchByName(name);
                break;
            case 8: departments.DisplayHierarchy(); break;
            case 9: departments.DisplayPreorder(); break;
        }
    }
    return 0;
}
