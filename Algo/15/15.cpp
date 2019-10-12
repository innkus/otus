// 15.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <random> // для std::random_device и std::mt19937
#include "graph.h"
#include "dfs-iterative.h"
#include "dfs-recursive.h"
#include "kosaraju.h"

// Реализовать алгоритм Косарайю
// 
// Входные данные :
// Граф задан вектором смежности int A[N][Smax].
// Это п.5 в структурах данных в лекции.Отличие только в том, что вершины нумеруются от 0 а не от 1, 
// и номера самой вершины первым столбцом в матрице не будет, будут только номера смежных вершин
// 
// Задание :
// Реализовать алгоритм Косарайю, рекурсивный вариант, как он был дан в лекции
// Если понадобится использование стека / очереди обязательно применение 
// собственных структур данных из предыдущих занятий
// 
// Выходные данные :
// Результат должен быть представлен в виде массива int[] component где элемент с 
// номером вершины содержит номер компонента
// 
// Дополнительное задание 1
// Реализовать итеративный поиск в глубину с сохранением состояния, что бы уже 
// пройденные уровни повторно не проходились
// 
// Дополнительное задание 2
// Реализовать поиск по критерию стоимости

void test_graf() {
  
  {
    int input[] = {
      0, 1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 1, 1, 0, 0, 
      0, 0, 0, 1, 0, 0, 1, 0,
      0, 0, 1, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 
      0, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 1, 0,
    };
    myGraph G(sqrt(sizeof(input) / sizeof(int)), input);
    G.print();
    G.invert();
    G.print();
  }

  {
    int input[] = {
      0, 1, 0, 1,
      0, 0, 1, 0,
      0, 0, 0, 0,
      0, 0, 1, 0
    };
    myGraph G(sqrt(sizeof(input) / sizeof(int)), input);
    G.print();    
    G.invert();
    G.print();
  }

  {
    int input[] = {
      0, 1, 1, 1,
      1, 0, 1, 0,
      1, 1, 0, 0,
      1, 0, 0, 0
    };
    myGraph G(4, input);
    G.print();

    // смежные вершины
    for (int v = 0; v < 4; ++v) {
      iteratorIncident i = G.incident(v);

      while (!i.isEnd()) {
        std::cout << i.get() << " ";
        i.next();
      }
      std::cout << "\n";
    }
  }
}

void test_dfs_recursive() {

  {
    int input[] = {
      0, 1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 1, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 1, 0,
      0, 0, 1, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 1, 0,
    };
    myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);
    myG.invert();

    dfsRecursive G((graph *)&myG);

    G.depthFirstSearch(0);
    G.print();
  }

  {
    int input[] = {
      0, 1, 0, 1,
      0, 0, 1, 0,
      0, 0, 0, 0,
      0, 0, 1, 0
    };

    myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);

    dfsRecursive G((graph *)&myG);
    G.print();

    G.depthFirstSearch(0);
    G.print();
    G.resultVertex.print();
  }

}

void test_dfs_iterative() {

  {
    int input[] = {
      0, 1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 1, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 1, 0,
      0, 0, 1, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 1, 0,
    };
    myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);
    myG.invert();

    dfsIterative G((graph *)&myG);
    
    G.depthFirstSearch();
    G.print();
  }

  {
    int input[] = {
      0, 1, 0, 1,
      0, 0, 1, 0,
      0, 0, 0, 0,
      0, 0, 1, 0
    };

    myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);

    dfsIterative G((graph *)&myG);
    G.print();
    G.depthFirstSearch();
    G.print();
  }
}


void test_stack() {
  Stack<int> x;
  int input[] = { 1,2,3,4,5 };
  for (int i = 0; i < sizeof(input) / sizeof(int); ++i) {
    x.push(input[i]);
    if (x.front() != input[i])
      std::cout << "ERROR push: " << input[i] << std::endl;
    x.print();
  }
  if(x.size()!= sizeof(input) / sizeof(int))
    std::cout << "ERROR size: " << x.size() << std::endl;

  int n = sizeof(input) / sizeof(int) - 1;
  while (!x.isEmpty()) {
    if(x.pop()!= input[n])
      std::cout << "ERROR pop: " << input[n] << std::endl;
    x.print();
    --n;
  }

  if (x.size() != 0)
    std::cout << "ERROR size: " << x.size() << std::endl;
}

