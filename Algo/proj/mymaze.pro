#-------------------------------------------------
#
# Project created by QtCreator 2017-02-13T18:21:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mymaze
TEMPLATE = app


SOURCES += main.cpp \
    mazeio.cpp \
    mainwindow.cpp \
    bin-maze.cpp \
    backtrack-maze.cpp \
    aldous-broder-maze.cpp \
    recursive-divide-maze.cpp \
	sidewinder-maze.cpp \
    statistic.cpp
    
HEADERS  += mazeio.h \
    mainwindow.h \
    bin-maze.h \
    backtrack-maze.h \
    aldous-broder-maze.h \
    stack.h \
    node.h \
    queue.h \
    iArray.h \
    vectorArray.h \
    iGraph.h \
    graph.h \
    kraskal.h \
    recursive-divide-maze.h \
	sidewinder-maze.h

    
RESOURCES = mymaze.qrc