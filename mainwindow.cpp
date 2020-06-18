#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <maze.h>
#include <iostream>
#include <QThread>
#include <QRectF>
#include "Direction.h"
#include "depthmazegenerator.h"
#include <QTimer>
#include "semirandommazesolver.h"
#include "mazesolution.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), mazeGenerated{false}, mazeSolved{false}
{
    ui->setupUi(this);
    mazeScene = new QGraphicsScene;
    ui->drawArea->setScene(mazeScene);
    ui->drawArea->show();
    //walls = new GuiCell[ui->widthSlider->value() * ui->heightSlider->value()];
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mazeScene;
}

void MainWindow::printWalls()
{
    for (int i = 0; i < mazeWidth * mazeHeight; ++i)
    {
        std::cout << i << ": ";
        for (int j = 0; j < innerWalls[i].size(); ++j)
        {
            std::cout << "(" << i << ", " << innerWalls[i][j].first << "), ";
        }
        std::cout << std::endl;
    }
}

void MainWindow::animateMazeGeneration()
{

}

void MainWindow::on_generateButton_clicked()
{
    DepthMazeGenerator generator;
    mazeHeight = ui->heightSlider->value();
    mazeWidth = ui->widthSlider->value();
    innerWalls = new std::vector<std::pair<int, QGraphicsLineItem*>>[mazeWidth*mazeHeight];
    mazeScene->clear();
    if (mazeGenerated == true)
    {
        delete maze;
        mazeGenerated = false;
    }
    maze = new Maze(mazeWidth, mazeHeight);
    connect(maze, SIGNAL(allWallsAdded()), this, SLOT(addAllWalls()));
    connect(maze, SIGNAL(allWallsRemoved()), this, SLOT(removeAllWalls()));
    connect(maze, SIGNAL(wallAdded(int, int, Direction)), this, SLOT(addWall(int, int, Direction)));
    connect(maze, SIGNAL(wallRemoved(int, int, Direction)), this, SLOT(removeWall(int, int, Direction)));
    generator.generateMaze(*maze);
    mazeGenerated = true;
}

void MainWindow::on_solveButton_clicked()
{
    if (mazeGenerated == true)
    {
        if (mazeSolved == true)
        {
            solutionRestarted();
        }
        SemiRandomMazeSolver solver;
        MazeSolution solution(std::make_pair(0,0), std::make_pair(mazeWidth, mazeHeight), mazeWidth, mazeHeight);
        connect(&solution, SIGNAL(moved(std::pair<int,int>, Direction)), this, SLOT(solutionMoved(std::pair<int,int>, Direction)));
        connect(&solution, SIGNAL(back()), this, SLOT(solutionBack()));
        connect(&solution, SIGNAL(restarted()), this, SLOT(solutionRestarted()));
        solver.solveMaze(*maze, solution);
    }
}

void MainWindow::on_animationToggle_clicked()
{
}

