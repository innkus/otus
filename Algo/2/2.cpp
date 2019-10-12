// arrays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <chrono>
#include <map>
#include "simpleArray.h"
#include "vectorArray.h"
#include "factorArray.h"
#include "matrixArray.h"
#include "linkedArray.h"
#include "priorityQueue.h"

// Неполный массив или очередь с приоритетом.
// 1 задание.Динамические массивы.
// Написать метод добавления и удаления элементов :
// void add(T item, int index);
// T remove(int index); // возвращает удаляемый элемент
// по индексу во все варианты динамических массивов :
// SingleArray, VectorArray, FactorArray, MatrixArray *.
// + 1 балл.
// 
// 2 задание.Таблица сравнения производительности.
// Сравнить время выполнения разных операций
// для разных массивов с разным порядком значений.
// * сделать обёртку над ArrayList() и тоже сравнить.
// Составить таблицу и приложить её скриншот.
// Сделать выводы и сформулировать их в нескольких предложениях.
// + 1 балл.
// 
// 3 задание.Приоритетная очередь(на выбор).
// Написать реализацию PriorityQueue - очередь с приоритетом.
// Варианты реализации - список списков, массив списков
// Методы к реализации :
// enqueue(int priority, T item) - поместить элемент в очередь
// T dequeue() - выбрать элемент из очереди
// + 2 балла
// 
// 4 задание(на выбор).
// Написать Реализацию класса SpaceArray массив массивов с неполным заполнением.
// Делать на основе одного из уже созданных массивов и / или списков.
// + 2 балла дополнительно.

// простое тестирование на функциональность
void testFunctions() {
  PriorityQueue<int> q;
  q.testBaseFunctions();

  SingleArray<int> s;
  s.testBaseFunctions();

  VectorArray<int> v(3);
  v.testBaseFunctions();

  FactorArray<int> f(3, 3);
  f.testBaseFunctions();

  MatrixArray<int> m(3);
  m.testBaseFunctions();

  LinkedArray<int> l;
  l.testBaseFunctions();
}

