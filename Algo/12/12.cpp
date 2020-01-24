// 12.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include "hashChain.h"
// TODO: add headers that you want to pre-compile here
#include <string>

// 1. Реализовать хеш - таблицу, использующую метод цепочек
// - дополнительно: для хранения внутри цепочек при достижении значительного числа 
// элементов(~32) заменять их на двоичное дерево поиска
// 
// 2. _Или_ : реализовать хеш - таблицу с открытой адресацией
// рассмотреть два различных варианта хеш - функций.


//
std::string& operator<< (std::string &out, const std::string &in) {
  out = out + in;
  return out;
}

// виды массивов для вставки
enum eTypeBuild {
  tbRandom = 0,    // случайный
  tbData10 = 1,    // перемешаны 10 %
  tbData5 = 2,    // перемешаны 5 элементов
  tbOrder4 = 3,    // 4 серии подряд
  tbMinBuild = tbRandom,
  tbMaxBuild = tbOrder4 + 1
};

#define ArrayMaxSize 10000
int massive[ArrayMaxSize];

// построение массива определенного типа
/// @param typeBuild тип перемешнивания \enum eTypeBuild
void buildUnsort(int *m, int size, int typeBuild) {

  for (int i = 0; i < size; i++)
    m[i] = i;
  std::random_device rd;
  std::mt19937 g(rd());

  switch (typeBuild) {
  case tbOrder4: {
    for (int i = 0; i < size / 4; i++) {
      m[i] = m[i+ size / 4] = m[i+ 2*size / 4] = m[i+ 3*size / 4] = i;
    }
    break;
  }
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

void baseFunctionsTest()
{
  int N = 10000;
  std::cout << "baseFunctionsTest: count=" << N << "\n";

  // цикл по видам данных массивов
  //for (int typeBuild = tbMinBuild; typeBuild < tbMaxBuild; ++typeBuild) {
  for (int typeBuild = tbOrder4; typeBuild < tbOrder4+1; ++typeBuild) {
    buildUnsort(massive, N, typeBuild);
    hashChain<int, int> table(3, 0.7, 3);
    
    // проверка вставка
    for (int i = 0; i < N; ++i) {
      table.insert(i, massive[i]);
      // table.print();
    }

    // проверка get
    for (int i = 0; i < N; ++i) {
      int value = table.get(i);
      if (value != massive[i])
        std::cout << "ERROR!!! " << i << " Data: " << massive[i] << " != " << value << "\n";
    }

  }
}


int main()
{
  baseFunctionsTest();

  // количество элементов для вставки
  int N[] = { 10, 20, 30,
              100,  200,  300,  400,  500,  600,  700,  800,  900, 1000,
                1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000,
                2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000,
                3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000,
                4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 4900, 5000,
            }; 

  std::cout
    << "LoadFactor;"
    << "BaseTableSize;"
    << "CurrentTableSize;"
    << "ElementCount;"
    << "mean;"
    << "deviation"
    << std::endl;
  ;

  for (float k = 0.65; k <= 0.95; k += 0.05) {
      
    for (int count = 0; count < sizeof(N) / sizeof(int); ++count) {

      // цикл по видам данных массивов
      buildUnsort(massive, N[count], tbRandom);

      hashChain<int, int> table(10, k, 10);
      for (int i = 0; i < N[count]; ++i) {
        table.insert(i, massive[i]);
      }
      std::cout << table.statistic() << std::endl; 

    }

  }  
}

