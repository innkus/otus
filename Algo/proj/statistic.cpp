#include <float.h>
#include <time.h>
#include <math.h>
#include <QFile>
#include <QMessageBox>
#include <chrono>
#include <random> 

#include "mainwindow.h"
#include "bin-maze.h"
#include "backtrack-maze.h"
#include "aldous-broder-maze.h"
#include "recursive-divide-maze.h"
#include "sidewinder-maze.h"

using namespace std::chrono;

using namespace mazeio;

void print(const QString &fileName, const statistics::stat_s &stat, 
  const QMap<int, generator*> &generators) {
  
  QFileInfo fi(fileName);
  QDir dir = fi.dir();
  QString baseName = fi.baseName();
  QString suffix = fi.completeSuffix();

  if(stat.mask & statistics::smDeadend) {

    QString oneFileStat = dir.absoluteFilePath(baseName + "-deadend." + suffix); 

    QFile outputFile(oneFileStat);
    if(!outputFile.open(QIODevice::WriteOnly))
      return;

    QTextStream stream(&outputFile);

    stream << "Deadend count" << endl;  
    stream << "Size;";
    for(int g = 0; g < gtMax; g++) {
      stream << generators[g]->title() << ";";
    }
    stream << endl;

    for(int size=stat.deadend.minSize; size < maxSize; size++) {
      stream << size << ";";
      for(int g = 0; g < gtMax; g++) {
        stream << stat.statistics[size].data[g].deadCount << ";";
      }
      stream << endl;
    }

    outputFile.close();
  }

  if(stat.mask & statistics::smCreateTime) {
    QString oneFileStat = dir.absoluteFilePath(baseName + "-gentime." + suffix); 

    QFile outputFile(oneFileStat);
    if(!outputFile.open(QIODevice::WriteOnly))
      return;

    QTextStream stream(&outputFile);

    stream << "Generation time, micro sec" << endl;  
    stream << "Size;";
    for(int g = 0; g < gtMax; g++) {
      stream << generators[g]->title() << ";";
    }
    stream << endl;

    for(int size=stat.gentime.minSize; size < maxSize; size++) {
      stream << size << ";";
      for(int g = 0; g < gtMax; g++) {
        stream << stat.statistics[size].data[g].createTimeMicrosec << ";";
      }
      stream << endl;
    }

    outputFile.close();

  }

  if(stat.mask & statistics::smPath) {

    {
      QString oneFileStat = dir.absoluteFilePath(baseName + "-pathlen." + suffix); 

      QFile outputFile(oneFileStat);
      if(!outputFile.open(QIODevice::WriteOnly))
        return;

      QTextStream stream(&outputFile);

      stream << "Min path length" << endl;  
      stream << "Size;";
      for(int g = 0; g < gtMax; g++) {
        stream << generators[g]->title() << ";";
      }
      stream << endl;

      for(int size=stat.path.minSize; size < maxSize; size++) {
        stream << size << ";";
        for(int g = 0; g < gtMax; g++) {
          stream << stat.statistics[size].data[g].pathLength << ";";
        }
        stream << endl;
      }

      outputFile.close();
    }

    {
      QString oneFileStat = dir.absoluteFilePath(baseName + "-pathtime." + suffix); 

      QFile outputFile(oneFileStat);
      if(!outputFile.open(QIODevice::WriteOnly))
        return;

      QTextStream stream(&outputFile);

      stream << "Find path time, micro sec" << endl;  
      stream << "Size;";
      for(int g = 0; g < gtMax; g++) {
        stream << generators[g]->title() << ";";
      }
      stream << endl;

      for(int size=stat.path.minSize; size < maxSize; size++) {
        stream << size << ";";
        for(int g = 0; g < gtMax; g++) {
          stream << stat.statistics[size].data[g].findPathTimeMicrosec << ";";
        }
        stream << endl;
      }

      outputFile.close();
    }

  }

  if(stat.mask & statistics::smOrientation) {
    QString oneFileStat = dir.absoluteFilePath(baseName + "-vandh." + suffix); 

    QFile outputFile(oneFileStat);
    if(!outputFile.open(QIODevice::WriteOnly))
      return;

    QTextStream stream(&outputFile);

    stream << "V & H passes" << endl;  
    stream << "Size;";
    for(int g = 0; g < gtMax; g++) {
      stream << generators[g]->title() << ";";
    }
    stream << endl;

    for(int size=stat.orient.minSize; size < maxSize; size++) {
      stream << size << ";";
      for(int g = 0; g < gtMax; g++) {
        stream << stat.statistics[size].data[g].orientation << ";";
      }
      stream << endl;
    }

    outputFile.close();

  }
}


