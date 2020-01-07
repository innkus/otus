#include <QApplication>
#include <QPainter>
#include <QSettings>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QToolBar>
#include <QComboBox>
#include <QWidgetAction>
#include <QThread>
#include <QMessageBox>
#include <QStatusBar>

#include "mainwindow.h"

#define default_maze_size (20)

Window::Window() 
{
  m_pCurrentMaze  = &m_xCurrentMaze;

  if(m_pCurrentMaze)
    m_pCurrentMaze->setSize(default_maze_size, default_maze_size);

  m_bShowNumbersPath = true;
  m_buildSpanningTree = false;
  m_statisticsFileName = QString("stat.txt");
  m_startStatisticTime = QTime();
  m_timer = nullptr;
  m_statOpt = mazeio::statistics::smAll;
  setProperty("statopt", m_statOpt);

  createActions();
  createMenus();

  //m_sizeLabel = new QLabel("");    
  //m_sizeLabel->setAlignment(Qt::AlignHCenter);  
  //m_sizeLabel->setMinimumSize(m_sizeLabel->sizeHint());
  //statusBar()->addWidget(m_sizeLabel);
}

//
void Window::mazeSizeChanged() {
  QObject* sizeAction = qobject_cast<QObject*>(sender());
  if(!sizeAction)
    return;

  int size = sizeAction->property("size").toUInt();
  onMazeClear();

  m_pCurrentMaze->bHeatMapMode = false;
  m_heatMapAction->setChecked(m_pCurrentMaze->bHeatMapMode);
  
  m_buildSpanningTree = false;
  m_spanningTreeAction->setChecked(m_buildSpanningTree);
  m_pCurrentMaze->buildSpanningTree(m_buildSpanningTree);

  m_pCurrentMaze->reset();
  m_pCurrentMaze->setSize(size, size);  
  onMazeUpdate(); 
}


void Window::mazeStatOptChanged() {
  QAction* optAction = qobject_cast<QAction*>(sender());

  uint opt = optAction->property("statopt").toUInt();
  if(optAction->isChecked()) {
    m_statOpt |= opt;
  } 
  else 
  {
    m_statOpt &= ~opt;
  }
  setProperty("statopt", m_statOpt);
}

Window::~Window()  {
  if(m_timer)
    delete m_timer;
}

void Window::resizeEvent(QResizeEvent *) {  
  repaint();
}

void Window::readSettings() {
  QSettings settings("InkaSoft", "mymaze");
  restoreGeometry(settings.value("widget/geometry").toByteArray());
  restoreState(settings.value("widget/windowState").toByteArray());
  if(m_pCurrentMaze) {
    int height = settings.value("maze/size-height", default_maze_size).toInt();
    int width = settings.value("maze/size-width", default_maze_size).toInt();
    m_pCurrentMaze->setSize(width, height);
  }
  onMazeUpdate();
}
 
void Window::closeEvent(QCloseEvent *event)
{
  QSettings settings("InkaSoft", "mymaze");
  settings.setValue("widget/geometry", saveGeometry());
  settings.setValue("widget/windowState", saveState());
  if(m_pCurrentMaze) {
    settings.setValue("maze/size-height", m_pCurrentMaze->height);
    settings.setValue("maze/size-width", m_pCurrentMaze->width);
  }
  QMainWindow::closeEvent(event);
}

void Window::paintEvent(QPaintEvent *) {

 QPainter painter(this);
 if(m_pCurrentMaze) {
   m_pCurrentMaze->draw(painter, this, QRect());
 }
}


