// CampusGraph.cpp
// Design: Implements loading and display of the campus graph.
#include "CampusGraph.h"
#include "Utils.h"
#include <fstream>
#include <queue>
using namespace std;

// Reason for each buildings.txt outcome
string GetBuildingResultMessage(BuildingResult result) {
    switch (result) {
        case BLD_RES_ACCEPTED:
            return "Accepted";
        case BLD_RES_BAD_FORMAT:
            return "Building line holds one name and no '|'";
        case BLD_RES_DUPLICATE:
            return "Duplicate building name (the first record is kept)";
    }
    return "unknown problem";
}

// Reason for each connections.txt outcome
string GetConnectionResultMessage(ConnectionResult result) {
    switch (result) {
        case CON_RES_ACCEPTED:
            return "Accepted";
        case CON_RES_BAD_FIELD_COUNT:
            return "Expected BuildingA|BuildingB";
        case CON_RES_EMPTY_NAME:
            return " Building name is empty";
        case CON_RES_UNKNOWN_BUILDING:
            return "Building is not in buildings.txt";
        case CON_RES_SELF_CONNECTION:
            return "Building cannot connect to itself";
        case CON_RES_DUPLICATE:
            return "Duplicate connection (including the reverse order)";
    }
    return "Unknown Error";
}

// Constructor, empty graph
CampusGraph::CampusGraph() : buildings(), adjacency(), connectionCount(0) {}

// True when the two buildings are already neighbors
// Catches B|A after A|B
bool CampusGraph::connectionExists(int first, int second) const {
    const vector<int>& neighbors = adjacency.at(first);
    for (size_t i = 0; i < neighbors.size(); i++) {
        if (neighbors.at(i) == second) {
            return true;
        }
    }
    return false;
}

// Breadth-first search with a queue. A building is marked visited only at the moment it is added to the queue, so it can never be queued twice
// Neighbors are taken in connection order
// visitOrder lists the reachable buildings in the order they are visited; distance holds the number of connections from start for each building, -1 means not reachable
void CampusGraph::breadthFirstSearch(int start, vector<int>& visitOrder, vector<int>& distance) const {
    vector<bool> visited(buildings.size(), false);
    distance.assign(buildings.size(), -1);
    visitOrder.clear();

    queue<int> pending;
    visited.at(start) = true;
    distance.at(start) = 0;
    pending.push(start);

    while (!pending.empty()) {
        int current = pending.front();
        pending.pop();
        visitOrder.push_back(current);

        const vector<int>& neighbors = adjacency.at(current);
        for (size_t i = 0; i < neighbors.size(); i++) {
            int next = neighbors.at(i);
            if (!visited.at(next)) {
                visited.at(next) = true;
                distance.at(next) = distance.at(current) + 1;
                pending.push(next);
            }
        }
    }
}

// Reads buildings.txt: one building name per line. Names must be unique
// First accepted spelling is kept
int CampusGraph::LoadBuildings(const string& directory, const string& filename) {
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

        string name = trim(line);
        BuildingResult result = BLD_RES_ACCEPTED;
        if (name.find('|') != string::npos) {
            result = BLD_RES_BAD_FORMAT;
        } else if (FindBuilding(name) >= 0) {
            result = BLD_RES_DUPLICATE;
        }

        if (result != BLD_RES_ACCEPTED) {
            cout << "Warning: " << filename << " line " << lineNumber << ": "
                 << GetBuildingResultMessage(result) << endl;
            rejected++;
            continue;
        }

        buildings.push_back(name);
        adjacency.push_back(vector<int>());
        accepted++;
    }
    file.close();

    cout << filename << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
    return accepted;
}

