#pragma once

#include <QPainter>
#include <QDebug>

#include "mazeio.h"

class Window;


// алгоритм рекурсивного деления
namespace recursive_divide {

  // https://ru.qwe.wiki/wiki/Maze_generation_algorithm#Simple_algorithms

  // Исходный лабиринт без стен. Назовем это камера. 
  // Камера делится произвольной парой стен. В трех из четырех новых стенок
  // делаем случайный проход. Затем рекурсивно повторить процесс для каждой 
  // камеры, пока все камеры не достигнут минимальный размер. 
  // 
  // В прямоугольном лабиринте, построить в случайных точках две стенки, 
  // которые перпендикулярны друг другу. Эти две стены разделить большую 
  // камеру на четыре меньших камер, разделенных четырьмя стенами. Выберите 
  // три из четырех стен случайным образом, и открыть одну клеточную ширину
  // отверстия в случайной точке в каждом из трех. Продолжайте таким образом,
  // рекурсивно, пока каждая камера не имеет ширину одной ячейки в любом из 
  // двух направлений.

  // камера ( [ro;row+height-1] [col;col+width-1])
  struct grid_s {
    int row, col, countRow, countCol;
    //
    grid_s(int _row, int _col, int _countRow, int _countCol) :
      row(_row), col(_col), countRow(_countRow), countCol(_countCol) {}
    //
    grid_s() :
      row(0), col(0), countRow(0), countCol(0) {}

    // построить горизонтальную стену вверху или внизу
    void buildHWall(mazeio::IO *pIO, uchar orientation);
    // построить вертикальную стену слева или справа
    void buildVWall(mazeio::IO *pIO, uchar orientation);
    // проделать дырку
    void makeHole(mazeio::IO *pIO, int posStart, uchar orientation);
    //
    void debugPrint() {
      // qDebug() << "r=" << row << ";" << countRow << " c=" << col << ";" << countCol;
    }
  };

  // лабиринт
  struct generator : mazeio::generator {

    //
    generator() : mazeio::generator() {}
    //
    generator(mazeio::IO *pIO) : mazeio::generator(pIO) {}
    //
    virtual int ID() const { return 3; }
    // наименование генератора
    virtual QString title() const { return QString::fromLatin1("Recursive divide"); }

    // построить лабиринт
    void doMaze() override;

    // делим камеру на 4
    bool divideGrid(const grid_s &currentChamber, grid_s *subGrids);

  };

};
