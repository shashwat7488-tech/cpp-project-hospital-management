#pragma once
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

/**
 * @class HospitalGraph
 * @brief Uses an Adjacency List to map the hospital layout and calculate shortest paths (Dijkstra's Algorithm).
 */
class HospitalGraph {
private:
    unordered_map<string, vector<pair<string, int>>> adjList;

public:
    HospitalGraph();
    
    void addEdge(const string& u, const string& v, int weight);
    void setupDefaultHospitalLayout();

    /** @brief O(V + E log V) Time | O(V + E) Space */
    void findShortestPath(const string& start, const string& end) const;

    /** @brief O(V) Time | O(1) Space */
    void displayLocations() const;
};
