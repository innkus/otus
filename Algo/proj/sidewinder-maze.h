#pragma once

#include <QPainter>
#include <QDebug>

#include "mazeio.h"

class Window;


// алгоритм sidewinder
namespace sidewinder {

  // https://habr.com/en/post/320140/

  // лабиринт
  struct generator : mazeio::generator {

    //
    generator() : mazeio::generator() {}
    //
    generator(mazeio::IO *pIO) : mazeio::generator(pIO) {}
    //
    virtual int ID() const { return 4; }
    // наименование генератора
    virtual QString title() const { return QString::fromLatin1("Sidewinder"); }

    // построить лабиринт
    void doMaze() override;

    // уберать стенку между двумя клетками
    void eraseWall(int pos1, int pos2);

  };

};
