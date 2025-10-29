#include "dijkstra.h"
#include <queue>
#include <algorithm> 


vector<double> dijkstra(int V, const vector<vector<pair<int,double>>>& adj, int src, vector<int>& prev) {
    prev.assign(V, -1);                    // to store previous vertex on shortest path
    vector<double> dist(V, numeric_limits<double>::infinity());
    dist[src] = 0.0;

    // Min-heap: {distance, vertex}
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> pq;
    pq.push({0.0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;         // outdated entry

        for (auto [v, w] : adj[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

vector<int> reconstructPath(int src, int dest, const vector<int>& prev) {
    vector<int> path;
    for (int at = dest; at != -1; at = prev[at])
        path.push_back(at);
    reverse(path.begin(), path.end());
    if (!path.empty() && path[0] == src)
        return path;
    return {}; // no path found
}
