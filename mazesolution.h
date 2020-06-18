#ifndef MAZESOLUTION_H
#define MAZESOLUTION_H

#include <utility>
#include <vector>
#include <Direction.h>
#include <QObject>

class MazeSolution : public QObject
{
    Q_OBJECT
public:
    MazeSolution(std::pair<int, int> start, std::pair<int,int> end, int width, int height);

    int getWidth();

    int getHeight();

    std::pair<int,int> getStartingCell();

    std::pair<int,int> getEndCell();

    std::pair<int,int> getCurrentCell();

    std::vector<std::pair<int,int>>& getCells();

    std::vector<Direction>& getMoves();

    bool isComplete();

    void extend(Direction direction);

    void backUp();

    void restart();

signals:
    void moved(std::pair<int,int> cell, Direction direction);
    void back();
    void restarted();

private:
    std::pair<int,int> startCell;
    std::pair<int,int> endCell;

    int width;
    int height;

    std::pair<int,int> currentCell;

    std::vector<std::pair<int,int>> cells;
    std::vector<Direction> moves;
};

#endif // MAZESOLUTION_H