void Window::createMenus()
{
  m_fileMenu = menuBar()->addMenu(tr("&Maze"));
  m_fileMenu->addAction(m_fileOpenAction);
  m_fileMenu->addAction(m_fileSaveAction);

  m_sizeMenu = menuBar()->addMenu(tr("&Size"));
  m_sizeMenu->addActions(m_mazeSizeActionList);

  m_generateMenu = menuBar()->addMenu(tr("&Generate"));    
  m_generateMenu->addAction(m_mazeBinAction);
  m_generateMenu->addAction(m_mazeBacktrackAction);
  m_generateMenu->addAction(m_mazeAldousAction);
  m_generateMenu->addAction(m_mazeRecursiveDivideAction);
  m_generateMenu->addAction(m_mazeSidewinderAction);
  m_generateMenu->addSeparator();
  m_generateMenu->addAction(m_mazeEmptyAction);

  m_actionMenu = menuBar()->addMenu(tr("&Actions"));
  m_actionMenu->addAction(m_mazePathLeeAction);
  m_actionMenu->addAction(m_mazePathLeeDeepAction);
  m_actionMenu->addAction(m_mazePathFloydAction);
  m_actionMenu->addSeparator();
  m_actionMenu->addAction(m_heatMapAction);  
  m_actionMenu->addAction(m_spanningTreeAction);

  m_settingsMenu = menuBar()->addMenu(tr("&Setttings"));
  m_settingsMenu->addAction(m_showNumbersPathAction);

  m_statMenu = menuBar()->addMenu(tr("&Statistics"));
  m_statMenu->addAction(m_statMazePacket);  
  m_statMenu->addAction(m_statOneMaze);
  m_statMenu->addSeparator();
  m_statMenu->addActions(m_mazeStatOptActionList);
  
}

#define ADD_SIZE_MENU(size) \
  {\
    QString stSize = QString::fromLatin1("  %1  ").arg(size, 2);\
    QAction *pAction = new QAction(stSize, this);\
    connect(pAction, &QAction::triggered, this, &Window::mazeSizeChanged);\
    pAction->setProperty("size", size);\
    pAction->setStatusTip(stSize);\
    m_mazeSizeActionList << pAction;\
  }

#define ADD_STATOPT_MENU(opt) \
  {\
    QString stOpt = QString::fromLatin1("%1").arg(mazeio::statistics::statoptName(opt));\
    QAction *pAction = new QAction(stOpt, this);\
    connect(pAction, &QAction::triggered, this, &Window::mazeStatOptChanged);\
    pAction->setProperty("statopt", opt);\
    pAction->setCheckable(true);\
    qDebug() << opt << m_statOpt << (m_statOpt & opt) << ((m_statOpt & opt) == opt);\
    pAction->setChecked((m_statOpt & opt) == opt);\
    pAction->setStatusTip(stOpt);\
    m_mazeStatOptActionList << pAction;\
  }


