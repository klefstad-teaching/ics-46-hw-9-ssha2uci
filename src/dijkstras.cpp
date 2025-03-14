#include "dijkstras.h"
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

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

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        int u = current.vertex;
        if (visited[u]) {
            continue;
        }
        visited[u] = true;

        for (auto& edge : G[u]) {
            int v = edge.dst;
            int w = edge.weight;

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
    // Print the vertices in the path
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i] << " ";
    }
    cout << endl;
    cout << "Total cost is " << total << endl;
}
