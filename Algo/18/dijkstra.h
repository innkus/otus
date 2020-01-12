#pragma once

#include "vectorArray.h"
#include "graph.h"
#include "global.h"
#include <string>

// https://e-maxx.ru/algo/dijkstra
// https://prog-cpp.ru/deikstra/

template <typename M>
struct vertex_data_s {
  // метка
  M label;
  // признак посещения вершины
  bool bVisited = false;
};

template <typename T>
class Dijkstra {
public:
  //
  Dijkstra(iGraph<T> *pGraph) {
    G = pGraph;
  }
  
  //
  VectorArray<edge<T, int>> getMinDistances(const T &startIndex, const T &finishIndex, const T &maxValue) {
    VectorArray<edge<T, int>> result;

    // создаем массив отдельных вершин где храним информацию о посещение и о значении метки
    VectorArray< vertex_data_s<T> > vertexList;

    for (T i = 0; i < G->vertexCount(); ++i) {
      vertex_data_s<T> vdata;
      vdata.label = (i == startIndex) ? 0 : maxValue;
      vertexList.add(vdata);
    }

    //
    int visitCount = G->vertexCount();
    while (visitCount) {
      // взять первый элемент из непосещенных 
      // в самом начале это будет стартовая вершина
      int currentVertex = getMinLabelVertex(vertexList, maxValue);
      if (currentVertex == -1) {
        // что-то пошло совсем не так 
        return VectorArray<edge<T, int>>();
      }

      
      // находим соседей этой вершины
      iteratorIncident<T> iterator = G->incident(currentVertex);

      // откидываем посещенных соседей, а остальных надо отсортировать
      // в порядке возрастания весов ребер
      PriorityQueue<T, T> sortList;
      for (; !iterator.isEnd(); iterator.next()) {
        T v = iterator.get();

        vertex_data_s<T> dataVertex = vertexList.get(v);
        if (dataVertex.bVisited)
          continue;
        // собираем вершинки в кучу ориентируясь на весь ребра
        // если вес 0, то пути нет
        T weight = G->weight(currentVertex, v);
        if(weight != 0)
          sortList.enqueue(weight, v);
      }

      // бежим по отсортированным соседям и корректируем метки
      vertex_data_s<T> currentVertexData = vertexList.get(currentVertex);
      while (sortList.countElements()) {
        // получаем номер вершины
        T closedVertex = sortList.dequeue();
        // 
        T weight = G->weight(currentVertex, closedVertex);
        vertex_data_s<T> closedVertexData = vertexList.get(closedVertex);
        if (currentVertexData.label + weight < closedVertexData.label) {
          // корректируем всё
          closedVertexData.label = currentVertexData.label + weight;
          vertexList.set(closedVertexData, closedVertex);
        }
      }

      // помечаем вершину посещенной
      currentVertexData.bVisited = true;
      vertexList.set(currentVertexData, currentVertex);
      visitCount--;
    }

    // всё обошли теперь надо восстановить
    
    // инвертируем иначе не восстановить 
    G->invert();

    int currentVertex = finishIndex;
    while (currentVertex != startIndex) {
      // получить соседей currentVertex
      iteratorIncident<T> iterator = G->incident(currentVertex);

      //берем метку вершины
      vertex_data_s<T> currentVertexData = vertexList.get(currentVertex);

      // бежим по соседям, бере вес ребра от соседа к текущей вершине
      // вычитаем из метки текущей вершины и сравниваем с меткой соседа
      // кто подойдет тот и есть наша следующая вершина
      for (; !iterator.isEnd(); iterator.next()) {
        // очередной сосед
        T closedVertex = iterator.get();
        // его метка
        vertex_data_s<T> closedVertexData = vertexList.get(closedVertex);
        // вес ребра от соседа к текщей вершине
        T weight = G->weight(currentVertex, closedVertex);
        if (weight + closedVertexData.label == currentVertexData.label) {
          // формируем ребро
          edge<T, int> e;
          e.a = closedVertex;
          e.b = currentVertex;
          result.add(e, 0);
          currentVertex = closedVertex;
          break;
        }
      }
    }
    // разворачиваем обратно
    G->invert();
    return result;
  }

private:
  // вернуть номер вершины с минимальной меткой
  int getMinLabelVertex(VectorArray< vertex_data_s<T> > &vertexList, const T &maxValue) {
    T minValue = maxValue;
    int minIndex = -1;
    for (int i = 0; i < vertexList.size(); ++i) {
      if (vertexList.get(i).bVisited)
        continue;
      if (vertexList.get(i).label < minValue) {
        minValue = vertexList.get(i).label;
        minIndex = i;
      }
    }
    return minIndex;
  }
  /*
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
  */

private:
  //
  iGraph<T> *G;
  //
  
private:
  /*
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
  */
};