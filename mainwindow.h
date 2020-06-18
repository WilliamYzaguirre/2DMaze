#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include "Direction.h"
#include <memory>
#include <queue>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct wallRemoval
{
    int x;
    int y;
    Direction direction;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void printWalls();

    void animateMazeGeneration();

private slots:
    void on_generateButton_clicked();

    void on_solveButton_clicked();

    void on_animationToggle_clicked();

    void updateView();

public slots:
    void addAllWalls();
    void removeAllWalls();
    void addWall(int x, int y, Direction direction);
    void removeWall(int x, int y, Direction direction);

private:
    Ui::MainWindow *ui;
    QGraphicsScene* mazeScene;
    //QGraphicsLineItem** walls;
    std::vector<std::pair<int, QGraphicsLineItem*>>* innerWalls;
    std::vector<QGraphicsLineItem*> allWalls;
    int mazeWidth;
    int mazeHeight;
    std::queue<wallRemoval> wallsToRemove;
};
#endif // MAINWINDOW_H
