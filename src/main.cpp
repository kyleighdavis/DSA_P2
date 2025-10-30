#include <string>
#include <iostream>
#include <cmath>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <vector>

#include <cctype>  // for upper/lowercase conversion
#include <limits>  // for numeric_limits


#include <chrono>           // for timing/ our clock maybe...

// the following 2 libraries should be working for Linux

#include <sys/resource.h>   
#include <sys/time.h>      

#include "Bridges.h"
#include "DataSource.h"
#include "data_src/City.h"
#include "GraphAdjList.h"
#include "dijkstra.h" 
using namespace bridges;
using namespace std;

double getDistance(double lat1, double lon1, double lat2, double lon2) {
    double rad = M_PI / 180.0; // M_PI is value for pi
    double latitude_distance = (lat2 - lat1) * rad;
    double longitude_distance = (lon2 - lon1) * rad;

    lat1 = lat1 * rad;
    lat2 = lat2 * rad;

    // some math formula stuff
    double half_chord = pow(sin(latitude_distance / 2), 2) + pow(sin(longitude_distance / 2), 2) * cos(lat1) * cos(lat2);
    double angular_distance = 2 * atan2(sqrt(half_chord), sqrt(1 - half_chord));

    double earthRad = 6371.0;
    return earthRad * angular_distance;
}




