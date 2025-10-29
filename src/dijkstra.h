#include <vector>
#include <string>
#include "GraphAdjList.h"

std::vector<std::string> dijkstra(const GraphAdjList<std::string, double>& graph, 
                                  const std::string& start, 
                                  const std::string& end);
