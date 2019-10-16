#pragma once

#include "vectorArray.h"
#include "graph.h"
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

};


