// Maze Pathfinding Visualizer: BFS, DFS, Dijkstra, A*
// Legend:  S start | E end | # wall | ~ water (cost 5) | . open (cost 1) | * path
// Compatible with Dev-C++ 5.11 (C++11).
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

typedef vector<string> Grid;
const int DR[4] = {-1, 1, 0, 0};
const int DC[4] = {0, 0, -1, 1};
const int INF = 1000000000;

struct Result {
    string name;
    bool found;
    int expanded;   // nodes taken out of the queue/stack/heap
    int steps;      // number of moves in the path
    int cost;       // total terrain cost of the path
    double micros;  // average run time
    vector<int> path;
};

int H, W, startR, startC, endR, endC;

int cellCost(char ch) { return ch == '~' ? 5 : 1; }
bool passable(const Grid &g, int r, int c) {
    return r >= 0 && r < H && c >= 0 && c < W && g[r][c] != '#';
}
int heuristic(int r, int c) { return abs(r - endR) + abs(c - endC); }  // Manhattan

void findSpecial(const Grid &g) {
    H = g.size(); W = g[0].size();
    for (int r = 0; r < H; r++)
        for (int c = 0; c < W; c++) {
            if (g[r][c] == 'S') { startR = r; startC = c; }
            if (g[r][c] == 'E') { endR = r; endC = c; }
        }
}

// Walk parent[] from the end back to the start
void buildPath(const Grid &g, const vector<int> &parent, Result &res) {
    res.path.clear(); res.steps = 0; res.cost = 0;
    int cur = endR * W + endC;
    while (cur != -1) {
        res.path.push_back(cur);
        cur = parent[cur];
    }
    reverse(res.path.begin(), res.path.end());
    res.steps = res.path.size() - 1;
    for (size_t i = 1; i < res.path.size(); i++)
        res.cost += cellCost(g[res.path[i] / W][res.path[i] % W]);
}

// ---------- BFS : queue, fewest steps (ignores terrain cost) ----------
Result runBFS(const Grid &g) {
    Result res; res.name = "BFS"; res.found = false; res.expanded = 0;
    vector<int> parent(H * W, -1);
    vector<bool> seen(H * W, false);
    queue<int> q;
    q.push(startR * W + startC); seen[startR * W + startC] = true;
    while (!q.empty()) {
        int cur = q.front(); q.pop(); res.expanded++;
        int r = cur / W, c = cur % W;
        if (r == endR && c == endC) { res.found = true; break; }
        for (int d = 0; d < 4; d++) {
            int nr = r + DR[d], nc = c + DC[d];
            if (passable(g, nr, nc) && !seen[nr * W + nc]) {
                seen[nr * W + nc] = true; parent[nr * W + nc] = cur; q.push(nr * W + nc);
            }
        }
    }
    if (res.found) buildPath(g, parent, res);
    return res;
}

// ---------- DFS : stack, finds *a* path (not necessarily short) ----------
Result runDFS(const Grid &g) {
    Result res; res.name = "DFS"; res.found = false; res.expanded = 0;
    vector<int> parent(H * W, -1);
    vector<bool> seen(H * W, false);
    stack<int> st;
    st.push(startR * W + startC); seen[startR * W + startC] = true;
    while (!st.empty()) {
        int cur = st.top(); st.pop(); res.expanded++;
        int r = cur / W, c = cur % W;
        if (r == endR && c == endC) { res.found = true; break; }
        for (int d = 0; d < 4; d++) {
            int nr = r + DR[d], nc = c + DC[d];
            if (passable(g, nr, nc) && !seen[nr * W + nc]) {
                seen[nr * W + nc] = true; parent[nr * W + nc] = cur; st.push(nr * W + nc);
            }
        }
    }
    if (res.found) buildPath(g, parent, res);
    return res;
}

// ---------- Dijkstra : min-heap, cheapest path ----------
typedef pair<int, int> P;  // (cost, cell)
Result runDijkstra(const Grid &g) {
    Result res; res.name = "Dijkstra"; res.found = false; res.expanded = 0;
    vector<int> parent(H * W, -1), dist(H * W, INF);
    priority_queue<P, vector<P>, greater<P> > pq;
    dist[startR * W + startC] = 0; pq.push(P(0, startR * W + startC));
    while (!pq.empty()) {
        P top = pq.top(); pq.pop();
        int cur = top.second;
        if (top.first > dist[cur]) continue;          // stale entry
        res.expanded++;
        int r = cur / W, c = cur % W;
        if (r == endR && c == endC) { res.found = true; break; }
        for (int d = 0; d < 4; d++) {
            int nr = r + DR[d], nc = c + DC[d];
            if (!passable(g, nr, nc)) continue;
            int nd = dist[cur] + cellCost(g[nr][nc]);
            if (nd < dist[nr * W + nc]) {
                dist[nr * W + nc] = nd; parent[nr * W + nc] = cur; pq.push(P(nd, nr * W + nc));
            }
        }
    }
    if (res.found) buildPath(g, parent, res);
    return res;
}

