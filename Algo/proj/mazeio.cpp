#include <QPaintEvent>
#include <QPainter>
#include <QMessageBox>
#include <QMap>
#include <float.h>
// #include "Queue.h"
//#include "vectorArray.h"
#include "mainwindow.h"
#include "mazeio.h"

#include <queue>

#include "graph.h"
#include "kraskal.h"

#define version_0 (0)

mazeio::IO::IO() {

  // стартовая позиция
  startPos = -1;
  // финишная позиция
  finishPos = -1;
  //
  heatPos = -1;
  //
  maxHeatValue = 0;
  //
  bHeatMapMode = false;
  //
  pathAlgo = mazeio::IO::paLee;
  //
  generatorName = QObject::tr("Empty");
}

//
mazeio::IO::~IO () {
}

// установить размеры лабиринта
void mazeio::IO::setSize(int _width, int _height) {
  width   = _width;
  height  = _height;
  statistics(true);
}

// отладочная печать
void mazeio::IO::debugPrint() const {
    return;
  for(int row = 0; row < height; ++row) {
    for(int col = 0; col< width; ++col) {
      data[row][col].debugPrint();
    }
  }
}

// 
void mazeio::IO::reset(uchar _ramka) {
  for(int row = 0; row < height; ++row) {
    for(int col = 0; col< width; ++col) {
      data[row][col] = sq::cell();
      data[row][col].ramka = _ramka;
    }
  }  
  generatorName = QObject::tr("Empty");
  resultPath.clear();
  resultSpanningTree.clear();
  statistics(true);
}