int main(int argc, char **argv) {

    // create bridges object
    Bridges bridges(1, "AriyaM", "211314957910");

    // set title
    bridges.setTitle("Accessing US City data");

    DataSource ds(&bridges);

    cout << "Retrieving a set of US cities" << endl;
    bridges.setDescription("A simple example of retrieving US city data from Bridges DataSource");


	//user input part.....

    string state;
    cout << "Enter state abbreviation: ";
    getline(cin, state);
    cout << endl;

	// 2. Ask for min/max population and number of cities
int minPop, maxPop, cityLimit;

while (true) {
    cout << "Enter minimum population (0 - 10000000): ";
    if (!(cin >> minPop) || minPop < 0 || minPop > 10000000) {
        cout << "Invalid input. Please enter a number between 0 and 10,000,000.\n";
        cin.clear();
        
        continue;
    }
    break;
}

while (true) {
    cout << "Enter maximum population (" << minPop << " - 10000000): ";
    if (!(cin >> maxPop) || maxPop < minPop || maxPop > 10000000) {
        cout << "Invalid input. Please enter a number between " << minPop << " and 10,000,000.\n";
        cin.clear();
        
        continue;
    }
    break;
}

while (true) {
    cout << "How many cities do you want to retrieve (1 - 500)? ";
    if (!(cin >> cityLimit) || cityLimit < 1 || cityLimit > 500) {
        cout << "Invalid input. Please enter a number between 1 and 500.\n";
        cin.clear();
        
        continue;
    }
    break;
}




// 3. Create city_params from user input
unordered_map<string, string> city_params{
    {"min_pop", to_string(minPop)},
    {"max_pop", to_string(maxPop)},
    {"state", state},
    {"limit", to_string(cityLimit)}
};

// 4. Now call getUSCities with this city_params

// given by bridges
    vector<City> us_cities = ds.getUSCities(city_params);
    

    for (int i = 0; i < us_cities.size(); i++) {
        cout << "\n" << us_cities[i].getCity() << "," << us_cities[i].getState() << ":"
             << " Population: " << us_cities[i].getPopulation()
             << ", Elevation: " << us_cities[i].getElevation()
             << ", Lat/Long: " << us_cities[i].getLatitude() << "," << us_cities[i].getLongitude();
    }

	cout << endl;
	cout << endl;

	cout << "These are all the cities that satisfy your input conditions in " << state 
	<< endl<< " (Population between " << minPop << " and " << maxPop 
     << ", showing up to " << cityLimit << " cities):\n";

    // check to make sure there are cities in that state
    bool found = false;
    for(int i = 0; i < us_cities.size(); i++) {
        string city_state = us_cities[i].getState();
        if (city_state == state) {
            found = true;
        }
    }

    if(!found) {
        cout << "No cities found in " << state << endl;
        return 0;
    } 
	

    GraphAdjList<string, double> city_graph;

    // format example to access edge_weights["Raleigh, NC, Charlotte, NC"];
    map<string, double> edge_weights;
    string startVertex;
    string endVertex;
    cout << endl;
    cout << "Now enter First City: ";
    getline(cin, startVertex);
    cout << "Enter Second City: ";
    getline(cin, endVertex);
    cout << endl;
	
	startVertex = startVertex + ", " + state;
	endVertex = endVertex + ", " + state;

    // adds all the cities as a vertex in the graph
    for(int i = 0; i < us_cities.size(); i++) {
        City c = us_cities[i];
        string city_id = c.getCity() + ", " + c.getState();
        city_graph.addVertex(city_id);
    }

    // math :(
    double minimum_latitude = 90.0;
    double maximum_latitude = -90.0;
    double minimum_longitude = 180.0;
    double maximum_longitude = -180.0;

    // used to get boundaries for the graph visualization
    for(int i = 0; i < us_cities.size(); i++) {
        double latitude = us_cities[i].getLatitude();
        double longitude = us_cities[i].getLongitude();
        if(latitude < minimum_latitude) {
            minimum_latitude = latitude;
        }
        if(latitude > maximum_latitude) {
            maximum_latitude = latitude;
        }
        if(longitude < minimum_longitude) {
            minimum_longitude = longitude;
        }
        if(longitude > maximum_longitude) {
            maximum_longitude = longitude;
        }
    }

    // used to spread out graph visualization and help with chunking
    double x_space = 2000.0 / (maximum_longitude - minimum_longitude);
    double y_space = 2000.0 / (maximum_latitude - minimum_latitude);

    for (int i = 0; i < us_cities.size(); i++) {
        string cityName = us_cities[i].getCity() + ", " + us_cities[i].getState();
        auto* vertex = city_graph.getVertex(cityName); //idk what the type is plz help this worked

        double x = (us_cities[i].getLongitude() - minimum_longitude) * x_space;
        double y = (us_cities[i].getLatitude() - minimum_latitude) * y_space;

        vertex->setLocation(x, y);
        vertex->getVisualizer()->setSize(8);
        vertex->getVisualizer()->setColor("grey");
    }

    // creating edges (connections between cities = # of neighbors)
    int neighbors = 10;
    for(int i = 0; i < us_cities.size(); i++) {
        string city1 = us_cities[i].getCity() + ", " + us_cities[i].getState();
        auto *vertex1 = city_graph.getVertex(city1);

        vector<pair<double, string>> distance_list;

        // adjusting the distance for 2d
        for(int j = 0; j < us_cities.size(); j++) {
            if(i == j) {
                continue;
            }

            string city2 = us_cities[j].getCity() + ", " + us_cities[j].getState();
            auto* vertex2 = city_graph.getVertex(city2);
            if(vertex2 == nullptr) {
                continue;
            }

            // pythagorean lol
            double x_distance = vertex1->getLocationX() - vertex2->getLocationX();
            double y_distance = vertex1->getLocationY() - vertex2->getLocationY();
            double c_distance = sqrt(x_distance * x_distance + y_distance * y_distance);

            distance_list.push_back({c_distance, city2});
        }

        sort(distance_list.begin(), distance_list.end());

        int count = 0;
        for(int k = 0; k < distance_list.size(); k++) {
            if(count >= neighbors) {
                break;
            }

            string city2 = distance_list[k].second;
            if(edge_weights.find(city1 + ", " + city2) != edge_weights.end()) {
                continue;
            }

            // get the index in the og list (us_cities)
            int city2_index;
            for(int n = 0; n < us_cities.size(); n++) {
                string nameCheck = us_cities[n].getCity() + ", " + us_cities[n].getState();
                if(nameCheck == city2) {
                    city2_index = n;
                    break;
                }
            }
            

            double dist = getDistance(us_cities[i].getLatitude(), us_cities[i].getLongitude(), us_cities[city2_index].getLatitude(), us_cities[city2_index].getLongitude());

            city_graph.addEdge(city1, city2, dist);
            city_graph.addEdge(city2, city1, dist);

            edge_weights[city1 + ", " + city2] = dist;
            edge_weights[city2 + ", " + city1] = dist;

            count++; // limits number of neighbors
        }
    }

    if(city_graph.getVertex(startVertex) == nullptr && city_graph.getVertex(endVertex) == nullptr) {
        cout << "Neither city found" << endl;
    } 
	else if(city_graph.getVertex(startVertex) == nullptr) {
        cout << "First city not found" << endl;
    } 
	else if(city_graph.getVertex(endVertex) == nullptr) {
        cout << "Second city not found" << endl;
    } 
	else {
        city_graph.getVertex(startVertex)->getVisualizer()->setColor("red");
        city_graph.getVertex(endVertex)->getVisualizer()->setColor("red");
        cout << "Showing fastest path from " << startVertex << " to " << endVertex << endl;
    }

	// this is where we use the "clock"

	// to compare 2 algorithms we measure runtime and memory usage 

	auto startTime = std::chrono::high_resolution_clock::now();

	vector<string> path = dijkstra(city_graph, edge_weights, startVertex, endVertex);

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	std::cout << "Dijkstra runtime: " << duration << " ms" << std::endl;

	// Memory usage (Linux/macOS)
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	std::cout << "Memory usage: " << usage.ru_maxrss << " KB" << std::endl;





	//temporary tester, get rid of later
	cout << "Shortest path: ";
	for(string city : path){
		cout << city << " -> ";
	}
	cout << "END" << endl;

	// Color path nodes red
	for (string city : path) {
		auto* v = city_graph.getVertex(city);
		if (v != nullptr)
			v->getVisualizer()->setColor("red");
	}



	// ok so now i want the user to know the Straight-line distance 
	// and our shortest path distance using dijkstra algorithm

	// Calculate total shortest path distance
	double shortestPathDistance = 0.0;
	for(int i = 0; i < path.size() - 1; i++) {
    string city1 = path[i];
    string city2 = path[i+1];
    shortestPathDistance += edge_weights[city1 + ", " + city2];
	}
	cout << "Shortest-path distance lol: " << shortestPathDistance << " km" << endl;

// Find indices of start and end city in us_cities
int startIndex = -1, endIndex = -1;
for(int i = 0; i < us_cities.size(); i++){
    string cityName = us_cities[i].getCity() + ", " + us_cities[i].getState();
    if(cityName == startVertex) startIndex = i;
    if(cityName == endVertex) endIndex = i;
}

// Straight-line distance
if(startIndex != -1 && endIndex != -1){
    double straightLine = getDistance(
        us_cities[startIndex].getLatitude(),
        us_cities[startIndex].getLongitude(),
        us_cities[endIndex].getLatitude(),
        us_cities[endIndex].getLongitude()
    );
    cout << "Straight-line distance yippee: " << straightLine << " km" << endl;
}




    // Re-add and highlight shortest path edges so they appear on top
	for (int i = 0; i < path.size() - 1; i++) {
    string city1 = path[i];
    string city2 = path[i + 1];
    double dist = edge_weights[city1 + ", " + city2];

    // re-add edge so it's drawn last (on top)
    city_graph.addEdge(city1, city2, dist);
    city_graph.addEdge(city2, city1, dist);

    auto* redEdge = city_graph.getLinkVisualizer(city1, city2);
    if (redEdge != nullptr) {
        redEdge->setColor("red");
        redEdge->setThickness(5);  // thicker than default blue edges
    }
}



    bridges.setDataStructure(&city_graph);
    bridges.visualize();

    return 0;
}