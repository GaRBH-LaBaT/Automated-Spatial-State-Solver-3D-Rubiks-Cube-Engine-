#pragma once
#include "Cube.hpp"
#include <vector>
#include <cstdint>
#include <string>

class PatternDatabase {
public:
    virtual ~PatternDatabase() = default;
    
    // Get heuristic value for a specific index
    virtual uint8_t getEstimate(uint32_t index) const = 0;
    
    // Load from disk or generate if missing
    virtual void loadOrGenerate(const std::string& filename) = 0;
};

class CornerDB : public PatternDatabase {
public:
    CornerDB();
    uint8_t getEstimate(uint32_t index) const override;
    void loadOrGenerate(const std::string& filename) override;
    
    static uint32_t getIndex(const Cube& c);
private:
    std::vector<uint8_t> table; // 88,179,840 bytes -> ~84 MB. Can be packed to nibbles (42 MB).
    void generate();
};

class Edge1DB : public PatternDatabase {
public:
    Edge1DB();
    uint8_t getEstimate(uint32_t index) const override;
    void loadOrGenerate(const std::string& filename) override;
    
    static uint32_t getIndex(const Cube& c);
private:
    std::vector<uint8_t> table; // 42,577,920 bytes -> ~40.6 MB. Can be packed to nibbles (20.3 MB).
    void generate();
};

class Edge2DB : public PatternDatabase {
public:
    Edge2DB();
    uint8_t getEstimate(uint32_t index) const override;
    void loadOrGenerate(const std::string& filename) override;
    
    static uint32_t getIndex(const Cube& c);
private:
    std::vector<uint8_t> table; // 42,577,920 bytes.
    void generate();
};
