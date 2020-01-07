// 17.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "iGraph.h"
#include "graph.h"
#include "kraskal.h"

// Реализовать алгоритм нахождения минимального остовного дерева
// Реализовать алгоритм Краскала
// 
// Граф задан вектором смежности int A[N][Smax].Это п.5 в структурах данных в лекции.
// Отличие только в том, что вершины нумеруются от 0 а не от 1, и номера самой вершины 
// первым столбцом в матрице не будет, будут только номера смежных вершин
// 
// Задание :
// Реализовать алгоритм Краскала
// Структура Union - Find собственной реализации.
// Если понадобится использование стека / очереди обязательно применение собственных 
// структур данных из предыдущих занятий
// Можно использовать стандартный массив[] встроенный в язык
// 
// Выходные данные :
// Результат должен быть представлен в виде массива Edge[] edges где Edge - класс, 
// содержащий пару вершин, которые соединяет это ребро
// Edge
// {
// int v1;
// int v2;
// }
// Для любителей компактного хранения можно упаковать в long два int - а :)
// Тогда результат будет long[] edges
// 
// Дополнительное задание 1
// Реализовать алгоритм Прима
// 
// Дополнительное задание 2
// Реализовать алгоритм Борувки


void test1()
{
  int M = 6;
  int *input = new int[M * M];
  memset(input, 0, M * M * sizeof(int));

  int E[] = {
    // v1  v2   w
        0,  1,  5  ,
        0,  5,  10 ,
        1,  2,  2  ,
        1,  3,  7  ,
        2,  3,  3  ,
        2,  4,  6  ,
        2,  5,  4  ,
        3,  4,  4  ,
        4,  5,  1  ,

  };

  for (int i = 0; i < sizeof(E) / sizeof(int); i += 3) {
    int index = E[i] * M + E[i + 1];
    input[index] = E[i + 2];
    index = E[i + 1] * M + E[i];
    input[index] = E[i + 2];
  }

  const char *msg = "Test 1";
  std::cout << msg << "\n";
  myGraph<int> myG(M, input);
  Kraskal<int> kl((iGraph<int>*)&myG);
  VectorArray<edge<int, int>> result = kl.doSpanningTree();
  global::printEdges(result);
  myG.printEdges(nullptr, result);
}


void test2()
{
  int input[] = {
    /*       0  1  2  3  4  
    /* 0 */  0, 3, 0, 0, 1, 
    /* 1 */  3, 0, 5, 0, 4, 
    /* 2 */  0, 5, 0, 2, 6, 
    /* 3 */  0, 0, 2, 0, 7, 
    /* 4 */  1, 4, 6, 7, 0, 
  };

  const char *msg = "Test 2";
  std::cout << msg << "\n";
  myGraph<int> myG(sqrt(sizeof(input) / sizeof(int)), input);
  Kraskal<int> kl((iGraph<int>*)&myG);
  VectorArray<edge<int, int>> result = kl.doSpanningTree();
  global::printEdges(result);
  myG.printEdges(nullptr, result);
}

void test3()
{
  int M = 9;
  int *input = new int[M * M];
  memset(input, 0, M * M * sizeof(int));

  int E[] = {
    // v1  v2  w
       0,  1,  4   ,
       0,  6,  8   ,
       1,  2,  8   ,
       1,  6,  11  ,
       2,  3,  7   ,
       2,  8,  4   ,
       2,  4,  2   ,
       3,  5,  9   ,
       3,  8,  14  ,
       4,  6,  7   ,
       4,  7,  6   ,
       5,  8,  10  ,
       6,  7,  1   ,
       7,  8,  2   ,
  };

  for (int i = 0; i < sizeof(E) / sizeof(int); i+=3) {
    int index = E[i] * M + E[i + 1];
    input[index] = E[i + 2];
    index = E[i+1] * M + E[i];
    input[index] = E[i + 2];
  }

  const char *msg = "Test 3";
  std::cout << msg << "\n";
  myG     raph<int> myG(M, input);
  Kraskal<int> kl((iGraph<int>*)&myG);
  VectorArray<edge<int, int>> result = kl.doSpanningTree();
  global::printEdges(result);
  myG.printEdges(nullptr, result);
}

