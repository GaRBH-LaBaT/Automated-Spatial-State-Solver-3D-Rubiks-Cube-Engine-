#include "PatternDatabase.hpp"
#include <fstream>
#include <iostream>
#include <queue>
#include <cstring>
#include <vector>
#include <bit> // for std::popcount



// ---------------------------------------------------------
// CornerDB
// ---------------------------------------------------------
CornerDB::CornerDB() : table((88179840 + 1) / 2, 0xFF) {} // Nibble packed, initialized to 0xF (15)

uint32_t CornerDB::getIndex(const Cube& c) {
    constexpr uint32_t mult[7] = {5040, 720, 120, 24, 6, 2, 1};
    uint32_t rank = 0;
    uint32_t available = 0xFF; // 8 bits

    for (int i = 0; i < 7; i++) {
        int val = c.getCornerPerm(i);
        uint32_t mask = (1 << val) - 1;
        rank += std::popcount(available & mask) * mult[i];
        available &= ~(1 << val);
    }
    
    uint32_t ori = 0;
    for (int i = 0; i < 7; i++) {
        ori = ori * 3 + c.getCornerOri(i);
    }
    return rank * 2187 + ori;
}

uint8_t CornerDB::getEstimate(uint32_t index) const {
    uint8_t byte = table[index / 2];
    if (index % 2 == 0) return byte >> 4;
    return byte & 0x0F;
}

void CornerDB::loadOrGenerate(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (in) {
        std::cout << "Loading CornerDB from " << filename << "...\n";
        in.read(reinterpret_cast<char*>(table.data()), table.size());
        return;
    }
    generate();
    std::ofstream out(filename, std::ios::binary);
    out.write(reinterpret_cast<const char*>(table.data()), table.size());
}

void CornerDB::generate() {
    std::cout << "Generating CornerDB... this may take a minute.\n";
    Cube solved;
    std::queue<Cube> q;
    std::queue<uint8_t> depth_q;
    
    q.push(solved);
    depth_q.push(0);
    
    uint32_t solved_idx = getIndex(solved);
    table[solved_idx / 2] &= 0x0F; // set high nibble to 0
    uint32_t count = 1;
    
    Move moves[] = {U, U2, U3, D, D2, D3, L, L2, L3, R, R2, R3, F, F2, F3, B, B2, B3};
    
    while (!q.empty()) {
        Cube curr = q.front(); q.pop();
        uint8_t d = depth_q.front(); depth_q.pop();
        
        if (d == 11) continue; // Cutoff for generation speed, corners usually don't exceed 11
        
        for (Move m : moves) {
            Cube next = curr;
            next.applyMove(m);
            uint32_t idx = getIndex(next);
            
            uint8_t val = (idx % 2 == 0) ? (table[idx / 2] >> 4) : (table[idx / 2] & 0x0F);
            if (val == 0xF) { // Unvisited
                if (idx % 2 == 0) {
                    table[idx / 2] = (table[idx / 2] & 0x0F) | ((d + 1) << 4);
                } else {
                    table[idx / 2] = (table[idx / 2] & 0xF0) | (d + 1);
                }
                count++;
                q.push(next);
                depth_q.push(d + 1);
            }
        }
    }
    std::cout << "CornerDB generated. States reached: " << count << "\n";
}

// ---------------------------------------------------------
// Edge1DB (First 6 Edges)
// ---------------------------------------------------------
Edge1DB::Edge1DB() : table((42577920 + 1) / 2, 0xFF) {}

uint32_t Edge1DB::getIndex(const Cube& c) {
    int ep[6], eo[6];
    
    #define EXTRACT_E1(pos) do { int e = c.getEdgePerm(pos); if(e < 6) { ep[e] = pos; eo[e] = c.getEdgeOri(pos); } } while(0)
    EXTRACT_E1(0); EXTRACT_E1(1); EXTRACT_E1(2); EXTRACT_E1(3);
    EXTRACT_E1(4); EXTRACT_E1(5); EXTRACT_E1(6); EXTRACT_E1(7);
    EXTRACT_E1(8); EXTRACT_E1(9); EXTRACT_E1(10); EXTRACT_E1(11);
    
    constexpr uint32_t mult[6] = {55440, 5040, 504, 56, 7, 1};
    uint32_t rank = 0;
    uint32_t available = 0xFFF; // 12 bits

    for (int i = 0; i < 6; i++) {
        int val = ep[i];
        uint32_t mask = (1 << val) - 1;
        rank += std::popcount(available & mask) * mult[i];
        available &= ~(1 << val);
    }
    
    uint32_t ori = (eo[0] << 5) | (eo[1] << 4) | (eo[2] << 3) | (eo[3] << 2) | (eo[4] << 1) | eo[5];
    return rank * 64 + ori;
}

uint8_t Edge1DB::getEstimate(uint32_t index) const {
    uint8_t byte = table[index / 2];
    if (index % 2 == 0) return byte >> 4;
    return byte & 0x0F;
}