// нарисовать лабиринт в заданной позиции лево/верх заданной длины и ширины
void mazeio::view::draw(QPainter &painter, Window *pWindow, const QRect &) {

  if(!height || !width)
    return;

  int maxAntialiasingSize = 300;

  QFlags<QPainter::RenderHint> prevHints = painter.renderHints();
  if(height*width < maxAntialiasingSize)
      painter.setRenderHint(QPainter::Antialiasing, true);

  QPen pen;
  pen.setCapStyle(Qt::RoundCap);
  pen.setJoinStyle(Qt::RoundJoin);

  QSize wSize = pWindow->size();
  QRect cellRect;    
  sizeCellPx = qMin( (wSize.width() - 2*marginPx) / width, (wSize.height() - 2*marginPx) / height);
  if(sizeCellPx < 4)
    sizeCellPx = 4;

  auto drawPosition = [this,&painter,&cellRect](const QColor &color, int row, int col, int position, bool bCross) {
    if(this->position(row, col) != position) 
      return;
    painter.save();
    if(bCross) 
    {
      QPen pen;
      pen.setWidth(2);
      pen.setCapStyle(Qt::RoundCap);
      pen.setJoinStyle(Qt::RoundJoin);
      painter.setPen(pen);

      int dr = 4;
      QRect rect = cellRect.adjusted(dr, dr, -dr, -dr);
      painter.drawRect(rect);
    }
    else 
    {
      painter.setPen(QPen(color));
      painter.setBrush(QBrush(color));
      int dr = 4;
      QRect rect = cellRect.adjusted(dr, dr, -dr, -dr);
      painter.drawRect(rect);
    }
    painter.restore();
  };

  for(int row = 0; row < height; ++row) {

    cellRect.setRect(marginPx, row*sizeCellPx + marginPx, sizeCellPx, sizeCellPx);

    for(int col = 0; col < width; ++col) {          

      data[row][col].draw(painter, pWindow, cellRect);          
      drawPosition(QColor(255,0,0,100)/*Qt::red*/   , row, col, startPos, false);
      drawPosition(QColor(0,128,0,100)/*Qt::green*/ , row, col, finishPos, false);          
      cellRect.translate(sizeCellPx, 0);
    }

  }

  if(resultSpanningTree.size()) {
    painter.save();
    QPen pen(QColor(0,240,240,100));
    pen.setWidth(8);
    painter.setPen(pen);
    for (int i = 0; i < resultSpanningTree.size(); ++i) {
      QRect cellStart = getCellRect(resultSpanningTree.get(i).a, pWindow);
      QRect cellStop  = getCellRect(resultSpanningTree.get(i).b, pWindow);
      // 
      painter.drawLine(cellStart.center(), cellStop.center());
    }
    painter.restore();
  }

  // отображение результата волнового алгоритма
  if(resultPath.size()>=2) {
    painter.save();

    painter.setRenderHint(QPainter::Antialiasing, true);
    QColor color(QColor(240,0,120/*192,114,192*/));
    QPen pen(color);
    pen.setWidth(4);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(QBrush(color));

    int currentPos = resultPath[resultPath.size()-1];
    QRect cellRect = getCellRect(currentPos, nullptr);
    QPoint currentPoint = cellRect.center();

    for(int num=resultPath.size()-2; num>=0; num--) {
      int nextPos = resultPath[num];
      cellRect = getCellRect(nextPos, nullptr);
      QPoint nextPoint =  cellRect.center();
      
      painter.drawLine(currentPoint, nextPoint);

      currentPos = nextPos;
      currentPoint = nextPoint;
    }
    painter.restore();
  }

  // отображение номеров для результата волнового алгоритма
  if(pWindow && pWindow->m_bShowNumbersPath) {
    QPen penWeight;
    penWeight.setColor(Qt::gray);

    int weightFontSize = sizeCellPx/5;
    QFont font2;
    if(weightFontSize>8) 
      font2.setPixelSize(weightFontSize);
    else
      weightFontSize = 0;

    QFont font;
    font.setPixelSize(sizeCellPx/2);
    painter.save();
    painter.setFont(font);
    
    for(int row = 0; row < height; ++row) {

      cellRect.setRect(marginPx, row*sizeCellPx + marginPx, sizeCellPx, sizeCellPx);

      for(int col = 0; col < width; ++col) {

        if(weightFontSize) {
          int pos = position(row,col);

          painter.save();
          painter.setPen(penWeight);
          painter.setFont(font2);
          painter.drawText(cellRect, QString::fromLatin1("%1").arg(pos), 
                  Qt::AlignTop | Qt::AlignLeft);
          painter.restore();
        }
        
        if(data[row][col].value != -1  && resultPath.size())
          painter.drawText(cellRect, QString::fromLatin1("%1").arg(data[row][col].value), 
            Qt::AlignVCenter | Qt::AlignHCenter);

        cellRect.translate(sizeCellPx, 0);
      }

    }
    painter.restore();
  }

  if(!prevHints.testFlag(QPainter::Antialiasing))
      painter.setRenderHint(QPainter::Antialiasing, false);

  // тепловая карта в самом конце
  bool bDrawHeat = true;
  if(bDrawHeat && heatMap.size()) {

    for(int row = 0; row < height; ++row) {

        cellRect.setRect(marginPx, row*sizeCellPx + marginPx, sizeCellPx, sizeCellPx);

        for(int col = 0; col < width; ++col) {
          int pos = position(row, col);
          //
          QColor color;

          if(heatMap[pos]==-1 || !maxHeatValue)
            color = QColor(Qt::gray);
          else 
          {
            int value = 255 * float(heatMap[pos]) / maxHeatValue;
            color = QColor(255-value, 255, 255-value);
          }
          QBrush brush(color);
          QPen pen(color);
          painter.setBrush(brush);
          painter.setPen(color);
          painter.drawRect(cellRect);

          drawPosition(Qt::black, row, col, heatPos, true);

          cellRect.translate(sizeCellPx, 0);
        }

    }
  }

  if(bStatistics) {
    painter.save();
    QFont font;
    font.setPixelSize(14);
    painter.setFont(font);

    QRect textRect;
    textRect.setRect( 
      2 * marginPx + width * sizeCellPx, 
      marginPx, 
      wSize.width() - 2 * marginPx - width * sizeCellPx, 
      wSize.height() - marginPx);

    QString text;
    text = QString::fromLatin1("Statistics\n\nDeadend count: %1\nPass count: %2").arg(statData.deadCount).arg(statData.orientation);  
    painter.drawText(textRect, text, Qt::AlignLeft | Qt::AlignTop);
    painter.restore();
  }
}

// расчитать позицию лабиринта по строке и столбцу
int mazeio::IO::position(int row, int col) {
  if(row < 0 || row >= height)
    return -1;
  if(col < 0 || col >= width)
    return -1;
  return row * width + col;
}


// волновой алгоритм (алгоритм Ли)
// https://en.wikipedia.org/wiki/Lee_algorithm
// https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%9B%D0%B8
// волновой алгоритм нахождения пути в лабиринте

