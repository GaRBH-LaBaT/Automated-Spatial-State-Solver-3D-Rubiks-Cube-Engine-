#include "Solver.hpp"
#include <iostream>
#include <algorithm>

Solver::Solver() {
    cornerDB.loadOrGenerate("corner.pdb");
    edge1DB.loadOrGenerate("edge1.pdb");
    edge2DB.loadOrGenerate("edge2.pdb");
}

std::vector<Move> Solver::solve(const Cube& start) {
    if (start.isSolved()) return {};

    uint32_t c_idx = CornerDB::getIndex(start);
    uint32_t e1_idx = Edge1DB::getIndex(start);
    uint32_t e2_idx = Edge2DB::getIndex(start);
    
    uint8_t h1 = cornerDB.getEstimate(c_idx);
    uint8_t h2 = edge1DB.getEstimate(e1_idx);
    uint8_t h3 = edge2DB.getEstimate(e2_idx);
    
    uint8_t bound = std::max({h1, h2, h3});
    std::vector<Move> path;
    
    std::cout << "Initial heuristic bound: " << (int)bound << "\n";
    
    while (bound <= 20) {
        std::cout << "Searching depth: " << (int)bound << "\n";
        path.clear();
        uint8_t t = search(start, 0, bound, NONE, NONE, path);
        if (t == 0) return path; // Solved
        if (t > 20) break;
        bound = t;
    }
    
    return path;
}

uint8_t Solver::search(Cube cube, uint8_t g, uint8_t bound, Move last_move, Move second_last_move, std::vector<Move>& path) {
    uint32_t c_idx = CornerDB::getIndex(cube);
    uint8_t h1 = cornerDB.getEstimate(c_idx);
    if (g + h1 > bound) return g + h1;
    
    uint32_t e1_idx = Edge1DB::getIndex(cube);
    uint8_t h2 = edge1DB.getEstimate(e1_idx);
    if (g + h2 > bound) return g + h2;
    
    uint32_t e2_idx = Edge2DB::getIndex(cube);
    uint8_t h3 = edge2DB.getEstimate(e2_idx);
    if (g + h3 > bound) return g + h3;
    
    uint8_t h = std::max({h1, h2, h3});
    if (h == 0 && cube.isSolved()) return 0;
    
    uint8_t min_t = 255;
    Move moves[] = {U, U2, U3, D, D2, D3, L, L2, L3, R, R2, R3, F, F2, F3, B, B2, B3};
    
    for (Move m : moves) {
        if (last_move != NONE) {
            int last_face = last_move / 3;
            int curr_face = m / 3;
            if (last_face == curr_face) continue; 
            if (curr_face == last_face - 1 && last_face % 2 == 1) continue; 
        }
        
        Cube next = cube;
        next.applyMove(m);
        path.push_back(m);
        
        uint8_t t = search(next, g + 1, bound, m, last_move, path);
        if (t == 0) return 0;
        if (t < min_t) min_t = t;
        
        path.pop_back();
    }
    
    return min_t;
}
