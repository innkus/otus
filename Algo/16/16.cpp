// 16.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <random> // для std::random_device и std::mt19937
#include "graph.h"

// http://mathhelpplanet.com/static.php?p=topologicheskaya-sortirovka-vershin-grafa

// Алгоритм Демукрона
// Реализовать алгоритм Демукрона
// 
// Граф задан вектором смежности int A[N][Smax].Это п.5 в структурах данных в лекции.Отличие только в том, что вершины нумеруются от 0 а не от 1, и номера самой вершины первым столбцом в матрице не будет, будут только номера смежных вершин
// 
// Задание :
// Реализовать алгоритм Демукрона
// Если понадобится использование стека / очереди обязательно применение собственных структур данных из предыдущих занятий
// Можно использовать стандартный массив[] встроенный в язык
// 
// Выходные данные :
// Результат должен быть представлен в виде массива int[][] level где первый индекс - номер уровня, на каждом уровне массив, с номерами вершин, принадлежащих этому уровню
// 
// Дополнительное задание 1
// Реализовать алгоритм Тарьяна
// 
// Дополнительное задание 2
// Реализовать алгоритм поиска мостов или точек сочленения
// 
// ВАЖНО!При размещении ответа укажите, на каком языке вы выполнили ДЗ.Это поможет нам ускорить его проверку.
// Критерии оценки : Критерии оценки : 1 балл - алгоритм запрограммирован но не работает, 2 балла - алгоритм работает верно, но не оптимально, или есть несоответствия требованиям(например, сделано способом, отличным от указанного) 3 балла - алгоритм работает верно и написан максимально эффективно(нет лишних действий замедляющих работу)
// 1 балл.Задание сдано в срок.
// 1 балл.Выполнено дополнительное задание 1
// 1 балл.Выполнено дополнительное задание 2
// Рекомендуем сдать до : 29.09.2019

// Алгоритм Демукрона
// 0. l = 0
// 1. Просуммировать столбцы, положить суммы в вектор
// 2. Если в векторе всё NULL - закончить. Иначе столбцы с 0 зафиксировать на уровне l. Сделать l=l+1
// 3. Для каждого столбца с 0 взять строку по номеру столбца, выделить узлы
// 4. Для каждого из выделенных узлов в векторе сделать - 1; для нулевых значений сделать NULL
// 5. Перейти на пункт 2

struct cell {
  // уменьшение значения в ячейка на 1
  void substruct() {
    if (bEmpty)
      return;
    if (value==0)
      bEmpty = true;
    else
      value--;
  }
  //
  void init() {
    bEmpty = false; value = 0;
  }
  // признак пустоты
  bool bEmpty;
  // значение
  int value;
};

class Demukron {
public:
  // инициализация графа
  Demukron(graph *pGraph){
    G = pGraph;
  }

  // построение
  bool topoSort(VectorArray<VectorArray<int>> &output) {

    int N = G->vertexCount();

    // создаем вектор для суммирования    
    cell *pWorkVector = new cell[N];

    int level = 0;

    // 1. Просуммировать столбцы, положить суммы в вектор
    for (int i = 0; i < N; ++i) {      
      pWorkVector[i].init();      
    }
    additionColumns(pWorkVector);

    // проверить все ли ячейки в векторе пустые
    while (!isEmptyColumns(pWorkVector)) {

      // столбцы с 0 зафиксировать на уровне l.
      // если столбцов с 0 нет - граф не сеть! - выходим
      VectorArray<int> vLevel = formLevel(pWorkVector);
      if (!vLevel.size()) {
        delete []pWorkVector;
        return false;
      }
      output.add(vLevel);
      // global::printTopoSort(output);
      level++;

      // Для каждого из выделенных узлов в векторе сделать - 1; для нулевых значений сделать NULL
      substructColumns(pWorkVector);
    }
    
    delete[]pWorkVector;
    return true;
  }
  
