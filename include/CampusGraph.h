// CampusGraph.h
// Purpose: Store an undirected campus graph and perform BFS reachability.
#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H
#include <string>
#include <vector>
#include <iostream>
class CampusGraph {
    vector<string> buildings;
    vector<vector<int>> neighbors;
    int edges = 0;
    int indexOf(const string& name) const;
public:
    void LoadBuildings(const string& directory, const string& filename);
    void LoadConnections(const string& directory, const string& filename);
    const vector<string>& GetBuildings() const { return buildings; }
    void Display() const;
    void DisplayStatistics() const;
    void BFS(const string& start) const;
};
#endif