// Инициализация
// 
// Пометить стартовую ячейку 
// d := 0 
// Распространение волны
// 
// ЦИКЛ
//   ДЛЯ каждой ячейки loc, помеченной числом d
//     пометить все соседние свободные непомеченные ячейки числом d + 1
//   КЦ
//   d := d + 1
// ПОКА (финишная ячейка не помечена) И (есть возможность распространения волны) 
// Восстановление пути
// 
// ЕСЛИ финишная ячейка помечена
// ТО
//   перейти в финишную ячейку
//   ЦИКЛ
//     выбрать среди соседних ячейку, помеченную числом на 1 меньше числа в текущей ячейке
//     перейти в выбранную ячейку и добавить её к пути
//   ПОКА текущая ячейка — не стартовая
//   ВОЗВРАТ путь найден
// ИНАЧЕ
//   ВОЗВРАТ путь не найден
void mazeio::IO::findPathLee() {

  resultPath.clear();
  resultPath.reserve(height*width);
  for(int row = 0; row < height; ++row) {
    for(int col = 0; col< width; ++col) {
      data[row][col].value = -1;
    }
  }

  if(startPos==-1 || finishPos==-1 || startPos == finishPos)
    return;

  int neighbor[8], neighborSize;
  bool bFinally = false;
  int currentNumber = 0;
  
  std::deque<int> queueCell;

  queueCell.push_back(startPos);
  getCell(startPos)->value = currentNumber;

  while(queueCell.size()) {
    // очередная ячейка из очереди
    int currentPos = queueCell.front();
    queueCell.pop_front();

    // путевой номер текущей ячейки
    int currentNumber = getCell(currentPos)->value;

    // обходим непосещенных соседей, проверем не конец ли, накидываем номер
    reachableNeighbor4(currentPos, neighbor, neighborSize);

    // для непосещенных соседей увеличиваем номер
    // проверяем не конец ли это
    // если конец выходим, иначе добавляем соседей в стек
    for(int i = 0; i < neighborSize; ++i) {
      if(getCell(neighbor[i])->value != -1)
        continue;
      getCell(neighbor[i])->value = currentNumber + 1;
      if(neighbor[i] == finishPos) {
        bFinally = true;
        break;
      }
      if(pathAlgo == paLee)
        queueCell.push_back(neighbor[i]);
      else
        queueCell.push_front(neighbor[i]);
    }

    if(bFinally)
      break;
  }

  if(!bFinally) {
    return;
  }

  int currentPos = finishPos;
  resultPath.push_back(currentPos);

  while(currentPos != startPos) {
    int currentNumber = getCell(currentPos)->value;
    // получить соседей
    reachableNeighbor4(currentPos, neighbor, neighborSize);
    // выбираем любого соседа с меньшим на 1 номером
    for(int i = 0; i < neighborSize; ++i) {
      if(getCell(neighbor[i])->value == currentNumber - 1) {
        currentPos = neighbor[i];
        resultPath.push_back(currentPos);
        break;
      }
    }
  } // while

}

// получить все соседние ячейки
void mazeio::IO::reachableNeighbor4(int currentPos, int *neighborPos, int &num) {
  
  int col = currentPos % width;
  int row = currentPos / width;

  struct pos_s {
    int row, col;
  };
  pos_s neighbors[4];
  num = 0;

  // левый сосед
  if((col-1)>=0 && !data[row][col].is(sq::side::siLeft)) {
    neighbors[num].col = col-1;
    neighbors[num].row = row; 
    neighborPos[num++] = position(neighbors[num].row, neighbors[num].col);
  }

  // правый сосед
  if((col+1)<width && !data[row][col].is(sq::side::siRight)) {
    neighbors[num].col = col+1;
    neighbors[num].row = row;
    neighborPos[num++] = position(neighbors[num].row, neighbors[num].col);
  }

  // верхний сосед
  if((row-1)>=0 && !data[row][col].is(sq::side::siTop)) {
    neighbors[num].col = col;
    neighbors[num].row = row-1;
    neighborPos[num++] = position(neighbors[num].row, neighbors[num].col);
  }

  // нижний сосед
  if((row+1)<height && !data[row][col].is(sq::side::siBottom)) {
    neighbors[num].col = col;
    neighbors[num].row = row+1;
    neighborPos[num++] = position(neighbors[num].row, neighbors[num].col);
  }
}

