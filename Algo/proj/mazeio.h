#pragma once

#include <QPainter>
#include <QSize>
#include <QRect>

#include <QDebug>
//#include "stack.h"
#include "vectorArray.h"
#include "iGraph.h"

#include <stack>
#include <vector>
#include <bitset>



// Попытки сделать изучение алгоритмов поиска пути проще
// https://habr.com/ru/post/323650/
// 
// Реализация алгоритма A*
// https://habr.com/ru/post/331220/
// 
// Введение в алгоритм A*
// https://habr.com/ru/post/331192/
// 
// Алгоритм поиска пути Jump Point Search
// https://habr.com/ru/post/162915/
// 
// Простое объяснение алгоритмов поиска пути и A*
// https://habr.com/ru/post/444828/
// 
// Лабиринты: классификация, генерирование, поиск решений
// https://habr.com/ru/post/445378/

class Window;
namespace mazeio { struct IO; };
namespace mazeio { struct generator; };

// одна клетка 
namespace sq {

  // стороны клетки
  enum side {
    siLeft    = 0x1, 
    siRight   = 0x2,
    siTop     = 0x4,
    siBottom  = 0x8,
    siVisit   = 0x10,   // признак посещенности клетки
    // клетка окружена стенами
    siAllWalls = siLeft | siRight | siTop | siBottom,
  };

  // клетка со перегородками
  struct cell {
    //
    cell() {
      ramka = siAllWalls;
      value = -1;
    }
    // установить флаг
    void set(uchar side) {
      ramka = ramka | side;
    }
    // сбросить флаг
    void reset(uchar side) {
      ramka = ramka & ~side;
    }
    //
    bool is(uchar side) {
      return (ramka & side) == side;
    }
    // отладочная печать
    void debugPrint() const {
      QString l;
      if(ramka & siLeft)
        l.append("L");
      else
        l.append(".");
      if(ramka & siRight)
        l.append("R");
      else
        l.append(".");
      if(ramka & siTop)
        l.append("T");
      else
        l.append(".");
      if(ramka & siBottom)
        l.append("B");
      else
        l.append(".");
      qDebug() << l;
    }
    // нарисовать клетку в заданной позиции лево/верх заданной длины и ширины
    void draw(QPainter &painter, Window *, QRect &rect) {
      QPen solidPen(QColor(0,0,0));
      QPen dotPen(Qt::gray);
      dotPen.setStyle(Qt::DotLine);
      QPen currentPen;

      if(ramka & siLeft) 
        currentPen = solidPen;
      else
        currentPen = dotPen;
      painter.setPen(currentPen);
      painter.drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());

      if(ramka & siRight) 
        currentPen = solidPen;
      else
        currentPen = dotPen;
      painter.setPen(currentPen);
      painter.drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());

      if(ramka & siTop) 
        currentPen = solidPen;
      else
        currentPen = dotPen;
      painter.setPen(currentPen);
      painter.drawLine(rect.left(), rect.top(), rect.right(), rect.top());
      
      if(ramka & siBottom) 
        currentPen = solidPen;
      else
        currentPen = dotPen;
      painter.setPen(currentPen);
      painter.drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
    }

    /// @param countUnvisited глобальный счетчик посещенности
    void setVisited(int &countUnvisited) {
      // уже посещалась - просто выходим
      if(ramka & siVisit)
        return;
      ramka |= siVisit;
      // учитываем в счетчике посещенных
      countUnvisited--;
    }
    // проверить посещали или нет клетку
    bool isVisited() const {
      return ramka & siVisit;
    }

    // определить является ли ячейка тупиком
    bool isDeadend() const {      
      if( (ramka & siAllWalls) == siAllWalls)
        return true;

      uchar sides[] = { siAllWalls & ~siLeft, siAllWalls & ~siRight, siAllWalls & ~siTop, siAllWalls & ~siBottom };
      for( int i=0; i<4; i++) {
        if( (ramka & sides[i]) == sides[i])
          return true;
      }
      return false;
    }

    // стенки клетки
    uchar ramka;
    short value;
  };
};

namespace mazeio {

  int const maxSize = 100;
  // максимальный размер матрицы смежности
  int const NE = maxSize*maxSize;
  // количество генераторов
  int const gtMax = 5;

