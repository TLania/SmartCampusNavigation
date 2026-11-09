// CampusGraph.cpp
// Purpose: Validate campus building and walkway files and implement BFS.
// Author: Integration draft; team should confirm module ownership.
#include "CampusGraph.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

// Find a building by trimmed, case-insensitive name.
int CampusGraph::indexOf(const string& name) const {
    string key = toUpperCase(trim(name));
    for (size_t i=0; i<buildings.size(); ++i)
        if (toUpperCase(buildings[i]) == key) return static_cast<int>(i);
    return -1;
}

// Load unique buildings, preserving input order.
void CampusGraph::LoadBuildings(const string& directory, const string& filename) {
    ifstream in(buildPath(directory, filename));
    if (!in) { cerr << "Error: cannot open " << filename << endl; cout << filename << ": 0 accepted, 0 rejected" << endl; return; }
    string line; int accepted=0, rejected=0, lineNo=0;
    while (getline(in,line)) {
        ++lineNo;
        if (isSkippableLine(line)) continue;
        string name=trim(line);
        string reason;
        if (name.empty()) reason="empty building name";
        else if (name.find('|')!=string::npos) reason="incorrect field count";
        else if (indexOf(name)>=0) reason="duplicate building";
        if (!reason.empty()) { ++rejected; cout << "Warning: " << filename << " line " << lineNo << ": " << reason << endl; continue; }
        buildings.push_back(name); neighbors.emplace_back(); ++accepted;
    }
    cout << filename << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
}

// Load unique, non-self undirected connections in input order.
void CampusGraph::LoadConnections(const string& directory, const string& filename) {
    ifstream in(buildPath(directory,filename));
    if (!in) { cerr << "Error: cannot open " << filename << endl; cout << filename << ": 0 accepted, 0 rejected" << endl; return; }
    string line; int accepted=0,rejected=0,lineNo=0;
    while (getline(in,line)) {
        ++lineNo;
        if (isSkippableLine(line)) continue;
        vector<string> fields; splitBatch(line,fields);
        string reason; int a=-1,b=-1;
        if (fields.size()!=2) reason="incorrect field count";
        else if (fields[0].empty() || fields[1].empty()) reason="empty building name";
        else {
            a=indexOf(fields[0]); b=indexOf(fields[1]);
            if (a<0 || b<0) reason="unknown building reference";
            else if (a==b) reason="self-connection";
            else if (find(neighbors[a].begin(),neighbors[a].end(),b)!=neighbors[a].end()) reason="duplicate connection";
        }
        if (!reason.empty()) { ++rejected; cout << "Warning: " << filename << " line " << lineNo << ": " << reason << endl; continue; }
        neighbors[a].push_back(b); neighbors[b].push_back(a); ++edges; ++accepted;
    }
    cout << filename << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
}

// Print buildings and their neighbors in insertion order.
void CampusGraph::Display() const {
    if (buildings.empty()) { cout << "No buildings are loaded.\n"; return; }
    for (size_t i=0;i<buildings.size();++i) {
        cout << buildings[i] << ":";
        if (neighbors[i].empty()) cout << " (no connections)";
        for (int n:neighbors[i]) cout << " " << buildings[n];
        cout << '\n';
    }
}

// Print total vertices, undirected edges, and each degree.
void CampusGraph::DisplayStatistics() const {
    cout << "Total buildings: " << buildings.size() << "\nTotal connections: " << edges << '\n';
    for (size_t i=0;i<buildings.size();++i) cout << buildings[i] << ": degree " << neighbors[i].size() << '\n';
}

// Visit each reachable building once, marking visited at enqueue.
void CampusGraph::BFS(const string& start) const {
    int origin=indexOf(start);
    if (origin<0) { cout << "Error: unknown building: " << trim(start) << '\n'; return; }
    vector<int> distance(buildings.size(),-1);
    queue<int> q; q.push(origin); distance[origin]=0; int count=0;
    while (!q.empty()) {
        int u=q.front();q.pop(); ++count;
        cout << buildings[u] << " (distance " << distance[u] << ")\n";
        for (int v:neighbors[u]) if (distance[v]<0) {
            distance[v]=distance[u]+1; q.push(v);
        }
    }
    cout << "Reachable: " << count << " of " << buildings.size() << " buildings\n";
}
