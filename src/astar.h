#ifndef ASTAR_H
#define ASTAR_H

#include <string>
#include <vector>
#include <map>
#include "Bridges.h"
#include "GraphAdjList.h"
#include "data_src/City.h"

using namespace std;
using namespace bridges;

// --- Function Prototypes ---
vector<string> aStar(
    GraphAdjList<string, double>& cityGraph,
    map<string, double>& edgeWeights,
    string startVertex,
    string endVertex,
    vector<bridges::dataset::City>& citiesForCords
);

#endif
