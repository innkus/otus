#pragma once

#include "vectorArray.h"
#include "graph.h"
#include "global.h"
#include <string>

// https://e-maxx.ru/algo/mst_kruskal
// https://e-maxx.ru/algo/dsu

// сортировка вставкой 
template <typename T>
void sortInsertInt(VectorArray<T> &m) {
  for (int i = 1; i < m.size(); ++i) {
    T x = m.get(i);
    int j = i - 1;
    while ((j >= 0) && (m.get(j) < x)) {
      // меняем
      m.set(m.get(j), j + 1); // "сдвиг" вправо
      j = j - 1;
      m.set(x, j + 1); // Вставка x в отсортированную часть
    }
  }
}

template <typename T>
class Kraskal {
public:
  //
  Kraskal(iGraph<T> *pGraph) {
    G = pGraph;
  }
  
  //
  VectorArray<edge<T, int>> doSpanningTree() {
    VectorArray<edge<T, int>> result;

    // создаем массив отдельных вершин
    VectorArray<T> *pVertexLists = new VectorArray<T>[G->vertexCount()];

    for (T i = 0; i < G->vertexCount(); ++i) {
      pVertexLists[i].add(i);
    }

    // создаем массив ребер и сортируем его
    VectorArray<edge<T,int>> eList;
    for (T row = 0; row < G->vertexCount(); ++row) {
      VectorArray<T> r = G->getRow(row);
      for (T col = row + 1; col < G->vertexCount(); ++col) {
        T x = r.get(col);
        if (x != 0) {
          edge<T,int> oneEdge(row, col, x);
          eList.add(oneEdge);
        }
      }
    }
    //printEdges(eList);
    sortInsertInt(eList);
    //printEdges(eList);
    
    result.clear();

    // бежим по ребрам
    for (T i = eList.size() - 1; i >=0 ; --i) {
      const edge<T,int> &e = eList.get(i);
      // перебираем массивы узлов и ищем вхождения 
      T firstIndex  = findInList(pVertexLists, e.a);
      T secondIndex = findInList(pVertexLists, e.b);
      if (firstIndex == secondIndex)
        continue;
      // сливаем вектора
      mergeList(pVertexLists, firstIndex, secondIndex);
      
      // помещаем ребра в результат
      result.add(e);
    }
    return result;
  }

private:
  //
  iGraph<T> *G;
  //
  
private:
  // найти номер множества, в которое входит вершина
  T findInList(const VectorArray<T> *pVertexLists, T index) {
    for (T i = 0; i < G->vertexCount(); ++i) {
      for (T j = 0; j < pVertexLists[i].size(); ++j) {
        if (pVertexLists[i].get(j) == index) {
          return i;
        }
      }
    }
    // криво!
    return T();
  }
  // слить два списка
  void mergeList(VectorArray<T> *pVertexLists,
    T firstIndex, T secondIndex) {
    if (firstIndex >= G->vertexCount())
      return;
    if (secondIndex >= G->vertexCount())
      return;

    for (T j = 0; j < pVertexLists[secondIndex].size(); ++j) {
      pVertexLists[firstIndex].add(pVertexLists[secondIndex].get(j));
    }
    pVertexLists[secondIndex].clear();
  }
};