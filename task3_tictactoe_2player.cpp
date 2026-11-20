// Task 3: Two-player Tic-Tac-Toe on a 3x3 board
#include <iostream>
using namespace std;

char board[9];
const int WINS[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};

void resetBoard() { for (int i = 0; i < 9; i++) board[i] = ' '; }

void printBoard() {
    cout << "\n";
    for (int r = 0; r < 3; r++) {
        cout << "  ";
        for (int c = 0; c < 3; c++) {
            int i = r * 3 + c;
            cout << (board[i] == ' ' ? char('1' + i) : board[i]);   // show cell number if empty
            if (c < 2) cout << " | ";
        }
        cout << "\n";
        if (r < 2) cout << "  ---------\n";
    }
    cout << "\n";
}

bool hasWon(char m) {
    for (int i = 0; i < 8; i++)
        if (board[WINS[i][0]] == m && board[WINS[i][1]] == m && board[WINS[i][2]] == m)
            return true;
    return false;
}

bool isFull() {
    for (int i = 0; i < 9; i++) if (board[i] == ' ') return false;
    return true;
}

int main() {
    char again = 'y';
    cout << "=== TIC-TAC-TOE (2 players) ===\n";

    while (again == 'y' || again == 'Y') {
        resetBoard();
        char marker[2];
        char m;
        while (true) {                                   // Player 1 chooses a marker
            cout << "Player 1, choose your marker (X or O): ";
            cin >> m;
            if (m == 'x' || m == 'X') { marker[0] = 'X'; marker[1] = 'O'; break; }
            if (m == 'o' || m == 'O') { marker[0] = 'O'; marker[1] = 'X'; break; }
            cout << "Invalid choice.\n";
        }

        int turn = 0;                                    // 0 = Player 1, 1 = Player 2
        while (true) {
            printBoard();
            int cell;
            while (true) {
                cout << "Player " << turn + 1 << " (" << marker[turn] << "), enter cell (1-9): ";
                if (cin >> cell && cell >= 1 && cell <= 9 && board[cell - 1] == ' ') break;
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid or occupied cell. Try again.\n";
            }
            board[cell - 1] = marker[turn];

            if (hasWon(marker[turn])) {
                printBoard();
                cout << "Player " << turn + 1 << " (" << marker[turn] << ") wins!\n";
                break;
            }
            if (isFull()) {
                printBoard();
                cout << "It's a draw!\n";
                break;
            }
            turn = 1 - turn;                             // switch player
        }
        cout << "Play again? (y/n): ";
        cin >> again;
    }
    cout << "Thanks for playing!\n";
    return 0;
}
