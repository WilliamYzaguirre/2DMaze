#include "maze.h"
#include <iostream>

Maze::Maze(int width, int height)
    :width{width}, height{height}, walls{new std::vector<int>[width*height]}
{

}

Maze::~Maze()
{
}

int Maze::getWidth() const noexcept
{
    return width;
}

int Maze::getHeight() const noexcept
{
    return height;
}

bool Maze::wallExists(int x, int y, Direction direction)
{
    int startCellLoc = y * width + x;
    int endCellLoc;
    switch (direction)
    {
    case Direction::up:
        endCellLoc = startCellLoc - width;
        break;

    case Direction::down:
        endCellLoc = startCellLoc + width;
        break;

    case Direction::left:
        endCellLoc = startCellLoc - 1;
        break;

    case Direction::right:
        endCellLoc = startCellLoc + 1;
        break;
    }

    for (int i = 0; i < walls[startCellLoc].size(); ++i)
    {
        if (walls[startCellLoc][i] == endCellLoc)
        {
            return true;
        }
    }
    return false;
}

void Maze::addWall(int x, int y, Direction direction)
{
    if (wallExists(x, y, direction))
    {
        return;
    }
    int startCellLoc = y * width + x;
    int endCellLoc;
    switch (direction)
    {
    case Direction::up:
        endCellLoc = startCellLoc - width;
        break;

    case Direction::down:
        endCellLoc = startCellLoc + width;
        break;

    case Direction::left:
        endCellLoc = startCellLoc - 1;
        break;

    case Direction::right:
        endCellLoc = startCellLoc + 1;
        break;
    }
    walls[startCellLoc].push_back(endCellLoc);
    walls[endCellLoc].push_back(startCellLoc);
    emit wallAdded(x, y, direction);
}

void Maze::removeWall(int x, int y, Direction direction)
{
    if (!wallExists(x,y,direction))
    {
        return;
    }
    int startCellLoc = y * width + x;
    int endCellLoc;
    switch (direction)
    {
    case Direction::up:
        endCellLoc = startCellLoc - width;
        break;

    case Direction::down:
        endCellLoc = startCellLoc + width;
        break;

    case Direction::left:
        endCellLoc = startCellLoc - 1;
        break;

    case Direction::right:
        endCellLoc = startCellLoc + 1;
        break;
    }

    for (int i = 0; i < walls[startCellLoc].size(); ++i)
    {
        if (walls[startCellLoc][i] == endCellLoc)
        {
            walls[startCellLoc].erase(walls[startCellLoc].begin() + i);
        }
    }
    for (int i = 0; i < walls[endCellLoc].size(); ++i)
    {
        if (walls[endCellLoc][i] == startCellLoc)
        {
            walls[endCellLoc].erase(walls[endCellLoc].begin() + i);
        }
    }
    emit wallRemoved(x, y, direction);
}

void Maze::addAllWalls()
{
    for (int i = 0; i < width * height; ++i)
    {
        //if you're not on the left side of the 2d array
        if (i % width != 0)
        {
            walls[i].push_back(i-1);
        }
        //if you're not on the first row
        if (i - width >= 0)
        {
            walls[i].push_back(i-width);
        }
        //if you're not on the last row
        if (i + width < width*height)
        {
            walls[i].push_back(i+width);
        }
        //if you're not on the far right
        if (i % width != (width - 1))
        {
            walls[i].push_back(i+1);
        }
    }
    emit allWallsAdded();
}

void Maze::removeAllWalls()
{
    for (int i = 0; i < width * height; ++i)
    {
        for (int j = 0 ; j < walls[i].size(); ++j)
        {
            walls[i].pop_back();
        }
    }
    emit allWallsRemoved();
}

Cell* Maze::getMaze()
{
    //return maze;
}

void Maze::printMaze()
{
    for (int i = 0; i < width*height; ++i)
    {
        std::cout << i << ": ";
        for (int j = 0; j < walls[i].size(); ++j)
        {
            std::cout << "(" << i << ", " << walls[i][j] << "), ";
        }
        std::cout << std::endl;
    }
}
