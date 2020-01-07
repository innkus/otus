#include <QPaintEvent>
#include <QPainter>
#include <float.h>
#include <time.h>
#include <stack>
#include <random> 
#include "mainwindow.h"
#include "recursive-divide-maze.h"

std::random_device rd; 
std::mt19937 mersenne(rd()); 

////////////////////////////////////////////////////////////////////////////////
// рекурсивное деление
////////////////////////////////////////////////////////////////////////////////

// В прямоугольном лабиринте, построить в случайных точках две стенки, 
// которые перпендикулярны друг другу. Эти две стены разделить большую 
// камеру на четыре меньших камер, разделенных четырьмя стенами. Выберите 
// три из четырех стен случайным образом, и открыть одну клеточную ширину
// отверстия в случайной точке в каждом из трех. Продолжайте таким образом,
// рекурсивно, пока каждая камера не имеет ширину одной ячейки в любом из 
// двух направлений.

// построить лабиринт
void recursive_divide::generator::doMaze() {

  if(!pIO)
    return;

  pIO->reset(0);
  pIO->generatorName = title();

  std::stack<grid_s> listGrid;

  grid_s firstGrid(0, 0, pIO->height+1, pIO->width+1);
  firstGrid.buildHWall(pIO, sq::siTop);
  firstGrid.buildHWall(pIO, sq::siBottom);
  firstGrid.buildVWall(pIO, sq::siLeft);
  firstGrid.buildVWall(pIO, sq::siRight);

  
  // Положить в очередь исходный лабиринт
  listGrid.push(firstGrid);

  while(listGrid.size()) {
    grid_s currentGrid = listGrid.top();
    listGrid.pop();

    grid_s subGrids[4];
    // делим камеру на 4
    if(divideGrid(currentGrid, subGrids)) {
      for(int sub=0; sub < 4; ++sub) {
        listGrid.push(subGrids[sub]);
      }
    }

  }

  pIO->statistics(true);
}

// делим камеру на 4
bool recursive_divide::generator::divideGrid(
  const grid_s &currentGrid, grid_s *subGrid) {
  
  // если размер камеры 1 по какому-то из измерений то не рассматриваем ее
  if(currentGrid.countCol<=2 || currentGrid.countRow<=2)
    return false;

  int subCol, subRow;
  if(currentGrid.countCol==3) {
    subCol = 1;
  } 
  else {
    subCol = mersenne() % (currentGrid.countCol - 2) + 1;
  }

  if(currentGrid.countRow==3) {
    subRow = 1;
  } 
  else {
    subRow = mersenne() % (currentGrid.countRow - 2) + 1;
  }

  //   
  // 0 | 2
  // -----
  // 1 | 3
  //   

  //LT
  subGrid[0] = grid_s(currentGrid.row, currentGrid.col, 
    subRow + 1, subCol + 1);
  subGrid[0].buildVWall(pIO, sq::siRight);
  subGrid[0].buildHWall(pIO, sq::siBottom);
    
  //LB
  subGrid[1] = grid_s(currentGrid.row + subRow, currentGrid.col, 
    currentGrid.countRow - subRow, subCol + 1);
  subGrid[1].buildVWall(pIO, sq::siRight);
  subGrid[1].buildHWall(pIO, sq::siTop);
  
  //RT
  subGrid[2] = grid_s(currentGrid.row, currentGrid.col + subCol, 
    subRow + 1, currentGrid.countCol - subCol);
  subGrid[2].buildVWall(pIO, sq::siLeft);
  subGrid[2].buildHWall(pIO, sq::siBottom);


  //RB
  subGrid[3] = grid_s(currentGrid.row + subRow, currentGrid.col + subCol, 
    currentGrid.countRow - subRow, currentGrid.countCol - subCol);
  subGrid[3].buildVWall(pIO, sq::siLeft);
  subGrid[3].buildHWall(pIO, sq::siTop);

  //
  subGrid[0].debugPrint();
  subGrid[2].debugPrint();

  subGrid[1].debugPrint();
  subGrid[3].debugPrint();

  // ищем стенку в которой не будем делать дырку
  int excludeWall = mersenne() /*rand()*/ % 4;
   
  // walls       // subGrids
  //   1         //    
  //   |         // 0 | 2
  // 0---2       // -----
  //   |         // 1 | 3
  //   3         //   
  
  int rowStart = -1;
  int colStart = -1;

  if(excludeWall != 1) {
    if(subGrid[0].countRow <= 2)
      rowStart = 0;
    else
      rowStart = mersenne()  % (subGrid[0].countRow - 1);
    // верхняя вертикальная перегородка
    subGrid[0].makeHole(pIO, rowStart, sq::siRight);
    subGrid[2].makeHole(pIO, rowStart, sq::siLeft);
  }
  if(excludeWall != 0) {
    if(subGrid[0].countCol <= 2)
      colStart = 0;
    else
      colStart = mersenne() % (subGrid[0].countCol - 1);
    // левая горизонтальная перегородка
    subGrid[0].makeHole(pIO, colStart, sq::siBottom);
    subGrid[1].makeHole(pIO, colStart, sq::siTop);
  }
  if(excludeWall != 3) {
    if(subGrid[1].countRow <= 2)
      rowStart = 0;
    else
      rowStart = mersenne() % (subGrid[1].countRow - 1);
    // нижняя вертикальная перегородка
    subGrid[1].makeHole(pIO, rowStart, sq::siRight);
    subGrid[3].makeHole(pIO, rowStart, sq::siLeft);
  }
  if(excludeWall != 2) {
    // правая горизонтальная перегородка
    if(subGrid[2].countCol <= 2)
      colStart = 0;
    else
      colStart = mersenne() % (subGrid[2].countCol - 1);
    subGrid[2].makeHole(pIO, colStart, sq::siBottom);
    subGrid[3].makeHole(pIO, colStart, sq::siTop);
  }

  return true;
}

