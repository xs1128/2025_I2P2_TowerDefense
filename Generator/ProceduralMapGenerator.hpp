#ifndef PROCEDURAL_MAP_GENERATOR_HPP
#define PROCEDURAL_MAP_GENERATOR_HPP

#include <vector>
#include <random>

class ProceduralMapGenerator {
public:
    static const int MapWidth = 20;
    static const int MapHeight = 13;
    
private:
    std::vector<std::vector<int>> map;
    std::mt19937 rng;
    
    // dirs for pathfinding (up, down, left, right)
    const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
public:
    ProceduralMapGenerator(unsigned int seed = std::random_device{}()) : rng(seed) {
        map.resize(MapHeight, std::vector<int>(MapWidth, 1));
    }
    
    bool isValid(int x, int y) const;
    bool hasValidPath(int startX, int startY, int endX, int endY) const;
    void generateGuaranteedPath(int startX, int startY, int endX, int endY);
    void generateObstacles(double obstacleRatio);
    void generateWindingPath();
    void generateBranchingPaths(int numBranches);
    std::vector<std::vector<int>> generateMap(int algorithm);
};

#endif
