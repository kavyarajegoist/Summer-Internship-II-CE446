// Task 5: Pong - Part 1 (static board, draw(), paddle movement with W / S)
// Windows console program (uses conio.h).
#include <iostream>
#include <cstdlib>
#include <conio.h>
using namespace std;

const int WIDTH = 40, HEIGHT = 20, PADDLE_SIZE = 5;
int paddleY = HEIGHT / 2 - PADDLE_SIZE / 2;          // top row of the paddle
int ballX = WIDTH / 2, ballY = HEIGHT / 2;           // ball stays still in this part

void draw() {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1 || x == WIDTH - 1)  cout << '#';   // top, bottom, right walls
            else if (x == 1 && y >= paddleY && y < paddleY + PADDLE_SIZE) cout << '|';  // paddle
            else if (x == ballX && y == ballY)                cout << 'O';   // ball
            else                                              cout << ' ';
        }
        cout << "\n";
    }
    cout << "W = up, S = down, Q = quit\n";
}

int main() {
    char key;
    draw();
    while (true) {
        key = _getch();                              // wait for a key press
        if (key == 'q' || key == 'Q') break;
        if ((key == 'w' || key == 'W') && paddleY > 1) paddleY--;                         // move up
        if ((key == 's' || key == 'S') && paddleY + PADDLE_SIZE < HEIGHT - 1) paddleY++;  // move down
        draw();
    }
    return 0;
}
