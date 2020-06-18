#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <maze.h>
#include <iostream>
#include <QThread>
#include <QRectF>
#include "Direction.h"
#include "depthmazegenerator.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
    DepthMazeGenerator* generator = new DepthMazeGenerator;
    mazeHeight = ui->heightSlider->value();
    mazeWidth = ui->widthSlider->value();
    innerWalls = new std::vector<std::pair<int, QGraphicsLineItem*>>[mazeWidth*mazeHeight];
    mazeScene->clear();
    Maze newMaze(mazeWidth, mazeHeight);
    connect(&newMaze, SIGNAL(allWallsAdded()), this, SLOT(addAllWalls()));
    connect(&newMaze, SIGNAL(allWallsRemoved()), this, SLOT(removeAllWalls()));
    connect(&newMaze, SIGNAL(wallAdded(int, int, Direction)), this, SLOT(addWall(int, int, Direction)));
    connect(&newMaze, SIGNAL(wallRemoved(int, int, Direction)), this, SLOT(removeWall(int, int, Direction)));
    generator->generateMaze(newMaze);
    delete generator;
}

void MainWindow::on_solveButton_clicked()
{

}

void MainWindow::on_animationToggle_clicked()
{
}

void MainWindow::updateView()
{
    mazeScene->update();
    ui->drawArea->update();
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
    mazeScene->update();
    ui->drawArea->update();
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
    QTimer* timer = new QTimer;
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateView()));
    timer->start(10000);
    //delete timer;
}