void test_kosaraju() {

 
  { // тест с занятия
    std::cout << "Test from study" << std::endl << std::endl;
    int input[] = {
      0, 1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 1, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 1, 0,
      0, 0, 1, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 1, 0,
    };
    myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);

    Kosaraju G((graph *)&myG);
    VectorArray<int> output;
    G.tightCoupling(output);
    global::printIndex(output);
  }
 
 
  { // одна компонента
    std::cout << "Test: 1 subgraph" << std::endl << std::endl;
    int input[] = {
      0, 1, 1, 0, 0, 0,
      1, 0, 0, 1, 0, 0,
      1, 0, 0, 0, 1, 0,
      0, 1, 0, 0, 0, 1,
      1, 0, 1, 0, 0, 1,
      0, 0, 0, 1, 1, 0,
    };
    myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);

    Kosaraju G((graph *)&myG);
    VectorArray<int> output;
    G.tightCoupling(output);
    global::printIndex(output);
  }
  
  { // 2 компоненты
    std::cout << "Test: 2 subgraphs" << std::endl << std::endl;
    int input[] = {
      0, 1, 1, 0, 0, 0,   0, 0, 0, 0, 0, 0,
      1, 0, 0, 1, 0, 0,   0, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 1, 0,   0, 0, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 1,   0, 0, 0, 0, 0, 0,
      1, 0, 1, 0, 0, 1,   0, 0, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 0,   0, 0, 0, 0, 0, 0,

      0, 0, 0, 0, 0, 1,   0, 1, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 0,   1, 0, 0, 1, 0, 0,
      0, 0, 0, 0, 0, 0,   1, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 0, 0,   0, 1, 0, 0, 0, 1,
      0, 0, 0, 0, 0, 0,   1, 0, 1, 0, 0, 1,
      0, 0, 0, 0, 0, 0,   0, 0, 0, 1, 1, 0,
    };
    myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);

    Kosaraju G((graph *)&myG);
    VectorArray<int> output;
    G.tightCoupling(output);
    global::printIndex(output);
  }

  
  { // 4 компоненты
    std::cout << "Test: 4 subgraphs" << std::endl << std::endl;
    int input[] = {
      0, 1, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,
      0, 0, 1, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,
      0, 0, 0, 1,   0, 0, 0, 1,   0, 0, 0, 0,   0, 0, 0, 0,
      1, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,

      0, 0, 0, 0,   0, 1, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,
      0, 0, 0, 0,   0, 0, 1, 0,   0, 0, 0, 0,   0, 0, 0, 0,
      0, 0, 0, 0,   0, 0, 0, 1,   0, 0, 0, 0,   0, 0, 0, 0,
      0, 0, 0, 0,   1, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,

      0, 0, 1, 0,   0, 0, 0, 0,   0, 1, 0, 0,   0, 0, 0, 0,
      0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 1, 0,   0, 0, 0, 0,
      0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 1,   0, 0, 0, 0,
      0, 0, 0, 0,   0, 0, 0, 0,   1, 0, 0, 0,   0, 0, 0, 0,

      0, 0, 0, 0,   0, 0, 0, 0,   1, 0, 0, 0,   0, 1, 0, 0,   
      0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 1, 0,   
      0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 1,   
      0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   1, 0, 0, 0,   

    };
    myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);

    Kosaraju G((graph *)&myG);
    VectorArray<int> output;
    G.tightCoupling(output);
    global::printIndex(output);
  }
  
  { // 4 компоненты
    std::cout << "Test: 5 subgraphs" << std::endl << std::endl;
    int input[] = {
      0, 1, 1, 1, 1,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,

    };
    myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);

    Kosaraju G((graph *)&myG);
    VectorArray<int> output;
    G.tightCoupling(output);
    global::printIndex(output);
  }

}

#define M (500)
void test_kosaraju1000() {

  { // тест с занятия
    int input[M * M];

    std::random_device rd;
    std::mt19937 mersenne(rd()); // инициализируем Вихрь Мерсенна случайным стартовым числом 
    for (int i = 0; i < M * M; ++i) {
      int x = mersenne() % 5;
      if(x == 1)
        input[i] = 1;
      else
        input[i] = 0;
    }
    for (int i = 0; i < M; ++i) {
      input[i*i] = 0;
    }

    std::cout << "Test: random " << M << " vertex" << std::endl << std::endl;
    myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);
    VectorArray<int> output;

    Kosaraju G((graph *)&myG);
    G.tightCoupling(output);
    global::printIndex(output);
  }
}

int main()
{   
   test_kosaraju();
   test_kosaraju1000();

  // test_dfs_iterative();
  // test_stack();
  // test_graf();
  // test_dfs_recursive();
  return 0;
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
