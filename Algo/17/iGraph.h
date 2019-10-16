#pragma once

#include "vectorArray.h"


// https://csacademy.com/app/graph_editor/
// https://graphonline.ru/
// http://www.webgraphviz.com/
// https://habr.com/ru/post/116758/
// http://lib.custis.ru/Graphviz
// https://www.stat.auckland.ac.nz/~paul/Reports/gridGraphviz/examples/
// https://www.absint.com/aisee/
// http://wingraphviz.sourceforge.net/wingraphviz/
// https://www.e-olymp.com/ru/blogs/posts/17

template <typename T>
struct edgeSimple {
  T a;
  T b;
  //
  edgeSimple() : a(0), b(0) {}
  //
  edgeSimple(T _a, T _b) : a(_a), b(_b) {}
};

template <typename T, typename W>
struct edge : edgeSimple<T> {
  W weight;

  //
  edge() : edgeSimple<T>(), weight(0) {}
  //
  edge(T _a, T _b, W _weight)
    : edgeSimple<T>(_a, _b), weight(_weight) {}
  //
  bool operator==(const edge &e) const {
    return weight == e.weight;
  }
  //
  bool operator>(const edge &e) const {
    return weight > e.weight;
  }
  //
  bool operator<(const edge &e) const {
    return weight < e.weight;
  }
};

template <typename T>
struct iteratorIncident {
  //
  iteratorIncident() {
    index = 0;
  }
  //
  iteratorIncident(VectorArray<T> _row, T prevIteratorPos) {
    if (prevIteratorPos == 0)
      index = -1;
    else
      index = prevIteratorPos;
    row = _row;
    next();
  }
  //
  bool isEnd() const {
    return index >= row.size();
  }
  //
  void next() {
    index++;
    while (index != row.size()) {
      int x = row.get(index);
      if (x == 0)
        index++;
      else
        return;
    }    
  }
  // номер вершины 
  T get() const {
    return index;
  }
  // строка смежности
  VectorArray<T> row;
  // текущий индекс
  T index = 0;
};

template <typename T>
struct iGraph {
  // количество вершин в графе
  virtual T vertexCount() const = 0;
  // итератор по смежным вершинам
  // начинать с вершины, следующей за prevIteratorPos
  // если prevIteratorPos == 0 начинаем с начала
  virtual iteratorIncident<T> incident(T index, T prevIteratorPos = 0) const = 0;
  // получить строку матрицы смежности
  virtual VectorArray<T> getRow(T row) const = 0;
  // инвертировать граф
  virtual void invert() = 0;
  //
  virtual void print(
    const char *msg = nullptr,
    const VectorArray<VectorArray<T>> *topoSort = nullptr
  ) const = 0;
  // печать с весом ребер + выделение цветом заданного списка ребер
  virtual void printEdges(
    const char *msg,
    const VectorArray<edge<T,int>> &v) const = 0;
  // проверить являетя ли граф ориентированным
  virtual bool isOrgraph() const = 0;
  // проверить есть ли ребро от vFrom до vTo
  virtual bool fromTo(T vFrom, T vTo) const = 0;
};

