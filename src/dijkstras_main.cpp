#include "dijkstras.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <graph_file>\n";
        return 1;
    }

    // Load the graph from the file
    Graph G;
    try {
        file_to_graph(argv[1], G);
    } catch (const runtime_error& e) {
        cerr << "Error reading graph: " << e.what() << endl;
        return 1;
    }

    // Prepare for Dijkstra
    vector<int> previous(G.numVertices, -1);
    // Run Dijkstra from vertex 0 (as per the assignment spec)
    vector<int> dist = dijkstra_shortest_path(G, 0, previous);

    // Print out shortest paths from 0 to each vertex
    for (int v = 0; v < G.numVertices; v++) {
        // Extract the path from 0 to v
        vector<int> path = extract_shortest_path(dist, previous, v);
        print_path(path, dist[v]);
    }

    return 0;
}
