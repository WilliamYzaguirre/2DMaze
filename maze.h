#ifndef MAZE_H
#define MAZE_H

#include "Direction.h"
#include <QObject>

struct Cell
{
    bool* up = nullptr;
    bool* down = nullptr;
    bool* left = nullptr;
    bool* right = nullptr;
};

class Maze : public QObject
{
    Q_OBJECT

public:
    Maze(int width, int height);

    ~Maze();

    int getWidth() const noexcept;

    int getHeight() const noexcept;

    bool wallExists(int x, int y, Direction direction) const;

    void addWall(int x, int y, Direction direction);

    void removeWall(int x, int y, Direction direction);

    void addAllWalls();

    void removeAllWalls();

    Cell* getMaze();

    void printMaze();

signals:
    void wallRemoved(int x, int y, Direction direction);
    void wallAdded(int x, int y, Direction direction);
    void allWallsAdded();
    void allWallsRemoved();

private:
    int width;
    int height;
    std::vector<int>* walls;

    //Cell* maze;
};

#endif // MAZE_H
