#include <QPaintEvent>
#include <QPainter>
#include <float.h>
#include <time.h>
#include <math.h>
#include "vectorArray.h"
#include "mainwindow.h"
#include "sidewinder-maze.h"

////////////////////////////////////////////////////////////////////////////////
// алгоритм sidewinder
////////////////////////////////////////////////////////////////////////////////

// Формальный алгоритм (для стандартного смещения):

// 1. Выбрать начальный ряд;
// 2. Выбрать начальную клетку ряда и сделать её текущей;
// 3. Инициализировать пустое множество;
// 4. Добавить текущую клетку в множество;
// 5. Решить, прокладывать ли путь направо;
// 6. Если проложили, то перейти к новой клетке и сделать её текущей. Повторить шаги 3-6;
// 7. Если не проложили, выбираем случайную клетку множества и прокладываем оттуда путь наверх. Переходим на следующий ряд и повторяем 2-7;
// 8. Продолжать, пока не будет обработан каждый ряд;

// построить лабиринт
void sidewinder::generator::doMaze() {

  if(!pIO)
    return;

  pIO->reset();
  pIO->generatorName = title();

  srand(time(NULL));
  
  int pos;

  // у первого ряда пробиваем правую стену сразу
  for(int col = 1; col < pIO->width - 1; ++col) {
    pos = pIO->position(0, col);
    pIO->getCell(pos)->reset(sq::siLeft | sq::siRight);
  }
  pos = pIO->position(0, 0);
  pIO->getCell(pos)->reset(sq::siRight);

  pos = pIO->position(0, pIO->width - 1);
  pIO->getCell(pos)->reset(sq::siLeft);

  for(int row = 1; row < pIO->height; ++row) {
    
    // пытаемся пойти направо
    VectorArray<int> cells;

    for(int col = 0; col < pIO->width; ++col) {
      pos = pIO->position(row, col);
      // добавить ячейку в множество
      cells.add(pos);
      // смотрим можем ли пойти направо
      bool bNextRight = false;
      if(col == pIO->width - 1) {
        bNextRight = false;
      }
      else {
        //
        bNextRight = rand() % 2 == 1;
      }
      // если можем - удаляем перегородку, идем на следующую ячейку
      if(bNextRight) {        
        pIO->getCell(pos)->reset(sq::siRight);
        pos = pIO->position(row, col+1);
        pIO->getCell(pos)->reset(sq::siLeft);
        continue;
      }
      
      // не можем - пробиваем дырку вверх выбрав произвольную ячейку из множества
      int index = -1;
      if(cells.size() == 1)
        index = 0;
      else
        index = rand() % cells.size();
      pos = cells.get(index);
      pIO->getCell(pos)->reset(sq::siTop);
      pos -= pIO->width;
      pIO->getCell(pos)->reset(sq::siBottom);
      
      // обнуляем множество
      cells.clear();

      // переходим на следующую ячейку
    }
  }

  pIO->statistics(true);
}

// уберать стенку между двумя клетками
void sidewinder::generator::eraseWall(int pos1, int pos2) {

    int col1 = pos1 % pIO->width;
    int row1 = pos1 / pIO->width;

    int col2 = pos2 % pIO->width;
    int row2 = pos2 / pIO->width;

    // на одной вертикали
    if(col1 == col2) {
        // меняем строки, чтобы первая строка была ниже
        if(row1 > row2) {
            row1 = row1 ^ row2;
            row2 = row2 ^ row1;
            row1 = row1 ^ row2;
        }
        // row1 меньше row2
        // у 1 убираем низ, у 2 убираем верх
        pIO->data[row1][col1].reset(sq::side::siBottom);
        pIO->data[row2][col2].reset(sq::side::siTop);
        return;
    }

    // на одной горизонтали
    if(row1 == row2) {
        // меняем столбцы
        if(col1 > col2) {
            col1 = col1 ^ col2;
            col2 = col2 ^ col1;
            col1 = col1 ^ col2;
        }
        // col1 меньше col2
        // у 1 убираем право, у 2 убираем лево
        pIO->data[row1][col1].reset(sq::side::siRight);
        pIO->data[row2][col2].reset(sq::side::siLeft);
        return;
    }
}
