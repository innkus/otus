#pragma once

#include <QPainter>
#include <QDebug>
#include "mazeio.h"

class Window;

namespace binary {

  //https://habr.com/ru/post/320140/

  // Формальный алгоритм (для северо-восточного смещения):
  // 
  // Выбрать начальную клетку;
  // Выбрать случайное направление для прокладывания пути. Если соседняя клетка в этом направлении выходит за границы поля, 
  // прокопать клетку в единственно возможном направлении;
  // Перейти к следующей клетке;
  // Повторять 2-3 до тех пор, пока не будут обработаны все клетки;

  // лабиринт
  struct generator : mazeio::generator {

    //
    generator() : mazeio::generator() {}
    //
    generator(mazeio::IO *pIO) : mazeio::generator(pIO) {}
    //
    virtual int ID() const { return 0; }
    // наименование генератора
    virtual QString title() const { return QString::fromLatin1("Binary"); }

    // построить лабиринт
    virtual void doMaze() override;

  };

};

