#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "iGraph.h"

//
class myGraph : graph {
public:
  //
  myGraph(int N, const int *data) {    
    init(N, data);
  }
  //
  void init(int N, const int *data) {
    matrixNeibor.clear();
    //
    for (int row = 0; row < N; ++row) {
      VectorArray<int> line;
      for (int col = 0; col < N; ++col) {
        line.add(data[row * N + col]);
        // признак орграфа
        if (data[row * N + col] != data[col * N + row])
          bOrgraph = true;
      }
      // скорректировать диагональные элементы
      line.set(0, row);
      matrixNeibor.add(line);
    }
  }
  // получить строку матрицы смежности
  virtual VectorArray<int> getRow(int row) const override {
    return matrixNeibor.get(row);
  }
  // количество вершин в графе
  virtual int vertexCount() const override {
    return matrixNeibor.size();
  }
  // итератор по смежным вершинам
  // начинать с вершины, следующей за prevIteratorPos
  // если prevIteratorPos == 0 начинаем с начала
  iteratorIncident incident(int index, int prevIteratorPos = 0) const override {
    if (index >= vertexCount())
      return iteratorIncident();

    VectorArray<int> row = matrixNeibor.get(index);
    return iteratorIncident(row, prevIteratorPos);
  }

  //
  void print(
    const char *msg,
    const VectorArray<VectorArray<int>> *topoSort
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
    int N = vertexCount();
    VectorArray<int> elements(N * N);
    for (int i = 0; i < N * N; ++i)
      elements.add(0);

    for (int row = 0; row < N; ++row) {
      VectorArray<int> r = matrixNeibor.get(row);
      for (int col = 0; col < N; ++col) {
        int x = r.get(col);
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
  virtual bool fromTo(int vFrom, int vTo) const override {
    if (vFrom >= matrixNeibor.size() || vTo >= matrixNeibor.size())
      return false;
    VectorArray<int> r = matrixNeibor.get(vFrom);
    return r.get(vTo);
  }

private:

  //
  void printGraphItem(
    const char *prefix,
    const char *postfix,
    const VectorArray<VectorArray<int>> *topoSort) const {

    std::string edgeView;
    if (bOrgraph) {
      edgeView = "\" -> \"";
    }
    else {
      std::cout << "graph G { rankdir = LR;\n";
      edgeView = "\" -- \"";
    }

    for (int row = 0; row < vertexCount(); ++row) {
      std::cout << "\"" << prefix << row << postfix << "\"\n";
    }

    if (topoSort) {
      for (int i = 0; i < topoSort->size(); ++i) {
        VectorArray<int> sub = topoSort->get(i);
        std::cout << "\tsubgraph cluster" << i + 1 << " {\n";
        std::cout << "\tlabel=\"group " << i + 1 << "\"\n";
        for (int j = 0; j < sub.size(); ++j) {
          std::cout << "\"" << prefix << sub.get(j) << postfix << "\"\n";
        }
        std::cout << "\t}\n";
      }
    }

    for (int row = 0; row < vertexCount(); ++row) {
      VectorArray<int> r = matrixNeibor.get(row);
      int startCol = bOrgraph ? 0 : row + 1;
      for (int col = startCol; col < r.size(); ++col) {
        if (r.get(col) == 1)
          std::cout
          << "\""
          << prefix << row << postfix
          << edgeView
          << prefix << col << postfix
          << "\"\n";
      }
    }
  }

protected:
  // матрица смежности
  VectorArray<VectorArray<int>> matrixNeibor;
  // признак орграфа
  bool bOrgraph;
};


