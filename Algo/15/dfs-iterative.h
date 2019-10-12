#pragma once

#include "vectorArray.h"
#include "stack.h"
#include "queue.h"
#include <string>

extern void global::print(const VectorArray<int> &v);

// поиск в глубину
class dfsIterative {

public:
  // порядок вершин при обходе
  VectorArray<int> resultVertex;
  Queue<VectorArray<int>> subGraphs;

public:
  //
  dfsIterative(graph *pGraph)  {
    G = pGraph;
    init();
  }

  // поиск в глубину с первой вершины
  bool depthFirstSearch() {
    init();    
    for (int v = 0; v < G->vertexCount(); ++v) {
      if (usedVertex.get(v))
        continue;
      dfsVisit(v);
    }

    // global::print(resultVertex);
    return true;
  }

  // обход со стартовой вершины
  void dfsVisit(int indexVertex) {

    stackVertex.push(indexVertex);
    usedVertex.set(1, indexVertex);

    while (!stackVertex.isEmpty()) {
      // смотрим что не вершине стеха
      int u = stackVertex.front();

      bool bEnd = true;
      // восстанавливаем положение итератора не предыдущем шаге
      int prevIteratorPos = contextVertex.get(u);
      // берем смежные ребра
      iteratorIncident iterator = G->incident(u, prevIteratorPos);
      for (; !iterator.isEnd(); iterator.next()) {
        int w = iterator.get();
        // если ребро не используется положим его в стек
        // и пойдем наверх
        if (!usedVertex.get(w)) {
          stackVertex.push(w);
          // помечаем вершину как использованную
          usedVertex.set(1, w);
          // сохраняем текущее положение
          contextVertex.set(w, u);
          bEnd = false;
          break;
        }
      }

      if (bEnd) {
        // закончили с этой вершиной u
        // так как все смежные ребра уже использованы или их нет
        // исключим ребра из стека и положем в результат
        stackVertex.pop();
        resultVertex.add(u);
      }
    }

  }

  // печать обхода графа в формате Graphviz
  void print() const {    
    global::print(resultVertex);
  }

  // поиск в глубину по заданному порядку
  void depthVertexSearch(const VectorArray<int> &vertex) {
        
    init();

    for (int pos = vertex.size() - 1; pos >= 0; --pos) {
      int v = vertex.get(pos);
      if (usedVertex.get(v))
        continue;
      dfsVisit(v);
      
      if (resultVertex.size()) {
        
        VectorArray<int> output;
        for (int i = 0; i < resultVertex.size(); ++i)
          output.add(resultVertex.get(i));
        subGraphs.enqueue(output);
        
        global::print(output);

        resultVertex.clear();
      }
    }   
  }

private:

  //
  void init() {    

    usedVertex.clear();
    contextVertex.clear();

    for (int i = 0; i < G->vertexCount(); ++i) {
      usedVertex.add(0, i);
      contextVertex.add(0, i);
    }

    stackVertex.clear();
    resultVertex.clear();
    subGraphs.clear();
  }

  // текст в узле
  std::string getVertexName(int index) const {
    if (index >= G->vertexCount())
      return std::string();
    else
      return std::to_string(index);
  }

private:
  Stack<int> stackVertex;
  // просмотренные вершины
  VectorArray<int>  usedVertex;
  // сохраняем текущее положение
  VectorArray<int>  contextVertex;

  graph *G;
};