// walls       // subGrids
//   1         //    
//   |         // 0 | 2
// 0---2       // -----
//   |         // 1 | 3
//   3         //   
void recursive_divide::grid_s::makeHole(mazeio::IO *pIO, int posStart, uchar orientation) {

  if(orientation & sq::siTop)  {
    int pos = pIO->position(row, col + posStart);
    pIO->getCell(pos)->reset(orientation);
  }
  if(orientation & sq::siBottom)  {
    int pos = pIO->position(row + countRow - 2, col + posStart);
    pIO->getCell(pos)->reset(orientation);
  }
  if(orientation & sq::siLeft)  {
    int pos = pIO->position(row + posStart, col);
    pIO->getCell(pos)->reset(orientation);
  }
  if(orientation & sq::siRight)  {
    int pos = pIO->position(row + posStart, col + countCol - 2);
    pIO->getCell(pos)->reset(orientation);
  }

}

// построить стену у ячейки с заданной позиции на заданную длину \enum sq::side
void recursive_divide::grid_s::buildHWall(mazeio::IO *pIO, uchar orientation) {
  if( !(orientation & sq::siTop) && !(orientation & sq::siBottom))
    return;
  
  int firstPos = -1, lastPos = -1;
  if(orientation & sq::siTop) {
    firstPos = pIO->position(row, col);
    lastPos = pIO->position(row, col + countCol - 2);
  }
  if(orientation & sq::siBottom) {
    firstPos = pIO->position(row + countRow - 2, col);
    lastPos = pIO->position(row + countRow - 2, col + countCol - 2);
  }

  if(firstPos==-1 || lastPos==-1)
    return;

  for(int pos = firstPos; pos <= lastPos; ++pos) {
    pIO->getCell(pos)->set(orientation);
  }
}

// построить стену у ячейки с заданной позиции на заданную высоту \enum sq::side
void recursive_divide::grid_s::buildVWall(mazeio::IO *pIO, uchar orientation) {
  if( !(orientation & sq::siLeft) && !(orientation & sq::siRight))
    return;
  
  int firstPos = -1, lastPos = -1;
  if(orientation & sq::siLeft) {
    firstPos = pIO->position(row, col);
    lastPos = pIO->position(row + countRow - 2, col);
  }
  if(orientation & sq::siRight) {
    firstPos = pIO->position(row, col + countCol - 2);
    lastPos = pIO->position(row + countRow - 2, col + countCol - 2);
  }

  if(firstPos==-1 || lastPos==-1)
    return;

  for(int pos = firstPos; pos <= lastPos; pos+=pIO->width) {
    pIO->getCell(pos)->set(orientation);
  } 
}


// https://weblog.jamisbuck.org/2011/1/27/maze-generation-growing-tree-algorithm


