#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <string>
#include <vector>
#include <map>
#include "Bridges.h"
#include "GraphAdjList.h"

using namespace std;
using namespace bridges;

// Function declaration for Dijkstra shortest path
// city_graph: the graph
// edge_weights: map storing all edge distances between cities
// startVertex, endVertex: the start and destination city names
vector<string> dijkstra(GraphAdjList<string, double, double>& city_graph,
                        map<string, double>& edge_weights,
                        string startVertex,
                        string endVertex);

#endif // DIJKSTRA_H
