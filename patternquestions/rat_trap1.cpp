#include <iostream>
#include <vector>
using namespace std;

bool solveMaze(vector<vector<int>>& maze, int m, int n, int row, int col, vector<vector<bool>>& visited) {
    // Base case: reached destination
    if (row == m - 1 && col == n - 1) {
        return true;
    }
    
    // Mark current cell as visited
    visited[row][col] = true;
    
    // Try moving right
    if (col + 1 < n && maze[row][col + 1] == 0 && !visited[row][col + 1]) {
        if (solveMaze(maze, m, n, row, col + 1, visited)) {
            return true;
        }
    }
    
    // Try moving down
    if (row + 1 < m && maze[row + 1][col] == 0 && !visited[row + 1][col]) {
        if (solveMaze(maze, m, n, row + 1, col, visited)) {
            return true;
        }
    }
    
    // Backtrack
    visited[row][col] = false;
    return false;
}

// Time complexity: O(m * n) in the worst case — each cell is visited at most once.
// Space complexity: O(m * n) due to the visited array and recursion stack (worst-case depth O(m+n)).

int main() {
    int m = 4, n = 4;
    
    vector<vector<int>> maze = {
        {0, 0, 0, 0},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 1, 0, 0}
    };
    
    vector<vector<bool>> visited(m, vector<bool>(n, false));
    
    // Check if source cell is blocked
    if (maze[0][0] == 1) {
        cout << "No path exists (source is blocked)" << endl;
        return 0;
    }
    
    // Start solving from (0, 0)
    if (solveMaze(maze, m, n, 0, 0, visited)) {
        cout << "Path exists from source to destination" << endl;
    } else {
        cout << "No path exists" << endl;
    }
    
    return 0;
}