// Reads connections.txt: BuildingA|BuildingB
// Order: field, count, empty names, unknown building, self-connection, duplicate. A connection is added to both buildings and counted once
int CampusGraph::LoadConnections(const string& directory, const string& filename) {
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
        ConnectionResult result = CON_RES_ACCEPTED;
        string unknownName = "";
        int first = -1;
        int second = -1;

        if (splitBatch(line, batch) != 2) {
            result = CON_RES_BAD_FIELD_COUNT;
        } else {
            string nameA = trim(batch.at(0));
            string nameB = trim(batch.at(1));

            if (nameA.empty() || nameB.empty()) {
                result = CON_RES_EMPTY_NAME;
            } else {
                first = FindBuilding(nameA);
                second = FindBuilding(nameB);
                if (first < 0) {
                    result = CON_RES_UNKNOWN_BUILDING;
                    unknownName = nameA;
                } else if (second < 0) {
                    result = CON_RES_UNKNOWN_BUILDING;
                    unknownName = nameB;
                } else if (first == second) {
                    result = CON_RES_SELF_CONNECTION;
                } else if (connectionExists(first, second)) {
                    result = CON_RES_DUPLICATE;
                }
            }
        }

        if (result != CON_RES_ACCEPTED) {
            cout << "Warning: " << filename << " line " << lineNumber << ": "
                 << GetConnectionResultMessage(result);
            if (result == CON_RES_UNKNOWN_BUILDING) {
                cout << ": " << unknownName;
            }
            cout << endl;
            rejected++;
            continue;
        }

        adjacency.at(first).push_back(second);
        adjacency.at(second).push_back(first);
        connectionCount++;
        accepted++;
    }
    file.close();

    cout << filename << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
    return accepted;
}

// Number of buildings
int CampusGraph::GetBuildingCount() const {
    return (int)buildings.size();
}

// Number of connections, each counted once
int CampusGraph::GetConnectionCount() const {
    return connectionCount;
}

// Building at a position, in buildings.txt order
string CampusGraph::GetBuildingAt(int index) const {
    return buildings.at(index);
}

// Copy of all building names, in buildings.txt order
vector<string> CampusGraph::GetBuildingNames() const {
    return buildings;
}

// Trims name, then compares. Internal spaces must match.
int CampusGraph::FindBuilding(const string& name) const {
    string wanted = toUpperCase(trim(name));
    if (wanted.empty()) {
        return -1;
    }
    for (size_t i = 0; i < buildings.size(); i++) {
        if (toUpperCase(buildings.at(i)) == wanted) {
            return (int)i;
        }
    }
    return -1;
}

// Prints all building, buildings.txt order and its neighbors, connection order
void CampusGraph::DisplayGraph() const {
    if (buildings.empty()) {
        cout << "No buildings are loaded." << endl;
        return;
    }

    cout << "Campus graph (building: neighbors in connection order):" << endl;
    for (size_t i = 0; i < buildings.size(); i++) {
        cout << buildings.at(i) << ": ";
        const vector<int>& neighbors = adjacency.at(i);
        if (neighbors.empty()) {
            cout << "no connections" << endl;
            continue;
        }
        for (size_t j = 0; j < neighbors.size(); j++) {
            if (j > 0) {
                cout << ", ";
            }
            cout << buildings.at(neighbors.at(j));
        }
        cout << endl;
    }
}

// Prints the total buildings, the total connections and the degree of every building, in buildings.txt order
void CampusGraph::DisplayCountsAndDegrees() const {
    if (buildings.empty()) {
        cout << "No buildings are loaded." << endl;
        return;
    }

    // Column width is dynamic
    size_t width = string("Building").size();
    for (size_t i = 0; i < buildings.size(); i++) {
        if (buildings.at(i).size() > width) {
            width = buildings.at(i).size();
        }
    }
    width = width + 2;

    cout << "Building" << string(width - string("Building").size(), ' ') << "Degree" << endl;
    for (size_t i = 0; i < buildings.size(); i++) {
        cout << buildings.at(i) << string(width - buildings.at(i).size(), ' ')
             << adjacency.at(i).size() << endl;
    }
    cout << "Total buildings: " << buildings.size() << ". Total connections: "
         << connectionCount << "." << endl;
}

// Runs BFS from the named building. An unknown or blank name is an error and the menu continues
void CampusGraph::DisplayReachable(const string& startName) const {
    string cleaned = trim(startName);
    if (cleaned.empty()) {
        cout << "Error: the building name cannot be blank." << endl;
        return;
    }

    int start = FindBuilding(cleaned);
    if (start < 0) {
        cout << "Error: unknown building: " << cleaned << endl;
        return;
    }

    vector<int> visitOrder;
    vector<int> distance;
    breadthFirstSearch(start, visitOrder, distance);

    for (size_t i = 0; i < visitOrder.size(); i++) {
        int index = visitOrder.at(i);
        cout << buildings.at(index) << " (distance " << distance.at(index) << ")" << endl;
    }
    cout << "Reachable: " << visitOrder.size() << " of " << buildings.size() << " buildings" << endl;
}
