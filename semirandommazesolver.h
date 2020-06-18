#ifndef SEMIRANDOMMAZESOLVER_H
#define SEMIRANDOMMAZESOLVER_H

#include "maze.h"
#include "random"
#include "mazesolution.h"

class SemiRandomMazeSolver
{
public:
    SemiRandomMazeSolver();

    void solveMaze(const Maze& maze, MazeSolution& mazeSolution);

    void recursionSolver(int x, int y, const Maze& maze, MazeSolution& mazeSolution);

    bool stuck(int x, int y, const Maze& maze);

private:
    int** deadEnd;

    unsigned int max_x;
    unsigned int max_y;

    std::vector<Direction> directions;

    std::random_device device;
    std::default_random_engine engine{device()};
};

#endif // SEMIRANDOMMAZESOLVER_H
