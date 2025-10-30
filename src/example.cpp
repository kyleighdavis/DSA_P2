#include <string>
#include <iostream>
#include <cmath>
#include <map>
#include <algorithm>
using namespace std;

#include "Bridges.h"
#include "DataSource.h"
#include "data_src/City.h"
#include "GraphAdjList.h"

using namespace bridges;

double getDistance(double lat1, double lon1, double lat2, double lon2) {
    double rad = M_PI / 180.0; // M_PI is value for pi
    double latitude_distance = (lat2 - lat1) * rad;
    double longitude_distance = (lon2 - lon1) * rad;

    lat1 = lat1 * rad;
    lat2 = lat2 * rad;

    // some math formula stuff
    double half_chord = pow(sin(latitude_distance / 2), 2) +
                        pow(sin(longitude_distance / 2), 2) * cos(lat1) * cos(lat2);
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

    string state;
    cout << "Enter state abbreviation: ";
    getline(cin, state);
    cout << endl;

    unordered_map<string, string> city_params{
        {"min_pop", "0"},
        {"max_pop", "10000000000"},
        {"state", state},
        {"limit", "1000"}};

    // given by bridges
    vector<City> us_cities = ds.getUSCities(city_params);
    cout << "US Cities (tested for limit of 25 cities, population over 200K, and lat/long Bounding Box: "
            "(34.025348,-85.352783), (36.800488,-75.300293):\n";
    for (int i = 0; i < us_cities.size(); i++) {
        cout << "\n" << us_cities[i].getCity() << "," << us_cities[i].getState() << ":"
             << " Population: " << us_cities[i].getPopulation()
             << ", Elevation: " << us_cities[i].getElevation()
             << ", Lat/Long: " << us_cities[i].getLatitude() << "," << us_cities[i].getLongitude();
    }

    // check to make sure there are cities in that state
    bool found = false;
    for (int i = 0; i < us_cities.size(); i++) {
        string city_state = us_cities[i].getState();
        if (city_state == state) {
            found = true;
        }
    }

    if (!found) {
        cout << "No cities found in " << state << endl;
        return 0;
    } else {
        cout << "Cities found in " << state << "!" << endl;
    }

    GraphAdjList<string, double> city_graph;
    // format example to access edge_weights["Raleigh, NC, Charlotte, NC"];
    map<string, double> edge_weights;
    string startVertex;
    string endVertex;
    cout << endl;
    cout << "Enter First City: ";
    getline(cin, startVertex);
    cout << "Enter Second City: ";
    getline(cin, endVertex);
    cout << endl;

    // adds all the cities as a vertex in the graph
    for (int i = 0; i < us_cities.size(); i++) {
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
    for (int i = 0; i < us_cities.size(); i++) {
        double latitude = us_cities[i].getLatitude();
        double longitude = us_cities[i].getLongitude();
        if (latitude < minimum_latitude) {
            minimum_latitude = latitude;
        }
        if (latitude > maximum_latitude) {
            maximum_latitude = latitude;
        }
        if (longitude < minimum_longitude) {
            minimum_longitude = longitude;
        }
        if (longitude > maximum_longitude) {
            maximum_longitude = longitude;
        }
    }

    // used to spread out graph visualization and help with chunking
    double x_space = 2000.0 / (maximum_longitude - minimum_longitude);
    double y_space = 2000.0 / (maximum_latitude - minimum_latitude);

    for (int i = 0; i < us_cities.size(); i++) {
        string cityName = us_cities[i].getCity() + ", " + us_cities[i].getState();
        auto *v = city_graph.getVertex(cityName);

        double x = (us_cities[i].getLongitude() - minimum_longitude) * x_space;
        double y = (us_cities[i].getLatitude() - minimum_latitude) * y_space;

        v->setLocation(x, y);
        v->getVisualizer()->setSize(8);
        v->getVisualizer()->setColor("grey");
    }

    // creating edges (connections between cities = # of neighbors)
    int neighbors = 10;
    for (int i = 0; i < us_cities.size(); i++) {
        string city1 = us_cities[i].getCity() + ", " + us_cities[i].getState();
        auto *v1 = city_graph.getVertex(city1);

        vector<pair<double, string>> distance_list;

        // adjusting the distance for 2d
        for (int j = 0; j < us_cities.size(); j++) {
            if (i == j) {
                continue;
            }

            string city2 = us_cities[j].getCity() + ", " + us_cities[j].getState();
            auto *v2 = city_graph.getVertex(city2);
            if (v2 == nullptr) {
                continue;
            }

            // pythagorean lol
            double x_distance = v1->getLocationX() - v2->getLocationX();
            double y_distance = v1->getLocationY() - v2->getLocationY();
            double c_distance = sqrt(x_distance * x_distance + y_distance * y_distance);

            distance_list.push_back({c_distance, city2});
        }

        sort(distance_list.begin(), distance_list.end());

        int count = 0;
        for (int k = 0; k < distance_list.size(); k++) {
            if (count >= neighbors) {
                break;
            }

            string city2 = distance_list[k].second;
            if (edge_weights.find(city1 + ", " + city2) != edge_weights.end()) {
                continue;
            }

            // get the index in the og list (us_cities)
            int city2_index = -1;
            for (int n = 0; n < us_cities.size(); n++) {
                string nameCheck = us_cities[n].getCity() + ", " + us_cities[n].getState();
                if (nameCheck == city2) {
                    city2_index = n;
                    break;
                }
            }
            if (city2_index == -1) { // means it's not in list
                continue;
            }

            double dist = getDistance(us_cities[i].getLatitude(), us_cities[i].getLongitude(),
                                      us_cities[city2_index].getLatitude(), us_cities[city2_index].getLongitude());

            city_graph.addEdge(city1, city2, dist);
            city_graph.addEdge(city2, city1, dist);

            edge_weights[city1 + ", " + city2] = dist;
            edge_weights[city2 + ", " + city1] = dist;

            count++; // limits number of neighbors
        }
    }

    if (city_graph.getVertex(startVertex) == nullptr && city_graph.getVertex(endVertex) == nullptr) {
        cout << "Neither city found" << endl;
    } else if (city_graph.getVertex(startVertex) == nullptr) {
        cout << "First city not found" << endl;
    } else if (city_graph.getVertex(endVertex) == nullptr) {
        cout << "Second city not found" << endl;
    } else {
        city_graph.getVertex(startVertex)->getVisualizer()->setColor("red");
        city_graph.getVertex(endVertex)->getVisualizer()->setColor("red");
        cout << "Showing fastest path from " << startVertex << " to " << endVertex << endl;
    }

    bridges.setDataStructure(&city_graph);
    bridges.visualize();

    return 0;
}