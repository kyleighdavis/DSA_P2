#include "dijkstra.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <queue>
using namespace std;

vector<string> dijkstra(bridges::GraphAdjList<string, double, double>& city_graph,
                        map<string, double>& edge_weights,
                        string startVertex,
                        string endVertex)
 {

    map<string, double> weights;
    map<string, string> previous;
    vector<pair<double, string>> pq;

    auto vertices = city_graph.getVertices();
    for (auto it = vertices->begin(); it != vertices->end(); it++) {
        string city_name = it->first;
        weights[city_name] = 600; // "infinity"
        previous[city_name] = "";
    }

    weights[startVertex] = 0.0;
    pq.push_back({0.0, startVertex});

    while (!pq.empty()) {
        // find city with smallest distance
        int min_index = 0;
        for (int i = 1; i < pq.size(); i++) {
            if (pq[i].first < pq[min_index].first) {
                min_index = i;
            }
        }

        string current_city = pq[min_index].second;
        pq.erase(pq.begin() + min_index);

        if (current_city == endVertex) {
            break;
        }

        for (auto it = edge_weights.begin(); it != edge_weights.end(); it++) {
            string connecting_cities = it->first;
            double weight = it->second;

            // parse the key to get each city name
            int second_comma;
            int comma_count = 0;

            for (int i = 0; i < connecting_cities.length(); i++) {
                if (connecting_cities[i] == ',') {
                    comma_count++;
                    if (comma_count == 2) {
                        second_comma = i;
                        break;
                    }
                }
            }

            string city1 = connecting_cities.substr(0, second_comma);
            string city2 = connecting_cities.substr(second_comma + 2);

            string neighbor = "";
            if (city1 == current_city) {
                neighbor = city2;
            } else if (city2 == current_city) {
                neighbor = city1;
            } else {
                continue;
            }

            double total_distance = weights[current_city] + weight;
            if (total_distance < weights[neighbor]) {
                weights[neighbor] = total_distance;
                previous[neighbor] = current_city;
                pq.push_back({total_distance, neighbor});
            }
        }
    }

    // stores the fastest path
    vector<string> path;
    string current_city = endVertex;
    while (current_city != "") {
        path.push_back(current_city);
        current_city = previous[current_city];
    }

    reverse(path.begin(), path.end());
    return path;
}
