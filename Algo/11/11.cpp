// 11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "global.h"
#include "vectorArray.h"

// Домашнее задание
// B - дерево или Дерево отрезков
// Выполнить на выбор :
// А.Реализовать алгоритм добавления и удаления элементов в В - дерево.
// Б.Реализовать алгоритм быстрого нахождения суммы элементов массива
// в заданном отрезке через формирования двоичного дерева отрезков.
// 
// ВАЖНО!При размещении ответа укажите, на каком языке вы выполнили ДЗ.Это поможет нам ускорить его проверку.
// Критерии оценки : Вариант А. 5 баллов.
// Вариант Б. 5 баллов.

template<typename T>
struct context_s {
  // 
  context_s(int left, int right) : L(left), R(right), N(0) {}
  // 
  virtual void init() = 0;
  // действие которое делаем с одним элементом при движении по интервалу
  virtual void toDoActionOneItem(const T &data, int pos) = 0;
  // операцию над парой элементов массива при заполнении массива
  virtual T toDoActionTwoItems(const T &data1, const T &data2) = 0;
  // заполнение конкретным значением пустого элемента достроенного до степени 2
  virtual void initEmptyItem(T &data) = 0;
  //
  virtual void print() const = 0;

  // границы интервала подсчета
  int L, R;
  // размер исходного массива дотянутый до степени двойки
  int N;
};

//------------------------------------------------------------
// структура для поиска максимума
//------------------------------------------------------------
struct context_max : context_s<int> {
  //
  context_max(int left, int right) :
    context_s(left, right) {}
  //
  virtual void init() {
    maxData = INT32_MIN;
    maxPos = -1;
  }
  //
  virtual void toDoActionOneItem(const int &data, int pos) override {
    if (maxData < data) {
      maxData = data;
      maxPos = pos;
    }
  }
  //
  virtual int toDoActionTwoItems(const int &data1, const int &data2) override {
    return data1 > data2 ? data1 : data2;
  }
  //
  virtual void initEmptyItem(int &data) override {
    data = INT32_MIN;
  }
  //
  virtual void print() const override {
    std::cout << "[" << L << ";" << R << "]: MaxValue=" << maxData << " pos=" << maxPos << std::endl;
  }
  // максимальное значение элемента в массиве
  int maxData;
  // положение максимального элемента в массиве
  int maxPos;
};

//------------------------------------------------------------
// структура для поиска минимума
//------------------------------------------------------------
struct context_min : context_s<int> {
  //
  context_min(int left, int right) :
    context_s(left, right) {}
  //
  virtual void init() {
    minData = INT32_MAX;
    minPos = -1;
  }
  //
  virtual void toDoActionOneItem(const int &data, int pos) override {
    if (minData > data) {
      minData = data;
      minPos = pos;
    }
  }
  //
  virtual int toDoActionTwoItems(const int &data1, const int &data2) override {
    return data1 < data2 ? data1 : data2;
  }
  //
  virtual void initEmptyItem(int &data) override {
    data = INT32_MAX;
  }
  //
  virtual void print() const override {
    std::cout << "[" << L << ";" << R << "]: MinValue=" << minData << " pos=" << minPos << std::endl;
  }
  // максимальное значение элемента в массиве
  int minData;
  // положение максимального элемента в массиве
  int minPos;
};

//------------------------------------------------------------
// структура для поиска суммы
//------------------------------------------------------------
struct context_sum : context_s<int> {
  //
  context_sum(int left, int right) :
    context_s(left, right) {}
  //
  virtual void init() {
    summ = 0;
  }
  //
  virtual void toDoActionOneItem(const int &data, int pos) override {
    summ += data;
  }
  //
  virtual int toDoActionTwoItems(const int &data1, const int &data2) override {
    return data1 + data2;
  }
  //
  virtual void initEmptyItem(int &data) override {
    data = 0;
  }
  //
  virtual void print() const override {
    std::cout << "[" << L << ";" << R << "]: Summ=" << summ << std::endl;
  }
  // сумма
  int summ;
};

