#include "dijkstras.h"
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

// A small helper struct for the priority queue
// storing (vertex, distance) and comparing by distance.
struct Node {
    int vertex;
    int dist;
    Node(int v, int d) : vertex(v), dist(d) {}
    // For a min-heap based on dist
    bool operator>(const Node& other) const {
        return dist > other.dist;
    }
};

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous)
{
    int n = G.numVertices;
    vector<int> dist(n, INF);
    vector<bool> visited(n, false);

    // Initialize the source distance and previous
    dist[source] = 0;
    previous.assign(n, -1);

    // Min-heap (priority queue) storing (distance, vertex)
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push(Node(source, 0));

    // Core Dijkstra loop
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        int u = current.vertex;
        if (visited[u]) {
            // Already finalized the distance to u
            continue;
        }
        visited[u] = true;

        // Relaxation step for each neighbor of u
        for (auto& edge : G[u]) {
            int v = edge.dst;
            int w = edge.weight;

            // If we can improve the distance to v through u
            if (!visited[v] && dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                previous[v] = u;
                pq.push(Node(v, dist[v]));
            }
        }
    }
    return dist;
}

vector<int> extract_shortest_path(const vector<int>& distances,
                                  const vector<int>& previous,
                                  int destination)
{
    // If distance is INF or out-of-bounds, no valid path
    if (destination < 0 || destination >= (int)distances.size() ||
        distances[destination] == INF) {
        return {}; // empty path
    }

    // Reconstruct path by walking backward through 'previous'
    stack<int> s;
    for (int v = destination; v != -1; v = previous[v]) {
        s.push(v);
    }

    // Now pop off the stack into a vector to get the path in correct order
    vector<int> path;
    while (!s.empty()) {
        path.push_back(s.top());
        s.pop();
    }
    return path;
}

void print_path(const vector<int>& path, int total)
{
    if (path.empty()) {
        // If you want to show a "no path" message, do it here:
        // cout << "No path found." << endl;
        return;
    }
    // Print the vertices in the path
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i + 1 < path.size()) {
            cout << " ";
        }
    }
    cout << endl;
    cout << "Total cost is " << total << endl;
}
