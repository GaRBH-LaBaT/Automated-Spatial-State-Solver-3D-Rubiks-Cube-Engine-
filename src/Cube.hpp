#pragma once

#include <cstdint>
#include <string>
#include <vector>

// 18 standard moves
enum Move {
    U=0, U2, U3,
    D, D2, D3,
    L, L2, L3,
    R, R2, R3,
    F, F2, F3,
    B, B2, B3,
    NONE
};

// String conversions
Move parseMove(const std::string& s);
std::string moveToString(Move m);

class Cube {
public:
    // Bits 0..23: Corner Permutations (8 * 3 bits)
    // Bits 24..39: Corner Orientations (8 * 2 bits)
    uint64_t corners;

    // Bits 0..47: Edge Permutations (12 * 4 bits)
    // Bits 48..59: Edge Orientations (12 * 1 bit)
    uint64_t edges;

    Cube(); // Initializes to solved state

    // Core bitwise interface
    inline uint64_t getCornerPerm(int i) const { return (corners >> (i * 3)) & 0x7ULL; }
    inline void setCornerPerm(int i, uint64_t val) { corners = (corners & ~(0x7ULL << (i * 3))) | (val << (i * 3)); }

    inline uint64_t getCornerOri(int i) const { return (corners >> (24 + i * 2)) & 0x3ULL; }
    inline void setCornerOri(int i, uint64_t val) { corners = (corners & ~(0x3ULL << (24 + i * 2))) | (val << (24 + i * 2)); }

    inline uint64_t getEdgePerm(int i) const { return (edges >> (i * 4)) & 0xFULL; }
    inline void setEdgePerm(int i, uint64_t val) { edges = (edges & ~(0xFULL << (i * 4))) | (val << (i * 4)); }

    inline uint64_t getEdgeOri(int i) const { return (edges >> (48 + i)) & 0x1ULL; }
    inline void setEdgeOri(int i, uint64_t val) { edges = (edges & ~(0x1ULL << (48 + i))) | (val << (48 + i)); }

    void applyMove(Move m);
    bool isSolved() const;

    static Cube fromScramble(const std::string& scramble);

private:
    void applyU();
    void applyD();
    void applyL();
    void applyR();
    void applyF();
    void applyB();


};
