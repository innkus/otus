#pragma once

#include <QPainter>
#include <QDebug>

#include "mazeio.h"

class Window;


// алгоритм Олдоса-Бродера
namespace aldous_broder {

  // https://habr.com/ru/post/321210/

  // Выбрать случайную вершину (клетку). Абсолютно случайную;
  // Выбрать случайную соседнюю вершину (клетку) и перейти в неё.
  // Если она не была посещена, добавить её в дерево (соединить с предыдущей, убрать между ними стену);
  // Повторять шаг 2, пока все клетки не будут посещены.

  // лабиринт
  struct generator : mazeio::generator {

    //
    generator() : mazeio::generator() {}
    //
    generator(mazeio::IO *pIO) : mazeio::generator(pIO) {}
    //
    virtual int ID() const { return 2; }
    // наименование генератора
    virtual QString title() const { return  QString::fromLatin1("Aldous-Broder"); }

    // построить лабиринт
    void doMaze() override;

    // выберать случайную клетку из соседних клеток (все 4 соседа)
    int randomNeighborCell(int pos) const;
    // уберать стенку между двумя клетками
    void eraseWall(int pos, int posNext);
  };

};