// получить координаты прямоугольника ячейки по его позиции
QRect mazeio::view::getCellRect(int pos, Window *) const {
  int col = pos % width;
  int row = pos / width;

  if(col<0 || col>=width || row<0 || row>=height)
    return QRect();
  else
    return QRect(marginPx + col*sizeCellPx, marginPx + row*sizeCellPx, sizeCellPx, sizeCellPx);
}


// сохранить описание
void mazeio::IO::save(const QString &fileName) const {
  QFile qf(fileName);
  if(!qf.open(QIODevice::WriteOnly)) {
    QMessageBox msgBox(QMessageBox::Warning, "Maze", QString::fromLatin1("File open error. Check %1.").arg(fileName));
    msgBox.exec();  
    return;
  }
  QTextStream stream(&qf);

  stream << version_0 << endl;
  // ширина лабиринта / высота лабиринта
  stream << width << " " << height << endl;
  // стартовая позиция / финишная позиция
  stream << startPos << " " << finishPos << endl;
  // имя генератора лабиринта
  stream << generatorName << endl;

  // описание лабиринта
  for(int row=0; row<height; ++row) {  
    QStringList l;
    l << QString::fromLatin1("%1").arg(row);
    for(int col=0; col<width; ++col) {
      l << QString::fromLatin1("%1").arg(data[row][col].ramka);
    }
    stream << l.join(" ");
    stream << endl;
  }
    
  qf.close();
}

// загрузить описание
void mazeio::IO::load(const QString &fileName) {
  QFile qf(fileName);
  if(!qf.open(QIODevice::ReadOnly)) {
      QMessageBox msgBox(QMessageBox::Warning, "Maze", QString::fromLatin1("File save error. Check %1.").arg(fileName));
      msgBox.exec();
      return;
  }
  QTextStream stream(&qf);

  int version;
  stream >> version;
  if(version_0 != version) {
      return;
      QMessageBox msgBox(QMessageBox::Critical, "Maze", QString::fromLatin1("Unknown file version. Check %1.").arg(fileName));
      msgBox.exec();
      return;
  }

  resultPath.clear();

  // ширина лабиринта / высота лабиринта
  stream >> width >> height;
  // стартовая позиция / финишная позиция
  stream >> startPos >> finishPos;
  // имя генератора лабиринта
  stream >> generatorName;

  // описание лабиринта
  while (!stream.atEnd()) {
    if(stream.status()!= QTextStream::Ok)
      break;
    QString str = stream.readLine().trimmed();
    qDebug() << str;
    if(str.isEmpty())
      continue;

    QStringList l = str.split(" ");
    qDebug() << l;

    if(l.size() != width + 1)
      continue;
    int row = l.at(0).toUInt();
    for(int col=0; col<width; ++col) {
      data[row][col].ramka = l.at(col+1).toUInt();
    }    
  }
  qf.size();
}


// https://jeremybytes.blogspot.com/2017/07/more-maze-programming-heat-map.html
// построение тепловой карты
void mazeio::IO::buildHeatMap() {
  
  heatMap.clear();
  
  if(heatPos == -1) {    
    return;
  }

  for(int pos = 0; pos < width * height; ++pos)
    heatMap.push_back(-1);

  int prevStarPos = startPos, prevFinishPos = finishPos;

  startPos = heatPos;

  int pathAlgoOld = pathAlgo;
  pathAlgo = paLee;

  for(finishPos = 0; finishPos < width * height; ++finishPos) {
    findPathLee();    
    if(resultPath.size())
      heatMap[finishPos] = resultPath.size();
  }

  heatMap[startPos] = 0;

  // найдем максимальное значение
  maxHeatValue = 0;
  for(const auto &h: heatMap) {
    if(h>maxHeatValue)
      maxHeatValue = h;
  }

  // qDebug() << "heat" << heatPos << heatMap[heatPos];
  startPos  = prevStarPos;
  finishPos = prevFinishPos;
  pathAlgo = pathAlgoOld;
}

