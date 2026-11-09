// main.cpp
// Design: Smart Campus Navigation and Facility Analytics System. 
// Processes a data-directory argument, loads four data files, and displays warnings and summaries before prompting the Milestone 1 menu until the user decides to exit.
#include <iostream>
#include <string>
#include "Utils.h"
#include "CampusGraph.h"
#include "FacilityManager.h"
#include "DepartmentTree.h"
using namespace std;

// Folder used when no command-line argument is given
const string DEFAULT_DATA_DIRECTORY = "data/";

// Menu
const int MENU_VIEW_FACILITIES = 1;
const int MENU_SEARCH_FACILITY_ID = 2;
const int MENU_VIEW_GRAPH = 3;
const int MENU_GRAPH_DEGREES = 4;
const int MENU_BFS = 5;
const int MENU_ADD_DEPARTMENT = 6;
const int MENU_SEARCH_DEPARTMENT = 7;
const int MENU_DEPARTMENT_HIERARCHY = 8;
const int MENU_PREORDER = 9;
const int MENU_EXIT = 10;
const int MENU_EXIT_ALTERNATE = 0; // option 0 also exits cleanly (UI-7)

// Prints menu
void showMenu() {
    cout << endl;
    cout << "===== Smart Campus Menu =====" << endl;
    cout << "1. View all facilities" << endl;
    cout << "2. Search facility by ID" << endl;
    cout << "3. View campus graph" << endl;
    cout << "4. Display graph counts and degrees" << endl;
    cout << "5. Find reachable buildings (BFS)" << endl;
    cout << "6. Add department" << endl;
    cout << "7. Search department" << endl;
    cout << "8. Display department hierarchy" << endl;
    cout << "9. Run preorder traversal" << endl;
    cout << "10. Exit" << endl;
}

// Returns false when the input ends
bool readLine(const string& prompt, string& line) {
    cout << prompt;
    if (!getline(cin, line)) {
        cout << endl;
        return false;
    }
    return true;
}

// Turns the text typed at the menu into a number
// Returns false for blank text, letters, signs, etc
// Whole line is read first
bool parseMenuChoice(const string& text, int& choice) {
    string cleaned = trim(text);
    if (cleaned.empty() || cleaned.size() > 6) {
        return false;
    }
    int value = 0;
    for (size_t i = 0; i < cleaned.size(); i++) {
        if (cleaned.at(i) < '0' || cleaned.at(i) > '9') {
            return false;
        }
        value = value * 10 + (cleaned.at(i) - '0');
    }
    choice = value;
    return true;
}

// Menu option 2, ask for an ID and search the hash table
void searchFacilityAction(const FacilityManager& facilities) {
    string id;
    if (!readLine("Enter facility ID: ", id)) {
        return;
    }
    if (trim(id).empty()) {
        cout << "Error: Facility ID cannot be blank." << endl;
        return;
    }
    facilities.SearchByID(id);
}

// Menu option 5, ask for a start building and runs BFS
void reachableAction(const CampusGraph& graph) {
    string name;
    if (!readLine("Enter starting building: ", name)) {
        return;
    }
    graph.DisplayReachable(name);
}

// Menu option 6, asks for a name, and a parent unless tree is empty
void addDepartmentAction(DepartmentTree& departments) {
    string name;
    if (!readLine("Enter new department name: ", name)) {
        return;
    }
    if (trim(name).empty()) {
        cout << "Error: Department name cannot be blank." << endl;
        return;
    }

    string parent = "NONE"; // an empty tree: the new department is the root
    if (!departments.IsEmpty()) {
        if (!readLine("Enter parent department name: ", parent)) {
            return;
        }
        if (trim(parent).empty()) {
            cout << "Error: Parent department name cannot be blank." << endl;
            return;
        }
    }
    departments.AddDepartmentWithReport(name, parent);
}

// Menu option 7, ask for a department name and search the tree
void searchDepartmentAction(const DepartmentTree& departments) {
    string name;
    if (!readLine("Enter department name: ", name)) {
        return;
    }
    departments.SearchByName(name);
}

int main(int argc, char* argv[]) {
    // Data directory: If given, otherwise "data/".
    string dataDirectory = DEFAULT_DATA_DIRECTORY;
    if (argc > 1) {
        dataDirectory = trim(argv[1]);
        if (dataDirectory.empty()) {
            dataDirectory = DEFAULT_DATA_DIRECTORY;
        }
    }

    cout << "Smart Campus Navigation and Facility Analytics System" << endl;
    cout << "Data directory: " << dataDirectory << endl;
    cout << endl;

    // Load order matters, facilities need the building names, and connections need the buildings
    CampusGraph graph;
    FacilityManager facilities;
    DepartmentTree departments;

    graph.LoadBuildings(dataDirectory, "buildings.txt");
    graph.LoadConnections(dataDirectory, "connections.txt");
    facilities.LoadFromFile(dataDirectory, "facilities.txt", graph.GetBuildingNames());
    departments.LoadFromFile(dataDirectory, "departments.txt");

    // Repeating menu
    bool running = true;
    while (running) {
        showMenu();
        string line;
        if (!readLine("Enter your choice: ", line)) {
            break; // input ended
        }

        int choice = -1;
        if (!parseMenuChoice(line, choice)) {
            cout << "Error: Enter a number from 1 to 10." << endl;
            continue;
        }

        if (choice == MENU_VIEW_FACILITIES) {
            facilities.DisplayAll();
        } else if (choice == MENU_SEARCH_FACILITY_ID) {
            searchFacilityAction(facilities);
        } else if (choice == MENU_VIEW_GRAPH) {
            graph.DisplayGraph();
        } else if (choice == MENU_GRAPH_DEGREES) {
            graph.DisplayCountsAndDegrees();
        } else if (choice == MENU_BFS) {
            reachableAction(graph);
        } else if (choice == MENU_ADD_DEPARTMENT) {
            addDepartmentAction(departments);
        } else if (choice == MENU_SEARCH_DEPARTMENT) {
            searchDepartmentAction(departments);
        } else if (choice == MENU_DEPARTMENT_HIERARCHY) {
            departments.DisplayHierarchy();
        } else if (choice == MENU_PREORDER) {
            departments.DisplayPreorder();
        } else if (choice == MENU_EXIT || choice == MENU_EXIT_ALTERNATE) {
            running = false;
        } else {
            cout << "Error: Enter a number from 1 to 10." << endl;
        }
    }

    cout << "Goodbye." << endl;
    return 0;
}
