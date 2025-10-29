#include "GraphUtils.h"
#include "GraphAdjList.h"
#include <cstdlib>
#include <vector>

using namespace bridges;
using namespace std;

void setRandomNodeLocations(bridges::datastructure::GraphAdjList<string>& graph,
                            const vector<string>& node_ids,
                            int spread) {
    for (auto& id : node_ids) {
        auto v = graph.getVertex(id);
        double x = ((double) rand() / RAND_MAX) * spread - spread / 2;
        double y = ((double) rand() / RAND_MAX) * spread - spread / 2;
        v->getVisualizer()->setLocation(x, y);
    }
}