//
void mazeio::IO::buildSpanningTree(bool bBuild) {
 
  resultSpanningTree.clear();
  if(!bBuild)
    return;  

  /*
  // построить матрицу смежности 
  int vertexCount = width * height;

  int *matrix = new int[vertexCount*vertexCount];
  memset(matrix, 0, vertexCount*vertexCount*sizeof(int));

  for(int pos = 0; pos < vertexCount; ++pos) {
    int neighborPos[4], num=0;
    matrix[pos*vertexCount+pos] = 0;

    // получить достижымих соседей
    reachableNeighbor4(pos, neighborPos, num);
    // отметить в матрице смежности
    for(int i=0; i<num; ++i) {
      if(neighborPos[i]<pos)
        continue;
      matrix[pos*vertexCount+neighborPos[i]] = pos+2;
      matrix[neighborPos[i]*vertexCount+pos] = pos+2;
    }

  }
  */

  int vertexCount = 0;
  int *matrix = nullptr;
  incidentMatrix(&matrix, vertexCount);

  myGraph<int> myG(vertexCount, matrix);
  delete []matrix;

  Kraskal<int> kl((iGraph<int>*)&myG);
  resultSpanningTree = kl.doSpanningTree();
  // global::qprintEdges(resultSpanningTree);  
}

// построить матрицу смежности
bool mazeio::IO::incidentMatrix(int **pMatrix, int &size, int blankCell) {

  // построить матрицу смежности 
  int vertexCount = width * height;

  int *matrix = new int[vertexCount*vertexCount];
  for(int pos = 0; pos < vertexCount*vertexCount; ++pos) {
    matrix[pos] = blankCell;
  }

  for(int pos = 0; pos < vertexCount; ++pos) {
    int neighborPos[4], num=0;
    matrix[pos*vertexCount+pos] = 0;

    // получить достижымих соседей
    reachableNeighbor4(pos, neighborPos, num);
    // отметить в матрице смежности
    for(int i=0; i<num; ++i) {
      if(neighborPos[i]<pos)
        continue;
      matrix[pos*vertexCount+neighborPos[i]] = pos+2;
      matrix[neighborPos[i]*vertexCount+pos] = pos+2;
    }

  }

  *pMatrix = matrix;

  size = vertexCount;
  return true;
}

// поставить или снять стенку если получится
void mazeio::IO::editWall(int pos1, int pos2) {

  int col1 = pos1 % width;
  int row1 = pos1 / width;

  int col2 = pos2 % width;
  int row2 = pos2 / width;

  // на одной вертикали
  if(col1 == col2) {
    // меняем строки, чтобы первая строка была ниже
    if(row1 > row2) {
      row1 = row1 ^ row2;
      row2 = row2 ^ row1;
      row1 = row1 ^ row2;      
    }
    // row1 меньше row2
    if(data[row1][col1].is(sq::side::siBottom) && data[row2][col2].is(sq::side::siTop)) {
      data[row1][col1].reset(sq::side::siBottom);
      data[row2][col2].reset(sq::side::siTop);
    } 
    else 
    {
      if(!data[row1][col1].is(sq::side::siBottom) && !data[row2][col2].is(sq::side::siTop)) {
        data[row1][col1].set(sq::side::siBottom);
        data[row2][col2].set(sq::side::siTop);
      }
    }
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
    if(data[row1][col1].is(sq::side::siRight) && data[row2][col2].is(sq::side::siLeft)) {
      data[row1][col1].reset(sq::side::siRight);
      data[row2][col2].reset(sq::side::siLeft);
    } 
    else 
    {
      if(!data[row1][col1].is(sq::side::siRight) && !data[row2][col2].is(sq::side::siLeft)) {
        data[row1][col1].set(sq::side::siRight);
        data[row2][col2].set(sq::side::siLeft);
      } 
    }
    return;
  }

}

// построить стену у ячейки с заданной позиции на заданную длину \enum sq::side
void mazeio::IO::buildHWall(int row, int col, int size, uchar orientation) {
  int lastCol = col + size - 1;
  int firstPos = position(row,col), lastPos = position(row, lastCol);

  if(firstPos==-1 || lastPos==-1)
    return;

  for(int pos = firstPos; pos <= lastPos; ++pos) {
    getCell(pos)->set(orientation);
  }
}