void Edge1DB::loadOrGenerate(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (in) {
        std::cout << "Loading Edge1DB from " << filename << "...\n";
        in.read(reinterpret_cast<char*>(table.data()), table.size());
        return;
    }
    generate();
    std::ofstream out(filename, std::ios::binary);
    out.write(reinterpret_cast<const char*>(table.data()), table.size());
}

void Edge1DB::generate() {
    std::cout << "Generating Edge1DB... this may take a few minutes.\n";
    Cube solved;
    std::queue<Cube> q;
    std::queue<uint8_t> depth_q;
    
    q.push(solved);
    depth_q.push(0);
    
    uint32_t solved_idx = getIndex(solved);
    table[solved_idx / 2] &= 0x0F; 
    uint32_t count = 1;
    
    Move moves[] = {U, U2, U3, D, D2, D3, L, L2, L3, R, R2, R3, F, F2, F3, B, B2, B3};
    
    while (!q.empty()) {
        Cube curr = q.front(); q.pop();
        uint8_t d = depth_q.front(); depth_q.pop();
        
        if (d == 10) continue; 
        
        for (Move m : moves) {
            Cube next = curr;
            next.applyMove(m);
            uint32_t idx = getIndex(next);
            
            uint8_t val = (idx % 2 == 0) ? (table[idx / 2] >> 4) : (table[idx / 2] & 0x0F);
            if (val == 0xF) { 
                if (idx % 2 == 0) table[idx / 2] = (table[idx / 2] & 0x0F) | ((d + 1) << 4);
                else table[idx / 2] = (table[idx / 2] & 0xF0) | (d + 1);
                count++;
                q.push(next);
                depth_q.push(d + 1);
            }
        }
    }
    std::cout << "Edge1DB generated. States reached: " << count << "\n";
}

// ---------------------------------------------------------
// Edge2DB (Last 6 Edges)
// ---------------------------------------------------------
Edge2DB::Edge2DB() : table((42577920 + 1) / 2, 0xFF) {}

uint32_t Edge2DB::getIndex(const Cube& c) {
    int ep[6], eo[6];
    
    #define EXTRACT_E2(pos) do { int e = c.getEdgePerm(pos); if(e >= 6) { ep[e - 6] = pos; eo[e - 6] = c.getEdgeOri(pos); } } while(0)
    EXTRACT_E2(0); EXTRACT_E2(1); EXTRACT_E2(2); EXTRACT_E2(3);
    EXTRACT_E2(4); EXTRACT_E2(5); EXTRACT_E2(6); EXTRACT_E2(7);
    EXTRACT_E2(8); EXTRACT_E2(9); EXTRACT_E2(10); EXTRACT_E2(11);
    
    constexpr uint32_t mult[6] = {55440, 5040, 504, 56, 7, 1};
    uint32_t rank = 0;
    uint32_t available = 0xFFF; // 12 bits

    for (int i = 0; i < 6; i++) {
        int val = ep[i];
        uint32_t mask = (1 << val) - 1;
        rank += std::popcount(available & mask) * mult[i];
        available &= ~(1 << val);
    }
    
    uint32_t ori = (eo[0] << 5) | (eo[1] << 4) | (eo[2] << 3) | (eo[3] << 2) | (eo[4] << 1) | eo[5];
    return rank * 64 + ori;
}

uint8_t Edge2DB::getEstimate(uint32_t index) const {
    uint8_t byte = table[index / 2];
    if (index % 2 == 0) return byte >> 4;
    return byte & 0x0F;
}

void Edge2DB::loadOrGenerate(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (in) {
        std::cout << "Loading Edge2DB from " << filename << "...\n";
        in.read(reinterpret_cast<char*>(table.data()), table.size());
        return;
    }
    generate();
    std::ofstream out(filename, std::ios::binary);
    out.write(reinterpret_cast<const char*>(table.data()), table.size());
}

void Edge2DB::generate() {
    std::cout << "Generating Edge2DB...\n";
    Cube solved;
    std::queue<Cube> q;
    std::queue<uint8_t> depth_q;
    
    q.push(solved);
    depth_q.push(0);
    
    uint32_t solved_idx = getIndex(solved);
    table[solved_idx / 2] &= 0x0F; 
    uint32_t count = 1;
    
    Move moves[] = {U, U2, U3, D, D2, D3, L, L2, L3, R, R2, R3, F, F2, F3, B, B2, B3};
    
    while (!q.empty()) {
        Cube curr = q.front(); q.pop();
        uint8_t d = depth_q.front(); depth_q.pop();
        
        if (d == 10) continue; 
        
        for (Move m : moves) {
            Cube next = curr;
            next.applyMove(m);
            uint32_t idx = getIndex(next);
            
            uint8_t val = (idx % 2 == 0) ? (table[idx / 2] >> 4) : (table[idx / 2] & 0x0F);
            if (val == 0xF) { 
                if (idx % 2 == 0) table[idx / 2] = (table[idx / 2] & 0x0F) | ((d + 1) << 4);
                else table[idx / 2] = (table[idx / 2] & 0xF0) | (d + 1);
                count++;
                q.push(next);
                depth_q.push(d + 1);
            }
        }
    }
    std::cout << "Edge2DB generated. States reached: " << count << "\n";
}
