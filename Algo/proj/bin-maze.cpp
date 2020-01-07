#include <QPaintEvent>
#include <QPainter>
#include <float.h>
#include <time.h>
#include <math.h>
#include "mainwindow.h"
#include "bin-maze.h"


////////////////////////////////////////////////////////////////////////////////
// простейший бинарный лабиринт
////////////////////////////////////////////////////////////////////////////////
// построить лабиринт
void binary::generator::doMaze() {

  if(!pIO)
    return;

  pIO->generatorName = title(); 

  srand(time(NULL));

  //
  for(int h = 0; h < pIO->height; ++h) {
  
    for(int w = 0; w < pIO->width; ++w) {

      // если мы в последней клетке первого ряда переходим на следующий ряд
      if( (w == pIO->width - 1) && (h == 0) )
        continue;

      // мы в крайнем правом стобце кроме право/верх - можем идти только вверх
      if(w == pIO->width - 1) {
        // сноcим верхнюю перегородку
        pIO->data[h][w].reset(sq::siTop);
        pIO->data[h-1][w].reset(sq::siBottom);
        continue;
      }

      // мы в первом ряду - можем идти только направо
      if(h == 0) {
        // сноcим правую перегородку
        pIO->data[h][w].reset(sq::siRight);
        pIO->data[h][w+1].reset(sq::siLeft);
        continue;
      }
      // наконец можем пойти туда куда хотим
      // генериуем номер новой позиции (1 - идем вверх, 0 - идем вправо)
      int pos = rand() % 2;
      if(pos == 1) {
        pIO->data[h][w].reset(sq::siTop);
        pIO->data[h-1][w].reset(sq::siBottom);
      }
      else {
        pIO->data[h][w].reset(sq::siRight);
        pIO->data[h][w+1].reset(sq::siLeft);
      }
    }
  } 

  pIO->statistics(true);
}

