#include <vector>
#include <utility>
#include <limits>
#include <queue>
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

vector<int> reconstructPath(int from, int to, const vector<int>& prev) {
    vector<int> path;
    for (int at = to; at != -1; at = prev[at])
        path.push_back(at);
    reverse(path.begin(), path.end());
    if (!path.empty() && path[0] == from)
        return path;
    return {}; // no path found
}

double findHeuristic(pair<float, float> tocord, pair<float, float> fromcord){
    double latDistance = (tocord.first - fromcord.first);
    double lonDistance = (tocord.second - fromcord.second);
    return sqrt((latDistance * latDistance) + (lonDistance * lonDistance));
}

vector<double> aStar(int numofvert, const vector<vector<pair<int,double>>>& adj, int from, vector<int>& prev){
    prev.assign(numofvert, -1);
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> visited;
    visited.push({0.0, from});
    vector<double> dist(numofvert, numeric_limits<double>::infinity());
    vector<double> known(numofvert, false)  ; //camefrom from wikipedia
    dist[from] = 0.0;
    vector<double> estimated(numofvert, numeric_limits<double>::infinity());
    estimated[from] = findHeuristic({0,0}, {0,0}); 

    while (!visited.empty()){
        
}