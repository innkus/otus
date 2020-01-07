#include <QPaintEvent>
#include <QPainter>
#include <float.h>
#include <time.h>
#include <math.h>
#include "mainwindow.h"
#include "aldous-broder-maze.h"

////////////////////////////////////////////////////////////////////////////////
// алгоритм Олдос-Бродера (случайное блуждание)
////////////////////////////////////////////////////////////////////////////////

// 1. Выбрать случайную вершину (клетку). Абсолютно случайную;
// 2. Выбрать случайную соседнюю вершину (клетку) и перейти в неё.
// Если она не была посещена, добавить её в дерево (соединить с предыдущей, убрать между ними стену);
// 3. Повторять шаг 2, пока все клетки не будут посещены.

// построить лабиринт
void aldous_broder::generator::doMaze() {

  if(!pIO)
    return;

  pIO->generatorName = title();

  srand(time(NULL));

  // 1. Сделайте начальную клетку текущей и отметьте ее как посещенную.
  int countUnvisited = pIO->width * pIO->height;
  int currentPos = 0;
  pIO->getCell(currentPos)->setVisited(countUnvisited);

  // Пока есть непосещенные клетки
  int maxRepeatCount = countUnvisited * countUnvisited;
  while(countUnvisited && maxRepeatCount--) {
    // 2. Выбрать случайную соседнюю вершину (клетку) и перейти в неё.
    int randomCell = randomNeighborCell(currentPos);

    // Если она не была посещена, добавить её в дерево (соединить с предыдущей, убрать между ними стену);
    if(!pIO->getCell(randomCell)->isVisited()) {
      pIO->getCell(randomCell)->setVisited(countUnvisited);
      // Уберите стенку между текущей клеткой и выбранной
      eraseWall(currentPos,randomCell);
    }
    currentPos = randomCell;
  }

  pIO->statistics(true);
}

// выбрать случайную клетку из соседних клеток
int  aldous_broder::generator::randomNeighborCell(int pos) const {

    int col = pos % pIO->width;
    int row = pos / pIO->width;

    struct pos_s {
        int row, col;
    };
    pos_s neighbors[4];
    int num = 0;

    // левый сосед
    if((col-1)>=0) {
        neighbors[num].col = col-1;
        neighbors[num++].row = row;
    }

    // правый сосед
    if((col+1)<pIO->width) {
        neighbors[num].col = col+1;
        neighbors[num++].row = row;
    }

    // верхний сосед
    if((row-1)>=0) {
        neighbors[num].col = col;
        neighbors[num++].row = row-1;
    }

    // нижний сосед
    if((row+1)<pIO->height) {
        neighbors[num].col = col;
        neighbors[num++].row = row+1;
    }

    if(num==0)
        return -1;

    int nNeighbor = rand() % num;
    return neighbors[nNeighbor].row * pIO->width + neighbors[nNeighbor].col;
}

// уберать стенку между двумя клетками
void aldous_broder::generator::eraseWall(int pos1, int pos2) {

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



