#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "iGraph.h"

//
template<typename T>
class myGraph : iGraph<T> {
public:
  //
  myGraph(T N, const T *data) : bOrgraph(false), bWeight(false) {
    init(N, data);
  }
  //
  void init(T N, const T *data) {
    matrixNeibor.clear();
    //
    for (T row = 0; row < N; ++row) {
      VectorArray<T> line;
      for (T col = 0; col < N; ++col) {
        line.add(data[row * N + col]);
        // признак орграфа
        if (data[row * N + col] != data[col * N + row])
          bOrgraph = true;
        if (data[row * N + col] != 0 && data[row * N + col] != 1)
          bWeight = true;
      }
      // скорректировать диагональные элементы
      line.set(0, row);
      matrixNeibor.add(line);
    }
  }
  // получить строку матрицы смежности
  virtual VectorArray<T> getRow(T row) const override {
    return matrixNeibor.get(row);
  }
  // количество вершин в графе
  virtual T vertexCount() const override {
    return matrixNeibor.size();
  }
  // итератор по смежным вершинам
  // начинать с вершины, следующей за prevIteratorPos
  // если prevIteratorPos == 0 начинаем с начала
  iteratorIncident<T> incident(T index, T prevIteratorPos) const override {
    if (index >= vertexCount())
      return iteratorIncident<T>();

    VectorArray<T> row = matrixNeibor.get(index);
    return iteratorIncident<T>(row, prevIteratorPos);
  }

  //
  void print(
    const char *msg,
    const VectorArray<VectorArray<T>> *topoSort
  ) const override {

    if (msg)
      std::cout << msg << "\n";

    std::string edgeView;
    if (bOrgraph) {
      std::cout << "digraph G { rankdir = LR;\n";
      edgeView = "\" -> \"";
    }
    else {
      std::cout << "graph G { rankdir = LR;\n";
      edgeView = "\" -- \"";
    }

    // печатаем исходный граф
    if (topoSort) {
      printGraphItem("_", "_", nullptr);
      std::cout << "\n";
      printGraphItem("", "", topoSort);
    }
    else
      printGraphItem("", "", nullptr);

    std::cout << "}\n\n";
  }

  // инвертировать граф
  void invert() override {
    // транспонируем матрицу
    T N = vertexCount();
    VectorArray<T> elements(N * N);
    for (T i = 0; i < N * N; ++i)
      elements.add(0);

    for (T row = 0; row < N; ++row) {
      VectorArray<T> r = matrixNeibor.get(row);
      for (T col = 0; col < N; ++col) {
        T x = r.get(col);
        elements.set(x, col * N + row);
      }
    }
    init(N, elements.data());
  }

  // проверить являетя ли граф ориентированным
  virtual bool isOrgraph() const override {
    return bOrgraph;
  }

  // проверить есть ли ребро от vFrom до vTo
  virtual bool fromTo(T vFrom, T vTo) const override {
    if (vFrom >= matrixNeibor.size() || vTo >= matrixNeibor.size())
      return false;
    VectorArray<T> r = matrixNeibor.get(vFrom);
    return r.get(vTo);
  }

  // печать с весом ребер + выделение цветом заданного списка ребер
  virtual void printEdges(
    const char *msg,
    const VectorArray<edge<T,int>> &v) const {

    if (msg)
      std::cout << msg << "\n";

    std::string edgeView;
    if (bOrgraph) {
      std::cout << "digraph G { rankdir = LR;\n";
      edgeView = "\" -> \"";
    }
    else {
      std::cout << "graph G { rankdir = LR;\n";
      edgeView = "\" -- \"";
    }

    printGraphItem("", "", nullptr, &v);

    std::cout << "}\n\n";
  }

private:

  //
  void printGraphItem(
    const char *prefix,
    const char *postfix,
    const VectorArray<VectorArray<T>> *topoSort,
    const VectorArray<edge<T, int>> *spanningEdges = nullptr
  ) const {

    std::string edgeView;
    if (bOrgraph) {
      edgeView = "\" -> \"";
    }
    else {
      edgeView = "\" -- \"";
    }

    for (T row = 0; row < vertexCount(); ++row) {
      std::cout << "\"" << prefix << row << postfix << "\"\n";
    }

    if (topoSort) {
      for (T i = 0; i < topoSort->size(); ++i) {
        VectorArray<T> sub = topoSort->get(i);
        std::cout << "\tsubgraph cluster" << i + 1 << " {\n";
        std::cout << "\tlabel=\"group " << i + 1 << "\"\n";
        for (T j = 0; j < sub.size(); ++j) {
          std::cout << "\"" << prefix << sub.get(j) << postfix << "\"\n";
        }
        std::cout << "\t}\n";
      }
    }

    for (T row = 0; row < vertexCount(); ++row) {
      VectorArray<T> r = matrixNeibor.get(row);
      T startCol = bOrgraph ? 0 : row + 1;
      for (T col = startCol; col < r.size(); ++col) {
        if (bWeight) {
          if (r.get(col) != 0) {
            std::cout
            << "\""
            << prefix << row << postfix
            << edgeView
            << prefix << col << postfix
            << "\" [label=\""
            << r.get(col)
            << "\"";

            // разбираемся с цветом
            if (spanningEdges) {
              if (indexInSpaningEdges(spanningEdges, row, col))
                std::cout << " color=red";
            }
           std::cout << "]\n";
          }
        }
        else {
          if (r.get(col) != 0)
            std::cout
            << "\""
            << prefix << row << postfix
            << edgeView
            << prefix << col << postfix
            << "\"\n";
        }
      }
    }
  }
private:
  // посмотреть входят ли вершины в ребро
  bool indexInSpaningEdges(const VectorArray<edge<T, int>> *spanningEdges,
    T row, T col) const {
    if (!spanningEdges)
      return false;
    for (T index = 0; index < spanningEdges->size(); ++index) {
      const edge<T, int> &e = spanningEdges->get(index);
      if (((e.a == row) && (e.b == col)) ||
        ((e.b == row) && (e.a == col))
        )
        return true;
    }
    return false;
  }

protected:
  // матрица смежности
  VectorArray<VectorArray<T>> matrixNeibor;
  // признак орграфа
  bool bOrgraph;
  // признак графа с весами
  bool bWeight;
};


