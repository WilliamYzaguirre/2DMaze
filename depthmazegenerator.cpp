#include "depthmazegenerator.h"
#include <iostream>
#include <QCoreApplication>
#include <QThread>

DepthMazeGenerator::DepthMazeGenerator()
    :engine{device()}
{

}

void DepthMazeGenerator::generateMaze(Maze &maze)
{
    maze.addAllWalls();

    QCoreApplication::processEvents();

    int currentx = 0;
    int currenty = 0;

    maxx = maze.getWidth();
    maxy = maze.getHeight();

    boardArray = new unsigned int*[maxx];
    for (int i = 0; i < maxx; ++i)
    {
        boardArray[i] = new unsigned int[maxy];
    }

    for (int i = 0; i < maxx; ++i)
    {
        for (int j = 0; j < maxy; ++j)
        {
            boardArray[i][j] = 0;
        }
    }

    recursion(currentx, currenty, maze);

    //for (int i = 0; i < maxx; ++i)
    //{
    //    for (int j = 0; j < maxy; ++j)
    //    {
    //        delete[] boardArray[i];
    //    }
    //    delete[] boardArray;
    //}

}

bool DepthMazeGenerator::stuck(int x, int y) const noexcept
{
    bool stuck = true;
    if (y - 1 >= 0)
    {
        if (boardArray[x][y-1] == 0)
        {
            stuck = false;
        }
    }
    if (y + 1 < maxy)
    {
        if (boardArray[x][y+1] == 0)
        {
            stuck = false;
        }
    }
    if (x - 1 >= 0)
    {
        if (boardArray[x-1][y] == 0)
        {
            stuck = false;
        }
    }
    if (x + 1 < maxx)
    {
        if (boardArray[x+1][y] == 0)
        {
            stuck = false;
        }
    }
    return stuck;
}

//this can be sped up by keeping count of how many spaces visited
bool DepthMazeGenerator::doneCheck() const noexcept
{
    bool check = true;
    for (int i = 0; i < maxx; ++i)
    {
        for (int j = 0; j < maxy; ++j)
        {
            if (boardArray[i][j] == 0)
            {
                check = false;
            }
        }
    }
    return check;
}

void DepthMazeGenerator::recursion(int x, int y, Maze &maze)
{
    QCoreApplication::processEvents();
    int random;
    std::uniform_int_distribution<int> distribution{0,4};
    while (!doneCheck())
    {
        random = distribution(engine);
        boardArray[x][y] = 1;

        switch (random)
        {
        case 0: //up
            if (y - 1 >= 0) {
                if (boardArray[x][y-1] != 1)
                {
                    maze.removeWall(x,y,Direction::up);
                    pastMoves.push_back(0);
                    recursion(x,y-1,maze);
                }
            }
            break;

        case 1: //down
            if (y + 1 < maxy) {
                if (boardArray[x][y+1] != 1)
                {
                    maze.removeWall(x,y,Direction::down);
                    pastMoves.push_back(1);
                    recursion(x,y+1,maze);
                }
            }
            break;


        case 2: //left
            if (x - 1 >= 0) {
                if (boardArray[x-1][y] != 1)
                {
                    maze.removeWall(x,y,Direction::left);
                    pastMoves.push_back(2);
                    recursion(x-1,y,maze);
                }
            }
            break;

        case 3: //right
            if (x + 1 < maxx) {
                if (boardArray[x+1][y] != 1)
                {
                    maze.removeWall(x,y,Direction::right);
                    pastMoves.push_back(3);
                    recursion(x+1,y,maze);
                }
            }
            break;
        }

        if (stuck(x,y))
        {
            switch (pastMoves.back())
            {
            case 0:
                pastMoves.pop_back();
                recursion(x,y+1,maze);
                break;

            case 1:
                pastMoves.pop_back();
                recursion(x,y-1,maze);
                break;

            case 2:
                pastMoves.pop_back();
                recursion(x+1,y,maze);
                break;

            case 3:
                pastMoves.pop_back();
                recursion(x-1,y,maze);
                break;
            }
        }
    }
}
