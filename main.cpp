//
// Created by Manju Muralidharan on 11/22/25.
//

// Comments added by Ashley Young on 12/4/25 to better understand the code before manipulation
// Code updated by Ashley Young on 12/5/25

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Directions for DFS (students must use these)
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

// ----------------------------------------------------------
// DO NOT MODIFY: Maze generation
// ----------------------------------------------------------
void generateMaze(vector<vector<int>>& maze, int N, int M) {
    srand(time(0));

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            int roll = rand() % 100;
            maze[r][c] = (roll < 70) ? 0 : 1;   // 0 = open, 1 = wall
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Choose a random boundary cell that is open
// ----------------------------------------------------------
pair<int,int> chooseBoundaryCell(const vector<vector<int>>& maze) {
    int N = maze.size();
    int M = maze[0].size();

    // This will execute until the program reaches an empty space-- which could be in one iteration
    while (true) {
        int side = rand() % 4; // Gives random number from 0 to 3
        int r, c;

        // Depending on what was generated, check the top or bottom row for a random column
        // OR left or right column for a random row
        if (side == 0) {          // top row
            r = 0;
            c = rand() % M;
        } else if (side == 1) {   // bottom row
            r = N - 1;
            c = rand() % M;
        } else if (side == 2) {   // left column
            r = rand() % N;
            c = 0;
        } else {                  // right column
            r = rand() % N;
            c = M - 1;
        }

        if (maze[r][c] == 0) {
            return {r, c};
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the maze with S and E markers
// ----------------------------------------------------------
void printMaze(const vector<vector<int>>& maze,
               int ent_r, int ent_c,
               int exit_r, int exit_c)
{
    int N = maze.size();
    int M = maze[0].size();

    cout << "\nMaze:\n";
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            if (r == ent_r && c == ent_c) {
                cout << "S ";
            } else if (r == exit_r && c == exit_c) {
                cout << "E ";
            } else {
                cout << maze[r][c] << " ";
            }
        }
        cout << "\n";
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the reconstructed path
// Students must fill parent[][] correctly during DFS
// ----------------------------------------------------------
void printPath(pair<int,int> exitcell,
               const vector<vector<int>>& parent_r,
               const vector<vector<int>>& parent_c,
               int ent_r, int ent_c)
{
    int r = exitcell.first;
    int c = exitcell.second;

    vector<pair<int,int>> path;

    // Walk backward from exit to entrance
    while (!(r == ent_r && c == ent_c)) {
        path.push_back({r, c});
        int pr = parent_r[r][c];
        int pc = parent_c[r][c];
        r = pr;
        c = pc;
    }
    path.push_back({ent_r, ent_c});

    cout << "\nPath from entrance to exit:\n";
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << "(" << path[i].first << ", " << path[i].second << ")\n";
    }
}

// ----------------------------------------------------------
// STUDENTS IMPLEMENT DFS HERE
// Add arguments, return type, and logic
// ----------------------------------------------------------
bool dfs(int r, int c,
        const vector<vector<int>>& maze,
        vector<vector<bool>>& visited,
        vector<vector<int>>& parent_r,
        vector<vector<int>>& parent_c,
        int exit_r, int exit_c) {
    // Direction variables
    int up = r + dr[0];
    int down = r + dr[2];
    int left = c + dc[3];
    int right = c + dc[1];

    // If currently at exit, return true
    if (r == exit_r && c == exit_c) {
        return true;
    }

    // If surrounded by immovable terrain, return false
    if ((up < 0 || visited[up][c] == true || maze[up][c] == 1) &&
        (down >= maze.size() || visited[down][c] == true || maze[down][c] == 1) &&
        (left < 0 || visited[r][left] == true || maze[r][left] == 1) &&
        (right >= maze[0].size() || visited[r][right] == true || maze[r][right] == 1)) {
        visited[r][c] = true;
        return false;
    }

    // Recursion: move up, down, left, or right if possible. Will visit all possible nodes until finding the exit
    if (up > -1 && maze[up][c] == 0 && !visited[up][c]) {
        visited[r][c] = true;
        parent_r[up][c] = r;
        parent_c[up][c] = c;
        if (dfs(up, c, maze, visited, parent_r, parent_c, exit_r, exit_c)) {
            return true;
        }
    }

    if (down < maze.size() && maze[down][c] == 0 && !visited[down][c]) {
        visited[r][c] = true;
        parent_r[down][c] = r;
        parent_c[down][c] = c;
        if (dfs(down, c, maze, visited, parent_r, parent_c, exit_r, exit_c)) {
            return true;
        }
    }

    if (left > -1 && maze[r][left] == 0 && !visited[r][left]) {
        visited[r][c] = true;
        parent_r[r][left] = r;
        parent_c[r][left] = c;
        if (dfs(r, left, maze, visited, parent_r, parent_c, exit_r, exit_c)) {
            return true;
        }
    }

    if (right < maze[0].size() && maze[r][right] == 0 && !visited[r][right]) {
        visited[r][c] = true;
        parent_r[r][right] = r;
        parent_c[r][right] = c;
        if (dfs(r, right, maze, visited, parent_r, parent_c, exit_r, exit_c)) {
            return true;
        }
    }

    return false; // cascading ended without a success
 }


// ----------------------------------------------------------
// MAIN PROGRAM (students add DFS calls and logic)
// ----------------------------------------------------------
int main() {
    int N, M;

    cout << "Enter maze dimensions N M: ";
    cin >> N >> M;

    vector<vector<int>> maze(N, vector<int>(M)); // Creates a big box of 0's that's N x M
    generateMaze(maze, N, M); // Fills the box with 0s and 1s randomly

    // Pick entrance and exit
    pair<int,int> entrance = chooseBoundaryCell(maze);
    pair<int,int> exitcell = chooseBoundaryCell(maze);

    // Check to make sure that the entrance is not the same as the exit
    while (exitcell == entrance) {
        exitcell = chooseBoundaryCell(maze);
    }

    // Splitting the two pair variables into 4 separate variables
    int ent_r = entrance.first;
    int ent_c = entrance.second;
    int exit_r = exitcell.first;
    int exit_c = exitcell.second;

    // Display the maze
    printMaze(maze, ent_r, ent_c, exit_r, exit_c);

    // Students must use these
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    vector<vector<int>> parent_r(N, vector<int>(M, -1));
    vector<vector<int>> parent_c(N, vector<int>(M, -1));

    // ------------------------------------------------------
    // STUDENT WORK:
    // Call your DFS, track visited, and fill parent_r and parent_c
    // ------------------------------------------------------
     bool found = dfs(ent_r, ent_c, maze, visited, parent_r, parent_c, exit_r, exit_c);

    // ------------------------------------------------------
    // STUDENT WORK:
    // If found, print the path
    // ------------------------------------------------------
     if (found) {
         printPath(exitcell, parent_r, parent_c, ent_r, ent_c);
     } else {
         cout << "\nNo path exists.\n";
     }

    return 0;
}