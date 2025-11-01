#include "astar.h"
#include <cmath>
#include <map>
#include <limits>
#include <queue>
#include <algorithm>
#include <utility>
#include <string>

using namespace std;
using namespace bridges;

// Forward declaration for the getDistance function defined in main.cpp
double getDistance(double lat1, double lon1, double lat2, double lon2);

// --- A* Algorithm Implementation ---
vector<string> aStar(GraphAdjList<string, double>& cityGraph,
                     map<string, double>& edgeWeights,
                     string startVertex,
                     string endVertex,
                     vector<bridges::dataset::City>& citiesForCords) {
    
    // Priority queue (min-heap) for nodes to explore
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> openSet;

    map<string, double> gScore;          // Distance from start
    map<string, double> fScore;          // Estimated total cost
    map<string, string> prev;            // For reconstructing path
    map<string, pair<double, double>> coords; // Stores lat/lon

    // Store coordinates for quick lookup
    for (auto& city : citiesForCords) {
        string name = city.getCity() + ", " + city.getState();
        coords[name] = {city.getLatitude(), city.getLongitude()};
    }

    // Initialize all scores to infinity
    for (auto& vertex : *cityGraph.getVertices()) {
        gScore[vertex.first] = numeric_limits<double>::infinity();
        fScore[vertex.first] = numeric_limits<double>::infinity();
    }

    gScore[startVertex] = 0.0;
    fScore[startVertex] = getDistance(
        coords[startVertex].first, coords[startVertex].second,
        coords[endVertex].first, coords[endVertex].second
    );

    openSet.push({fScore[startVertex], startVertex});

    while (!openSet.empty()) {
        auto [currentF, current] = openSet.top();
        openSet.pop();

        //if the path is longer than the previous path skip
        if (currentF > fScore[current]) continue;

        if (current == endVertex)
            break;

        // 
        for (auto edge : cityGraph.outgoingEdgeSetOf(current)) {
            string neighbor = edge.to();
            double weight = edge.getEdgeData();
            double tentative = gScore[current] + weight;

            if (tentative < gScore[neighbor]) {
                prev[neighbor] = current;
                gScore[neighbor] = tentative;
                fScore[neighbor] = gScore[neighbor] + getDistance(
                    coords[neighbor].first, coords[neighbor].second,
                    coords[endVertex].first, coords[endVertex].second
                );
                openSet.push({fScore[neighbor], neighbor});
            }
        }
    }

    //reconstruct path
    vector<string> path;
    string current = endVertex;
    while (prev.find(current) != prev.end()) {
        path.push_back(current);
        current = prev[current];
    }
    path.push_back(startVertex);
    reverse(path.begin(), path.end());

    return path;
}
