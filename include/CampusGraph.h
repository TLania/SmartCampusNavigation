// CampusGraph.h
// Purpose: Store an undirected campus graph and perform BFS reachability.
// Author: Integration draft; team should confirm module ownership.
#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H
#include <string>
#include <vector>
class CampusGraph {
    std::vector<std::string> buildings;
    std::vector<std::vector<int>> neighbors;
    int edges = 0;
    int indexOf(const std::string& name) const;
public:
    void LoadBuildings(const std::string& directory, const std::string& filename);
    void LoadConnections(const std::string& directory, const std::string& filename);
    const std::vector<std::string>& GetBuildings() const { return buildings; }
    void Display() const;
    void DisplayStatistics() const;
    void BFS(const std::string& start) const;
};
#endif
