#include "dijkstra.h"
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <string>



using namespace std;
using namespace bridges;

vector<string> dijkstra(GraphAdjList<string, double, double>& city_graph,
                        map<string, double>& edge_weights,
                        string startVertex,
                        string endVertex) {

    map<string,double> dist;
    map<string,string> prev;

    // get vertex names from Bridges
    unordered_map<string, Element<double>*>* vertexMap = city_graph.getVertices();
    unordered_map<string, Element<double>*>::iterator it;
    for (it = vertexMap->begin(); it != vertexMap->end(); ++it) {
        string cityName = it->first;
        dist[cityName] = numeric_limits<double>::infinity();
        prev[cityName] = "";
    }

    dist[startVertex] = 0.0;

    // Build adjacency list from edge_weights for efficiency
    map<string, vector<pair<string, double>>> adj;
    for (map<string, double>::iterator edgeIt = edge_weights.begin(); edgeIt != edge_weights.end(); ++edgeIt) {
        string key = edgeIt->first;
        double weight = edgeIt->second;

        size_t comma_pos1 = key.find(",");
        size_t comma_pos2 = key.find(",", comma_pos1 + 2); // skip ", "
        string city1 = key.substr(0, comma_pos2);
        string city2 = key.substr(comma_pos2 + 2);

        adj[city1].push_back({city2, weight});
        adj[city2].push_back({city1, weight});
    }

    // Min-heap for Dijkstra
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> pq;
    pq.push({0.0, startVertex});

    while (!pq.empty()) {
        pair<double, string> current = pq.top();
        pq.pop();
        double current_dist = current.first;
        string u = current.second;

        if (u == endVertex) break;
        if (current_dist > dist[u]) continue;

        // Use adjacency list for neighbors (O(1) per neighbor)
        for (size_t i = 0; i < adj[u].size(); i++) {
            string neighbor = adj[u][i].first;
            double weight = adj[u][i].second;

            double alt = dist[u] + weight;
            if (alt < dist[neighbor]) {
                dist[neighbor] = alt;
                prev[neighbor] = u;
                pq.push({alt, neighbor});
            }
        }
    }

    // Reconstruct path
    vector<string> path;
    string current = endVertex;
    while (current != "") {
        path.push_back(current);
        current = prev[current];
    }
    reverse(path.begin(), path.end());
    return path;
}