// тест изменение зерна массивов для массивов с возможностью изменения шага заполнения
void testSeed() {
  timeMesuare tm;
  
  {   
    std::cout << "\n\n";
    int N = 100000;
    std::cout << "VectorArray, testSeed elemCount=" << N << std::endl;
    std::cout << "seed;InsertBegin,ms;RemoveBegin,ms;InsertEnd,ms;RemoveEnd,ms;InsertMiddle,ms;RemoveMiddle,ms\n";
    for (uint64_t i = 100; i < 1200; i += 100) {

      VectorArray<int> vectorArray(i);
      std::cout << i << ";";

      // вставка в начало
      tm.start();
      for (int j = 0; j < N; ++j) {
        vectorArray.add(j,0);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // удаление с начала
      tm.start();
      while (vectorArray.size()) {
        vectorArray.remove(0);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // вставка в конец
      tm.start();
      for (int j = 0; j < N; ++j) {
        vectorArray.add(j, j);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // удаление с конца
      tm.start();
      while (vectorArray.size()) {
        vectorArray.remove(vectorArray.size()-1);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // вставка в середину
      tm.start();
      for (int j = 2*N-1; j >=N; --j) {
        vectorArray.add(j, j/2);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // удаление с середины
      tm.start();
      while (vectorArray.size()) {
        vectorArray.remove(vectorArray.size() / 2);
      }
      tm.stop();
      tm.print(); std::cout << ";\n";

    }

  } 
 
  {
    std::cout << "\n\n";
    int N = 100000;
    std::cout << "FactorArray, testSeed elemCount=" << N << std::endl;
    std::cout << "seed;InsertBegin,ms;RemoveBegin,ms;InsertEnd,ms;RemoveEnd,ms;InsertMiddle,ms;RemoveMiddle,ms\n";
    for (uint64_t i = 100; i < 1200; i += 100) {

      FactorArray<int> vectorArray(i);
      std::cout << i << ";";

      // вставка в начало
      tm.start();
      for (int j = 0; j < N; ++j) {
        vectorArray.add(j, 0);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // удаление с начала
      tm.start();
      while (vectorArray.size()) {
        vectorArray.remove(0);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // вставка в конец
      tm.start();
      for (int j = 0; j < N; ++j) {
        vectorArray.add(j, j);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // удаление с конца
      tm.start();
      while (vectorArray.size()) {
        vectorArray.remove(vectorArray.size() - 1);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // вставка в середину
      tm.start();
      for (int j = 2 * N - 1; j >= N; --j) {
        vectorArray.add(j, j / 2);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // удаление с середины
      tm.start();
      while (vectorArray.size()) {
        vectorArray.remove(vectorArray.size() / 2);
      }
      tm.stop();
      tm.print(); std::cout << ";\n";

    }

  }
 
  {
    std::cout << "\n\n";
    int N = 100000;
    std::cout << "MatrixArray, testSeed elemCount=" << N << std::endl;
    std::cout << "seed;InsertBegin,ms;RemoveBegin,ms;InsertEnd,ms;RemoveEnd,ms;InsertMiddle,ms;RemoveMiddle,ms\n";
    for (uint64_t i = 100; i < 1200; i += 100) {

      MatrixArray<int> vectorArray(i);
      std::cout << i << ";";

      // вставка в начало
      tm.start();
      for (int j = 0; j < N; ++j) {
        vectorArray.add(j, 0);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // удаление с начала
      tm.start();
      while (vectorArray.size()) {
        vectorArray.remove(0);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // вставка в конец
      tm.start();
      for (int j = 0; j < N; ++j) {
        vectorArray.add(j, j);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // удаление с конца
      tm.start();
      while (vectorArray.size()) {
        vectorArray.remove(vectorArray.size() - 1);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // вставка в середину
      tm.start();
      for (int j = 2 * N - 1; j >= N; --j) {
        vectorArray.add(j, j / 2);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // удаление с середины
      tm.start();
      while (vectorArray.size()) {
        vectorArray.remove(vectorArray.size() / 2);
      }
      tm.stop();
      tm.print(); std::cout << ";\n";

    }

  }


}

// тестирование на вставку в конец и удаление из начала для
// простого массива и массива с очередью
void testSimpleAndLinked() {

  timeMesuare tm;
  int N1 = 100000;
  
  {
    int N = N1;
    std::cout << "SingleArray, elemCount=" << N << std::endl;
    std::cout << "InsertEnd,ms;RemoveStart,ms\n";
    for (uint64_t iCount = 100; iCount < N; iCount += 100) {

      SingleArray<int> vectorArray;
      std::cout << iCount << ";";

      // вставка в конец
      tm.start();
      for (int j = 0; j < iCount; ++j) {
        vectorArray.add(j, j);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // удаление с начала
      tm.start();
      while (vectorArray.size()) {
        vectorArray.remove(0);
      }
      tm.stop();
      tm.print(); std::cout << "\n";
    }

  }
  
  std::cout << "\n\n";

  {    
    int N = N1;
    std::cout << "LinkedArray, elemCount=" << N << std::endl;
    std::cout << "InsertEnd,ms;RemoveStart,ms\n";
    for (uint64_t iCount = 100; iCount < N; iCount += 100) {

      LinkedArray<int> vectorArray;
      std::cout << iCount << ";";

      // вставка в конец
      tm.start();
      for (int j = 0; j < iCount; ++j) {
        vectorArray.add(j, j);
      }
      tm.stop();
      tm.print(); std::cout << ";";

      // удаление с начала
      tm.start();
      while (vectorArray.size()) {
        vectorArray.remove(0);
      }
      tm.stop();
      tm.print(); std::cout << "\n";
    }

  }

}

void test() {
  
  SingleArray<IArray<int>*> arrays;
  
  SingleArray<int> singleArray;
  arrays.add((IArray<int>*)&singleArray);

  VectorArray<int> vectorArray(100);
  arrays.add((IArray<int>*)&vectorArray);

  FactorArray<int> factorArray(100);
  arrays.add((IArray<int>*)&factorArray);
  
  MatrixArray<int> matrixArray(100);
  arrays.add((IArray<int>*)&matrixArray);

  LinkedArray<int> linkedArray;
  arrays.add((IArray<int>*)&linkedArray);
  
  for (int i = 0; i < arrays.size(); ++i) {
    std::cout << arrays.get(i)->name() << ";count;intoBegin;fromBegin;intoMiddle;fromMiddle;intoEnd;fromEnd\n";
    arrays.get(i)->test(100); std::cout << "\n";
    arrays.get(i)->test(200); std::cout << "\n";
    arrays.get(i)->test(400); std::cout << "\n";
    arrays.get(i)->test(800); std::cout << "\n";
    arrays.get(i)->test(1600);       std::cout << "\n";
    arrays.get(i)->test(3200);       std::cout << "\n";
    arrays.get(i)->test(6400);       std::cout << "\n";
    arrays.get(i)->test(12800);      std::cout << "\n";
    arrays.get(i)->test(15000);      std::cout << "\n";
    arrays.get(i)->test(20000);      std::cout << "\n";
    arrays.get(i)->test(25600);      std::cout << "\n";
    arrays.get(i)->test(40000);      std::cout << "\n";
    arrays.get(i)->test(51200);      std::cout << "\n";
    arrays.get(i)->test(70000);      std::cout << "\n";
    arrays.get(i)->test(90000);      std::cout << "\n";
    arrays.get(i)->test(102400);     std::cout << "\n";
    arrays.get(i)->test(150000);     std::cout << "\n";
    arrays.get(i)->test(204800);     std::cout << "\n";
    arrays.get(i)->test(250000);     std::cout << "\n";
    arrays.get(i)->test(300000);     std::cout << "\n";
    arrays.get(i)->test(350000);     std::cout << "\n";
    arrays.get(i)->test(409600);     std::cout << "\n";
    std::cout << "\n";
  }
}

int main()
{
  test();
  // тестирование базовой функциональнсти
  // testFunctions();

  // тестирование массивов с именением зерна для
  // VectorArray FactorArray MatrixArray
  // testSeed();

  // тестирование на вставку в конец и удаление из начала для
  // простого массива и массива с очередью
  // testSimpleAndLinked();

  return 0;

  SingleArray<IArray<int>*> arrays;
  /*
  SingleArray<int> singleArray;
  arrays.add((IArray<int>*)&singleArray);

  VectorArray<int> vectorArray(3);
  arrays.add((IArray<int>*)&vectorArray);
  
  FactorArray<int> factorArray(100);
  arrays.add((IArray<int>*)&factorArray);
  */
  MatrixArray<int> matrixArray(3);
  arrays.add((IArray<int>*)&matrixArray);

  /*
  for (int i = 0; i < arrays.size(); ++i) {
    test4<int>(arrays.get(i));
  }
  */

  for (int i = 0; i < arrays.size(); ++i) {
    /*
    arrays.get(i)->testAdd(100);
    arrays.get(i)->testAdd(1000);
    arrays.get(i)->testAdd(10000);
    arrays.get(i)->testAdd(30000);
    arrays.get(i)->testAdd(50000);
    arrays.get(i)->testAdd(80000);
    */
    arrays.get(i)->testAdd(200000);
    arrays.get(i) ->testAdd(300000);
  }

  return 0;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
