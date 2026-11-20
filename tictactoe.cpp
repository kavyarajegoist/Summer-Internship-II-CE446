// Tic-Tac-Toe vs unbeatable AI  (Minimax + Alpha-Beta pruning)
// Human = X, Computer = O.  Compatible with Dev-C++ 5.11 (C++11).
#include <iostream>
#include <limits>
using namespace std;

char b[9];
const int WINS[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};

void resetBoard() { for (int i = 0; i < 9; i++) b[i] = ' '; }

void printBoard() {
    cout << "\n";
    for (int r = 0; r < 3; r++) {
        cout << "  ";
        for (int c = 0; c < 3; c++) {
            int i = r * 3 + c;
            cout << (b[i] == ' ' ? char('1' + i) : b[i]);
            if (c < 2) cout << " | ";
        }
        cout << "\n";
        if (r < 2) cout << "  ---------\n";
    }
    cout << "\n";
}

char winner() {
    for (int i = 0; i < 8; i++) {
        char a = b[WINS[i][0]];
        if (a != ' ' && a == b[WINS[i][1]] && a == b[WINS[i][2]]) return a;
    }
    return ' ';
}

bool boardFull() {
    for (int i = 0; i < 9; i++) if (b[i] == ' ') return false;
    return true;
}

// Minimax: AI ('O') is the maximiser, human ('X') the minimiser.
// Score = +10 - depth if AI wins (prefer faster wins), depth - 10 if human wins, 0 draw.
int minimax(int depth, bool aiTurn, int alpha, int beta, bool prune, long long &nodes) {
    nodes++;
    char w = winner();
    if (w == 'O') return 10 - depth;
    if (w == 'X') return depth - 10;
    if (boardFull()) return 0;

    if (aiTurn) {
        int best = -1000;
        for (int i = 0; i < 9; i++) {
            if (b[i] != ' ') continue;
            b[i] = 'O';
            int s = minimax(depth + 1, false, alpha, beta, prune, nodes);
            b[i] = ' ';
            if (s > best) best = s;
            if (best > alpha) alpha = best;
            if (prune && beta <= alpha) break;       // beta cut-off
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 9; i++) {
            if (b[i] != ' ') continue;
            b[i] = 'X';
            int s = minimax(depth + 1, true, alpha, beta, prune, nodes);
            b[i] = ' ';
            if (s < best) best = s;
            if (best < beta) beta = best;
            if (prune && beta <= alpha) break;       // alpha cut-off
        }
        return best;
    }
}

int bestMove(bool prune, long long &nodes) {
    int bestScore = -1000, move = -1;
    nodes = 0;
    for (int i = 0; i < 9; i++) {
        if (b[i] != ' ') continue;
        b[i] = 'O';
        int s = minimax(1, false, -1000, 1000, prune, nodes);
        b[i] = ' ';
        if (s > bestScore) { bestScore = s; move = i; }
    }
    return move;
}

int readCell() {
    int c;
    while (true) {
        cout << "Your move (1-9): ";
        if (cin >> c && c >= 1 && c <= 9 && b[c - 1] == ' ') return c - 1;
        if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        cout << "Invalid or occupied cell, try again.\n";
    }
}

int main() {
    int humanWins = 0, aiWins = 0, draws = 0;
    char again = 'y';
    cout << "=== TIC-TAC-TOE vs AI (Minimax + Alpha-Beta) ===\n";
    cout << "You are X, computer is O.\n";

    while (again == 'y' || again == 'Y') {
        resetBoard();
        char first;
        cout << "\nWho plays first? (h = human, c = computer): ";
        cin >> first;
        bool aiTurn = (first == 'c' || first == 'C');

        while (true) {
            printBoard();
            if (aiTurn) {
                long long plainNodes, abNodes;
                int m1 = bestMove(false, plainNodes);   // plain minimax
                int m2 = bestMove(true, abNodes);       // alpha-beta
                cout << "Computer thinking...\n";
                cout << "  Plain Minimax explored  : " << plainNodes << " nodes\n";
                cout << "  Alpha-Beta explored     : " << abNodes << " nodes";
                if (plainNodes > 0)
                    cout << "  (" << 100 - (abNodes * 100 / plainNodes) << "% fewer)";
                cout << "\n  Same move chosen        : " << (m1 == m2 ? "yes" : "no") << "\n";
                b[m2] = 'O';
                cout << "Computer plays cell " << m2 + 1 << "\n";
            } else {
                b[readCell()] = 'X';
            }
            char w = winner();
            if (w != ' ') {
                printBoard();
                if (w == 'X') { cout << "You win! (this should be impossible...)\n"; humanWins++; }
                else          { cout << "Computer wins.\n"; aiWins++; }
                break;
            }
            if (boardFull()) { printBoard(); cout << "It's a draw.\n"; draws++; break; }
            aiTurn = !aiTurn;
        }
        cout << "\nScore -> You: " << humanWins << "  Computer: " << aiWins << "  Draws: " << draws << "\n";
        cout << "Play again? (y/n): ";
        cin >> again;
    }
    cout << "Thanks for playing!\n";
    return 0;
}