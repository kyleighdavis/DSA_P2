#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include "GraphAdjList.h"
#include <vector>
#include <string>

namespace bridges {
namespace utils {

void setRandomNodeLocations(bridges::datastructure::GraphAdjList<std::string>& graph,
                            const std::vector<std::string>& node_ids,
                            int spread);

} // namespace utils
} // namespace bridges

#endif