// построить стену у ячейки с заданной позиции на заданную высоту \enum sq::side
void mazeio::IO::buildVWall(int row, int col, int size, uchar orientation) {
  int lastRow = row + size - 1;
  int firstPos = position(row,col), lastPos = position(lastRow, col);

  if(firstPos==-1 || lastPos==-1)
    return;

  for(int pos = firstPos; pos <= lastPos; pos+=width) {
    getCell(pos)->set(orientation);
  }
}

//
void mazeio::IO::statistics(bool bDoStatistics) {
  if(!bDoStatistics) {
    statData.clear();
    return;
  }

  mazeio::statistics *pS = new mazeio::statistics(QString(), statistics::smAll);

  // посчитать число тупиков
  statData.deadCount = pS->calcDeadend(this);

  // посчитать количество проходов по горизонтали и вертикали
  int hPassCount, vPassCount;
  pS->calculatePass(this, hPassCount, vPassCount);
  statData.orientation = hPassCount + vPassCount;
}


// https://e-maxx.ru/algo/floyd_warshall_algorithm
// https://habr.com/ru/post/119158/
// https://neerc.ifmo.ru/wiki/index.php?title=%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%A4%D0%BB%D0%BE%D0%B9%D0%B4%D0%B0
// http://khpi-iip.mipk.kharkiv.edu/library/datastr/book_sod/kgsu/din_0124.html
// http://brestprog.by/topics/floyd/


// алгоритм Флойда Уоршелла
void mazeio::IO::findPathFloyd() {

  resultPath.clear();
  resultPath.reserve(height*width);

  for(int row = 0; row < height; ++row) {
    for(int col = 0; col< width; ++col) {
      data[row][col].value = -1;
    }
  }

  if(startPos==-1 || finishPos==-1 || startPos == finishPos)
    return;

  // матрица смежности
  int size = 0;
  int *matrix = nullptr;  
  incidentMatrix(&matrix, size, INT_MAX);

  int *prev = new int[size*size];
  for(int i = 0; i < size; ++i) {
    for(int j = 0; j < size; ++j) {
      prev[i*size+j] = -1;
    }
  }

  // qDebug() << "-----------------------------------------";
  // for(int i=0; i<size; i++) {
  //   QStringList l;
  //   for(int j=0; j<size; j++) {
  //     l << QString::fromLatin1("%1").arg(prev[i][j]);
  //   }
  //   qDebug() << l.join(" ");
  // }

  int n = size;
  for (int k = 0; k < n; k++) {        //текущая вершина, используемая для улучшения
    // qDebug() << QString::fromLatin1("******** k=%1").arg(k);
    for (int i = 0; i < n; i++) {
      // QStringList l;
      for (int j = 0; j < n; j++) {
        if( i != j && j != i && i != k && matrix[i*n + k] < INT_MAX && matrix[k*n + j] < INT_MAX ) {

          if( matrix[i*n + k] + matrix[k*n + j] < matrix[i*n +j] || matrix[i*n +j] == INT_MAX) {
            matrix[i*n + j] = qMin(matrix[i*n +j], matrix[i*n + k] + matrix[k*n + j]);          
            prev[i*size+j] = k;
          }
        }
         //l << QString::fromLatin1("%1").arg(matrix[i*n + j]);
      }
      // qDebug() << l.join(" ");
    }
  }

  //qDebug() << "-----------------------------------------";
  //for(int i=0; i<size; i++) {
  //  QStringList l;
  //  for(int j=0; j<size; j++) {
  //    l << QString::fromLatin1("%1").arg(prev[i][j]);
  //  }
  //  qDebug() << l.join(" ");
  //}

  if(matrix[startPos*size + finishPos] != INT_MAX) {

    std::stack<int> ss;
    ss.push(startPos*size + finishPos);
    resultPath.push_back(startPos);

    while(ss.size()) {
      int index = ss.top(); 
      ss.pop();
      int i = index / size;
      int j = index % size;
      if(prev[i*size+j] == -1)
        resultPath.push_back(j);
      else {
        ss.push(prev[i*size+j]*size + j);
        ss.push(i*size + prev[i*size+j]);      
      }
    }
  
    for(int pos=0; pos<resultPath.size(); pos++) {
      //qDebug() << resultPath.at(pos);
      int row = resultPath.at(pos) / width;
      int col = resultPath.at(pos) % width;
      getCell(position(row,col))->value = pos;
    }
  }

  if(matrix)
    delete []matrix;
  if(prev)
    delete []prev;
}