  // просуммировать столбцы графа и сложить суммы в вектор
  void additionColumns(cell *pWorkVector) {
    // 
    for (int row = 0; row < G->vertexCount(); ++row) {      
      VectorArray<int> r = G->getRow(row);
      for (int col = 0; col < r.size(); ++col) {        
        pWorkVector[col].value += r.get(col);
      }
    }
  }
  // проверить все ли ячейки в векторе пустые
  bool isEmptyColumns(const cell *pWorkVector) const {
    for (int col = 0; col < G->vertexCount(); ++col) {
      if (!pWorkVector[col].bEmpty)
        return false;
    }
    return true;
  }
  // сформировать вектор из позиций нулевых элементов
  VectorArray<int> formLevel(const cell *pWorkVector) const {
    VectorArray<int> v;
    for (int col = 0; col < G->vertexCount(); ++col) {
      if (!pWorkVector[col].bEmpty && (pWorkVector[col].value == 0))
        v.add(col);
    }
    return v;
  }
  // Для каждого из выделенных узлов в векторе сделать - 1; для нулевых значений сделать NULL
  void substructColumns(cell *pWorkVector) {
    // пройти по всем 0 в векторе
    // взять для них строки из матрицы
    // вычесть из вектора полученную строку
    VectorArray<int> zerro;
    for (int col = 0; col < G->vertexCount(); ++col) {
      if (!pWorkVector[col].bEmpty && (pWorkVector[col].value == 0))
        zerro.add(col);
    }

    for (int col = 0; col < zerro.size(); ++col) {
      
      // нулевой элемент в векторе
      int posZero = zerro.get(col);
      pWorkVector[posZero].substruct();

      // берем строку из графа по номеру нулевого элемента
      VectorArray<int> r = G->getRow(posZero);
      for (int vCol = 0; vCol < G->vertexCount(); ++vCol) {
        if(r.get(vCol)==1)
          pWorkVector[vCol].substruct();
      }
    }

  }

private:
  graph *G;
};

void test1()
{
  int input[] = {
    0, 1, 1, 1, 1,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,

  };
  myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);

  Demukron G((graph *)&myG);
  VectorArray<VectorArray<int>> output;
  G.topoSort(output);

  const char *msg = "Test 1: topoSort graph";
  std::cout << msg << "\n";
  global::printTopoSort(output);
  myG.print(msg, &output);
}

void test2()
{
  int input[] = {
    0, 1, 0, 1, 1,
    0, 0, 1, 1, 0,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 0,

  };
  myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);

  Demukron G((graph *)&myG);
  VectorArray<VectorArray<int>> output;
  G.topoSort(output);

  const char *msg = "Test 2: topoSort graph";
  std::cout << msg << "\n";
  global::printTopoSort(output);
  myG.print(msg, &output);
}

void test3()
{
  int input[] = {
    0, 0, 1, 0,   0, 0, 0, 0,   0, 0, 0, 0,   1, 0,
    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   1, 0,
    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0,
    0, 0, 1, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0,

    0, 0, 1, 0,   0, 0, 0, 0,   1, 1, 0, 0,   0, 0,
    0, 0, 0, 1,   0, 0, 0, 0,   0, 0, 1, 1,   1, 0,
    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 1, 0,   0, 0,
    0, 1, 0, 1,   0, 1, 1, 0,   0, 0, 0, 0,   0, 0,

    1, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 1,
    1, 0, 0, 0,   0, 0, 1, 0,   0, 0, 0, 1,   0, 0,
    0, 0, 1, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0,
    0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0,

    0, 0, 1, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0,
    0, 0, 0, 0,   0, 1, 0, 0,   0, 0, 0, 0,   0, 0,
  };
  myGraph myG(sqrt(sizeof(input) / sizeof(int)), input);

  Demukron G((graph *)&myG);
  VectorArray<VectorArray<int>> output;
  G.topoSort(output);

  const char *msg = "Test 3: from study";
  std::cout << msg << "\n";
  global::printTopoSort(output);
  myG.print(msg, &output);
}

#define M (15)
void test4()
{
  int input[M*M];
  
  std::random_device rd;
  std::mt19937 mersenne(rd()); // инициализируем Вихрь Мерсенна случайным стартовым числом 
    
  for (int i = 0; i < M*M; ++i) {
    input[i] = 0;
  }

  for (int i = 0; i < M; ++i) {
    for (int j = i+1; j < M; ++j) {
      input[i*M+j] = mersenne() % 2;
    }
  }

  myGraph myG(M, input);

  Demukron G((graph *)&myG);
  VectorArray<VectorArray<int>> output;
  G.topoSort(output);

  const char *msg = "Test 4: random";
  std::cout << msg << "\n";
  global::printTopoSort(output);
  myG.print(msg, &output);
}


int main()
{
  test1();
  test2();
  test3();
  test4();
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


    // проверка является ли вектор пустым
auto fCheckEmpty = [](const cell *pWorkVector, int N) {
  for (int i = 0; i < N; ++i) {
    if (!pWorkVector[i].bEmpty)
      return false;
  }
  return true;
};