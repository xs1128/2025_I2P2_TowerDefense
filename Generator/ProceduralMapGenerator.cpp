#include "ProceduralMapGenerator.hpp"
#include <queue>
#include <algorithm>

// check within bounds
bool ProceduralMapGenerator::isValid(int x, int y) const {
    return x >= 0 && x < MapHeight && y >= 0 && y < MapWidth;
}

// BFS to check if path exists from start to end
bool ProceduralMapGenerator::hasValidPath(int startX, int startY, int endX, int endY) const {
    if (map[startX][startY] == 1 || map[endX][endY] == 1) return false;
    
    std::vector<std::vector<bool>> visited(MapHeight, std::vector<bool>(MapWidth, false));
    std::queue<std::pair<int, int>> q;
    
    q.push({startX, startY});
    visited[startX][startY] = true;
    
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        
        if (x == endX && y == endY) return true;
        
        // go through each dir
        for (auto [dx, dy] : directions) {
            int nx = x + dx, ny = y + dy;
            if (isValid(nx, ny) && !visited[nx][ny] && map[nx][ny] == 0) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }
    return false;
}

// Generate a guaranteed path from start to end
void ProceduralMapGenerator::generateGuaranteedPath(int startX, int startY, int endX, int endY) {
    // Simple path generation - can be made more sophisticated
    int x = startX, y = startY;
    map[x][y] = 0;
    
    while (x != endX || y != endY) {
        // Randomly choose to move towards target in x or y direction
        if (x != endX && (y == endY || rng() % 2)) {
            x += (endX > x) ? 1 : -1;
        } else if (y != endY) {
            y += (endY > y) ? 1 : -1;
        }
        map[x][y] = 0;
    }
}

// generate random blue blocks while preserving path
void ProceduralMapGenerator::generateObstacles(double obstacleRatio = 0.4) {
    int totalCells = MapWidth * MapHeight;
    int targetObstacles = static_cast<int>(totalCells * obstacleRatio);
    int currentObstacles = 0;
    
    // create a list of all positions
    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            if (map[i][j] == 1) {
                // if placable
                positions.push_back({i, j});
            }
        }
    }
    
    // random selection
    std::shuffle(positions.begin(), positions.end(), rng);
    
    for (auto [x, y] : positions) {
        if (currentObstacles >= targetObstacles) break;
        
        map[x][y] = 1;
        if (hasValidPath(0, 0, MapHeight - 1, MapWidth - 1)) {
            currentObstacles++;
        } else {
            map[x][y] = 0;
        }
    }
}

void ProceduralMapGenerator::generateWindingPath() {
    // start from top-left corner
    int x = 0, y = 0;
    map[x][y] = 0;
    
    std::vector<std::pair<int, int>> path;
    path.push_back({x, y});
    
    // find and create a winding path
    while (x < MapHeight - 1 || y < MapWidth - 1) {
        std::vector<std::pair<int, int>> validMoves;
        
        // Prefer moving towards the goal but allow some randomness
        if (x < MapHeight - 1) validMoves.push_back({x + 1, y});
        if (y < MapWidth - 1) validMoves.push_back({x, y + 1});
        
        // Occasionally allow backward moves for winding effect
        if (rng() % 4 == 0) {
            if (x > 0) validMoves.push_back({x - 1, y});
            if (y > 0) validMoves.push_back({x, y - 1});
        }
        
        if (!validMoves.empty()) {
            auto [nx, ny] = validMoves[rng() % validMoves.size()];
            
            // avoid loops
            bool tooClose = false;
            for (auto [px, py] : path) {
                if (abs(px - nx) + abs(py - ny) <= 1 && !(px == x && py == y)) {
                    tooClose = true;
                    break;
                }
            }
            
            if (!tooClose || validMoves.size() == 1) {
                x = nx; y = ny;
                map[x][y] = 0;
                path.push_back({x, y});
            }
        } else {
            // fallback
            if (x < MapHeight - 1) x++;
            else if (y < MapWidth - 1) y++;
            map[x][y] = 0;
            path.push_back({x, y});
        }
    }
}

// generate branching paths
void ProceduralMapGenerator::generateBranchingPaths(int numBranches = 3) {
    for (int branch = 0; branch < numBranches; branch++) {
        // Find a random point on existing path
        std::vector<std::pair<int, int>> pathPoints;
        for (int i = 0; i < MapHeight; i++) {
            for (int j = 0; j < MapWidth; j++) {
                if (map[i][j] == 0) {
                    pathPoints.push_back({i, j});
                }
            }
        }
        
        if (pathPoints.empty()) continue;
        
        auto [startX, startY] = pathPoints[rng() % pathPoints.size()];
        
        // Create a short branch
        int branchLength = 3 + rng() % 5;
        int x = startX, y = startY;
        
        for (int step = 0; step < branchLength; step++) {
            std::vector<std::pair<int, int>> validMoves;
            
            for (auto [dx, dy] : directions) {
                int nx = x + dx, ny = y + dy;
                if (isValid(nx, ny) && map[nx][ny] == 1) {
                    validMoves.push_back({nx, ny});
                }
            }
            
            if (validMoves.empty()) break;
            
            auto [nx, ny] = validMoves[rng() % validMoves.size()];
            x = nx; y = ny;
            map[x][y] = 0;
        }
    }
}

// main generation function
std::vector<std::vector<int>> ProceduralMapGenerator::generateMap(int algorithm = 0) {
    // init map with all valid construct areas
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            map[i][j] = 1;
        }
    }
    
    switch (algorithm) {
        case 0:                
            generateGuaranteedPath(0, 0, MapHeight - 1, MapWidth - 1);
            break;
        case 1: 
            generateWindingPath();
            break;
        case 2:
            generateWindingPath();
            generateBranchingPaths(2 + rng() % 3);
            break;
    }
    
    // ensure start and end points are always paths
    map[0][0] = 0;
    map[MapHeight - 1][MapWidth - 1] = 0;
    
    return map;
}

