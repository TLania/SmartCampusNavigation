// CampusGraph.h
// Design: The campus map consists of buildings represented as vertices and walkways as undirected, unweighted edges. Loads buildings.txt and connections.txt.
// Utilizes adjacency list. buildings are stored in the order they appear in `buildings.txt`. Each entry in `adjacency[i]` contains the neighbors of building i, reflecting the order of connections as they were accepted.
// Each connection is recorded at both endpoints but counted once. The list is dynamic. Used for display, degree calculation, and BFS.
#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Outcomes of checking buildings.txt record
enum BuildingResult {
    BLD_RES_ACCEPTED,
    BLD_RES_BAD_FORMAT,
    BLD_RES_DUPLICATE
};

// Outcomes of checking connections.txt record
enum ConnectionResult {
    CON_RES_ACCEPTED,
    CON_RES_BAD_FIELD_COUNT,
    CON_RES_EMPTY_NAME,
    CON_RES_UNKNOWN_BUILDING,
    CON_RES_SELF_CONNECTION,
    CON_RES_DUPLICATE
};

// Takes a result and turns it into a readable reason for a warning
string GetBuildingResultMessage(BuildingResult result);
string GetConnectionResultMessage(ConnectionResult result);

class CampusGraph {
private:
    vector<string> buildings;          // buildings in buildings.txt order
    vector< vector<int> > adjacency;   // neighbors of each building
    int connectionCount;               // Connections, each counted once

    // Private helper functions
    bool connectionExists(int first, int second) const;
    void breadthFirstSearch(int start, vector<int>& visitOrder, vector<int>& distance) const;

public:
    // Constructor
    CampusGraph();

    // File input. Prints a warning per rejected record, then returns the number of accepted records
    int LoadBuildings(const string& directory, const string& filename);
    int LoadConnections(const string& directory, const string& filename);

    // Accessors
    int GetBuildingCount() const;
    int GetConnectionCount() const;
    string GetBuildingAt(int index) const;
    vector<string> GetBuildingNames() const;

    // Finds a building by name
    // Returns its position, or -1 when there is no such building.
    int FindBuilding(const string& name) const;

    // Display building with its neighbors in connection order
    void DisplayGraph() const;

    // Display total buildings, total connections and the degree of all building
    void DisplayCountsAndDegrees() const;

    // BFS from the named building: prints the reachable buildings in visit order with their distance in connections, then "Reachable: X of Y".
    // An unknown name prints an error and nothing else.
    void DisplayReachable(const string& startName) const;
};

#endif // CAMPUS_GRAPH_H
