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


struct iteratorIncident {
  //
  iteratorIncident() {
    index = 0;
  }
  //
  iteratorIncident(VectorArray<int> _row, int prevIteratorPos) {
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
  int get() const {
    return index;
  }
  // строка смежности
  VectorArray<int> row;
  // текущий индекс
  int index = 0;
};

struct graph {
  // количество вершин в графе
  virtual int vertexCount() const = 0;
  // итератор по смежным вершинам
  // начинать с вершины, следующей за prevIteratorPos
  // если prevIteratorPos == 0 начинаем с начала
  virtual iteratorIncident incident(int index, int prevIteratorPos = 0) const = 0;
  // получить строку матрицы смежности
  virtual VectorArray<int> getRow(int row) const = 0;
  // инвертировать граф
  virtual void invert() = 0;
  //
  virtual void print(
    const char *msg = nullptr,
    const VectorArray<VectorArray<int>> *topoSort = nullptr
  ) const = 0;
  // проверить являетя ли граф ориентированным
  virtual bool isOrgraph() const = 0;
  // проверить есть ли ребро от vFrom до vTo
  virtual bool fromTo(int vFrom, int vTo) const = 0;
};

