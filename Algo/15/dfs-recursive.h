#pragma once

#include "vectorArray.h"
#include "queue.h"
#include <string>
#include "graph.h"

enum vertexColor {
  eWhite, // вершина еще не была посещена
  eGray,  // вершина была посещена, но может иметь смежную вершину, которая еще не была посещена
  eBlack, // вершана была посещена, и смежные с ней вершины тоже обойдены
};

// поиск в глубину
class dfsRecursive {

public:
  // порядок вершин при обходе
  Queue<int> resultVertex;

public:
  //
  dfsRecursive(graph *pGraph)  {
    G = pGraph;
    init();
  }

  // поиск в глубину со стартовой вершины
  bool depthFirstSearch(int startVertex) {
    
    init();

    if (startVertex >= G->vertexCount())
      return false;

    for (int v = 0; v < G->vertexCount(); ++v) {
      if (color.get(v) != eWhite)
        continue;
      // начинаем обход
      dfsVisit(v);
    }
    return true;
  }

  // печать обхода графа в формате Graphviz
  void print() const {
    
    G->print();

    std::cout << "\n\n\n";

    std::string edgeView;
    if (G->isOrgraph()) {
      std::cout << "digraph G { rankdir = LR;\n";
      edgeView = "\" -> \"";
    }
    else {
      std::cout << "graph G { rankdir = LR;\n";
      edgeView = "\" -- \"";
    }

    std::cout << "graph DFS { rankdir = LR;\n";
    for (int row = 0; row < G->vertexCount(); ++row) {
      std::string s = getVertexName(row);

      std::cout << "\"" << row << "\" [label=" << getVertexName(row) << "]\n";
    }
    for (int row = 0; row < G->vertexCount(); ++row) {
      VectorArray<int> r = G->getRow(row);
      for (int col = row + 1; col < r.size(); ++col) {
        if (r.get(col) == 1)
          std::cout << "\"" << row << edgeView << col << "\"\n";
      }
    }
    std::cout << "}\n";

    // печатаем вершины, в label пишем метки времени
    resultVertex.print();
  }

private:

  //
  void init() {
    for (int i = 0; i < G->vertexCount(); ++i) {
      // цвет
      color.add(eWhite, i);
      prevVertex.add(-1, i);
      // метка входа в узел
      timeIn.add(-1, i);
      // метка выхода из узла
      timeOut.add(-1, i);
      // порядок просмотра вершин
      order.add(-1, i);
    }

    // счетчик вхождений
    timeCount = 0;
    // порядок обхода
    orderNum = 0;
  }

  // обход соседей вершины indexVertex
  void dfsVisit(int indexVertex) {
    // заходим в вершину
    color.set(eGray, indexVertex);
    // отмечаем заход в узле
    timeIn.set(++timeCount, indexVertex);
    //
    order.set(++orderNum, indexVertex);

    // итератор по соседям
    iteratorIncident iterator = G->incident(indexVertex);

    for ( ; !iterator.isEnd(); iterator.next()) {
      int v = iterator.get();
      if (color.get(v) == eWhite) {
        // у вершины v предшественник  indexVertex 
        prevVertex.set(indexVertex, v);
        dfsVisit(v);
      }
    }

    // отмечаем выход из узла
    timeOut.set(++timeCount, indexVertex);
    resultVertex.enqueue(indexVertex);
  }

  // текст в узле
  std::string getVertexName(int index) const {
    if (index >= G->vertexCount())
      return std::string();
    std::string out 
      = std::string("\"") + std::to_string(index) + "\\n" 
      + "prev=" + std::to_string(prevVertex.get(index)) + " "
      + "order=" + std::to_string(order.get(index)) + "\\n" + std::string("{")
      + std::to_string(timeIn.get(index)) + std::string(",")
      + std::to_string(timeOut.get(index)) + std::string("}\"");
    return out;
  }

private:
  // счетчик цветов вершин
  VectorArray<int>  color;
  // метка входа в узел
  VectorArray<int> timeIn;
  // метка выхода из узла
  VectorArray<int> timeOut;
  // порядок просмотра вершин
  VectorArray<int> order;
  // предшественник вершин при обходе
  VectorArray<int> prevVertex;
  // метка времени
  int timeCount = 0;
  // вершины по порядку обхода
  int orderNum = 0;
  //
  graph *G;
};
