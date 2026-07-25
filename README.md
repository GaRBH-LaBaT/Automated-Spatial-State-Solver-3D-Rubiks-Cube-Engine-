# 🧩 Automated Spatial State Solver & 3D Rubik's Cube Engine

<div align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/4/44/Rubiks_cube_solved.svg" width="200" alt="Rubik's Cube">
  <p><strong>A hyper-optimized, mathematically optimal C++ solver using IDA* and Pattern Databases.</strong></p>
</div>

## 📖 Overview
This project is an advanced, high-performance Artificial Intelligence engine designed to find the **absolute shortest path** to solve any scrambled Rubik's Cube. 

Based on Richard E. Korf's seminal 1997 AAAI paper *"Finding Optimal Solutions to Rubik's Cube Using Pattern Databases"*, this engine proves that the Rubik's Cube can be solved optimally in 20 moves or less using advanced heuristic search algorithms.

## ⚙️ Architectural Highlights
- **Zero-Overhead Abstractions**: Written in Modern C++ (C++20), utilizing deterministic memory management (RAII) and manual control over CPU cache lines.
- **Bitwise State Representation**: The entire 54-facelet cube is compressed into just two `uint64_t` (16 bytes) integers. Transitions occur via lightning-fast bitwise shift and mask operations.
- **Iterative Deepening A* (IDA*)**: Evaluates nodes strictly on the stack, requiring zero heap allocation in the hot loop ($O(d)$ space complexity), effectively preventing memory bandwidth thrashing.
- **Pattern Databases**: Pre-computes 42MB+ of heuristic databases using combinatorial ranks and Lehmer encoding for precise minimal perfect hashing.

## ⚠️ Important Note on Deep Scrambles
> [!CAUTION]
> **The 15-Move Bottleneck**
> 
> The IDA* algorithm scales exponentially via the equation $O(b^{d-h})$. While scrambles requiring up to 14 moves will be solved instantly, scrambles requiring 15 or more optimal moves will drastically slow down the engine. 
> 
> **For a 20-move worst-case scramble, the engine will safely output the absolute mathematically perfect path, but it may take several days of raw compute time to evaluate the trillions of nodes.** This is the expected mathematical limit of the hardware, not a flaw in the code!

## 🚀 Step-by-Step Compilation and Usage

This project uses **CMake** to compile the C++ source code into a runnable executable. Follow these exact steps to compile and run the engine on your machine.

### 1. Build the Engine
First, open your terminal in the root directory of this project and run the following commands to create the build files and compile the executable:

```bash
# Create a build directory and enter it
mkdir build
cd build

# Tell CMake to configure the project
cmake ..

# Compile the project into a Release executable
cmake --build . --config Release
```

### 2. Run the Solver
Once compiled, you can run the executable directly from the terminal. You must pass the scramble string inside quotation marks as an argument.

**For an easy scramble (Solves Instantly):**
```bash
.\Release\solver.exe "R2 U2 F2 D' L2 U B2 R2 F2 D R2 U' F' L'"
```

**For a hard scramble (Will take a long time!):**
```bash
.\Release\solver.exe "U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2"
```

## 🧠 Valid Moves
The scramble string uses standard WCA notation. The valid moves are:
- `U`, `D`, `L`, `R`, `F`, `B` (90-degree clockwise turns)
- `U2`, `D2`, `L2`, `R2`, `F2`, `B2` (180-degree turns)
- `U'`, `D'`, `L'`, `R'`, `F'`, `B'` (90-degree counter-clockwise turns, can also be written as `U3`, `D3`, etc.)
