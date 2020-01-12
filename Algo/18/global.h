#pragma once

#include "vectorArray.h"
#include "graph.h"
#include "priorityQueue.h"
#include <string>

namespace global {
  void print(const VectorArray<int> &v) {
    std::cout << "vector" << " (" << v.size() << ") :";
    for (int i = 0; i < v.size(); ++i) {
      std::cout << v.get(i) << " ";
    }
    std::cout << "\n";
  }
  void printIndex(const VectorArray<int> &v) {
    std::cout << "vector" << " (" << v.size() << ") :";
    std::cout << "\n";
    for (int i = 0; i < v.size(); ++i) {
      std::cout << std::setw(2) << i << " ";
    }
    std::cout << "\n";
    for (int i = 0; i < v.size(); ++i) {
      std::cout << std::setw(2) << v.get(i) << " ";
    }
    std::cout << "\n\n";
  }

  void printTopoSort(const VectorArray<VectorArray<int>> &v) {
    std::cout << "vector of vectors" << " (" << v.size() << ") :\n";
    for (int i = 0; i < v.size(); ++i) {
      VectorArray<int> subv = v.get(i);
      print(subv);
    }
    std::cout << "\n";
  }

  // печать вектора с ребрами
  void printEdges(const VectorArray<edge<int, int>> &v) {
    std::cout << "vector" << " (" << v.size() << ") :\n";
    for (int i = 0; i < v.size(); ++i) {
      std::cout << "[" << v.get(i).a << ";" << v.get(i).b << "]=" << v.get(i).weight << "\n";
    }
    std::cout << "\n";
  }

  // печать векторов с ребрами для всех вершин кроме начальной
  //void printEdges(PriorityQueue<int, VectorArray<edge<int, int>>> &vv) {
  //
  //  std::cout << "PriorityQueue" << " (" << vv.countPriotity() << ") elemCount=" << vv.countElements() << " >>\n";
  //
  //  int i = 1;
  //  Queue<PNode<int, VectorArray<edge<int, int>>>*> *pQueue = vv.getQueue();
  //  Node <PNode<int, VectorArray<edge<int, int>>>*> *curr = pQueue->getHead();
  //  while (curr) {
  //    PNode<VectorArray<edge<int, int>>> p = curr->getItem();
  //    std::cout << i++ << ") pri=" << p->priority << " >> ";
  //    global::print(queue);
  //    curr = curr->getNext();
  //  }
  //}

  //}
  
  void print(const Queue<int> &queue) {
    std::cout << "queue" << " (" << queue.size() << ") :";
    Node<int> *curr = queue.getHead();
    while (curr) {
      std::cout << curr->getItem() << " ";
      curr = curr->getNext();
    }
    std::cout << "\n";
  }

  //
  // void print() const {
  //   std::cout << "PriorityQueue" << " (" << listQueue.size() << ") elemCount=" << length << " >>\n";
  // 
  //   int i = 1;
  //   Node <PNode<P, T>*> *curr = listQueue.getHead();
  //   while (curr) {
  //     PNode<P, T>* p = curr->getItem();
  //     std::cout << i++ << ") pri=" << p->priority << " >> ";
  //     global::print(queue);
  //     curr = curr->getNext();
  //   }
  // }

};


