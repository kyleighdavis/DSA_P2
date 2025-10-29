#include <string>
#include <iostream>
#include <cmath>
#include <map>
#include <algorithm>
using namespace std;
#include <unordered_map>  
#include <vector>         


#include "Bridges.h"
#include "DataSource.h" 
#include "data_src/City.h"
#include "GraphAdjList.h"
#include "dijkstra.h"



using namespace bridges;

double getDistance(double lat1, double lon1, double lat2, double lon2){
	double rad = M_PI / 180.0;
	double latitude_distance = (lat2 - lat1) * rad;
	double longitude_distance = (lon2 - lon1) * rad;

	lat1 = lat1 * rad;
	lat2 = lat2 * rad;

	double half_chord = pow(sin(latitude_distance / 2), 2) + pow(sin(longitude_distance / 2), 2) * cos(lat1) * cos(lat2);
	double angular_distance = 2 * atan2(sqrt(half_chord), sqrt(1 - half_chord));

	double earthRad = 6371.0;
	return earthRad * angular_distance;
}

int main(int argc, char **argv) {

	// create bridges object
	Bridges bridges (1, "AriyaM", "211314957910");


	// set title
	bridges.setTitle("Accessing US City data");

	DataSource ds (&bridges);

	cout << "Retrieving a set of US cities" << endl;
    bridges.setDescription("A simple example of retrieving US city data from Bridges DataSource");

	string state;
	cout << "Enter state abbreviation: ";
	getline(cin, state);
	cout << endl;

	unordered_map<string, string> city_params {
			{"min_pop","500"},
			{"max_pop","1000000"},
			{"state", state},
			{"limit", "30"}
		};

	vector<City>  us_cities = ds.getUSCities(city_params);
	cout << "US Cities (tested for limit of 25 cities, population over 200K, and lat/long Bounding Box: (34.025348,-85.352783), (36.800488,-75.300293):\n";
	for (int i = 0; i < us_cities.size(); i++){
		cout << "\n" << us_cities[i].getCity() << "," << us_cities[i].getState() << ":" <<
		" Population: " <<  us_cities[i].getPopulation()  <<
		", Elevation: "  <<  us_cities[i].getElevation()
		<< ", Lat/Long: " << us_cities[i].getLatitude() << "," << us_cities[i].getLongitude();
	}

	bool found = false;
	for(int i = 0; i < us_cities.size(); i++){
		string city_state = us_cities[i].getState();
		if(city_state == state){
			found = true;
		}
	}

	if(!found){
		cout << "No cities found in " << state << endl;
		return 0;
	}
	else{
		cout << "Cities found in " << state << "!" << endl;
	}
    
   GraphAdjList<string, double> city_graph;
	
   //format example to access edge_weights["Raleigh, NC, Charlotte, NC"];

	map<string, double> edge_weights;
	string startVertex;
	string endVertex;
	cout << endl;
	cout << "Enter First City: ";
	getline(cin, startVertex);
	cout << "Enter Second City: ";
	getline(cin, endVertex);
	cout << endl;

	//Fix;
	// I added 2 lines to make sure 
	// it automatically add the state abbreviation (already entered above) ---
	
	startVertex = startVertex + ", " + state;
	endVertex = endVertex + ", " + state;

	for(int i = 0; i < us_cities.size(); i++){
		City c = us_cities[i];
		string city_id = c.getCity() + ", " + c.getState();
		city_graph.addVertex(city_id);
	}

	int k = 3;  // number of nearest neighbors
for(int i = 0; i < us_cities.size(); i++){
    vector<pair<double,int>> distances;  // distance, index
    for(int j = 0; j < us_cities.size(); j++){
        if(i == j) continue;
        double distance = getDistance(us_cities[i].getLatitude(), us_cities[i].getLongitude(),
                                      us_cities[j].getLatitude(), us_cities[j].getLongitude());
        distances.push_back({distance, j});
    }
    sort(distances.begin(), distances.end());  // sort by distance
    for(int n = 0; n < min(k, (int)distances.size()); n++){
        int j = distances[n].second;
        string city1 = us_cities[i].getCity() + ", " + us_cities[i].getState();
        string city2 = us_cities[j].getCity() + ", " + us_cities[j].getState();

        // Add only if edge not already added
        if(edge_weights.find(city1 + "," + city2) == edge_weights.end()){
            city_graph.addEdge(city1, city2, distances[n].first);
            city_graph.addEdge(city2, city1, distances[n].first);
            edge_weights[city1 + "," + city2] = distances[n].first;
            edge_weights[city2 + "," + city1] = distances[n].first;
        }
    }
}



	if(city_graph.getVertex(startVertex) == nullptr && city_graph.getVertex(endVertex) == nullptr){
		cout << "Neither city found" << endl;
	}
	else if(city_graph.getVertex(startVertex) == nullptr){
		cout << "First city not found" << endl;
	}
	else if(city_graph.getVertex(endVertex) == nullptr){
		cout << "Second city not found" << endl;
	}
	else{
		city_graph.getVertex(startVertex)->getVisualizer()->setColor("red");
		city_graph.getVertex(endVertex)->getVisualizer()->setColor("red");
		cout << "Showing fastest path from " << startVertex << " to " << endVertex << endl;
	}

    
	
    // Build adjacency list based on existing edges in city_graph
vector<vector<pair<int,double>>> adj(us_cities.size());
map<string,int> cityIndex;
for(int i = 0; i < us_cities.size(); i++) {
    string name = us_cities[i].getCity() + ", " + us_cities[i].getState();
    cityIndex[name] = i;
}

// Only add edges that exist in city_graph
for(auto &edgePair : edge_weights) {
    string cities = edgePair.first; // "City1, City2"
    double weight = edgePair.second;
    
    size_t commaPos = cities.find(",");
    string city1 = cities.substr(0, commaPos);
    string city2 = cities.substr(commaPos + 2); // skip ", "

    int u = cityIndex[city1];
    int v = cityIndex[city2];

    adj[u].push_back({v, weight});
    adj[v].push_back({u, weight});
}

// Run Dijkstra
vector<int> prev;
int src = cityIndex[startVertex];
int dest = cityIndex[endVertex];
vector<double> dist = dijkstra(us_cities.size(), adj, src, prev);

// Reconstruct shortest path and mark edges red
vector<int> path = reconstructPath(src, dest, prev);
if(!path.empty()) {
    for(size_t i = 0; i + 1 < path.size(); i++) {
        string uName = us_cities[path[i]].getCity() + ", " + us_cities[path[i]].getState();
        string vName = us_cities[path[i+1]].getCity() + ", " + us_cities[path[i+1]].getState();
        if(city_graph.getLinkVisualizer(uName, vName) != nullptr) {
            city_graph.getLinkVisualizer(uName, vName)->setColor("red");
            city_graph.getLinkVisualizer(vName, uName)->setColor("red");
        }
    }
    cout << "Shortest path marked in red!\n";
} else {
    cout << "No path found between " << startVertex << " and " << endVertex << endl;
}


    bridges.setDataStructure(&city_graph);
    bridges.visualize();
       

	return 0;
}

