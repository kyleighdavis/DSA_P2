#include <string>

using namespace std;

#include "Bridges.h"
#include "DataSource.h"
#include "data_src/City.h"
#include "GraphAdjList.h"
#include <cstdlib>
#include <ctime>

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
			{"min_pop","0"},
			{"max_pop","1000000"},
			
			{"limit", "100"}
		};

	vector<City>  us_cities = ds.getUSCities(city_params);
	cout << "US Cities (tested for limit of 25 cities, population over 200K, and lat/long Bounding Box: (34.025348,-85.352783), (36.800488,-75.300293):\n";
	for (auto c : us_cities)
		cout << "\n" << c.getCity() << "," << c.getState() << ":" <<
			" Population: " <<  c.getPopulation()  <<
			", Elevation: "  <<  c.getElevation()
			<< ", Lat/Long: " << c.getLatitude() << "," << c.getLongitude();

    
    GraphAdjList<string> city_graph;
	vector<string> city_ids;

	for (const auto& c : us_cities) {
    string city_id = c.getCity() + ", " + c.getState();
    city_graph.addVertex(city_id);
    city_ids.push_back(city_id);
}
    srand(time(NULL));

int neighbors = 3;  // number of neighbors per city

for (size_t i = 0; i < city_ids.size(); ++i) {
    for (int j = 0; j < neighbors; ++j) {
        int rand_idx = rand() % city_ids.size();
        if (rand_idx != i) {
            city_graph.addEdge(city_ids[i], city_ids[rand_idx]);
        }
    }
}



	
    bridges.setDataStructure(&city_graph);
    bridges.visualize();
       

	return 0;
}