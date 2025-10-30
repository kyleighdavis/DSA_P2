#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <string>
#include <vector>
#include <map>
#include "Bridges.h"
#include "GraphAdjList.h"

using namespace std;
using namespace bridges;

vector<string> dijkstra(bridges::GraphAdjList<string, double, double>& city_graph,
                        map<string, double>& edge_weights,
                        string startVertex,
                        string endVertex);


#endif