void MainWindow::addAllWalls()
{
    int start = 30;
    int end = 670;
    int xincrement = (end - start) / mazeWidth;
    int yincrement = (end - start) / mazeHeight;
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    for (int i = 0; i < mazeHeight; ++i)
    {
        for (int j = 0; j < mazeWidth; ++j)
        {
            int cellLoc = i * mazeWidth + j;
            /////////////////////////First, check if outer wall needs to be added/////////////////////////
            //if you're on the left side of the 2d array
            if (cellLoc % mazeWidth == 0)
            {
                QGraphicsLineItem* line = mazeScene->addLine(double(start + j * xincrement), double(start + i * yincrement), double(start + j * xincrement), double(start + i * yincrement + yincrement), pen);
                allWalls.push_back(line);
            }

            //if you're on the first row
            if (cellLoc - mazeWidth < 0)
            {
                QGraphicsLineItem* line = mazeScene->addLine(double(start + j * xincrement), double(start + i * yincrement), double(start + j * xincrement + xincrement), double(start + i * yincrement), pen);
                allWalls.push_back(line);
            }

            //if you're on the last row
            if (cellLoc + mazeWidth >= mazeWidth*mazeHeight)
            {
                QGraphicsLineItem* line = mazeScene->addLine(double(start+j*xincrement), double(start + i * yincrement + yincrement), double(start+j*xincrement+xincrement),double(start + i * yincrement + yincrement), pen);
                allWalls.push_back(line);
            }

            //if you're on the far right
            if (cellLoc % mazeWidth == mazeWidth - 1)
            {
                QGraphicsLineItem* line = mazeScene->addLine(double(start+j*xincrement + xincrement), double(start + i * yincrement), double(start+j*xincrement + xincrement), double(start + i * yincrement + yincrement), pen);
                allWalls.push_back(line);
            }

            /////////////////////////////Then, check if inner wall needs to be added/////////////////////////////
            //if you're not on the left side of the 2d array, left line

            if (cellLoc % mazeWidth != 0)
            {
                //check if graph edge already exists by checking for the reverse,
                //if so, make the line of this pair the same as reverse pair
                bool found = false;
                if (innerWalls[cellLoc-1].size() != 0)
                {
                    for (unsigned int k = 0; k < innerWalls[cellLoc-1].size(); ++k)
                    {
                        if (innerWalls[cellLoc-1][k].first == cellLoc)
                        {
                            std::pair<int, QGraphicsLineItem*> temp = std::make_pair(cellLoc-1, innerWalls[cellLoc-1][k].second);
                            innerWalls[cellLoc].push_back(temp);
                            found = true;
                        }
                    }
                }
                //if the reverse pair doesn't exist, make a new line
                if (found == false)
                {
                    QGraphicsLineItem* line = mazeScene->addLine(double(start + j * xincrement), double(start + i * yincrement), double(start + j * xincrement), double(start + i * yincrement + yincrement), pen);
                    innerWalls[cellLoc].push_back(std::make_pair(cellLoc-1, line));
                    allWalls.push_back(line);
                }
            }

            //if you're not on the first row, top line
            if (cellLoc - mazeWidth >= 0)
            {
                //check if graph edge already exists by checking for the reverse,
                //if so, make the line of this pair the same as reverse pair
                bool found = false;
                if (innerWalls[cellLoc-mazeWidth].size() != 0)
                {
                    for (unsigned int k = 0; k < innerWalls[cellLoc-mazeWidth].size(); ++k)
                    {
                        if (innerWalls[cellLoc-mazeWidth][k].first == cellLoc)
                        {
                            std::pair<int, QGraphicsLineItem*> temp = std::make_pair(cellLoc-mazeWidth, innerWalls[cellLoc-mazeWidth][k].second);
                            innerWalls[cellLoc].push_back(temp);
                            found = true;
                        }
                    }
                }
                //if the reverse pair doesn't exist, make a new line
                if (found == false)
                {
                    QGraphicsLineItem* line = mazeScene->addLine(double(start + j * xincrement), double(start + i * yincrement), double(start + j * xincrement + xincrement), double(start + i * yincrement), pen);
                    innerWalls[cellLoc].push_back(std::make_pair(cellLoc-mazeWidth, line));
                    allWalls.push_back(line);

                }
                //innerWalls[i].push_back(i-width);
            }

            //if you're not on the last row, bottom line
            if (cellLoc + mazeWidth < mazeWidth*mazeHeight)
            {
                //check if graph edge already exists by checking for the reverse,
                //if so, make the line of this pair the same as reverse pair
                bool found = false;
                if (innerWalls[cellLoc+mazeWidth].size() != 0)
                {
                    for (unsigned int k = 0; k < innerWalls[cellLoc+mazeWidth].size(); ++k)
                    {
                        if (innerWalls[cellLoc+mazeWidth][k].first == cellLoc)
                        {
                            std::pair<int, QGraphicsLineItem*> temp = std::make_pair(cellLoc+mazeWidth, innerWalls[cellLoc+mazeWidth][k].second);
                            innerWalls[cellLoc].push_back(temp);
                            found = true;
                        }
                    }
                }
                //if the reverse pair doesn't exist, make a new line
                if (found == false)
                {
                    QGraphicsLineItem* line = mazeScene->addLine(double(start + j * xincrement), double(start + i * yincrement + yincrement), double(start + j * xincrement + xincrement), double(start + i * yincrement + yincrement), pen);
                    innerWalls[cellLoc].push_back(std::make_pair(cellLoc+mazeWidth, line));
                    allWalls.push_back(line);
                }
                //innerWalls[i].push_back(i+mazeWidth);
            }

            //if you're not on the far right

            if (cellLoc % mazeWidth != (mazeWidth - 1))
            {
                //check if graph edge already exists by checking for the reverse,
                //if so, make the line of this pair the same as reverse pair
                bool found = false;
                if (innerWalls[cellLoc+1].size() != 0)
                {
                    for (unsigned int k = 0; k < innerWalls[cellLoc+1].size(); ++k)
                    {
                        if (innerWalls[cellLoc+1][k].first == cellLoc)
                        {
                            std::pair<int, QGraphicsLineItem*> temp = std::make_pair(cellLoc+1, innerWalls[cellLoc+1][k].second);
                            innerWalls[cellLoc].push_back(temp);
                            found = true;
                        }
                    }
                }
                //if the reverse pair doesn't exist, make a new line
                if (found == false)
                {
                    QGraphicsLineItem* line = mazeScene->addLine(double(start + j * xincrement + xincrement), double(start + i * yincrement), double(start + j * xincrement + xincrement), double(start + i * yincrement + yincrement), pen);
                    innerWalls[cellLoc].push_back(std::make_pair(cellLoc+1, line));
                    allWalls.push_back(line);
                }
                //innerWalls[i].push_back(i+1);
            }

        }
    }
    if (ui->animationToggle->isChecked())
    {
        QThread::msleep(550 - ui->animationSlider->value());
    }
}

