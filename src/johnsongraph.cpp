#include <string>

using namespace std;

#include "Bridges.h"
#include "DataSource.h"
#include "data_src/City.h"
#include "GraphAdjList.h"
#include <cstdlib>
#include <ctime>
#include "GraphUtils.h"
#include <algorithm>
#include <cctype>


#include <iostream>
#include <vector>
#include <unordered_map>



using namespace bridges;
using namespace std;


// Normalize function (convert to lowercase and remove spaces)
string normalize(const string& s) {
    string result;
    for (char c : s)
        if (!isspace(c)) result += tolower(c);
    return result;
}

// this program illustrates how to access the data of the US and World 
//	city data
int main(int argc, char **argv) {
	srand(time(NULL));
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
			{"max_pop","200000"},
			
			{"limit", "666"}
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

	for (auto& c : us_cities) {
    string id = c.getCity() + ", " + c.getState();
    city_graph.addVertex(id);
	city_ids.push_back(id);

    // Optional: highlight big cities
    // <-- Insert here
	//setRandomNodeLocations(city_graph, city_ids, 2000);  // Spread nodes so they don't all stack in center
}
	// Ask user for two cities and highlight them if they exist
string city1, city2;
bool valid = false;
while (!valid) {
    cout << "\nEnter the first city: ";
    getline(cin, city1);
    cout << "Enter the second city: ";
    getline(cin, city2);

    string found1_id = "";
    string found2_id = "";

    // Search for the full vertex ID by city name
    for (const auto& id : city_ids) {
        if (id.find(city1) != string::npos) {
            found1_id = id;
        }
        if (id.find(city2) != string::npos) {
            found2_id = id;
        }
    }

    if (!found1_id.empty() && !found2_id.empty()) {
        city_graph.getVertex(found1_id)->getVisualizer()->setColor("red");
        city_graph.getVertex(found2_id)->getVisualizer()->setColor("red");
        valid = true;
    } else {
        cout << "One or both cities were not found in the dataset. Please try again.\n";
    }
}

	// setRandomNodeLocations(city_graph, city_ids,200);  // 2000 is the spread range, you can adjust as needed

    

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