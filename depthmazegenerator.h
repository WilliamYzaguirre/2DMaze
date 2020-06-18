#ifndef DEPTHMAZEGENERATOR_H
#define DEPTHMAZEGENERATOR_H

#include <maze.h>
#include <vector>
#include <random>

class DepthMazeGenerator
{
public:
    DepthMazeGenerator();

    void generateMaze(Maze& maze);

    void recursion(int x, int y, Maze& maze);

    bool doneCheck() const noexcept;

    bool stuck(int x, int y) const noexcept;

private:
    unsigned int** boardArray;

    std::vector<int> pastMoves;

    unsigned int maxx;
    unsigned int maxy;

    std::random_device device;
    std::default_random_engine engine;//{device()};

};

#endif // DEPTHMAZEGENERATOR_H