// наименование опции
// ой, криваяяя реализация, просто позор
QString statistics::statoptName(const uchar &statopt) {
  QMap<uchar, QString> mapName;

  mapName[smDeadend]      = QObject::tr("Deadends");
  mapName[smCreateTime]   = QObject::tr("Generation time");
  mapName[smPath]         = QObject::tr("Min path");
  mapName[smOrientation]  = QObject::tr("Passes");
  
  return mapName[statopt];
};

// сбор статистики
void statistics::doCollection() {

  // создаем лабиринт разных размеров разными алгоритмами

  ////////////////////////////////////////////////////////////////////
  // подсчет тупиков
  ////////////////////////////////////////////////////////////////////  
  stat_s statistics;
  statistics.mask = mask;

  QMap<int, generator*> generators;
  generator* g;

  g = (generator*)new binary::generator;
  generators[g->ID()] = g;

  g = (generator*)new backtrack::generator;
  generators[g->ID()] = g;

  g = (generator*)new aldous_broder::generator;
  generators[g->ID()] = g;

  g = (generator*)new recursive_divide::generator;
  generators[g->ID()] = g;

  g = (generator*)new sidewinder::generator;
  generators[g->ID()] = g;

  IO maze;

  if(statistics.mask & smDeadend) {
    
    for(int size=statistics.deadend.minSize; size<maxSize; ++size) {        
     
      for(auto &generator: generators) {

        maze.reset();
        maze.setSize(size, size);
        generator->pIO = &maze;

        generator->doMaze();

        //////////////////////////////////////////////////////////    
        // Тупики
        __int64 deadendCount = 0;
        for(int i=0; i<statistics.deadend.repeatCount; ++i) {
          generator->doMaze();
          deadendCount += calcDeadend(&maze);
        }
        deadendCount /= statistics.deadend.repeatCount;
        statistics.statistics[size].data[generator->ID()].deadCount = deadendCount;
        //////////////////////////////////////////////////////////
      }
       
    }
  }

  if(statistics.mask & statistics::smCreateTime) {

    for(int size=statistics.gentime.minSize; size<maxSize; ++size) {        

      for(auto &generator: generators) {

        maze.reset();
        maze.setSize(size, size);
        generator->pIO = &maze;

        __int64 elapsed;
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();

        for(int i=0; i<statistics.gentime.repeatCount; ++i) {
          generator->doMaze();
        }

        end = std::chrono::system_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>
          (end - start).count();
        statistics.statistics[size].data[generator->ID()].createTimeMicrosec = elapsed / float(1000);
      }
    }
  }

  if(statistics.mask & statistics::smPath) {

    for(int size=statistics.path.minSize; size<maxSize; ++size) {        

      for(auto &generator: generators) {

        maze.reset();
        maze.setSize(size, size);
        generator->pIO = &maze;
        
        int64_t path = 0;

        __int64 elapsed;
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();

        for(int i=0; i<statistics.path.repeatCount; ++i) {

          generator->doMaze();                    
          for(int col = 0; col < maze.width; ++col) {
            maze.setStartPos(0, col);
            maze.setFinishPos(maze.height-1, col);
            maze.findPathLee();
            path += maze.resultPath.size();
          }

        }

        end = std::chrono::system_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>
          (end - start).count();

        //
        statistics.statistics[size].data[generator->ID()].pathLength = 
          path / (statistics.path.repeatCount * maze.width);

        statistics.statistics[size].data[generator->ID()].findPathTimeMicrosec = 
          elapsed / float(1000);
      }
    }
  }


  if(statistics.mask & statistics::smOrientation) {
    
    for(int size=statistics.orient.minSize; size<maxSize; ++size) {        

      for(auto &generator: generators) {

        maze.reset();
        maze.setSize(size, size);
        generator->pIO = &maze;
        
        double vhole = 0, hhole = 0;
        
        for(int i=0; i<statistics.orient.repeatCount; ++i) {

          generator->doMaze();    
          int v,h;
          // посчитать количество проходов по вертикали и горизонтали
          calculatePass(&maze, h, v);
          vhole += h;
          hhole += v;
        }

        vhole /= statistics.orient.repeatCount;
        hhole /= statistics.orient.repeatCount;
        

        //
         statistics.statistics[size].data[generator->ID()].orientation = vhole + hhole;
      }
    }
  }

  print(fileName, statistics, generators); 

  qDeleteAll(generators);
}

