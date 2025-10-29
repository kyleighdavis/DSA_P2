#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <utility> // for std::pair
#include <limits>  // for infinity

// Computes shortest distances from src to all vertices
// V: number of vertices
// adj: adjacency list (vector of vector of {neighbor, weight})
// src: starting vertex
// prev: stores the previous vertex on the shortest path
std::vector<double> dijkstra(int V, const std::vector<std::vector<std::pair<int,double>>>& adj, int src, std::vector<int>& prev);

// Reconstructs the shortest path from src to dest using prev array
std::vector<int> reconstructPath(int src, int dest, const std::vector<int>& prev);

#endif
