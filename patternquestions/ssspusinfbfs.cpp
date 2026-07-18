#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void bfsSSSP(vector<int> graph[], int V, int source)
{
    vector<int> dist(V, -1);      // Distance from source
    vector<int> parent(V, -1);    // Parent of each node
    queue<int> q;

    dist[source] = 0;
    parent[source] = -1;          // Source has no parent
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
                parent[neighbour] = node;
                q.push(neighbour);
            }
        }
    }

    // Print Distance and Parent
    cout << "Vertex\tDistance\tParent\n";
    for (int i = 0; i < V; i++)
    {
        cout << i << "\t" << dist[i] << "\t\t" << parent[i] << endl;
    }

    // Print shortest paths
    cout << "\nShortest Paths from Source " << source << ":\n";

    for (int i = 0; i < V; i++)
    {
        if (dist[i] == -1)
        {
            cout << "No path to Vertex " << i << endl;
            continue;
        }

        vector<int> path;
        int current = i;

        while (current != -1)
        {
            path.push_back(current);
            current = parent[current];
        }

        cout << "Path to " << i << ": ";
        for (int j = path.size() - 1; j >= 0; j--)
        {
            cout << path[j];
            if (j != 0)
                cout << " -> ";
        }

        cout << " (Distance = " << dist[i] << ")" << endl;
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