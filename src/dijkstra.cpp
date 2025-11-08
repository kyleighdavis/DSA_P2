#include "dijkstra.h"
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;
using namespace bridges;


// Here are all the references:
// - GeeksforGeeks Dijkstra Algorithm shortest path: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/
// - Bridges Graph Library: https://bridgesuncc.github.io/
// - C++ Priority Queue Reference: https://en.cppreference.com/w/cpp/container/priority_queue


// we basically use an unordered_map version instead of a min-heap priority queue.

vector<string> dijkstra(GraphAdjList<string, double, double>& city_graph,
                        unordered_map<string, double>& edge_weights,
                        string startVertex,
                        string endVertex) {

    // Store shortest known distance to each city
    unordered_map<string,double> dist;
    
    // Store previous city in the shortest path (for reconstruction later on)
    unordered_map<string,string> prev;

    // get vertex names from Bridges

    // 1. Then we initialize distances and previous pointers

    unordered_map<string, Element<double>*>* vertexMap = city_graph.getVertices();
    unordered_map<string, Element<double>*>::iterator it;

    for (it = vertexMap->begin(); it != vertexMap->end(); ++it) {
        string cityName = it->first;
        dist[cityName] = numeric_limits<double>::infinity(); //set it to infinity
        prev[cityName] = "";
    }

    // Obviously the distance from start to itself should be 0
    dist[startVertex] = 0.0;

    // 2. Build adjacency list from edge_weights for efficiency

    // Reference for map iteration: https://cplusplus.com/reference/map/map/begin/

    unordered_map<string, vector<pair<string, double>>> adj;
    for (unordered_map<string, double>::iterator edgeIt = edge_weights.begin(); edgeIt != edge_weights.end(); ++edgeIt) {
        string key = edgeIt->first;
        double weight = edgeIt->second;

        // we need to extract the two city names from the string key

        size_t comma_pos1 = key.find(",");
        size_t comma_pos2 = key.find(",", comma_pos1 + 2); // skip ", "
        string city1 = key.substr(0, comma_pos2);
        string city2 = key.substr(comma_pos2 + 2);

        // I forget to do that at first
        // but we should add both directions since this is an undirected graph
        
        adj[city1].push_back({city2, weight});
        adj[city2].push_back({city1, weight});
    }

    // 3. Instead of a priority queue (min-heap), 
    // we’ll use an unordered_map to track unvisited cities and manually find the smallest distance each time.

    unordered_map<string, bool> visited;
    for (auto& v : *vertexMap) {
        visited[v.first] = false;
    }

    // 4. Our Dijkstra’s main loop
    // Remember the classic version: 
    // We just repeatedly extract the nearest unvisited city and relax its edges.
    
    while (true) {
        // find unvisited city with smallest distance (O(V) scan)
        string u = "";
        double min_dist = numeric_limits<double>::infinity();

        for (auto& entry : dist) {
            if (!visited[entry.first] && entry.second < min_dist) {
                min_dist = entry.second;
                u = entry.first;
            }
        }

        // If we can’t find any unvisited node (or reached end), we stop
        if (u == "" || u == endVertex) break;

        visited[u] = true;

        // Reference from geekforseeks again: https://www.geeksforgeeks.org/dsa/dijkstras-shortest-path-algorithm-greedy-algo-7/ 
       
        // Use adjacency list for neighbors (O(1) per neighbor)

        //  We traverse through all neighbors of the current city

        for (size_t i = 0; i < adj[u].size(); i++) {
            string neighbor = adj[u][i].first;
            double weight = adj[u][i].second;

            if (visited[neighbor]) continue;

            double alt = dist[u] + weight;
            if (alt < dist[neighbor]) {
                dist[neighbor] = alt;
                prev[neighbor] = u;    // Don't forget to record the path.
            }
        }
    }


    // 5. Finally we can reconstruct THE SHORTEST path (like backtrack I think)

    // stores the fastest path

    vector<string> path;
    string current = endVertex;
    
    // backwards from end → start
    while (current != "") {
        path.push_back(current);
        current = prev[current];
    }

    // But we have to reverse the path to get correct order: start → end
    reverse(path.begin(), path.end());
    
    // Return all cities in the shortest path 
    return path; 
}