void Window::createActions()
{
  QList<int> listSize;
  listSize <<  2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 12<< 15 << 20 << 25 << 30 << 35 << 40 << 45 << 50; 
  foreach(const auto &size, listSize) {
    ADD_SIZE_MENU(size)
  }

  QList<uchar> listStatOpt;
  listStatOpt <<  0x1 << 0x2 << 0x4 << 0x8; 
  foreach(const auto &opt, listStatOpt) {
    ADD_STATOPT_MENU(opt)
  }

  m_statMazePacket = new QAction(tr("Maze packet"), this);
  m_statMazePacket->setStatusTip(tr("Calculate maze packet statistics"));  
  connect(m_statMazePacket, &QAction::triggered, this, &Window::onStatMazePacket);

  m_statOneMaze = new QAction(tr("Current maze"), this);
  m_statOneMaze->setStatusTip(tr("Calculate statistics for current maze"));  
  m_statOneMaze->setCheckable(true);
  m_statOneMaze->setChecked(m_pCurrentMaze->bStatistics);
  connect(m_statOneMaze, &QAction::triggered, this, &Window::onStatOneMaze);

  m_mazeEmptyAction = new QAction(tr("&Empty maze"), this);
  m_mazeEmptyAction->setStatusTip(tr("Build empty maze"));  
  connect(m_mazeEmptyAction, &QAction::triggered, this, &Window::onMazeEmptyAction);

  m_spanningTreeAction = new QAction(tr("&Spanning tree"), this);
  m_spanningTreeAction->setStatusTip(tr("Build spanning tree"));
  m_spanningTreeAction->setCheckable(true);
  m_spanningTreeAction->setChecked(m_buildSpanningTree);
  connect(m_spanningTreeAction, &QAction::triggered, this, &Window::onSpanningTreeAction);

  m_heatMapAction = new QAction(tr("&Heat map"), this);
  m_heatMapAction->setStatusTip(tr("Build heat map"));
  m_heatMapAction->setCheckable(true);
  m_heatMapAction->setChecked(m_pCurrentMaze->bHeatMapMode);
  connect(m_heatMapAction, &QAction::triggered, this, &Window::onHeatMapAction);

  m_fileOpenAction = new QAction(tr("&Open"), this);
  m_fileOpenAction->setStatusTip(tr("Load maze from file"));
  connect(m_fileOpenAction, &QAction::triggered, this, &Window::onFileOpenAction);

  m_fileSaveAction = new QAction(tr("&Save"), this);
  m_fileSaveAction->setStatusTip(tr("Save maze to file"));
  connect(m_fileSaveAction, &QAction::triggered, this, &Window::onFileSaveAction);

  m_mazeRecursiveDivideAction = new QAction(tr("Recursive divide maze"), this);
  m_mazeRecursiveDivideAction->setStatusTip(tr("Build recursive divide maze"));
  connect(m_mazeRecursiveDivideAction, &QAction::triggered, this, &Window::onMazeRecursiveDivideAction);

  m_mazeBinAction = new QAction(tr("&Binary maze"), this);
  m_mazeBinAction->setStatusTip(tr("Build binary maze"));
  connect(m_mazeBinAction, &QAction::triggered, this, &Window::onMazeBinAction);

  m_mazeBacktrackAction = new QAction(tr("&Backtrack maze"), this);
  m_mazeBacktrackAction->setStatusTip(tr("Build backtrack maze"));
  connect(m_mazeBacktrackAction, &QAction::triggered, this, &Window::onMazeBacktrackAction);

  m_mazeAldousAction = new QAction(tr("&Aldous-Broder maze"), this);
  m_mazeAldousAction->setStatusTip(tr("Build Aldous-Broder maze"));
  connect(m_mazeAldousAction, &QAction::triggered, this, &Window::onMazeAldousAction);

  m_mazeSidewinderAction = new QAction(tr("Sidewinder maze"), this);
  m_mazeSidewinderAction->setStatusTip(tr("Build sidewinder maze"));
  connect(m_mazeSidewinderAction, &QAction::triggered, this, &Window::onMazeSidewinderAction);

  ////////////////////////////////////////////////////////////////////////////////////
  m_mazePathLeeAction = new QAction(tr("Build Lee path broadwise"), this);
  m_mazePathLeeAction->setStatusTip(tr("Build Lee path broadwise"));
  m_mazePathLeeAction->setCheckable(true);  
  connect(m_mazePathLeeAction, &QAction::triggered, this, &Window::onMazePathLee);

  m_mazePathLeeDeepAction = new QAction(tr("Build Lee path deep"), this);
  m_mazePathLeeDeepAction->setStatusTip(tr("Build Lee path deep"));
  m_mazePathLeeDeepAction->setCheckable(true);  
  connect(m_mazePathLeeDeepAction, &QAction::triggered, this, &Window::onMazePathLeeDeep);

  m_mazePathFloydAction = new QAction(tr("Build Floyd path"), this);
  m_mazePathFloydAction->setStatusTip(tr("Build Floyd path"));
  m_mazePathFloydAction->setCheckable(true);    
  connect(m_mazePathFloydAction, &QAction::triggered, this, &Window::onMazePathFloyd);

  setCheckedPathAlgo();
  ////////////////////////////////////////////////////////////////////////////////////
  
  m_showNumbersPathAction = new QAction(tr("&Path numbers"), this);
  m_showNumbersPathAction->setStatusTip(tr("Show/hide path numbers on maze"));
  m_showNumbersPathAction->setCheckable(true);
  m_showNumbersPathAction->setChecked(m_bShowNumbersPath);
  connect(m_showNumbersPathAction, &QAction::triggered, this, &Window::onShowNumbersPathAction);

  //QList<QAction*> listActions;

  //QToolBar* pToolBar = new QToolBar("Tools");
  //addToolBar(Qt::RightToolBarArea, pToolBar);
  //QComboBox *sizeComboBox = new QComboBox();
  //for(int i=2; i<=50; i++) {
  //  sizeComboBox->addItem(QString::fromLatin1("  %1  ").arg(i,2), i);
  //}
  //QWidgetAction *mazeSizeAction = new QWidgetAction(this);
  //mazeSizeAction->setDefaultWidget(sizeComboBox);
  //pToolBar->addAction(mazeSizeAction);
  //connect(sizeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(mazeSizeChanged(int)));
  //
  //setContextMenuPolicy(Qt::NoContextMenu);
}

