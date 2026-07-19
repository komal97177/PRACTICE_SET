#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Node
{
public:
    int cell;
    int dist;

    Node(int c, int d)
    {
        cell = c;
        dist = d;
    }
};

int minDiceThrows(int N, vector<pair<int, int>> ladders,
                  vector<pair<int, int>> snakes)
{
    // board[i] = destination after snake/ladder
    vector<int> board(N + 1, 0);

    // Add ladders
    for (auto x : ladders)
    {
        board[x.first] = x.second - x.first;
    }

    // Add snakes
    for (auto x : snakes)
    {
        board[x.first] = x.second - x.first;
    }

    vector<bool> visited(N + 1, false);
    vector<int> parent(N + 1, -1);

    queue<Node> q;

    q.push(Node(1, 0));
    visited[1] = true;

    while (!q.empty())
    {
        Node curr = q.front();
        q.pop();

        int cell = curr.cell;

        if (cell == N)
        {
            cout << "Minimum Dice Throws = " << curr.dist << endl;

            // Print Path
            vector<int> path;
            while (cell != -1)
            {
                path.push_back(cell);
                cell = parent[cell];
            }

            cout << "Path : ";
            for (int i = path.size() - 1; i >= 0; i--)
            {
                cout << path[i];
                if (i != 0)
                    cout << " -> ";
            }
            cout << endl;

            return curr.dist;
        }

        // Try dice values 1 to 6
        for (int dice = 1; dice <= 6; dice++)
        {
            int next = cell + dice;

            if (next <= N)
            {
                next = next + board[next];

                if (!visited[next])
                {
                    visited[next] = true;
                    parent[next] = cell;
                    q.push(Node(next, curr.dist + 1));
                }
            }
        }
    }

    return -1;
}

int main()
{
    int N = 36;

    vector<pair<int, int>> ladders = {
        {2, 15},
        {5, 7},
        {9, 27},
        {18, 29},
        {25, 35}
    };

    vector<pair<int, int>> snakes = {
        {17, 4},
        {20, 6},
        {24, 16},
        {32, 30},
        {34, 12}
    };

    minDiceThrows(N, ladders, snakes);

    return 0;
}