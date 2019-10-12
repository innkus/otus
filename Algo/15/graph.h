#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "iGraph.h"

//
class myGraph : graph {
public:
  // загрузка матрицы смежности из файла 
  // задается квадратная матрица
  // между элементами пробелы
  myGraph(std::string &fileName) {
    std::ifstream inFile(fileName);

    std::string line;
    while (std::getline(inFile, line))
    {
      // считали строку теперь надо разобрать
      int a = std::stoi(line.data());
    }
  }
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
  void print() const override {
    /*
    graph G{
      rankdir = LR;
      "To"[fontcolor = white fillcolor = black style = filled]
      "Web"[fillcolor = gray style = filled fontcolor = white]
      Welcome -- To
      To -- Web
      "To1/2\nggg" -- "GraphViz!"
    }
    */
    std::string edgeView;
    if (bOrgraph) {
      std::cout << "digraph G { rankdir = LR;\n";
      edgeView = "\" -> \"";
    }
    else {
      std::cout << "graph G { rankdir = LR;\n";
      edgeView = "\" -- \"";
    }

    for(int row = 0; row < vertexCount(); ++row) {
      std::cout << "\"" << row << "\"\n";
    }
    for (int row = 0; row < vertexCount(); ++row) {
      VectorArray<int> r = matrixNeibor.get(row);
      int startCol = bOrgraph ? 0 : row + 1;
      for (int col = startCol; col < r.size(); ++col) {
        if(r.get(col)==1)
          std::cout << "\"" << row << edgeView << col << "\"\n";
      }
    }
    std::cout << "}\n";
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

  // печать подграфовов сильной связности
  virtual void printSubGraphs
    (const Queue<VectorArray<int>> &subGraphs) const override {

    std::string edgeView;
    if (bOrgraph) {
      std::cout << "digraph G { rankdir = LR;\n";
      edgeView = "\" -> \"";
    }
    else {
      std::cout << "graph G { rankdir = LR;\n";
      edgeView = "\" -- \"";
    }

    std::cout << std::endl;

    //////////////////////////////////////////////////////
    // исходный граф
    for (int row = 0; row < vertexCount(); ++row) {
      std::cout << "\"_" << row << "_\"\n";
    }
    std::cout << std::endl;

    for (int row = 0; row < vertexCount(); ++row) {
      VectorArray<int> r = matrixNeibor.get(row);
      int startCol = bOrgraph ? 0 : row + 1;
      for (int col = startCol; col < r.size(); ++col) {
        if (r.get(col) == 1)
          std::cout << "\"_" << row << "_" << edgeView << "_" << col << "_\"\n";
      }
    }
    std::cout << std::endl;
    //////////////////////////////////////////////////////

    //////////////////////////////////////////////////////
    // результат
    for (int row = 0; row < vertexCount(); ++row) {
      std::cout << "\"" << row << "\"\n";
    }
    std::cout << std::endl;

    Node<VectorArray<int>> *curr = subGraphs.getHead();
    while (curr) {

      const VectorArray<int> &v = curr->getItem();

      for (int v1 = 0; v1 < v.size(); ++v1) {
        for (int v2 = 0; v2 < v.size(); ++v2) {
          int vertex1 = v.get(v1), vertex2 = v.get(v2);
          if (fromTo(vertex1, vertex2))
            std::cout << "\"" << vertex1 << edgeView << vertex2 << "\"\n";
        }
      }
      curr = curr->getNext();
    }
    //////////////////////////////////////////////////////

    std::cout << "}\n";
  }

protected:
  // матрица смежности
  VectorArray<VectorArray<int>> matrixNeibor;
  // признак орграфа
  bool bOrgraph;
};