//
void Window::setCheckedPathAlgo() {
  if(!m_pCurrentMaze)
    return;

  m_mazePathFloydAction->setChecked(m_pCurrentMaze->pathAlgo == mazeio::IO::paFloyd);
  m_mazePathLeeDeepAction->setChecked(m_pCurrentMaze->pathAlgo == mazeio::IO::paLeeDeep);
  m_mazePathLeeAction->setChecked(m_pCurrentMaze->pathAlgo == mazeio::IO::paLee);
}

//
void Window::onMazeAction(mazeio::generator *generator) {

  if(!m_pCurrentMaze || !generator)
      return;

  m_pCurrentMaze->bHeatMapMode = false;
  m_heatMapAction->setChecked(m_pCurrentMaze->bHeatMapMode);
  
  m_buildSpanningTree = false;
  m_spanningTreeAction->setChecked(m_buildSpanningTree);

  int start = m_pCurrentMaze->startPos, finish = m_pCurrentMaze->finishPos;

  m_pCurrentMaze->reset();
  onMazeClear();

  generator->doMaze();

  m_pCurrentMaze->startPos = start;
  m_pCurrentMaze->finishPos = finish;
  m_pCurrentMaze->findPath();
  m_pCurrentMaze->buildSpanningTree(m_buildSpanningTree);
  m_pCurrentMaze->debugPrint();
  setWindowTitleLocal();
  update();
}

//
void Window::onMazeRecursiveDivideAction() {
  qDebug() << "TODO onMazeRecursiveDivideAction";
  if(!m_pCurrentMaze)
      return;
  recursive_divide::generator generator(m_pCurrentMaze);
  onMazeAction(&generator);
}

void Window::onMazeBinAction() {
  qDebug() << "TODO onMazeBinAction";
  if(!m_pCurrentMaze)
      return;
  binary::generator generator(m_pCurrentMaze);
  onMazeAction(&generator);
}

void Window::onMazeBacktrackAction() {
  qDebug() << "TODO onMazeBacktrackAction";
  if(!m_pCurrentMaze)
      return;
  backtrack::generator generator(m_pCurrentMaze);
  onMazeAction(&generator);
}

void Window::onMazeAldousAction() {
  qDebug() << "TODO onMazeAldousAction";
  if(!m_pCurrentMaze)
      return;
  aldous_broder::generator generator(m_pCurrentMaze);
  onMazeAction(&generator);
}

void Window::onMazeSidewinderAction() {
  qDebug() << "TODO onMazeSidewinderAction";
  if(!m_pCurrentMaze)
      return;
  sidewinder::generator generator(m_pCurrentMaze);
  onMazeAction(&generator);
}

void Window::mouseDoubleClickEvent ( QMouseEvent * event ) {
  if(!event || !m_pCurrentMaze || !m_pCurrentMaze->sizeCellPx)
    return;

  QPoint cursorPoint = event->pos();
  qDebug() << cursorPoint.x() << " " << cursorPoint.y();

  QSize wSize = size();
  int row = -1, col = -1;
  if(cursorPoint.x() - m_pCurrentMaze->marginPx >=0 )
    col = (cursorPoint.x() - m_pCurrentMaze->marginPx) / m_pCurrentMaze->sizeCellPx;
  if(cursorPoint.y() - m_pCurrentMaze->marginPx >=0)
    row = (cursorPoint.y() - m_pCurrentMaze->marginPx) / m_pCurrentMaze->sizeCellPx;
  qDebug() << row << " " << col;


  if(qApp->mouseButtons().testFlag(Qt::MouseButton::LeftButton)) {

    Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers();
    if(modifiers.testFlag(Qt::ShiftModifier)) 
    {
      // режим редактирования
      editCellMaze(cursorPoint);
    }
    else 
    {

      if(m_pCurrentMaze->bHeatMapMode) {
        m_pCurrentMaze->setHeatPos(row, col);
        m_pCurrentMaze->buildHeatMap();          
      } 
      else 
      {
        m_pCurrentMaze->setStartPos(row, col);
      }

    }

  }

  if(qApp->mouseButtons().testFlag(Qt::MouseButton::RightButton)) {

    Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers();
    if(modifiers.testFlag(Qt::ShiftModifier)) 
    {
    }
    else 
    {
      if(m_pCurrentMaze->bHeatMapMode)
      {
        // не отрабатываем правую кнопку
      }
      else 
      {
        m_pCurrentMaze->setFinishPos(row, col);
      }
    }

  }   

  onMazeUpdate();
}

