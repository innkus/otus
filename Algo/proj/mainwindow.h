#pragma once 

#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QMenu>
#include <QLabel>
#include <qmenubar.h>

#include "mazeio.h"
#include "backtrack-maze.h"
#include "bin-maze.h"
#include "aldous-broder-maze.h"
#include "recursive-divide-maze.h"
#include "sidewinder-maze.h"

class StatisticsWorker : public QObject {     
	Q_OBJECT

private:
	mazeio::statistics *mazeStat; 	
  QString  fileName; 
  uchar mask;

public:
	StatisticsWorker(uchar statopt);
	~StatisticsWorker();

	void setFile(const QString &file) { fileName = file; } /* имя файла статистики*/

public slots:
	void process(); 	/*  создает и запускает статистику*/
	void stop();    	/*  останавливает статистику*/

signals:
	void finished(); 	/* сигнал о завершении  работы статистики*/
};

class Session : public QObject {
	Q_OBJECT

public:
  Session() {}
	~Session();

	void statistics(const QString &fileName, Window *pMainWindow);

signals:
  void stopAll(); //остановка всех потоков

private:
	void addThread(QObject *pObject);    
	void stopThreads();
  
  QString fileName;

};

class Window : public QMainWindow
{
    Q_OBJECT

public:
  Window();
  virtual ~Window();
  void readSettings();

private:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);
  void closeEvent(QCloseEvent *event);
  void mouseDoubleClickEvent (QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

  void createMenus();
  void createActions();

  QAction *m_mazeBinAction;
  QAction *m_mazeBacktrackAction;
  QAction *m_mazeAldousAction; 
  QAction *m_mazeRecursiveDivideAction;
  QAction *m_mazeSidewinderAction;

  QAction *m_mazePathLeeAction;
  QAction *m_mazePathLeeDeepAction;
  QAction *m_mazePathFloydAction;

  QAction *m_showNumbersPathAction;
  QAction *m_fileOpenAction;
  QAction *m_fileSaveAction;
  QAction *m_heatMapAction;
  QAction *m_spanningTreeAction;
  QAction *m_mazeEmptyAction;

  QAction *m_statMazePacket;
  QAction *m_statOneMaze;

  QList<QAction*> m_mazeSizeActionList;
  QList<QAction*> m_mazeStatOptActionList;

  QLabel *m_sizeLabel;
  //
  void onStatMazePacket();
  //
  void onStatOneMaze();
  //
  void onMazeSidewinderAction();
  //
  void onMazeRecursiveDivideAction();
  // построить бинарный лабиринт
  void onMazeBinAction();
  // 
  void onMazeBacktrackAction();
  //
  void onMazeAldousAction();

  // волновой алгоритм
  void onMazePathLee();
  // волновой алгоритм
  void onMazePathLeeDeep();
  // Флойд Уоршелл
  void onMazePathFloyd();

  //
  void onShowNumbersPathAction();
  //
  void onFileOpenAction();
  //
  void onFileSaveAction();
  //
  void onHeatMapAction();
  //
  void onSpanningTreeAction();
  //
  void onMazeEmptyAction();

private:
  //
  void onMazeAction(mazeio::generator *generator);
  // действия которые выполняются при создании нового лабиринта
  // или при построении волнового пути или еще что-то
  void onMazeUpdate();
  // сброс расчетных настроек
  void onMazeClear();
  // отредактировать лабиринт в конкретной точке
  void editCellMaze(const QPoint &cursorPoint);
  //
  void setWindowTitleLocal();
  //
  void setCheckedPathAlgo();

public:

  // отрисовка в ячейках номеров для волнового алгоритма
  bool m_bShowNumbersPath;
  // надо или нет строить остовное дерево
  bool m_buildSpanningTree;

public slots:
  //
  void mazeSizeChanged();
  //
  void onStatisticsFinished();
  //
  void updateTime();      
  //
  void mazeStatOptChanged();

protected:

  QMenu *m_fileMenu;
  QMenu *m_generateMenu;
  QMenu *m_actionMenu;
  QMenu *m_settingsMenu;
  QMenu *m_sizeMenu;
  QMenu *m_statMenu;

  mazeio::view *m_pCurrentMaze;
  mazeio::view m_xCurrentMaze;

  QString m_statisticsFileName;
  Session m_statSession;
  QTimer *m_timer;
  QTime m_startStatisticTime;
  QString m_statString;
  uchar m_statOpt;
};

