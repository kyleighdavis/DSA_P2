#include <string>
#include <iostream>
#include <cmath>
#include <map>
using namespace std;

#include "Bridges.h"
#include "DataSource.h" 
#include "data_src/City.h"
#include "GraphAdjList.h"


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

	
    // --- Input first state and get its cities ---
string state1;
cout << "Enter first state abbreviation (e.g., NC, CA, NY): ";
getline(cin, state1);

unordered_map<string, string> city_params1 {
    {"min_pop", "0"},
    {"max_pop", "20000000"},
    {"state", state1},
    {"limit", "600"}          // fetch small number for BRIDGES
};

vector<City> cities1 = ds.getUSCities(city_params1);
if (cities1.empty()) {
    cout << "No cities found in " << state1 << endl;
    return 0;
}

cout << "Cities found in " << state1 << ":" << endl;
for (const auto& c : cities1) {
    cout << c.getCity() << ", " << c.getState() << endl;
}

// --- Input second state and get its cities ---
string state2;
cout << "\nEnter second state abbreviation (e.g., NC, CA, NY): ";
getline(cin, state2);

unordered_map<string, string> city_params2 {
    {"min_pop", "0"},
    {"max_pop", "20000000"},
    {"state", state2},
    {"limit", "600"}
};

vector<City> cities2 = ds.getUSCities(city_params2);
if (cities2.empty()) {
    cout << "No cities found in " << state2 << endl;
    return 0;
}

cout << "Cities found in " << state2 << ":" << endl;
for (const auto& c : cities2) {
    cout << c.getCity() << ", " << c.getState() << endl;
}

// --- Ask user for one city from each state ---
string city1_input, city2_input;
string selectedCity1, selectedCity2;

// Validate first city
// Validate first city
while (true) {
    cout << "\nEnter the first city from " << state1 << ": ";
    getline(cin, city1_input);

    bool found = false;
    for (const auto& c : cities1) {
        if (c.getCity() == city1_input) {
            selectedCity1 = c.getCity() + ", " + c.getState();
            found = true;
            break;
        }
    }
    if (found) break;
    cout << "City not found in " << state1 << ". Please try again.\n";
}

// Validate second city
// Validate second city
while (true) {
    cout << "Enter the second city from " << state2 << ": ";
    getline(cin, city2_input);

    bool found = false;
    for (const auto& c : cities2) {
        if (c.getCity() == city2_input) {
            selectedCity2 = c.getCity() + ", " + c.getState();
            found = true;
            break;
        }
    }
    if (found) break;
    cout << "City not found in " << state2 << ". Please try again.\n";
}


// --- Combine both states' cities into a single vector for the graph ---
vector<City> all_cities = cities1;
all_cities.insert(all_cities.end(), cities2.begin(), cities2.end());

    GraphAdjList<string, double> city_graph;
	//format example to access edge_weights["Raleigh, NC, Charlotte, NC"];
	map<string, double> edge_weights;
	
	for(int i = 0; i < all_cities.size(); i++){
    City c = all_cities[i];
    string city_id = c.getCity() + ", " + c.getState();
    city_graph.addVertex(city_id);
}



for(int i = 0; i < all_cities.size(); i++){
    for(int j = i + 1; j < all_cities.size(); j++){
        double distance = getDistance(all_cities[i].getLatitude(), all_cities[i].getLongitude(),
                                      all_cities[j].getLatitude(), all_cities[j].getLongitude());
        string city1 = all_cities[i].getCity() + ", " + all_cities[i].getState();
        string city2 = all_cities[j].getCity() + ", " + all_cities[j].getState();
        city_graph.addEdge(city1, city2, distance);
        city_graph.addEdge(city2, city1, distance);

         

        edge_weights[city1 + "," + city2] = distance;
        edge_weights[city2 + "," + city1] = distance;

       
    }
}

// --- Add labels for all cities ---
for (int i = 0; i < all_cities.size(); i++) {
    string city_id = all_cities[i].getCity() + ", " + all_cities[i].getState();
    city_graph.getVertex(city_id)->setLabel(city_id); // show city label
}







	if(city_graph.getVertex(selectedCity1) == nullptr && city_graph.getVertex(selectedCity2) == nullptr){
    cout << "Neither city found" << endl;
}
else if(city_graph.getVertex(selectedCity1) == nullptr){
    cout << "First city not found" << endl;
}
else if(city_graph.getVertex(selectedCity2) == nullptr){
    cout << "Second city not found" << endl;
}
else{
    city_graph.getVertex(selectedCity1)->getVisualizer()->setColor("red");
    city_graph.getVertex(selectedCity2)->getVisualizer()->setColor("red");
    cout << "Showing fastest path from " << selectedCity1 << " to " << selectedCity2 << endl;
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