void statistics::finishCollection() {
}


// посчитать число тупиков
int statistics::calcDeadend(IO *pIO) {
  int deadendCount = 0;
  for(int pos=0; pos<pIO->width*pIO->height; ++pos) {
    deadendCount += pIO->getCell(pos)->isDeadend();
  }
  return deadendCount;
}

// посчитать количество проходов по горизонтали и вертикали
void statistics::calculatePass(IO *pIO, int &hPassCount, int &vPassCount) {
  hPassCount = vPassCount = 0;

  for(int pos=0; pos<pIO->height*pIO->width; ++pos) {
    if((pIO->getCell(pos)->ramka & sq::siBottom) != sq::siBottom)
      hPassCount++;
    if((pIO->getCell(pos)->ramka & sq::siTop) != sq::siTop)
      hPassCount++;
    if((pIO->getCell(pos)->ramka & sq::siRight) != sq::siRight)
      vPassCount++;
    if((pIO->getCell(pos)->ramka & sq::siLeft) != sq::siLeft)
      vPassCount++;
  }
  vPassCount /=2;
  hPassCount /=2;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// https://habr.com/ru/post/150274/
StatisticsWorker::StatisticsWorker(uchar statopt)
{
	mazeStat = NULL;
  mask = statopt;
}

StatisticsWorker::~ StatisticsWorker()
{
	if (mazeStat != NULL) {
		delete mazeStat;
	}
}

void StatisticsWorker::process()
{  
  if(fileName.isEmpty()) {
		emit finished();
		return;
	}
	mazeStat = new statistics(fileName, mask);
  mazeStat->doCollection();
  emit finished();
}

void StatisticsWorker::stop() {
	if(mazeStat != NULL) {
		
	} 
	return ;
}

//////////////////////////////////////////////////////////////////////////////////////////////
void Session::addThread(QObject *pObject) 
{
  uchar statopt = pObject->property("statopt").toUInt();
  
  StatisticsWorker* worker = new StatisticsWorker(statopt);
  
	QThread* thread = new QThread;
  worker->setFile(fileName); /* передаем список файлов для обработки */
	worker->moveToThread(thread);

  connect(thread, SIGNAL(started()), worker, SLOT(process()));
  // при запуске потока будет вызван метод process(), который создаст объект статистики, который будет работать в новом потоке 

  connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
  // при завершении работы объекта статитстики, обертка передаст потоку сигнал finished() , вызвав срабатывание слота quit()

  connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
  // обертка пометит себя для удаления при окончании сбора статистики

  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));  
  // поток пометит себя для удаления, по окончании сбора статистики. Удаление будет произведено только после полной остановки потока. 

  connect(thread, SIGNAL(finished()), pObject, SLOT(onStatisticsFinished()));  

  thread->start();
  // Запускаем поток, он запускает StatisticsWorker::process(), который создает mazeio::statistic и запускает сбор статистики */

	return ;
}


void Session::statistics(const QString &fileName, Window *pMainWindow)
{
  this->fileName = fileName;
	stopThreads();
  addThread(pMainWindow);
}

Session::~Session()
{
	stopThreads();  /* останавливаем и удаляем потоки  при окончании работы сессии */
}

void Session::stopThreads()  /* принудительная остановка всех потоков */
{
	emit  stopAll(); 
}