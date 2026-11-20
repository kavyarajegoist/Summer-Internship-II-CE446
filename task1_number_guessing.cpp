// Task 1: Number Guessing Game
// The computer picks a number (1-100); the player guesses with high/low hints.
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand((unsigned)time(NULL));            // seed the random number generator
    int secret = rand() % 100 + 1;          // random number in 1..100
    int guess, attempts = 0;

    cout << "=== NUMBER GUESSING GAME ===\n";
    cout << "I have picked a number between 1 and 100. Can you guess it?\n";

    while (true) {
        cout << "Enter your guess: ";
        if (!(cin >> guess)) {              // input was not a number
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }
        if (guess < 1 || guess > 100) {
            cout << "Your guess must be between 1 and 100.\n";
            continue;
        }
        attempts++;
        if (guess < secret)      cout << "Too low! Try a higher number.\n";
        else if (guess > secret) cout << "Too high! Try a lower number.\n";
        else {
            cout << "Correct! The number was " << secret << ".\n";
            cout << "You guessed it in " << attempts << " attempt(s).\n";
            break;                          // game ends on the correct guess
        }
    }
    return 0;
}
