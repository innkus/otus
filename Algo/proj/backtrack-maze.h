#pragma once

#include <QPainter>
#include <QDebug>
#include <stack>
#include "mazeio.h"

class Window;


//
namespace backtrack {
  // https://habr.com/ru/post/262345/

  // Замечание: предполагается, что изначально у каждой клетки есть стенки со всех четырех сторон, которые отделяют ее от соседних клеток.
  // 1. Сделайте начальную клетку текущей и отметьте ее как посещенную.
  // 2. Пока есть непосещенные клетки
  //   1. Если текущая клетка имеет непосещенных «соседей»
  //     1. Протолкните текущую клетку в стек
  //     2. Выберите случайную клетку из соседних
  //     3. Уберите стенку между текущей клеткой и выбранной
  //     4. Сделайте выбранную клетку текущей и отметьте ее как посещенную.
  //   2. Иначе если стек не пуст
  //     1. Выдерните клетку из стека
  //     2. Сделайте ее текущей
  //   3. Иначе
  //     1. Выберите случайную непосещенную клетку, сделайте ее текущей и отметьте как посещенную.


  // лабиринт
  struct generator : mazeio::generator {

    //
    generator() : mazeio::generator() {}
    //
    generator(mazeio::IO *pIO) : mazeio::generator(pIO) {}
    //
    virtual int ID() const { return 1; }
    // наименование генератора
    virtual QString title() const { return QString::fromLatin1("Backtrack"); }

    // построить лабиринт
    virtual void doMaze() override;

    // проверить имеет ли клетка непосещенных соседей
    bool cellHasUnvisitedHeighbors(int pos);
    // выберать случайную клетку из соседних непосещенных клеток
    int randomUnvisitedNeighborCell(int pos) const;

    // вытащить клетку из стека
    void pushStack(int pos);
    // проверить является ли стек пустым
    bool isEmptyStack() const;
    // положить клетку в стек
    int popStack();

    // уберать стенку между двумя клетками
    void eraseWall(int pos, int posNext);
    // выбрать случайную непосещенную клетку из всего массива непосещенных клеток
    int findUnvisitedCell();

    // описание лабиринта
    //Stack<int> stackCell;
    std::stack<int> stackCell;
  };

};