  struct generator {

    //
    generator() : pIO(nullptr) {}
    //
    generator(IO*_pIO) : pIO(_pIO) {}
    //
    virtual int ID() const = 0;
    // наименование генератора
    virtual QString title() const = 0;
    // построить лабиринт
    virtual void doMaze() = 0;   
    //
    IO *pIO;
  };

  // статистики
  class statistics : public QObject
  {
    Q_OBJECT
    public:
    //
    struct stat_1Size_x_1Maze {
      // тупики
      int deadCount;
      // время создания лабиринта
      __int64 createTimeMicrosec;
      // средняя длина пути от верхней горизонтали до нижней горизонтали
      int pathLength;
      //
      double orientation;
      // время поиска
      __int64 findPathTimeMicrosec;
      //
      stat_1Size_x_1Maze() {
        clear();
      }
      //
      void clear() {
        deadCount = 0;
        createTimeMicrosec = 0;
        pathLength = 0;
        orientation = 0;
        findPathTimeMicrosec = 0;
      }
    };

    enum statMask {
      smDeadend = 0x1,          // вычисление тупиков
      smCreateTime = 0x2,       // время создания лабиринта
      smPath = 0x4,             // отношение пути к длине лабиринта + время поиска
      smOrientation = 0x8,      // отношение количеств горизонтальных и вертикальных проходов 
      smAll = smDeadend | smCreateTime | smPath | smOrientation,
    };

    // наименование опции
    static QString statoptName(const uchar &statopt);

    struct stat_1Size {
      // статитика по одному размеру для всех генераторов
      statistics::stat_1Size_x_1Maze data[gtMax];
    };

    struct stat_s {

      struct schema {
        int minSize;
        int repeatCount;
        schema(int size, int count) :
          minSize(size), repeatCount(count) {}
        schema() : minSize(0), repeatCount(0) {}
      };

      stat_1Size statistics[maxSize];

      schema deadend;
      schema gentime;
      schema path;
      schema orient;

      uchar mask;

      //
      stat_s () {
        deadend = schema( 2, 20 );
        gentime = schema( 10, 20 );
        path = schema( 10, 20 );
        orient = schema( 2, 10 );
        mask = 15;
      }
    };

    public:
      //
      statistics(const QString &fileName, uchar mask) {
        this->fileName = fileName;
        this->mask = mask;
      }

      // посчитать число тупиков
      int calcDeadend(IO *pIO);

      // посчитать количество проходов по горизонтали и вертикали
      void calculatePass(IO *pIO, int &hPassCount, int &vPassCount);

    public slots:
      // сбор статистики
      void doCollection();
      //
      void finishCollection();


    private:

    private:
      QString fileName;
      uchar mask;
  };

  struct IO {

    enum pathAlgorithm {
      paLee = 0,
      paLeeDeep = 1,
      paFloyd = 2
    };

    //
    IO();

    //
    virtual ~IO ();

    // наименование лабиринта
    virtual QString title() {

      QStringList msgList;            
      msgList << QString::fromLatin1("%1x%2").arg(height).arg(width);

      if(!generatorName.isEmpty())
        msgList << generatorName;

      switch(pathAlgo) {
      case paLee:
        msgList << QString::fromLatin1("Lee broadwise path"); break;
      case paLeeDeep:
        msgList << QString::fromLatin1("Lee deep path"); break;
      case paFloyd:
        msgList << QString::fromLatin1("Floyd path"); break;
      }

      if(resultPath.size())
        msgList << QString::fromLatin1("Min path %1").arg(resultPath.size()-1);

      if(resultSpanningTree.size())
          msgList << QString::fromLatin1("Spanning tree");

      if(bHeatMapMode)
          msgList << QString::fromLatin1("Heat map");
      return msgList.join(" / ");
    }

    // волновой алгоритм нахождения пути в лабиринте
    virtual void findPath() {

      switch(pathAlgo) {        
        //
        case paLeeDeep: {
          findPathLee();
          break;
        }
        //
        case paFloyd: {
          findPathFloyd();
          break;
        }
        //
        case paLee:        
        default: {
          findPathLee();
          break;
        }
      }
    }

