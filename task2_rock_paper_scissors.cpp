// Task 2: Rock-Paper-Scissors against the computer (single round, then exit)
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const char* nameOf(int c) {
    if (c == 1) return "Rock";
    if (c == 2) return "Paper";
    return "Scissors";
}

int main() {
    srand((unsigned)time(NULL));
    int user;

    cout << "=== ROCK - PAPER - SCISSORS ===\n";
    cout << "1. Rock\n2. Paper\n3. Scissors\n";
    while (true) {
        cout << "Enter your choice (1-3): ";
        if (cin >> user && user >= 1 && user <= 3) break;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Please enter 1, 2 or 3.\n";
    }

    int comp = rand() % 3 + 1;              // computer's random choice
    cout << "\nYou chose      : " << nameOf(user) << "\n";
    cout << "Computer chose : " << nameOf(comp) << "\n\n";

    // With Rock=1, Paper=2, Scissors=3, the user wins when (user - comp) mod 3 == 1
    int diff = (user - comp + 3) % 3;
    if (diff == 0)      cout << "Result: It's a draw!\n";
    else if (diff == 1) cout << "Result: You win!\n";
    else                cout << "Result: You lose!\n";
    return 0;
}