void MainWindow::removeAllWalls()
{

}

void MainWindow::addWall(int x, int y, Direction direction)
{

}

void MainWindow::removeWall(int x, int y, Direction direction)
{
    //struct wallRemoval wall{x,y,direction};
    //wallsToRemove.push(wall);
    int startCellLoc = y * mazeWidth + x;
    int endCellLoc;
    switch (direction)
    {
    case Direction::up:
        endCellLoc = startCellLoc - mazeWidth;
        break;

    case Direction::down:
        endCellLoc = startCellLoc + mazeWidth;
        break;

    case Direction::left:
        endCellLoc = startCellLoc - 1;
        break;

    case Direction::right:
        endCellLoc = startCellLoc + 1;
        break;
    }

    for (int i = 0; i < innerWalls[startCellLoc].size(); ++i)
    {
        if (innerWalls[startCellLoc][i].first == endCellLoc)
        {
            mazeScene->removeItem(innerWalls[startCellLoc][i].second);
            innerWalls[startCellLoc][i].second = nullptr;
        }
    }
    for (int i = 0; i < innerWalls[endCellLoc].size(); ++i)
    {
        if (innerWalls[endCellLoc][i].first == startCellLoc)
        {
            innerWalls[endCellLoc][i].second = nullptr;
        }
    }
    if (ui->animationToggle->isChecked())
    {
        QThread::msleep(550 - ui->animationSlider->value());
    }
}

void MainWindow::solutionMoved(std::pair<int,int> cell, Direction direction)
{
    int start = 30;
    int end = 670;
    int xincrement = (end - start) / mazeWidth;
    int yincrement = (end - start) / mazeHeight;
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    switch (direction)
    {
    case Direction::up:
        solutionMoves.push_back(mazeScene->addLine(double(start + cell.first * xincrement + (.5 * xincrement)), double(start + cell.second * yincrement + (.5 * yincrement)), double(start + cell.first * xincrement + (.5 * xincrement)), double(start + cell.second * yincrement + (.5 * yincrement) - yincrement), pen));
        break;
    case Direction::down:
        solutionMoves.push_back(mazeScene->addLine(double(start + cell.first * xincrement + (.5 * xincrement)), double(start + cell.second * yincrement + (.5 * yincrement)), double(start + cell.first * xincrement + (.5 * xincrement)), double(start + cell.second * yincrement + (.5 * yincrement) + yincrement), pen));
        break;
    case Direction::left:
        solutionMoves.push_back(mazeScene->addLine(double(start + cell.first * xincrement + (.5 * xincrement)), double(start + cell.second * yincrement + (.5 * yincrement)), double(start + cell.first * xincrement + (.5 * xincrement) - xincrement), double(start + cell.second * yincrement + (.5 * yincrement)), pen));
        break;
    case Direction::right:
        solutionMoves.push_back(mazeScene->addLine(double(start + cell.first * xincrement + (.5 * xincrement)), double(start + cell.second * yincrement + (.5 * yincrement)), double(start + cell.first * xincrement + (.5 * xincrement) + xincrement), double(start + cell.second * yincrement + (.5 * yincrement)), pen));
        break;
    }
    if (ui->animationToggle->isChecked())
    {
        QThread::msleep(550 - ui->animationSlider->value());
    }
}

void MainWindow::solutionBack()
{
    mazeScene->removeItem(solutionMoves.back());
    solutionMoves.pop_back();
    if (ui->animationToggle->isChecked())
    {
        QThread::msleep(550 - ui->animationSlider->value());
    }
}

void MainWindow::solutionRestarted()
{
    for (int i = 0; i < solutionMoves.size(); ++i)
    {
        mazeScene->removeItem(solutionMoves.back());
        solutionMoves.pop_back();
    }
}




