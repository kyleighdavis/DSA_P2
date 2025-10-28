#include <string>
#include <iostream>
using namespace std;

#include "Bridges.h"
#include "DataSource.h" 
#include "data_src/City.h"
#include "GraphAdjList.h"


using namespace bridges;

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
	for (auto c : us_cities)
		cout << "\n" << c.getCity() << "," << c.getState() << ":" <<
			" Population: " <<  c.getPopulation()  <<
			", Elevation: "  <<  c.getElevation()
			<< ", Lat/Long: " << c.getLatitude() << "," << c.getLongitude();

    
    GraphAdjList<string> city_graph;
	string input;
	cout << "Enter us City";
	getline(cin, input);

    string fayetteville_id = "Fayetteville, NC";
    string raleigh_id = "Raleigh, NC";
    string charlotte_id = "Charlotte, NC";
    city_graph.addVertex(fayetteville_id);
    city_graph.addVertex(raleigh_id);
    city_graph.addVertex(charlotte_id);
	//city_graph.addVertex(input);
    city_graph.addEdge(fayetteville_id, raleigh_id);
    city_graph.addEdge(raleigh_id, charlotte_id);
    city_graph.addEdge(charlotte_id, fayetteville_id);
	//city_graph.addEdge(input, fayetteville_id);

	if(city_graph.getVertex(input) != nullptr){
		city_graph.getVertex(input)->getVisualizer()->setColor("red");
		cout << "Selected City: " << input << endl;
	}
	else{
		cout << "City not found" << endl;
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
