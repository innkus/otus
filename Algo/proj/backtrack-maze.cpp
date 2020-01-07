#include <QPaintEvent>
#include <QPainter>
#include <float.h>
#include <time.h>
#include <math.h>
#include "mainwindow.h"
#include "backtrack-maze.h"

////////////////////////////////////////////////////////////////////////////////
// простой идеальный лабиринт (без циклов с одним путем от точки до точки)
////////////////////////////////////////////////////////////////////////////////

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

// построить лабиринт
void backtrack::generator::doMaze() {

  if(!pIO)
    return;

  pIO->generatorName = title();

  srand(time(NULL));

  // 1. Сделайте начальную клетку текущей и отметьте ее как посещенную.  
  int countUnvisited = pIO->width * pIO->height;
  int currentPos = 0;
  pIO->getCell(currentPos)->setVisited(countUnvisited);

// 2. Пока есть непосещенные клетки  
  while(countUnvisited) {
    //1. Если текущая клетка имеет непосещенных «соседей»
    if(cellHasUnvisitedHeighbors(currentPos)) 
    {
      //1. Протолкните текущую клетку в стек
      pushStack(currentPos);
      //2. Выберите случайную клетку из соседних непосещенных клеток
      int randomCell = randomUnvisitedNeighborCell(currentPos);
      if(randomCell == -1) {
        // что-то пошло не так - завершаемся аварийно
        return;
      }
      //3. Уберите стенку между текущей клеткой и выбранной
      eraseWall(currentPos,randomCell);
      //4. Сделайте выбранную клетку текущей и отметьте ее как посещенную.
      currentPos = randomCell;
      pIO->getCell(currentPos)->setVisited(countUnvisited);
      //
      continue;
    } 
    //2. Иначе если стек не пуст
    if(!isEmptyStack()) 
    {
      //1. Выдерните клетку из стека
      //2. Сделайте ее текущей
      currentPos = popStack();
      if(currentPos == -1) {
        // что-то пошло не так - завершаемся аварийно
        return;
      }

    }
    else 
    {
      //1. Выберите случайную непосещенную клетку, сделайте ее текущей и отметьте как посещенную.
      currentPos = findUnvisitedCell();
      if(currentPos == -1) {
        // что-то пошло не так - завершаемся аварийно
        return;
      }
      pIO->getCell(currentPos)->setVisited(countUnvisited);
    }
  }

  pIO->statistics(true);
}

// проверить имеет ли клетка непосещенных соседей
bool backtrack::generator::cellHasUnvisitedHeighbors(int pos) {
  int col = pos % pIO->width;
  int row = pos / pIO->width;

  // вернхий сосед
  if ((col - 1) >= 0 && !pIO->data[row][col - 1].isVisited()) {
    return true;
  }

  // нижний сосед
  if ((col + 1) < pIO->width && !pIO->data[row][col + 1].isVisited()) {
    return true;
  }

  // левый сосед
  if ((row - 1) >= 0 && !pIO->data[row - 1][col].isVisited()) {
    return true;
  }

  // правый сосед
  if ((row + 1) < pIO->height && !pIO->data[row + 1][col].isVisited()) {
    return true;
  }

  return false;
}

// выбрать случайную клетку из соседних непосещенных клеток
int  backtrack::generator::randomUnvisitedNeighborCell(int pos) const {

  int col = pos % pIO->width;
  int row = pos / pIO->width;

  struct pos_s {
    int row, col;
  };
  pos_s neighbors[4];
  int num = 0;

  // левый сосед
  if((col-1)>=0  && !pIO->data[row][col-1].isVisited() ) {
    neighbors[num].col = col-1;
    neighbors[num++].row = row; 
  }

  // правый сосед
  if((col+1)<pIO->width && !pIO->data[row][col+1].isVisited() ) {
    neighbors[num].col = col+1;
    neighbors[num++].row = row;
  }

  // верхний сосед
  if((row-1)>=0  && !pIO->data[row-1][col].isVisited() ) {
    neighbors[num].col = col;
    neighbors[num++].row = row-1;
  }

  // нижний сосед
  if((row+1)<pIO->height && !pIO->data[row+1][col].isVisited() ) {
    neighbors[num].col = col;
    neighbors[num++].row = row+1;
  }

  if(num==0)
    return -1;

  int nNeighbor = rand() % num;  
  return neighbors[nNeighbor].row * pIO->width + neighbors[nNeighbor].col;
}

// вытащить клетку из стека
int backtrack::generator::popStack() {
  if(!stackCell.size())
    return -1;
  else {
    int value = stackCell.top();
    stackCell.pop();
    return value;
  }
}

// проверить является ли стек пустым
bool backtrack::generator::isEmptyStack() const {
  return stackCell.size() == 0;
}

// положить клетку в стек
void backtrack::generator::pushStack(int pos) {
  stackCell.push(pos);
}

// уберать стенку между двумя клетками
void backtrack::generator::eraseWall(int pos1, int pos2) {

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

// выбрать случайную непосещенную клетку из всего массива непосещенных клеток
int backtrack::generator::findUnvisitedCell() {
  for(int i=0; i< pIO->width * pIO->height; ++i) {
    if(!pIO->getCell(i)->isVisited())
      return i;
  }
  return -1;
}