void Window::keyPressEvent(QKeyEvent *) {

}

void Window::keyReleaseEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Escape: {
       if(m_pCurrentMaze) {

        if(m_pCurrentMaze->bHeatMapMode)
        {
          // не будем убирать карту
          // отмена режима только по меню
          // m_pCurrentMaze->setHeatPos();
          // m_pCurrentMaze->buildHeatMap();
        } 
        else 
        {
          m_pCurrentMaze->setStartPos();
          m_pCurrentMaze->setFinishPos();
          m_pCurrentMaze->findPath();
        }

        setWindowTitleLocal();
        update();
      }
      break;
    }
    default: {
    }
  }
}

// действия которые выполняются при создании нового лабиринта
// или при построении волнового пути или еще что-то
void Window::onMazeUpdate() {
  
  if(!m_pCurrentMaze)
    return;

  m_pCurrentMaze->findPath();    
  
  setWindowTitleLocal();
  m_pCurrentMaze->debugPrint();
  update();  
}

// сброс расчетных настроек
void Window::onMazeClear() {
  if(!m_pCurrentMaze)
    return;

  m_pCurrentMaze->setStartPos();
  m_pCurrentMaze->setFinishPos();
  m_pCurrentMaze->setHeatPos();
  m_pCurrentMaze->buildHeatMap();
  //m_pCurrentMaze->buildSpanningTree(false);
}

void Window::onShowNumbersPathAction() {
  m_bShowNumbersPath = !m_bShowNumbersPath;
  update();
}

//
void Window::onFileOpenAction() {
  QString loadFileName = QFileDialog::getOpenFileName(this, "Open maze", QString(), QString("Maze files (*.maze);;All files (*.*)"));
  if(m_pCurrentMaze && !loadFileName.isEmpty()) 
    m_pCurrentMaze->load(loadFileName);
  onMazeUpdate();
}
//
void Window::onFileSaveAction() {
  QString saveFileName = QFileDialog::getSaveFileName(this, QString("Save maze"), QString("noname.maze"), QString("Maze files (*.maze)"));
  if(m_pCurrentMaze && !saveFileName.isEmpty())
    m_pCurrentMaze->save(saveFileName);
}

// тепловая карта
void Window::onHeatMapAction() {
  if(!m_pCurrentMaze)
    return;

  m_pCurrentMaze->bHeatMapMode = !m_pCurrentMaze->bHeatMapMode;
  if(m_pCurrentMaze->bHeatMapMode)
    m_pCurrentMaze->setHeatPos(0,0);
  else
    m_pCurrentMaze->setHeatPos();
  m_pCurrentMaze->buildHeatMap();
  onMazeUpdate();
}

// остовное дерево
void Window::onSpanningTreeAction() {
  if(!m_pCurrentMaze)
    return;

  m_buildSpanningTree = !m_buildSpanningTree;
  m_pCurrentMaze->buildSpanningTree(m_buildSpanningTree);
  onMazeUpdate();
}

//
void Window::onMazeEmptyAction() {
  if(!m_pCurrentMaze)
    return;

  m_pCurrentMaze->reset();
  onMazeClear();
  onMazeUpdate();
}

