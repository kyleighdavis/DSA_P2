#include <string>
#include "Bridges.h"
#include "DataSource.h"
#include "GraphAdjList.h"
#include <iostream>

using namespace std;
using namespace bridges;

int main() {
    try {

        // 


        Bridges bridges(0, "kyleighdavis", "753555741873");
        bridges.setTitle("A Simple Adjacency list based Graph Example");
        bridges.setDescription("Demonstrate how to create a graph with a few nodes and display it");

        GraphAdjList<string> graph;

        string kevin_bacon = "Kevin Bacon",
               denzel_washington = "Denzel Washington",
               morgan_freeman = "Morgan Freeman",
               tom_cruise = "Tom Cruise",
               angelina_jolie = "Angelina Jolie",
               amy_adams = "Amy Adams",
               brad_pitt = "Brad Pitt";

        graph.addVertex(kevin_bacon);
        graph.addVertex(denzel_washington);
        graph.addVertex(morgan_freeman);
        graph.addVertex(tom_cruise);
        graph.addVertex(angelina_jolie);
        graph.addVertex(amy_adams);
        graph.addVertex(brad_pitt);

        graph.addEdge(kevin_bacon, denzel_washington);
        graph.addEdge(kevin_bacon, morgan_freeman);
        graph.addEdge(kevin_bacon, angelina_jolie);
        graph.addEdge(amy_adams, angelina_jolie);
        graph.addEdge(tom_cruise, amy_adams);
        graph.addEdge(angelina_jolie, brad_pitt);
        graph.addEdge(brad_pitt, denzel_washington);

        bridges.setDataStructure(&graph);

        bridges.visualize();  // sends data to BRIDGES server

        return 0;
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
}
