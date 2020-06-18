#include "semirandommazesolver.h"
#include "Direction.h"
#include <QCoreApplication>
#include "mazesolution.h"
#include <iostream>



SemiRandomMazeSolver::SemiRandomMazeSolver()
{

}

void SemiRandomMazeSolver::solveMaze(const Maze& maze, MazeSolution& mazeSolution) {
    std::cout << "END: " << mazeSolution.getEndCell().first << ", " << mazeSolution.getEndCell().second << std::endl;
    std::cout << "Solver: started" <<std::endl;
    mazeSolution.restart();
    std::cout << "Solver: restarted" <<std::endl;
    int x = mazeSolution.getStartingCell().first;
    int y = mazeSolution.getStartingCell().second;

    max_x = mazeSolution.getWidth();
    max_y = mazeSolution.getHeight();

    //std::cout << mazeSolution.getEndingCell().first << " " << mazeSolution.getEndingCell().second << std::endl;

    deadEnd = new int*[max_x];
    for (int i = 0; i < max_x; ++i) {
        deadEnd[i] = new int[max_y];
    }

    for (int i = 0; i < max_x; i++) {
        for (int j = 0; j < max_y; j++) {
            deadEnd[i][j] = 0;
        }
    }

    std::cout << "Solver: recursion starting" <<std::endl;
    recursionSolver(x, y, maze, mazeSolution);
    std::cout << "Solver: recursion done" <<std::endl;

    //for (int i = 0; i < max_x; i++) {
    //    delete[] deadEnd[i];
    //}
    //delete[] deadEnd;
}

bool SemiRandomMazeSolver::stuck(int x, int y, const Maze& maze) {
    int exist = 0;
    int block = 0;
    if (y - 1 >= 0) {
        exist++;
        if (maze.wallExists(x, y, Direction::up) || deadEnd[x][y - 1] == 1) {
            block++;
        }
    }
    if (y + 1 < max_y) {
        exist++;
        if (maze.wallExists(x, y, Direction::down) || deadEnd[x][y+1] == 1) {
            block++;
        }
    }
    if (x - 1 >= 0) {
        exist++;
        if (maze.wallExists(x, y, Direction::left)  || deadEnd[x-1][y] == 1) {
            block++;
        }
    }
    if (x + 1 < max_x) {
        exist++;
        if (maze.wallExists(x, y, Direction::right) || deadEnd[x+1][y] == 1) {
            block++;
        }
    }
    if (exist == block) {return true;}
    else {return false;}
}

void SemiRandomMazeSolver::recursionSolver(int x, int y, const Maze& maze, MazeSolution& mazeSolution) {
    QCoreApplication::processEvents();
    int random;
    std::discrete_distribution<int> distribution{1, 5, 1, 5};
    while (!mazeSolution.isComplete()) {
        std::cout << "Solver: loop started" <<std::endl;
        random = distribution(engine);
        deadEnd[x][y] = 1;

        switch(random) {
            case 0: //up
                std::cout << "Solver: case 0" <<std::endl;
                if (y - 1 >= 0) {
                    if (!maze.wallExists(x, y, Direction::up) && deadEnd[x][y-1] != 1) {
                        mazeSolution.extend(Direction::up);
                        //std::cout << mazeSolution.getCurrentCell().first << " " << mazeSolution.getCurrentCell().second << std::endl;
                        directions.push_back(Direction::up);
                        recursionSolver(x, y - 1, maze, mazeSolution);
                    }
                }
                break;
            case 1: //down
                std::cout << "Solver: case 1" <<std::endl;
                if (y + 1 < max_y) {
                    if (!maze.wallExists(x, y, Direction::down) && deadEnd[x][y+1] != 1) {
                        mazeSolution.extend(Direction::down);
                        //std::cout << mazeSolution.getCurrentCell().first << " " << mazeSolution.getCurrentCell().second << std::endl;
                        directions.push_back(Direction::down);
                        recursionSolver(x, y + 1, maze, mazeSolution);
                    }
                }
                break;
            case 2: //left
                std::cout << "Solver: case 2" <<std::endl;
                if (x - 1 >= 0) {
                    if (!maze.wallExists(x, y, Direction::left) && deadEnd[x - 1][y] != 1) {
                        mazeSolution.extend(Direction::left);
                        //std::cout << mazeSolution.getCurrentCell().first << " " << mazeSolution.getCurrentCell().second << std::endl;
                        directions.push_back(Direction::left);
                        recursionSolver(x - 1, y, maze, mazeSolution);
                    }
                }
                break;
            case 3: //right
                std::cout << "Solver: case 3" <<std::endl;
                if (x + 1 < max_x) {
                    if (!maze.wallExists(x, y, Direction::right) && deadEnd[x + 1][y] != 1) {
                        mazeSolution.extend(Direction::right);
                        //std::cout << mazeSolution.getCurrentCell().first << " " << mazeSolution.getCurrentCell().second << std::endl;
                        directions.push_back(Direction::right);
                        recursionSolver(x + 1, y, maze, mazeSolution);
                    }
                }
                break;
        }
        if (stuck(x, y, maze) && !mazeSolution.isComplete()) {
            mazeSolution.backUp();
            switch(directions.back()) {
                case Direction::up:
                    directions.pop_back();
                    recursionSolver(x, y+1, maze, mazeSolution);
                    break;
                case Direction::down:
                    directions.pop_back();
                    recursionSolver(x, y-1, maze, mazeSolution);
                    break;
                case Direction::left:
                    directions.pop_back();
                    recursionSolver(x+1, y, maze, mazeSolution);
                    break;
                case Direction::right:
                    directions.pop_back();
                    recursionSolver(x-1, y, maze, mazeSolution);
                    break;
            }
        }
    }
}