// отредактировать лабиринт в конкретной точке
void Window::editCellMaze(const QPoint &cursorPoint) {
  
  if(!m_pCurrentMaze)
    return;

  int row = -1, col = -1;
  if(cursorPoint.x() - m_pCurrentMaze->marginPx >=0 )
    col = (cursorPoint.x() - m_pCurrentMaze->marginPx) / m_pCurrentMaze->sizeCellPx;
  if(cursorPoint.y() - m_pCurrentMaze->marginPx >=0)
    row = (cursorPoint.y() - m_pCurrentMaze->marginPx) / m_pCurrentMaze->sizeCellPx;

  if( row == -1 || col == -1)
    return;

  int dR = 15;
  QRect cursorRect;
  cursorRect.setRect(cursorPoint.x() - dR, cursorPoint.y() - dR, 2*dR, 2*dR);

  int editCells[9], num = 0;

  for(int r=row-1; r<=row+1; ++r) {
    if(row == -1)
      continue;

    for(int c=col-1; c<=col+1; ++c) {
      if(col == -1)
        continue;
      int currentPos = m_pCurrentMaze->position(r,c);
      QRect rect = m_pCurrentMaze->getCellRect(currentPos, this);
      if(!rect.isEmpty() && rect.intersects(cursorRect)) {
        editCells[num++] = currentPos;
      }
    
    }
  }
  
  for(int a=0; a<num; ++a) {
    for(int b=a+1; b<num; ++b) {
      // поставить или снять стенку если получится
      m_pCurrentMaze->editWall(editCells[a], editCells[b]);
    }
  }


  if(m_buildSpanningTree)
    m_pCurrentMaze->buildSpanningTree(m_buildSpanningTree);
}

//
void Window::onStatisticsFinished() {

  if(m_timer) {
    m_timer->stop();
    m_startStatisticTime = QTime();
  }

  QMessageBox msgBox;
     msgBox.setWindowIcon(QIcon(":/res/title.png"));
  msgBox.setIcon(QMessageBox::Information);
  msgBox.setText("Statistics collection completed.");

  QFileInfo fi(m_statisticsFileName);
  QDir dir = fi.dir();
  QString baseName = fi.baseName();
  QString suffix = fi.completeSuffix();
  QString oneFileStat = dir.absoluteFilePath(baseName + "-XXX." + suffix); 

  msgBox.setInformativeText(tr("Statistics is written into %1").arg(oneFileStat));
  msgBox.exec();
  m_statString.clear();
  setWindowTitleLocal();
}

//
void Window::updateTime() {
  uint dT = m_startStatisticTime.secsTo(QTime::currentTime());
  uint h = dT / 3600;
  uint m = (dT - h * 3600) / 60;
  uint s = (dT - h * 3600) % 60;
  m_statString = QString::fromLatin1(" / Statistics %1:%2:%3")
    .arg(h,2,10,QChar('0')).arg(m,2,10,QChar('0')).arg(s,2,10,QChar('0'));
  setWindowTitleLocal();
}

//
void Window::setWindowTitleLocal() {
  if(m_statString.isEmpty())
    setWindowTitle(m_pCurrentMaze->title());
  else
    setWindowTitle(m_pCurrentMaze->title() + m_statString);
}

//
void Window::onStatMazePacket() {

  QString fileName = QFileDialog::getSaveFileName(
    this, QString("Save statistic"), m_statisticsFileName, QString("Txt files (*.txt)"));

  if(fileName.isEmpty())
    return;

  m_statisticsFileName = fileName;

  if(!m_timer) {
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    m_timer->setInterval(1000);  
  }
  m_startStatisticTime = QTime::currentTime();  
  m_timer->start();

  m_statSession.statistics(m_statisticsFileName, this);

}

//
void Window::onStatOneMaze() {
  if(!m_pCurrentMaze)
    return;
  m_pCurrentMaze->bStatistics = !m_pCurrentMaze->bStatistics;
  m_pCurrentMaze->statistics(m_pCurrentMaze->bStatistics);
  onMazeUpdate();
}

// волновой алгоритм
void Window::onMazePathLee() {
  if(!m_pCurrentMaze)
    return;

  m_pCurrentMaze->pathAlgo = mazeio::IO::paLee;
  setCheckedPathAlgo();
  onMazeUpdate();
}


// волновой алгоритм
void Window::onMazePathLeeDeep() {
  if(!m_pCurrentMaze)
    return;

  m_pCurrentMaze->pathAlgo = mazeio::IO::paLeeDeep;
  setCheckedPathAlgo();
  onMazeUpdate();
}

// Флойд Уоршелл
void Window::onMazePathFloyd() {
  if(!m_pCurrentMaze)
    return;

  m_pCurrentMaze->pathAlgo = mazeio::IO::paFloyd;
  setCheckedPathAlgo();
  onMazeUpdate();
}