// #include "Bridges.h"
// #include "DataSource.h"
// #include "GraphAdjList.h"

// #include <iostream>
// #include <string>
// #include <unordered_map>
// #include <fstream>
// #include "data_src/OSMData.h"
// #include "data_src/OSMVertex.h"
// #include "data_src/OSMEdge.h"

// using namespace std;
// using namespace bridges;

// int main(int argc, char **argv) {

// 	// create Bridges object
// 	Bridges bridges (2, "AriyaM", "211314957910");

// 	DataSource ds(bridges);
// //	OSMData osm_data = ds.getOSMData("Charlotte, North Carolina");
// 	OSMData osm_data = ds.getOSMData(41.03133177632377, -98.02593749997456,
// 						42.008577297430456, -97.94531249997696);

// 	vector<OSMVertex> vertices = osm_data.getVertices();
// 	vector<OSMEdge> edges = osm_data.getEdges();

// 	double coords[2];
// 	cout << "Number of Vertices [Charlotte]:" << vertices.size() << endl;
// 	cout << "Number of Edges [Charlotte]:" << edges.size() << endl;
// 	cout << "edge length of edge " << edges[0].getEdgeLength() << endl;
// 	cout << "destination of edge " << 

// 	// get cartesian coordinate  location of first vertex
// 	osm_data.getVertices()[0].getCartesianCoords(coords);
// 	cout << "Location of first vertex [Cartesian Coord]: " <<  coords[0] << ","
// 		<< coords[1] << endl;
// 	return 0;
// }