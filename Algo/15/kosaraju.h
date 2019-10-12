#pragma once

#include "vectorArray.h"
#include "queue.h"
#include <string>
#include "dfs-iterative.h"
#include "dfs-recursive.h"

class Kosaraju {
public:
  //
  Kosaraju(graph *pGraph) {
    G = pGraph;
  }
  // найти компоненты сильной связанности
  void tightCoupling(VectorArray<int> &output) {
    // 1. Построить граф H с обратными(инвертированными) рёбрами
    G->invert();
    
    // 2. Выполнить в H поиск в глубину и найти f[u] — порядок окончания
    // обработки вершины u
    dfsIterative dfsInput(G);
    dfsInput.depthFirstSearch();
    // global::print(dfsInput.resultVertex);
    
    // 3. Выполнить поиск в глубину в G, перебирая вершины во внешнем
    // цикле в порядке убывания f[u]
    
    // инвертировали обратно
    G->invert();
    // G->print();

    // обход вершинам полученным на 2-м шагу
    dfsIterative dfsOutput(G);
    dfsOutput.depthVertexSearch(dfsInput.resultVertex);
    
    // Полученные на 3 - ем этапе деревья поиска в глубину будут являться
    // компонентами сильной связности графа G

    // формируем выход 
    // Результат должен быть представлен в виде массива int[] component где элемент с 
    // номером вершины содержит номер компонента
    output.clear();
    for (int i = 0; i < G->vertexCount(); ++i) {
      output.add(0);
    }
    int componentNumber = 0;
    Node<VectorArray<int>> *curr = dfsOutput.subGraphs.getHead();
    while (curr) {
      const VectorArray<int> &v = curr->getItem();
      for (int i = 0; i < v.size(); ++i) {
        int _vertex = v.get(i);
        output.set(componentNumber, _vertex);
      }
      componentNumber++;
      curr = curr->getNext();
    }

    // печать подграфовов сильной связности
    G->printSubGraphs(dfsOutput.subGraphs);
  }
private:
  //
  graph *G;
};