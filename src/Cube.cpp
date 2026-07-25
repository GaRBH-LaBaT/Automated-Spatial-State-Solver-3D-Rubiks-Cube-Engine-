#include "Cube.hpp"
#include <sstream>
#include <stdexcept>

// Fast modulo arithmetic for small numbers
inline uint64_t fastMod3(uint64_t x) { return x >= 3 ? x - 3 : x; }
inline uint64_t fastMod2(uint64_t x) { return x & 1; }

// Compile-time generated parallel bit manipulation
template <int a, int b, int c, int d, int oriA, int oriB, int oriC, int oriD>
inline void cornerMultiply(uint64_t& corners) {
    uint64_t pA = (corners >> (a * 3)) & 7, oA = (corners >> (24 + a * 2)) & 3;
    uint64_t pB = (corners >> (b * 3)) & 7, oB = (corners >> (24 + b * 2)) & 3;
    uint64_t pC = (corners >> (c * 3)) & 7, oC = (corners >> (24 + c * 2)) & 3;
    uint64_t pD = (corners >> (d * 3)) & 7, oD = (corners >> (24 + d * 2)) & 3;

    constexpr uint64_t mask = ~((0x7ULL << (a * 3)) | (0x7ULL << (b * 3)) | (0x7ULL << (c * 3)) | (0x7ULL << (d * 3)) |
                                (0x3ULL << (24 + a * 2)) | (0x3ULL << (24 + b * 2)) | (0x3ULL << (24 + c * 2)) | (0x3ULL << (24 + d * 2)));

    corners = (corners & mask) |
              (pC << (d * 3)) | (fastMod3(oC + oriD) << (24 + d * 2)) |
              (pB << (c * 3)) | (fastMod3(oB + oriC) << (24 + c * 2)) |
              (pA << (b * 3)) | (fastMod3(oA + oriB) << (24 + b * 2)) |
              (pD << (a * 3)) | (fastMod3(oD + oriA) << (24 + a * 2));
}

template <int a, int b, int c, int d, int oriA, int oriB, int oriC, int oriD>
inline void edgeMultiply(uint64_t& edges) {
    uint64_t pA = (edges >> (a * 4)) & 15, oA = (edges >> (48 + a)) & 1;
    uint64_t pB = (edges >> (b * 4)) & 15, oB = (edges >> (48 + b)) & 1;
    uint64_t pC = (edges >> (c * 4)) & 15, oC = (edges >> (48 + c)) & 1;
    uint64_t pD = (edges >> (d * 4)) & 15, oD = (edges >> (48 + d)) & 1;

    constexpr uint64_t mask = ~((0xFULL << (a * 4)) | (0xFULL << (b * 4)) | (0xFULL << (c * 4)) | (0xFULL << (d * 4)) |
                                (0x1ULL << (48 + a)) | (0x1ULL << (48 + b)) | (0x1ULL << (48 + c)) | (0x1ULL << (48 + d)));

    edges = (edges & mask) |
            (pC << (d * 4)) | (fastMod2(oC + oriD) << (48 + d)) |
            (pB << (c * 4)) | (fastMod2(oB + oriC) << (48 + c)) |
            (pA << (b * 4)) | (fastMod2(oA + oriB) << (48 + b)) |
            (pD << (a * 4)) | (fastMod2(oD + oriA) << (48 + a));
}

// Solved state definition
Cube::Cube() {
    corners = 0;
    for (int i = 0; i < 8; i++) {
        setCornerPerm(i, i);
        setCornerOri(i, 0);
    }
    edges = 0;
    for (int i = 0; i < 12; i++) {
        setEdgePerm(i, i);
        setEdgeOri(i, 0);
    }
}

bool Cube::isSolved() const {
    static const Cube solved;
    return (corners == solved.corners) && (edges == solved.edges);
}

// -----------------------------------------------------
// Basic Face Turns
// Corners: 0:UBL, 1:UBR, 2:UFR, 3:UFL, 4:DBL, 5:DBR, 6:DFR, 7:DFL
// Edges: 0:UB, 1:UR, 2:UF, 3:UL, 4:DB, 5:DR, 6:DF, 7:DL, 8:BL, 9:BR, 10:FR, 11:FL
// -----------------------------------------------------

void Cube::applyU() {
    cornerMultiply<0, 1, 2, 3, 0, 0, 0, 0>(corners);
    edgeMultiply<0, 1, 2, 3, 0, 0, 0, 0>(edges);
}

void Cube::applyD() {
    cornerMultiply<7, 6, 5, 4, 0, 0, 0, 0>(corners);
    edgeMultiply<7, 6, 5, 4, 0, 0, 0, 0>(edges);
}

void Cube::applyL() {
    cornerMultiply<4, 0, 3, 7, 2, 1, 2, 1>(corners);
    edgeMultiply<8, 3, 11, 7, 0, 0, 0, 0>(edges);
}

void Cube::applyR() {
    cornerMultiply<1, 5, 6, 2, 1, 2, 1, 2>(corners);
    edgeMultiply<1, 9, 5, 10, 0, 0, 0, 0>(edges);
}

void Cube::applyF() {
    cornerMultiply<3, 2, 6, 7, 1, 2, 1, 2>(corners);
    edgeMultiply<2, 10, 6, 11, 1, 1, 1, 1>(edges);
}

void Cube::applyB() {
    cornerMultiply<0, 4, 5, 1, 1, 2, 1, 2>(corners);
    edgeMultiply<0, 8, 4, 9, 1, 1, 1, 1>(edges);
}

void Cube::applyMove(Move m) {
    if (m == NONE) return;
    int base = m / 3;
    int times = (m % 3) + 1;
    switch (base) {
        case 0: applyU(); if (times > 1) applyU(); if (times > 2) applyU(); break;
        case 1: applyD(); if (times > 1) applyD(); if (times > 2) applyD(); break;
        case 2: applyL(); if (times > 1) applyL(); if (times > 2) applyL(); break;
        case 3: applyR(); if (times > 1) applyR(); if (times > 2) applyR(); break;
        case 4: applyF(); if (times > 1) applyF(); if (times > 2) applyF(); break;
        case 5: applyB(); if (times > 1) applyB(); if (times > 2) applyB(); break;
    }
}

Move parseMove(const std::string& s) {
    if (s == "U") return U; if (s == "U2") return U2; if (s == "U'") return U3;
    if (s == "D") return D; if (s == "D2") return D2; if (s == "D'") return D3;
    if (s == "L") return L; if (s == "L2") return L2; if (s == "L'") return L3;
    if (s == "R") return R; if (s == "R2") return R2; if (s == "R'") return R3;
    if (s == "F") return F; if (s == "F2") return F2; if (s == "F'") return F3;
    if (s == "B") return B; if (s == "B2") return B2; if (s == "B'") return B3;
    return NONE;
}

std::string moveToString(Move m) {
    switch (m) {
        case U: return "U"; case U2: return "U2"; case U3: return "U'";
        case D: return "D"; case D2: return "D2"; case D3: return "D'";
        case L: return "L"; case L2: return "L2"; case L3: return "L'";
        case R: return "R"; case R2: return "R2"; case R3: return "R'";
        case F: return "F"; case F2: return "F2"; case F3: return "F'";
        case B: return "B"; case B2: return "B2"; case B3: return "B'";
        default: return "";
    }
}

Cube Cube::fromScramble(const std::string& scramble) {
    Cube c;
    std::stringstream ss(scramble);
    std::string token;
    while (ss >> token) {
        Move m = parseMove(token);
        if (m != NONE) {
            c.applyMove(m);
        }
    }
    return c;
}