//------------------------------------------------------------
// Найти значение максимального элемента на указанном отрезке 
// массива и количество максимальных элементов на этом отрезке.
//------------------------------------------------------------
struct max_count_s {
  // максимум
  int maxValue;
  // количество максимумов
  int count;
};
struct context_maxcount : context_s<max_count_s> {
  //
  context_maxcount(int left, int right) :
    context_s(left, right) {}
  //
  virtual void init() {    
    result.maxValue = INT32_MIN;
    result.count = 1;
  }
  //
  virtual void toDoActionOneItem(const max_count_s &data, int pos) override {
    if (result.maxValue < data.maxValue) {
      result = data;
    }
    else if (result.maxValue == data.maxValue) {
      result.count += data.count;
    }
  }
  //
  virtual max_count_s toDoActionTwoItems(const max_count_s &data1, const max_count_s &data2) override {
    max_count_s m;
    if (data1.maxValue > data2.maxValue)
      return data1;
    if (data1.maxValue < data2.maxValue)
      return data2;
    m.maxValue = data1.maxValue;
    m.count = data1.count + data2.count;
    return m;
  }
  //
  virtual void initEmptyItem(max_count_s &data) override {
    data.maxValue = INT32_MIN;
    data.count = 1;
  }
  //
  virtual void print() const override {
    std::cout << "[" << L << ";" << R << "]: maxValue=" << result.maxValue << " count=" << result.count << std::endl;
  }
  max_count_s result;
};



template <typename T>
void toDoIntervalCalculation(const T *massiveData, context_s<T> &context) {
  context.init();
  int L = context.L + context.N, R = context.R + context.N;
  while (L <= R) {
    if (L % 2 == 1)
      context.toDoActionOneItem(massiveData[L], L);
    if (R % 2 == 0)
      context.toDoActionOneItem(massiveData[R], R);
    // сжимаем отрезок
    L = (L + 1) / 2;
    R = (R - 1) / 2;
  }
}

// 
template <typename T>
void calculate(const VectorArray<T> &v,   
  context_s<T> &context) {
  context.N = 1 << int(log2(v.size()) + 1);
  int N = +context.N;

  VectorArray<T> m(2 * N);
  for (int i = 0; i < 2 * N; ++i) {
    T data;
    context.initEmptyItem(data);
    m.add(data);
  }
  
  for (int i = N; i < N + v.size(); ++i) {
    m.set(v.get(i - N), i);
  }
  //global::print(m);

  for (int i = N - 1; i > 0; --i) {
    T data = context.toDoActionTwoItems(m.get(2*i), m.get(2 * i+1));
    m.set(data, i); 
  }
  //global::print(m);

  toDoIntervalCalculation<T>(m.data(), context);
}

void test1() {
  
  int x[] = { 0,1,2,3,4,5,6,7,8,9,10 };
  VectorArray<int> v;
  for (int i = 0; i < sizeof(x) / sizeof(int); ++i) {
    v.add(x[i], i);
  }
  global::print(v);

  context_max contextMax(3,9);  
  calculate(v, contextMax);
  contextMax.print();
  if (contextMax.maxData != 9)
    std::cout << "ERROR max" << std::endl;
  else
    std::cout << "Ok" << std::endl;

  context_min contextMin(4, 10);
  calculate(v, contextMin);
  contextMin.print();
  if (contextMin.minData != 4)
    std::cout << "ERROR min" << std::endl;
  else
    std::cout << "Ok" << std::endl;

  context_sum contextSum(2, 7);
  calculate(v, contextSum);
  contextSum.print();
  if (contextSum.summ != 27)
    std::cout << "ERROR sunmm" << std::endl;
  else
    std::cout << "Ok" << std::endl;

}

//------------------------------------------------------------
// Найти значение максимального элемента на указанном отрезке 
// массива и количество максимальных элементов на этом отрезке.
//------------------------------------------------------------
void test2() {

           //  0  1  2  3  4  5  6   7   8  9  10 11 12 13
  int x[] = { 10,10,22,10, 3, 4, 10, 22, 5, 7, 7, 7, 5, 3 };
  VectorArray<max_count_s> v;
  for (int i = 0; i < sizeof(x) / sizeof(int); ++i) {
    max_count_s data;
    data.maxValue = x[i]; data.count = 1;
    v.add(data, i);
  }

  int input[] = {
    2, 4,
    0, 3,
    3, 6,
    0, 7,
    8, 12, 
    10, 13
  };

  int output[] = {
    22, 1,
    22, 1,
    10, 2,
    22, 2,
    7, 3,
    7, 2
  };

  for(int i=0; i<sizeof(input)/sizeof(int); i+=2)
  {
    context_maxcount contextMaxCount(input[i], input[i+1]);
    calculate(v, contextMaxCount);
    contextMaxCount.print();
    if (contextMaxCount.result.maxValue != output[i] ||
      contextMaxCount.result.count != output[i + 1])
      std::cout << "ERROR: " << i << std::endl;
    else
      std::cout << "Ok" << std::endl;
  }
}

int main()
{
  test1();
  test2();
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
