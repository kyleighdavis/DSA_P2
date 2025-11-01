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

// The following 2 libraries should be working for Linux

#include <sys/resource.h>   
#include <sys/time.h>      

#include "Bridges.h"
#include "DataSource.h"
#include "data_src/City.h"
#include "GraphAdjList.h"
#include "dijkstra.h" 
using namespace bridges;
using namespace std;

//reference: https://www.geeksforgeeks.org/dsa/program-distance-two-points-earth/
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



//anything bridges related was gathered from: https://bridgesuncc.github.io/doc/cxx-api/current/html/classbridges_1_1dataset_1_1_o_s_m_data.html
//                                            https://bridgesuncc.github.io/tutorials/Data_OSM.html
//                                            https://bridgesuncc.github.io/tutorials/Graph.html
int main(int argc, char **argv) {

    // create bridges object
    Bridges bridges(1, "AriyaM", "211314957910");

    // set title
    bridges.setTitle("Accessing US City data");

    DataSource ds(&bridges);

    cout << "Retrieving a set of US cities" << endl;
    bridges.setDescription("A simple example of retrieving US city data from Bridges DataSource");


	//user input part.....

    //1. Ask for state abbreviation (case insensitive)

    string state;
    set<string> validStates = {
    "AL","AK","AZ","AR","CA","CO","CT","DE","FL","GA","HI","ID","IL","IN","IA","KS",
    "KY","LA","ME","MD","MA","MI","MN","MS","MO","MT","NE","NV","NH","NJ","NM","NY",
    "NC","ND","OH","OK","OR","PA","RI","SC","SD","TN","TX","UT","VT","VA","WA","WV","WI","WY"
    };

    while (true) {
        cout << "Enter state abbreviation (e.g., FL, TX. Lowercase is fine too!): ";
        getline(cin, state);

        // convert input to uppercase
        
        for (char& c : state) {
            c = toupper(c);
        }

        if (validStates.find(state) != validStates.end()) {
            break;  // valid state, exit loop
        } else {
            cout << "Invalid state abbreviation. Please try again.\n";
        }
    }

	// 2. Ask for min/max population and number of cities
    int min_pop;
    int max_pop;
    int city_limit;

    while (true) {
        string temp;
        cout << "Enter minimum population (0 - 10000000): ";
        getline(cin, temp);
        min_pop = stoi(temp);
        if (min_pop < 0 || min_pop > 10000000) {
            cout << "Invalid input. Please enter a number between 0 and 10 million.\n";
            
            continue;
        }
        break;
    }

    while (true) {
        string temp;
        cout << "Enter maximum population (" << min_pop << " - 10000000): ";
        getline(cin, temp);
        max_pop = stoi(temp);
        if (max_pop < min_pop || max_pop > 10000000) {
            cout << "Invalid input. Please enter a number between " << min_pop << " and 10,000,000.\n";
            
            continue;
        }
        break;
    }

    while (true) {
        string temp;
        cout << "How many cities do you want to retrieve (1 - 1000)? ";
        getline(cin, temp);
        city_limit = stoi(temp);
        if (city_limit < 1 || city_limit > 1000) {
            cout << "Invalid input. Please enter a number between 1 and 1000.\n";
            
            continue;
        }
        break;
    }

    // creating edges (connections between cities = # of neighbors)
    int neighbors;

    // 3. Ask for the neigbor cities (let the user decide):

    // creating edges (connections between cities = # of neighbors)
    while (true) {
        string temp;
        cout << "Enter the maximum number of neighboring cities you want to connect for each city (0-10): ";
        getline(cin, temp);
        neighbors = stoi(temp);
        if (neighbors >= 0 && neighbors <= 10) {
            break;
        } else {
            cout << "Invalid input. Please enter an integer between 0 and 10." << endl;
        }
    }



    // 4. generate city_params from user input
    unordered_map<string, string> city_params{
        {"min_pop", to_string(min_pop)},
        {"max_pop", to_string(max_pop)},
        {"state", state},
        {"limit", to_string(city_limit)}
    };

    // 4. Now call getUSCities with this city_params

    // This is given by bridges
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
	<< endl<< " (Population between " << min_pop << " and " << max_pop 
     << ", showing up to " << city_limit << " cities):\n";

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

    //reference: https://learn.microsoft.com/en-us/answers/questions/883272/find-max-min-latitude-and-longitude-from-coordinat
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
    //reference: https://stackoverflow.com/questions/4953150/convert-lat-longs-to-x-y-co-ordinates
    double x_space = 2000.0 / (maximum_longitude - minimum_longitude);
    double y_space = 2000.0 / (maximum_latitude - minimum_latitude);

    for (int i = 0; i < us_cities.size(); i++) {
        string cityName = us_cities[i].getCity() + ", " + us_cities[i].getState();
        auto* vertex = city_graph.getVertex(cityName); //idk what the type is plz help this worked

        //reference: https://stackoverflow.com/questions/4953150/convert-lat-longs-to-x-y-co-ordinates.%E2%80%9D
        double x = (us_cities[i].getLongitude() - minimum_longitude) * x_space;
        double y = (us_cities[i].getLatitude() - minimum_latitude) * y_space;

        vertex->setLocation(x, y);
        vertex->getVisualizer()->setSize(8);
        vertex->getVisualizer()->setColor("grey");
    }


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

            //need to do this if we use getDistance it WILL CRASH (getDistance is O(1) but math functions take a LONG time)
            //reference: https://www.geeksforgeeks.org/dsa/program-calculate-distance-two-points/
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

	

    auto startTime = std::chrono::high_resolution_clock::now();
    vector<string> path = dijkstra(city_graph, edge_weights, startVertex, endVertex);
    auto endTime = std::chrono::high_resolution_clock::now();


	//temporary tester, get rid of later
	cout << "Shortest path: ";
	for(string city : path){
		cout << city << " -> ";
	}
	cout << "END" << endl;
    cout << endl;

	// Color path nodes red
	for (string city : path) {
		auto* v = city_graph.getVertex(city);
		if (v != nullptr)
			v->getVisualizer()->setColor("red");
	}



	// Ok so now we want the user to know both the Straight-line distance 
	// and our shortest path distance using dijkstra algorithm and A*

	// Calculate  shortest path distance using dijkstra algorithm

	double shortestPathDistance = 0.0;
	for(int i = 0; i < path.size() - 1; i++) {
        string city1 = path[i];
        string city2 = path[i+1];
        shortestPathDistance += edge_weights[city1 + ", " + city2];
    }
	cout << "Shortest-path distance using dijkstra algorithm: " 
    << shortestPathDistance << " km" << endl;

    // Find indices of start and end city in us_cities
    int startIndex = -1, endIndex = -1;
    for(int i = 0; i < us_cities.size(); i++){
        string cityName = us_cities[i].getCity() + ", " + us_cities[i].getState();
        if(cityName == startVertex) startIndex = i;
        if(cityName == endVertex) endIndex = i;
    }

    // Straight-line distance from getdistance function
    if(startIndex != -1 && endIndex != -1){
        double straightLine = getDistance(
            us_cities[startIndex].getLatitude(),
            us_cities[startIndex].getLongitude(),
            us_cities[endIndex].getLatitude(),
            us_cities[endIndex].getLongitude()
        );
        cout << "Straight-line distance yippee: " << straightLine << " km" << endl;
        cout << endl;
    }

    // This is where we use the "clock"

	// We compare the 2 algorithms in terms of runtime and memory usage 

    // Both metrics have C++ header file provided already.

    // Runtime first.

	

	

	
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	std::cout << "Dijkstra runtime: " << duration << " ms" << std::endl;

	// Memory usage (Linux/macOS)

	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	std::cout << "Memory usage: " << usage.ru_maxrss << " KB" << std::endl;
    cout<<endl;


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