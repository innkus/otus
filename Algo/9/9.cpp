// 9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include "bin-tree.h"
#include "avl-tree-hei.h"
#include "avl-tree-bal.h"

// Реализовать АВЛ или Декартово дерево
// Вариант 1. Написать реализацию АВЛ - дерева
// Методы к реализации :
// smallLeft / RightRotation - малое левое / правое вращение
// bigLeft / RightRotation - большое левое / правое вращение, написать через вызов малых вращений
// insert
// remove
// rebalance
// 
// Вариант 2. Написать реализацию Декартово дерева.
// Методы:
// merge
// split
// add
// remove
// Критерии оценки : +5 баллов за реализацию АВЛ - дерева
// + 5 баллов за реализацию Декартово дерева


// виды массивов для вставки
enum eTypeBuild {
  tbRandom = 0,    // случайный
  tbData10 = 1,    // перемешаны 10 %
  tbData5 = 2,    // перемешаны 5 элементов
  tbMinBuild = tbRandom,
  tbMaxBuild = tbData5 + 1
};

// построение массива определенного типа
/// @param typeBuild тип перемешнивания \enum eTypeBuild
void buildUnsort(int *m, int size, int typeBuild) {

  for (int i = 0; i < size; i++)
    m[i] = i;
  std::random_device rd;
  std::mt19937 g(rd());

  switch (typeBuild) {
  case tbRandom: {
    std::shuffle(m, m + size, g);
    //copy(m, m + size, std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
    break;
  }
  case tbData10: {
    // все перемешиваем
    std::shuffle(m, m + size, g);
    // пробежимся и каждый к-тый оставляем а остальные по порядку ставим
    for (int i = 0; i < size; i++)
    {
      // оставляем перемешанными каждый десятый элемент
      if ((i + 1) % 10 != 0)
        m[i] = i;
    }
    //copy(m, m + size, std::ostream_iterator<int>(std::cout, " "));
    break;
  }
  case tbData5: {
    int K = size / (5 + 1);
    // все перемешиваем
    std::shuffle(m, m + size, g);
    // оставляем перемешанными наши 5 элементов
    for (int i = 0; i < size; i++)
    {
      if ((i + 1) % K != 0)
        m[i] = i;
    }
    break;
  }
  }
}

class timeMesuare {
public:
  void start() {
    elapsed = 0;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    startTime = std::chrono::system_clock::now();
  }
  void stop() {
    endTime = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>
      (endTime - startTime).count();
  }
  void print() {
    std::cout << elapsed;
  }
  __int64 printData() const { 
    return elapsed; 
  }
private:
  std::chrono::time_point<std::chrono::system_clock> startTime;
  std::chrono::time_point<std::chrono::system_clock> endTime;
  __int64 elapsed;
};

#define TIMES 100

// сравнение вдоичного поиска и авл
void testCompareAvlAndBinTree(int64_t count, int *massiveData, IBinTree<int, int> *tree,
  __int64 *_tmInsert, __int64 *_tmFind) {

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  timeMesuare tmInsert, tmFind;
  *_tmInsert = *_tmFind = 0;

  for (int times = 0; times < TIMES; ++times) {
    tmInsert.start();
    for (int i = 0; i < count; ++i) {
      tree->insert(massiveData[i], massiveData[i]);
    }
    tmInsert.stop();
    *_tmInsert += tmInsert.printData();

    tmFind.start();
    for (int i = 0; i < count; ++i) {
      int x = tree->find(massiveData[i]);
      if (x != massiveData[i])
        std::cout << "ERROR FIND!!!!\n";
    }
    tmFind.stop();
    *_tmFind += tmFind.printData();

    for (int i = 0; i < count; ++i) {
      tree->remove(massiveData[i]);
    }
  }

  *_tmInsert /= TIMES;
  *_tmFind /= TIMES;
}

typedef IBinTree<int, int>*  ptreeio;

// максимальное количество данных
#define ArrayMaxSize 100000

int main()
{
  int massive[ArrayMaxSize];

  { 
    // добавление элементов в дерево + поиск
    // собираем статистику для сравнения производительности
    BinTree<int, int> tree;
    AvlTreeHeight<int, int> tree_h;
    ptreeio mass[2] = {&tree, &tree_h};


    int N[] = {  100,  200,  300,  400,  500,  600,  700,  800,  900, 1000,
                1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000,
                2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000,
                3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000,                
              };

    for (int numTree = 0; numTree < sizeof(tree)/sizeof(IBinTree<int, int>*); ++numTree) {
      
      std::cout << mass[numTree]->name() << ";count;insRand;findRand;ins10%;find10%;ins5;find5\n";

      for (int count = 0; count < sizeof(N) / sizeof(int); ++count) {
      
        std::cout << ";" << N[count] << ";";

        // цикл по видам данных массивов
        __int64 tmInsert, tmFind;
        for (int typeBuild = tbMinBuild; typeBuild < tbMaxBuild; ++typeBuild) {

          // строим разные массивы для разнообразия
          // потом будем усреднять времена
          buildUnsort(massive, N[count], typeBuild);
          // 
          testCompareAvlAndBinTree( N[count], massive, mass[numTree], &tmInsert, &tmFind);
          std::cout << tmInsert << ";" << tmFind << ";";
        }
              
        std::cout << std::endl;

      }// count

      std::cout << std::endl;
    }// numTree

  }

#if 0
  BinTree<int,int> tree;
  tree.testBaseFunctions();

  AvlTreeHeight<int, int> tree_h;
  tree_h.testBaseFunctions();

  AvlTreeBal<int,int> tree_b;
  tree_b.testBaseFunctions();
#endif
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
