#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void bfsSSSP(vector<int> graph[], int V, int source)
{
    vector<int> dist(V, -1);   // -1 means unvisited
    queue<int> q;

    dist[source] = 0;
    q.push(source);

    while (!q.empty())
    {
        int node = q.front();
        q.pop();

        // Visit all neighbours
        for (int neighbour : graph[node])
        {
            if (dist[neighbour] == -1)
            {
                dist[neighbour] = dist[node] + 1;
                q.push(neighbour);
            }
        }
    }

    cout << "Shortest Distance from Source " << source << ":\n";
    for (int i = 0; i < V; i++)
    {
        cout << "Vertex " << i << " -> " << dist[i] << endl;
    }
}

int main()
{
    int V = 9;
    vector<int> graph[V];

    // Undirected Graph
    graph[0].push_back(1);
    graph[0].push_back(2);

    graph[1].push_back(0);
    graph[1].push_back(3);
    graph[1].push_back(4);

    graph[2].push_back(0);
    graph[2].push_back(5);

    graph[3].push_back(1);

    graph[4].push_back(1);
    graph[4].push_back(6);
    graph[4].push_back(7);

    graph[5].push_back(2);

    graph[6].push_back(4);

    graph[7].push_back(4);
    graph[7].push_back(8);

    graph[8].push_back(7);

    int source = 0;

    bfsSSSP(graph, V, source);

    return 0;
}