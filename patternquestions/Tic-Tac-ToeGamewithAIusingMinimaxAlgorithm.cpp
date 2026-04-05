#include <iostream>
#include <vector>
#include <limits>
using namespace std;

class TicTacToe {
private:
    vector<vector<char>> board;
    char currentPlayer;
    
    void initializeBoard() {
        board = vector<vector<char>>(3, vector<char>(3, ' '));
    }
    
    void displayBoard() {
        cout << "\nCurrent Board:\n";
        cout << "-------------\n";
        for (int i = 0; i < 3; i++) {
            cout << "| ";
            for (int j = 0; j < 3; j++) {
                cout << board[i][j] << " | ";
            }
            cout << "\n-------------\n";
        }
    }
    
    bool isMoveValid(int row, int col) {
        return row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ';
    }
    
    bool checkWin(char player) {
        // Check rows and columns
        for (int i = 0; i < 3; i++) {
            if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
                (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
                return true;
            }
        }
        
        // Check diagonals
        if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
            (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
            return true;
        }
        
        return false;
    }
    
    bool isBoardFull() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    return false;
                }
            }
        }
        return true;
    }
    
    int evaluateBoard() {
        if (checkWin('X')) return 10;
        if (checkWin('O')) return -10;
        return 0;
    }
    
    int minimax(int depth, bool isMaximizing) {
        int score = evaluateBoard();
        
        if (score == 10) return score - depth;
        if (score == -10) return score + depth;
        if (isBoardFull()) return 0;
        
        if (isMaximizing) {
            int bestScore = numeric_limits<int>::min();
            
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'X';
                        bestScore = max(bestScore, minimax(depth + 1, false));
                        board[i][j] = ' ';
                    }
                }
            }
            return bestScore;
        } else {
            int bestScore = numeric_limits<int>::max();
            
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'O';
                        bestScore = min(bestScore, minimax(depth + 1, true));
                        board[i][j] = ' ';
                    }
                }
            }
            return bestScore;
        }
    }
    
    void aiMove() {
        int bestScore = numeric_limits<int>::min();
        int bestRow = -1, bestCol = -1;
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    int score = minimax(0, false);
                    board[i][j] = ' ';
                    
                    if (score > bestScore) {
                        bestScore = score;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }
        
        board[bestRow][bestCol] = 'X';
        cout << "AI plays at position (" << bestRow + 1 << ", " << bestCol + 1 << ")\n";
    }
    
public:
    TicTacToe() : currentPlayer('X') {
        initializeBoard();
    }
    
    void play() {
        int row, col;
        bool gameOver = false;
        
        cout << "Welcome to Tic-Tac-Toe!\n";
        cout << "You are 'O', AI is 'X'\n";
        
        while (!gameOver) {
            displayBoard();
            
            if (currentPlayer == 'O') {
                cout << "Your turn (O). Enter row and column (1-3): ";
                cin >> row >> col;
                row--; col--;
                
                if (isMoveValid(row, col)) {
                    board[row][col] = 'O';
                    currentPlayer = 'X';
                } else {
                    cout << "Invalid move! Try again.\n";
                    continue;
                }
            } else {
                cout << "AI's turn (X)...\n";
                aiMove();
                currentPlayer = 'O';
            }
            
            if (checkWin('O')) {
                displayBoard();
                cout << "Congratulations! You win!\n";
                gameOver = true;
            } else if (checkWin('X')) {
                displayBoard();
                cout << "AI wins! Better luck next time.\n";
                gameOver = true;
            } else if (isBoardFull()) {
                displayBoard();
                cout << "It's a draw!\n";
                gameOver = true;
            }
        }
    }
};

int main() {
    TicTacToe game;
    game.play();
    return 0;
}