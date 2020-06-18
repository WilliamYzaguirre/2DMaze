#include "mazesolution.h"

MazeSolution::MazeSolution(std::pair<int, int> start, std::pair<int,int> end, int width, int height)
    :startCell{start}, endCell{end}, width{width}, height{height}
{
    currentCell = startCell;
    cells.push_back(startCell);
}

int MazeSolution::getWidth()
{
    return width;
}

int MazeSolution::getHeight()
{
    return height;
}

std::pair<int, int> MazeSolution::getStartingCell()
{
    return startCell;
}

std::pair<int, int> MazeSolution::getEndCell()
{
    return endCell;
}

std::pair<int, int> MazeSolution::getCurrentCell()
{
    return currentCell;
}

std::vector<std::pair<int, int> > &MazeSolution::getCells()
{
    return cells;
}

std::vector<Direction> &MazeSolution::getMoves()
{
    return moves;
}

bool MazeSolution::isComplete()
{
    if (currentCell.first + 1 == endCell.first && currentCell.second + 1 == endCell.second)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MazeSolution::extend(Direction direction)
{
    switch (direction)
    {
    case Direction::up:
        emit moved(currentCell, Direction::up);
        currentCell.second -= 1;
        cells.push_back(currentCell);
        moves.push_back(Direction::up);
        break;

    case Direction::down:
        emit moved(currentCell, Direction::down);
        currentCell.second += 1;
        cells.push_back(currentCell);
        moves.push_back(Direction::down);
        break;
    case Direction::left:
        emit moved(currentCell, Direction::left);
        currentCell.first -= 1;
        cells.push_back(currentCell);
        moves.push_back(Direction::left);
        break;
    case Direction::right:
        emit moved(currentCell, Direction::right);
        currentCell.first += 1;
        cells.push_back(currentCell);
        moves.push_back(Direction::right);
        break;
    }
}

void MazeSolution::backUp()
{
    switch (moves.back())
    {
    case Direction::up:
        currentCell.second += 1;
        cells.pop_back();
        moves.pop_back();
        break;
    case Direction::down:
        currentCell.second -= 1;
        cells.pop_back();
        moves.pop_back();
        break;
    case Direction::left:
        currentCell.first += 1;
        cells.pop_back();
        moves.pop_back();
        break;
    case Direction::right:
        currentCell.first -= 1;
        cells.pop_back();
        moves.pop_back();
        break;
    }
    emit back();
}

void MazeSolution::restart()
{
    cells.clear();
    moves.clear();
    currentCell = startCell;
    cells.push_back(currentCell);
    emit restarted();
}