void test4()
{
  int M = 6;
  int *input = new int[M * M];
  memset(input, 0, M * M * sizeof(int));

  int E[] = {
    // v1  v2   w
        0,  1,  6  ,
        0,  2,  8  ,
        0,  3,  5  ,
        0,  4,  10 ,
        0,  5,  4  ,
        1,  2,  7  ,
        1,  3,  7  ,
        1,  4,  6  ,
        1,  5,  2  ,
        2,  3,  5  ,
        2,  4,  10 ,
        2,  5,  7  ,
        3,  4,  7  ,
        3,  5,  10 ,
        4,  5,  3  ,
  };

  for (int i = 0; i < sizeof(E) / sizeof(int); i += 3) {
    int index = E[i] * M + E[i + 1];
    input[index] = E[i + 2];
    index = E[i + 1] * M + E[i];
    input[index] = E[i + 2];
  }

  const char *msg = "Test 4";
  std::cout << msg << "\n";
  myGraph<int> myG(M, input);
  Kraskal<int> kl((iGraph<int>*)&myG);
  VectorArray<edge<int, int>> result = kl.doSpanningTree();
  global::printEdges(result);
  myG.printEdges(nullptr, result);
}

void test5()
{
  int M = 8;
  int *input = new int[M * M];
  memset(input, 0, M * M * sizeof(int));

  int E[] = {
    // v1  v2   w
        0,  7,  19 ,
        0,  1,  6  ,
        0,  3,  25 ,
        1,  2,  17 ,
        1,  3,  11 ,
        2,  3,  8  ,
        3,  4,  2  ,
        4,  5,  21 ,
        4,  6,  14 ,
        6,  7,  9  ,
  };

  for (int i = 0; i < sizeof(E) / sizeof(int); i += 3) {
    int index = E[i] * M + E[i + 1];
    input[index] = E[i + 2];
    index = E[i + 1] * M + E[i];
    input[index] = E[i + 2];
  }

  const char *msg = "Test 5";
  std::cout << msg << "\n";
  myGraph<int> myG(M, input);
  Kraskal<int> kl((iGraph<int>*)&myG);
  VectorArray<edge<int, int>> result = kl.doSpanningTree();
  global::printEdges(result);
  myG.printEdges(nullptr, result);
}

void test6()
{
  int M = 7;
  int *input = new int[M * M];
  memset(input, 0, M * M * sizeof(int));

  int E[] = {
    // v1  v2   w
        0,  1,  7  ,
        0,  3,  5  ,
        1,  2,  8  ,
        1,  3,  9  ,
        1,  4,  7  ,
        2,  4,  5  ,
        3,  4,  15 ,
        3,  5,  6  ,
        4,  6,  9  ,
        4,  5,  8  ,
        5,  6,  11 ,
  };

  for (int i = 0; i < sizeof(E) / sizeof(int); i += 3) {
    int index = E[i] * M + E[i + 1];
    input[index] = E[i + 2];
    index = E[i + 1] * M + E[i];
    input[index] = E[i + 2];
  }

  const char *msg = "Test 6";
  std::cout << msg << "\n";
  myGraph<int> myG(M, input);
  Kraskal<int> kl((iGraph<int>*)&myG);
  VectorArray<edge<int, int>> result = kl.doSpanningTree();
  global::printEdges(result);
  myG.printEdges(nullptr, result);
}

void test7()
{
  int input[] = {
    /*       0  1  2  3  4  5
    /* 0 */  0, 2, 0, 0, 8, 7,
    /* 1 */  2, 0, 1, 0, 9, 0,
    /* 2 */  0, 1, 0, 5, 6, 4,
    /* 3 */  0, 0, 5, 0, 0, 6,
    /* 4 */  8, 9, 6, 0, 0, 3,
    /* 5 */  7, 0, 4, 6, 3, 0
  };

  const char *msg = "Test 7";
  std::cout << msg << "\n";
  myGraph<int> myG(sqrt(sizeof(input) / sizeof(int)), input);
  Kraskal<int> kl((iGraph<int>*)&myG);
  VectorArray<edge<int, int>> result = kl.doSpanningTree();
  global::printEdges(result);
  myG.printEdges(nullptr, result);
}

void test8()
{
  int input[] = {
    /*       0  1  2  3  4  
    /* 0 */  0, 4, 3, 2, 1, 
    /* 1 */  4, 0, 6, 0, 0, 
    /* 2 */  3, 6, 0, 5, 0, 
    /* 3 */  2, 0, 5, 0, 5, 
    /* 4 */  1, 0, 0, 5, 0, 
  };

  const char *msg = "Test 8";
  std::cout << msg << "\n";
  myGraph<int> myG(sqrt(sizeof(input) / sizeof(int)), input);
  Kraskal<int> kl((iGraph<int>*)&myG);
  VectorArray<edge<int, int>> result = kl.doSpanningTree();
  global::printEdges(result);
  myG.printEdges(nullptr, result);
}

int main()
{
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
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
