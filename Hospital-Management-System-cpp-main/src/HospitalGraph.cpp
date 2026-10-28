#include "HospitalGraph.h"
#include <iostream>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

HospitalGraph::HospitalGraph() {
    setupDefaultHospitalLayout();
}

void HospitalGraph::addEdge(const string& u, const string& v, int weight) {
    adjList[u].push_back({v, weight});
    adjList[v].push_back({u, weight}); 
}

void HospitalGraph::setupDefaultHospitalLayout() {
    addEdge("Entrance", "Reception", 10);
    addEdge("Reception", "Emergency", 20);
    addEdge("Reception", "Pharmacy", 15);
    addEdge("Emergency", "ICU", 30);
    addEdge("Emergency", "Operation Theater", 50);
    addEdge("Reception", "General Ward", 40);
    addEdge("General Ward", "Doctor Offices", 20);
    addEdge("Doctor Offices", "Pharmacy", 25);
    addEdge("ICU", "Operation Theater", 10);
}

void HospitalGraph::displayLocations() const {
    cout << "--- Available Hospital Locations ---\n";
    for (const auto& pair : adjList) cout << "- " << pair.first << "\n";
}

void HospitalGraph::findShortestPath(const string& start, const string& end) const {
    if (adjList.find(start) == adjList.end() || adjList.find(end) == adjList.end()) {
        cout << "Invalid start or end location.\n"; return;
    }

    unordered_map<string, int> distances;
    unordered_map<string, string> previous;
    for (const auto& pair : adjList) distances[pair.first] = numeric_limits<int>::max();
    distances[start] = 0;

    using pdi = pair<int, string>;
    priority_queue<pdi, vector<pdi>, greater<pdi>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto currentPair = pq.top();
        int currentDist = currentPair.first;
        string currentLoc = currentPair.second;
        pq.pop();

        if (currentLoc == end) break;
        if (currentDist > distances[currentLoc]) continue;

        for (const auto& neighbor : adjList.at(currentLoc)) {
            if (int newDist = currentDist + neighbor.second; newDist < distances[neighbor.first]) {
                distances[neighbor.first] = newDist;
                previous[neighbor.first] = currentLoc;
                pq.push({newDist, neighbor.first});
            }
        }
    }

    if (distances[end] == numeric_limits<int>::max()) { cout << "No path found.\n"; return; }

    vector<string> path;
    for (string at = end; at != ""; at = previous[at]) path.push_back(at);
    reverse(path.begin(), path.end());

    cout << "Shortest Path (" << distances[end] << "m): ";
    for (size_t i = 0; i < path.size(); ++i) cout << path[i] << (i < path.size() - 1 ? " -> " : "\n");
}
