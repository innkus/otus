#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);  
  a.setStyleSheet("Window { background-image: url(:/res/cross.png); }");

  Window win;

  win.setWindowIcon(QIcon(":/res/title.png"));
  win.setWindowTitle("Maze");
  win.readSettings();
  win.show();

  return a.exec();
}


// Сохранять Л в файл
// Загружать Л из файла
// Построить остовное дерево
// Построить матрицу смежности
// Показывать матрицу смежности ?
// Справка по Л в html
// Какие статистики по Л ?
// Алгоритм обход по правилу правой руки
// Алгоритм Дейкстры ?
// + написать номера для алгоритма вширь
// анимация волнового алгоритмв
// сделать возможность включения отключения 
// + - отображения чисел волнового алгоритма
//  - пути волнового алгоритма
// вводить размер лабиринта
// + разнести IO на IO и IORender
// создать тепловую карту

// https://sheba.spb.ru/za/matematika-labirinta-1987.htm
// http://www.astrolog.org/labyrnth/algrithm.htm
// http://weblog.jamisbuck.org/2011/1/20/maze-generation-wilson-s-algorithm

// https://jeremybytes.blogspot.com/2017/07/
// http://programmeringsbloggen.no/2019/02/11/maze2/

// тепловая карта
// https://jeremybytes.blogspot.com/2017/07/more-maze-programming-heat-map.html