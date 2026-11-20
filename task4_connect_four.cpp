// Task 4: Connect Four (6 rows x 7 columns, two players)
#include <iostream>
using namespace std;

const int ROWS = 6, COLS = 7;
char g[ROWS][COLS];

void resetBoard() {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++) g[r][c] = '.';
}

void printBoard() {
    cout << "\n";
    for (int c = 1; c <= COLS; c++) cout << " " << c << " ";
    cout << "\n";
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) cout << " " << g[r][c] << " ";
        cout << "\n";
    }
    cout << "\n";
}

// Drop a disc: find the bottom-most empty cell of the column. Returns the row, or -1 if full.
int dropDisc(int col, char p) {
    for (int r = ROWS - 1; r >= 0; r--)
        if (g[r][col] == '.') { g[r][col] = p; return r; }
    return -1;
}

// Check 4 in a row through (r, c) in 4 directions: horizontal, vertical, two diagonals
bool checkWin(int r, int c, char p) {
    const int dr[4] = {0, 1, 1, 1};
    const int dc[4] = {1, 0, 1, -1};
    for (int d = 0; d < 4; d++) {
        int count = 1;
        for (int s = -1; s <= 1; s += 2) {               // look both ways along the direction
            int nr = r + s * dr[d], nc = c + s * dc[d];
            while (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && g[nr][nc] == p) {
                count++;
                nr += s * dr[d];
                nc += s * dc[d];
            }
        }
        if (count >= 4) return true;
    }
    return false;
}

bool boardFull() {
    for (int c = 0; c < COLS; c++) if (g[0][c] == '.') return false;   // top row has space?
    return true;
}

int main() {
    char again = 'y';
    cout << "=== CONNECT FOUR ===\nPlayer 1 = X, Player 2 = O\n";

    while (again == 'y' || again == 'Y') {
        resetBoard();
        char players[2] = {'X', 'O'};
        int turn = 0;

        while (true) {
            printBoard();
            int col, row;
            while (true) {
                cout << "Player " << turn + 1 << " (" << players[turn] << "), choose column (1-7): ";
                if (cin >> col && col >= 1 && col <= COLS) {
                    row = dropDisc(col - 1, players[turn]);
                    if (row != -1) break;
                    cout << "That column is full.\n";
                } else {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid column.\n";
                }
            }
            if (checkWin(row, col - 1, players[turn])) {
                printBoard();
                cout << "Player " << turn + 1 << " (" << players[turn] << ") wins!\n";
                break;
            }
            if (boardFull()) {
                printBoard();
                cout << "It's a draw!\n";
                break;
            }
            turn = 1 - turn;
        }
        cout << "Play again? (y/n): ";
        cin >> again;
    }
    cout << "Thanks for playing!\n";
    return 0;
}