    // волновой алгоритм нахождения пути в лабиринте
    virtual void findPathLee();
    // алгоритм Флойда Уоршелла
    virtual void findPathFloyd();

    // построение тепловой карты
    virtual void buildHeatMap();
    //
    void buildSpanningTree(bool bBuild);

    // сохранить описание
    virtual void save(const QString &fileName) const;
    // загрузить описание
    virtual void load(const QString &fileName);

    // сбросить предыдущий лабиринт
    virtual void reset(uchar _ramka = sq::siAllWalls);
    // отладочная печать
    virtual void debugPrint() const;
    // установить размеры лабиринта
    void setSize(int _width, int _height);

    // установить старт лабиринта
    void setStartPos(int row=-1, int col=-1) {
      if(row==-1 || col==-1) {
        startPos = -1;
        return;
      }
      int newPos = position(row, col);
      if(newPos != finishPos)
          startPos = newPos;
    }
    // установить финиш лабиринта
    void setFinishPos(int row=-1, int col=-1) {
      if(row==-1 || col==-1) {
        finishPos = -1;
        return;
      }
      int newPos = position(row, col);
      if(newPos != startPos)
        finishPos = newPos;
    }
    // установить цент тепла лабиринта
    void setHeatPos(int row=-1, int col=-1) {
      int currentPos = position(row, col);
      heatPos = (heatPos == currentPos) ? -1 : currentPos;
    }

    // расчитать позицию лабиринта по строке и столбцу
    int position(int row, int col);
    // небезопасная функция получение ячейки по линейной позиции
    sq::cell *getCell(int pos) {
      return &data[pos / width][pos % width];
    }
    // получить все соседние ячейки которые достижимы из currentPos
    void reachableNeighbor4(int currentPos, int *neighbor, int &size);

    // поставить или снять стенку если получится
    void editWall(int pos1, int pos2);
    // построить стену у ячейки с заданной позиции на заданную длину \enum sq::side
    void buildHWall(int row, int col, int size, uchar orientation);
    // построить стену у ячейки с заданной позиции на заданную высоту \enum sq::side
    void buildVWall(int row, int col, int size, uchar orientation);

    // построить матрицу смежности
    bool incidentMatrix(int **pMatrix, int &size, int blankCell = 0);

    //
    void statistics(bool bDoStatistics);

    // построить матрицу смежности
    // ширина лабиринта
    int width;    
    // высота лабиринта
    int height;   
    
    // имя генератора лабиринта
    QString generatorName;
    
    // описание лабиринта
    sq::cell data[maxSize][maxSize];

    // стартовая позиция
    int startPos;
    // финишная позиция
    int finishPos;

    // центр тепла
    int heatPos;
    // максимальное тепловое значение
    int maxHeatValue;
    // тепловая карта
    QVector<int> heatMap;

    // путь от старта до финиша волновым алгоритмом / Флойда - Уоршелла
    std::vector<int> resultPath;
    // минимальное остовное дерево
    VectorArray<edge<int, int>> resultSpanningTree;
    // режим построения тепловой карты
    bool bHeatMapMode;
    // какой алгоритм используется для поиска пути \enum pathAlgorithm
    int pathAlgo;

    // статистика
    statistics::stat_1Size_x_1Maze statData;
  };

  // отображение лабиринта
  struct view : IO {

    //
    view() : IO() {
      // размер отступа слева/сверху при отображении лабиринта в пх 
      marginPx = 30;
      // размер ячейки в пх при отрисовке
      sizeCellPx = 0;
      //
      bStatistics = false;
    }

    //
    virtual ~view () {}

    // нарисовать лабиринт
    virtual void draw(QPainter &painter, Window *pWindow, const QRect &rect);

    virtual QString title() override {
      QStringList msgList;
      msgList << IO::title(); 
      return msgList.join(" / ");
    } 

    // получить координаты прямоугольника ячейки по его позиции
    QRect getCellRect(int pos, Window *pWindow) const;

    // размер отступа слева/сверху при отображении лабиринта в пх 
    int marginPx;
    // размер ячейки в пх при отрисовке
    int sizeCellPx;
    //
    bool bStatistics;
  };

};

