// Task 6: One-player Pong (ball moves every frame, bounces, game over when missed)
// Windows console program (uses conio.h and windows.h).
#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
using namespace std;

const int WIDTH = 40, HEIGHT = 20, PADDLE_SIZE = 5;

int paddleY, ballX, ballY, dx, dy, score, delayMs;

void resetGame() {
    paddleY = HEIGHT / 2 - PADDLE_SIZE / 2;
    ballX = WIDTH / 2;  ballY = HEIGHT / 2;
    dx = 1;  dy = 1;                                  // ball starts moving right and down
    score = 0;
    delayMs = 90;                                     // frame delay (smaller = faster)
}

void draw() {
    // move the cursor to the top-left instead of clearing the screen (no flicker)
    COORD pos; pos.X = 0; pos.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

    string out;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1 || x == WIDTH - 1) out += '#';
            else if (x == 1 && y >= paddleY && y < paddleY + PADDLE_SIZE) out += '|';
            else if (x == ballX && y == ballY) out += 'O';
            else out += ' ';
        }
        out += '\n';
    }
    out += "Score: " + to_string(score) + "    (W = up, S = down, Q = quit)\n";
    cout << out;
}

void handleInput(bool &quit) {
    while (_kbhit()) {                                // read all pending key presses
        char k = _getch();
        if (k == 'q' || k == 'Q') quit = true;
        if ((k == 'w' || k == 'W') && paddleY > 1) paddleY--;
        if ((k == 's' || k == 'S') && paddleY + PADDLE_SIZE < HEIGHT - 1) paddleY++;
    }
}

// Move the ball one step. Returns false if the player missed the ball.
bool updateBall() {
    int nx = ballX + dx, ny = ballY + dy;

    if (ny <= 0 || ny >= HEIGHT - 1) {                // bounce off top / bottom wall
        dy = -dy;
        ny = ballY + dy;
    }
    if (nx >= WIDTH - 1) {                            // bounce off right wall
        dx = -dx;
        nx = ballX + dx;
    }
    if (nx <= 1) {                                    // ball reached the paddle column
        if (ny >= paddleY && ny < paddleY + PADDLE_SIZE) {
            dx = -dx;                                 // hit: bounce back
            nx = ballX + dx;
            score++;
            if (score % 3 == 0 && delayMs > 30) delayMs -= 10;   // speed up
        } else {
            return false;                             // miss: game over
        }
    }
    ballX = nx;  ballY = ny;
    return true;
}

int main() {
    system("cls");
    cout << "=== PONG ===\nUse W / S to move the paddle. Don't miss the ball!\n";
    cout << "Press any key to start...";
    _getch();
    system("cls");

    resetGame();
    bool quit = false;
    while (!quit) {
        draw();
        handleInput(quit);
        if (!updateBall()) break;                     // game ends automatically on a miss
        Sleep(delayMs);
    }
    system("cls");
    cout << "=========== GAME OVER ===========\n";
    cout << "Final score: " << score << "\n";
    return 0;
}
