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

	unordered_map<string, string> city_params {
			{"min_pop","200000"},
			{"max_pop","1000000"},
			{"state", "NC"},
			{"limit", "25"}
		};

	vector<City>  us_cities = ds.getUSCities(city_params);
	cout << "US Cities (tested for limit of 25 cities, population over 200K, and lat/long Bounding Box: (34.025348,-85.352783), (36.800488,-75.300293):\n";
	for (int i = 0; i < us_cities.size(); i++){
		cout << "\n" << us_cities[i].getCity() << "," << us_cities[i].getState() << ":" <<
		" Population: " <<  us_cities[i].getPopulation()  <<
		", Elevation: "  <<  us_cities[i].getElevation()
		<< ", Lat/Long: " << us_cities[i].getLatitude() << "," << us_cities[i].getLongitude();
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

	for(int i = 0; i < us_cities.size(); i++){
		City c = us_cities[i];
		string city_id = c.getCity() + ", " + c.getState();
		city_graph.addVertex(city_id);
	}

	for(int i = 0; i < us_cities.size(); i++){
		for(int j = i + 1; j < us_cities.size(); j++){
			double distance = getDistance(us_cities[i].getLatitude(), us_cities[i].getLongitude(), us_cities[j].getLatitude(), us_cities[j].getLongitude());

			string city1 = us_cities[i].getCity() + ", " + us_cities[i].getState();
			string city2 = us_cities[j].getCity() + ", " + us_cities[j].getState();
			city_graph.addEdge(city1, city2, distance);
			city_graph.addEdge(city2, city1, distance);

			edge_weights[city1 + "," + city2] = distance;
			edge_weights[city2 + "," + city1] = distance;
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
