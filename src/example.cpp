#include <string>
#include <iostream>
using namespace std;

#include "Bridges.h"
#include "DataSource.h"
#include "data_src/City.h"
#include "GraphAdjList.h"

using namespace bridges;

// this program illustrates how to access the data of the US and World 
//	city data
int main(int argc, char **argv) {

	// create bridges object
	Bridges bridges (1, "AriyaM", "211314957910");


	// set title
	bridges.setTitle("Accessing US City data");

	DataSource ds (&bridges);

	cout << "Retrieving a set of US cities" << endl;
    bridges.setDescription("A simple example of retrieving US city data from Bridges DataSource");
	// Parameters to the query are through an unordered map
	// you may use any subset of parameters to filter the US city data
	// results will be filtered to satisfy all specified parameters
	// for example if you provide population ane elevation ranges, then only those
	// cities matching those ranges will be retrieved

	// Parameters include
	// population range - specify  min and max population values
	// elevation range - specify  min and max elevation values - note elevation can be
	//   				negative (below sealevel!
	// Lat/Long bounding box -- specified by minLatLong, maxLatLong pairs of values
	// state  - state name -- cities within that state will be retrieved
	// city   - city name  -- if it matches, it will be retrieved
	// limit  - limit the output to a specified number of cities


	// return upto 10 cities in the North Carolina, using the 
	// population, and limit parametes

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
	cin >> input;

    string fayetteville_id = "Fayetteville, NC";
    string raleigh_id = "Raleigh, NC";
    string charlotte_id = "Charlotte, NC";
    city_graph.addVertex(fayetteville_id);
    city_graph.addVertex(raleigh_id);
    city_graph.addVertex(charlotte_id);
	city_graph.addVertex(input);
    city_graph.addEdge(fayetteville_id, raleigh_id);
    city_graph.addEdge(raleigh_id, charlotte_id);
    city_graph.addEdge(charlotte_id, fayetteville_id);
	city_graph.addEdge(input, fayetteville_id);
    bridges.setDataStructure(&city_graph);
    bridges.visualize();
       

	return 0;
}