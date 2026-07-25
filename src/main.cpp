#include <iostream>
#include <string>
#include <chrono>
#include "Cube.hpp"
#include "Solver.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " \"SCRAMBLE STRING\"\n";
        std::cerr << "Example: " << argv[0] << " \"U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2\"\n";
        return 1;
    }
    
    std::string scramble = argv[1];
    std::cout << "Scramble: " << scramble << "\n";
    
    Cube start = Cube::fromScramble(scramble);
    if (start.isSolved()) {
        std::cout << "Cube is already solved!\n";
        return 0;
    }
    
    // Initialize solver (loads or generates PDBs)
    std::cout << "Initializing solver and loading pattern databases...\n";
    auto init_start = std::chrono::high_resolution_clock::now();
    Solver solver;
    auto init_end = std::chrono::high_resolution_clock::now();
    std::cout << "Initialization took " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(init_end - init_start).count() 
              << " ms.\n\n";
    
    std::cout << "Starting IDA* Search...\n";
    auto search_start = std::chrono::high_resolution_clock::now();
    std::vector<Move> solution = solver.solve(start);
    auto search_end = std::chrono::high_resolution_clock::now();
    
    std::cout << "\n=== Solution Found ===\n";
    std::cout << "Moves (" << solution.size() << "): ";
    for (Move m : solution) {
        std::cout << moveToString(m) << " ";
    }
    std::cout << "\n";
    
    std::cout << "Search time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(search_end - search_start).count() 
              << " ms.\n";
              
    return 0;
}