// ---------- A* : min-heap ordered by f = g + h (Manhattan) ----------
Result runAStar(const Grid &g) {
    Result res; res.name = "A*"; res.found = false; res.expanded = 0;
    vector<int> parent(H * W, -1), gScore(H * W, INF);
    priority_queue<P, vector<P>, greater<P> > pq;     // (f, cell)
    int s = startR * W + startC;
    gScore[s] = 0; pq.push(P(heuristic(startR, startC), s));
    while (!pq.empty()) {
        P top = pq.top(); pq.pop();
        int cur = top.second;
        int r = cur / W, c = cur % W;
        if (top.first > gScore[cur] + heuristic(r, c)) continue;   // stale entry
        res.expanded++;
        if (r == endR && c == endC) { res.found = true; break; }
        for (int d = 0; d < 4; d++) {
            int nr = r + DR[d], nc = c + DC[d];
            if (!passable(g, nr, nc)) continue;
            int ng = gScore[cur] + cellCost(g[nr][nc]);
            if (ng < gScore[nr * W + nc]) {
                gScore[nr * W + nc] = ng; parent[nr * W + nc] = cur;
                pq.push(P(ng + heuristic(nr, nc), nr * W + nc));
            }
        }
    }
    if (res.found) buildPath(g, parent, res);
    return res;
}

typedef Result (*Algo)(const Grid &);

// Run an algorithm many times to get a measurable average time
Result runTimed(Algo f, const Grid &g) {
    const int REPS = 200;
    clock_t t0 = clock();
    Result res;
    for (int i = 0; i < REPS; i++) res = f(g);
    clock_t t1 = clock();
    res.micros = (double)(t1 - t0) * 1000000.0 / CLOCKS_PER_SEC / REPS;
    return res;
}

void printMaze(const Grid &g, const Result &res) {
    Grid out = g;
    for (size_t i = 0; i < res.path.size(); i++) {
        int r = res.path[i] / W, c = res.path[i] % W;
        if (out[r][c] != 'S' && out[r][c] != 'E') out[r][c] = '*';
    }
    for (int r = 0; r < H; r++) cout << "  " << out[r] << "\n";
}

void showResult(const Grid &g, const Result &res) {
    cout << "\n--- " << res.name << " ---\n";
    if (!res.found) { cout << "  No path exists.\n"; return; }
    printMaze(g, res);
    cout << "  Nodes expanded: " << res.expanded << " | Steps: " << res.steps
         << " | Path cost: " << res.cost << "\n";
}

void compareAll(const Grid &g) {
    Algo fs[4] = {runBFS, runDFS, runDijkstra, runAStar};
    cout << "\n" << left << setw(10) << "Algorithm" << setw(10) << "Found"
         << setw(10) << "Expanded" << setw(8) << "Steps" << setw(8) << "Cost"
         << setw(12) << "Time(us)" << "\n";
    cout << string(58, '-') << "\n";
    for (int i = 0; i < 4; i++) {
        Result r = runTimed(fs[i], g);
        cout << left << setw(10) << r.name << setw(10) << (r.found ? "yes" : "no")
             << setw(10) << r.expanded << setw(8) << (r.found ? r.steps : 0)
             << setw(8) << (r.found ? r.cost : 0) << fixed << setprecision(1)
             << setw(12) << r.micros << "\n";
    }
    cout << "\nExpanded = nodes processed. Fewer is better. Cost counts water cells as 5.\n";
}

Grid fixedMaze() {
    Grid g;
    g.push_back("###############################");
    g.push_back("#S    #       ~~~~    #       #");
    g.push_back("# ### # ##### ~~~~ ## # ##### #");
    g.push_back("# #   #     # ~~~~    #     # #");
    g.push_back("# # ####### # ######### ### # #");
    g.push_back("# #       # #           #   # #");
    g.push_back("# ####### # ##### ####### ### #");
    g.push_back("#       # #     # #     #     #");
    g.push_back("####### # ##### # # ### ##### #");
    g.push_back("#       #       #   #   ~~~~ E#");
    g.push_back("###############################");
    return g;
}

Grid randomMaze(int h, int w, int wallPct) {
    Grid g(h, string(w, ' '));
    for (int r = 0; r < h; r++)
        for (int c = 0; c < w; c++) {
            if (r == 0 || c == 0 || r == h - 1 || c == w - 1) { g[r][c] = '#'; continue; }
            int x = rand() % 100;
            if (x < wallPct) g[r][c] = '#';
            else if (x < wallPct + 10) g[r][c] = '~';
        }
    g[1][1] = 'S'; g[h - 2][w - 2] = 'E';
    return g;
}

int main() {
    srand((unsigned)time(NULL));
    Grid g = fixedMaze();
    findSpecial(g);
    int ch = -1;
    while (ch != 0) {
        cout << "\n===== MAZE PATHFINDER =====\n";
        cout << "1. BFS\n2. DFS\n3. Dijkstra\n4. A*\n5. Compare all (table)\n";
        cout << "6. Load built-in maze\n7. Generate random maze\n8. Show maze\n0. Exit\nChoice: ";
        if (!(cin >> ch)) { cin.clear(); cin.ignore(10000, '\n'); ch = -1; continue; }
        switch (ch) {
            case 1: showResult(g, runBFS(g)); break;
            case 2: showResult(g, runDFS(g)); break;
            case 3: showResult(g, runDijkstra(g)); break;
            case 4: showResult(g, runAStar(g)); break;
            case 5: compareAll(g); break;
            case 6: g = fixedMaze(); findSpecial(g); cout << "Built-in maze loaded.\n"; break;
            case 7: g = randomMaze(15, 40, 22); findSpecial(g); cout << "Random maze generated.\n";
                    for (int r = 0; r < H; r++) cout << "  " << g[r] << "\n"; break;
            case 8: for (int r = 0; r < H; r++) cout << "  " << g[r] << "\n"; break;
            case 0: cout << "Bye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    }
    return 0;
}