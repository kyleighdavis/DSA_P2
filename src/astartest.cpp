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
 // honestly for our purposes i think they should be the same but idk you tlel me later
double findDistance(pair<float, float> tocord, pair<float, float> fromcord){
    double latDistance = (tocord.first - fromcord.first);
    double lonDistance = (tocord.second - fromcord.second);
    return sqrt((latDistance * latDistance) + (lonDistance * lonDistance));
}

vector<double> aStar(int numofvert, const vector<vector<pair<int,double>>>& adj, int from, int to, vector<int>& prev){
    prev.assign(numofvert, -1);
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> toBeVisited; //openset from wikipedia
    toBeVisited.push({0.0, from});
    vector<double> dist(numofvert, numeric_limits<double>::infinity()); //gscore from wikipedia
    vector<int> known(numofvert, -1)  ; //camefrom from wikipedia
    dist[from] = 0.0;
    vector<double> estimatedShortest(numofvert, numeric_limits<double>::infinity()); //fscore from wikipedia
    estimatedShortest[from] = findHeuristic(coords[from], coords[to]); //when importing into the actual thing
    //i think we have to use coordinates to find heuristic of first node

    while (!toBeVisited.empty()){
        pair <double, int> lowestNode = toBeVisited.top(); //lowest node is current from wikipedia
        int current = lowestNode.second;
        if (current == to){
            return dist;
        }
        toBeVisited.pop();
        for (auto [neighbor, weight] : adj[current]){
            double potentialPath = dist[current] + weight; //in the program were gonna use like actual longitude and latitude of the current node and the neighbor
            if (potentialPath < dist[neighbor]){
                known[neighbor] = current;
                dist[neighbor] = potentialPath;
                estimatedShortest[neighbor] = dist[neighbor] + findHeuristic(coords[neighbor], coords[to]);
                toBeVisited.push({estimatedShortest[neighbor], neighbor});  
            }
        }
    } 
    return {};  
}