#pragma once
#include "Cube.hpp"
#include "PatternDatabase.hpp"
#include <vector>

class Solver {
public:
    Solver();
    
    // Returns sequence of moves to solve the cube
    std::vector<Move> solve(const Cube& start);

private:
    CornerDB cornerDB;
    Edge1DB edge1DB;
    Edge2DB edge2DB;

    // The IDA* recursive search function
    // Returns the new bound, or 0 if solved
    uint8_t search(Cube cube, uint8_t g, uint8_t bound, Move last_move, Move second_last_move, std::vector<Move>& path);
};
