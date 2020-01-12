// 18.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "dijkstra.h"
#include "priorityQueue.h"

// Реализовать алгоритм Дейкстры
// Реализовать классику всех времен и народов, алгоритм Дейкстры : )
// 
// Граф задан вектором смежности int A[N][Smax].Это п.5 в структурах данных в лекции.Отличие только в том, что вершины нумеруются от 0 а не от 1, и номера самой вершины первым столбцом в матрице не будет, будут только номера смежных вершин
// 
// Задание :
// Реализовать алгоритм Дейкстры
// Если понадобится использование дерева / кучи обязательно применение собственных структур данных из предыдущих занятий
// Можно использовать стандартный массив[] встроенный в язык
// 
// Выходные данные :
// Результат должен быть представлен в виде массива Edge[] edges где Edge - класс, содержащий пару вершин, которые соединяет это ребро
// Edge
// {
// int v1;
// int v2;
// }
// Для любителей компактного хранения можно упаковать в long два int - а :)
// Тогда результат будет long[] edges
// 
// Дополнительное задание 1
// "Расскажи своей бабушке".
// Рассказать идею алгоритма Дейкстры совей бабушке так, что бы она это поняла.Поделиться своим опытом в слаке.Не забыть приложить ссылку на пост в задании
// 
// Дополнительное задание 2
// Реализовать алгоритм Флойда - Уоршалла или Беллама - Форда на выбор


void test1()
{
  int M = 6;
  int *input = new int[M * M];
  memset(input, 0, M * M * sizeof(int));

  int E[] = {
    // v1  v2   w
        1,  2,  7  ,
        1,  3,  9  ,
        1,  6,  14 ,
        2,  1,  7  ,
        2,  3,  10 ,
        2,  4,  15 ,
        3,  1,  9  ,
        3,  2,  10 ,
        3,  4,  11 ,
        3,  6,  2  ,
        4,  2,  15 ,
        4,  3,  11 ,
        4,  5,  6  ,
        5,  4,  6  ,
        5,  6,  9  ,
        6,  1,  14 ,
        6,  3,  2  ,
        6,  5,  9  ,
  };

  for (int i = 0; i < sizeof(E) / sizeof(int); i += 3) {
    E[i]--; E[i + 1]--;
    int index = E[i] * M + E[i + 1];
    input[index] = E[i + 2];
    index = E[i + 1] * M + E[i];
    input[index] = E[i + 2];
  }

  const char *msg = "Test 1";
  std::cout << msg << "\n";
  myGraph<int> myG(M, input);
  Dijkstra<int> dij((iGraph<int>*)&myG);
  VectorArray <edge<int, int>>
    edges = dij.getMinDistances(0, 4, INT_MAX);

  // печать в формате webgraphviz
  myG.printEdges(nullptr, edges, 1);
}

void test2()
{
  int M = 6;
  int *input = new int[M * M];
  memset(input, 0, M * M * sizeof(int));

  int E[] = {
    // v1  v2   w
        0,  1,  3 ,
        0,  2,  9 ,
        1,  2,  5 ,
        1,  3,  8 ,
        2,  3,  2 ,
        2,  4,  1 ,
        3,  5,  4 ,
        4,  5,  6 
  };

  for (int i = 0; i < sizeof(E) / sizeof(int); i += 3) {    
    int index = E[i] * M + E[i + 1];
    input[index] = E[i + 2];
  }

  const char *msg = "Test 2";
  std::cout << msg << "\n";
  myGraph<int> myG(M, input);
  Dijkstra<int> dij((iGraph<int>*)&myG);
  VectorArray <edge<int, int>>
    edges = dij.getMinDistances(0, 5, INT_MAX);

  // печать в формате webgraphviz
  myG.printEdges(nullptr, edges, 0);
}

void test3()
{
  int M = 8;
  int *input = new int[M * M];
  memset(input, 0, M * M * sizeof(int));

  int E[] = {
    // v1  v2   w
        1,  5,  2 ,
        2,  1,  1 ,
        2,  3,  1 ,
        2,  6,  3 ,
        2,  5,  4 ,
        3,  6,  1 ,
        5,  4,  3 ,
        5,  8,  1 ,
        6,  7,  2 , 
        6,  4,  5 ,
        7,  4,  2 ,
        8,  4,  1 ,
  };

  for (int i = 0; i < sizeof(E) / sizeof(int); i += 3) {
    E[i]--; E[i + 1]--;
    int index = E[i] * M + E[i + 1];
    input[index] = E[i + 2];
  }

  const char *msg = "Test 3";
  std::cout << msg << "\n";
  myGraph<int> myG(M, input);
  Dijkstra<int> dij((iGraph<int>*)&myG);
  VectorArray <edge<int, int>>
    edges = dij.getMinDistances(1, 3, INT_MAX);

  // печать в формате webgraphviz
  myG.printEdges(nullptr, edges, 1);
}

void test4()
{
  int M = 5;
  int *input = new int[M * M];
  memset(input, 0, M * M * sizeof(int));

  int E[] = {
    // v1  v2   w
        1,  2,  10 ,
        1,  5,  100 ,
        1,  4,  30 ,
        2,  3,  50 ,
        3,  5,  10 ,
        4,  5,  60 ,
        4,  3,  20 
  };

  for (int i = 0; i < sizeof(E) / sizeof(int); i += 3) {
    E[i]--; E[i + 1]--;
    int index = E[i] * M + E[i + 1];
    input[index] = E[i + 2];
  }

  const char *msg = "Test 4";
  std::cout << msg << "\n";
  myGraph<int> myG(M, input);
  Dijkstra<int> dij((iGraph<int>*)&myG);
  int offset = 1;
  VectorArray <edge<int, int>>
    edges = dij.getMinDistances(1 - offset, 5 - offset, INT_MAX);

  // печать в формате webgraphviz
  myG.printEdges(nullptr, edges, offset);
}

void test5()
{
  int M = 6;
  int *input = new int[M * M];
  memset(input, 0, M * M * sizeof(int));

  int E[] = {
    // v1  v2   w
        1,  2,  1 ,
        2,  3,  5 ,
        2,  4,  2 ,
        2,  6,  7 ,
        3,  4,  1 ,
        3,  6,  1 ,
        4,  1,  2 ,
        4,  3,  1 ,
        4,  5,  4 ,
        5,  4,  4 ,
        6,  5,  1 ,
  };

  for (int i = 0; i < sizeof(E) / sizeof(int); i += 3) {
    E[i]--; E[i + 1]--;
    int index = E[i] * M + E[i + 1];
    input[index] = E[i + 2];
  }

  const char *msg = "Test 5";
  std::cout << msg << "\n";
  myGraph<int> myG(M, input);
  Dijkstra<int> dij((iGraph<int>*)&myG);
  int offset = 1;
  VectorArray <edge<int, int>>
    edges = dij.getMinDistances(1 - offset, 5 - offset, INT_MAX);

  // печать в формате webgraphviz
  myG.printEdges(nullptr, edges, offset);
}

int main()
{
  test1();
  test2();
  test3();
  test4();
  test5();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
