// 6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

//
void print(int *Array, int size) {
  for (int i = 0; i < size; i++) {
    std::cout << Array[i] << " ";
  }
  std::cout << std::endl;
}

//
int Parent(int index) {
  return floor((index - 1) / 2);
}

//
int LeftChild(int index) {
  return index * 2 + 1;
}

//
int RightChild(int index) {
  return index * 2 + 2;
}

// Простой обмен !!!!
void Swap(int *Heap, int i, int j) {
  int x = Heap[i];
  Heap[i] = Heap[j];
  Heap[j] = x;
}

// (Пирамида, индекс, ограничения на массив)
// рекурсия
void Drown(int *Heap, int i, int size) {

  int left = LeftChild(i);
  int right = RightChild(i);

  int largest;
  if ((left < size) && (Heap[left] > Heap[i])) {  
    largest = left;
  }
  else {
    largest = i;
  }

  if ((right < size) && (Heap[right] > Heap[largest]))
    largest = right;

  if (largest != i) {
    Swap(Heap, i, largest);
    Drown(Heap, largest, size); // рекурсия    
  }
}

// (Пирамида, индекс, ограничения на массив)
// итеративный алгоритм
void DrownIter(int *Heap, int i, int size) {

  int iBase = i;

  while (true) {
    int left = LeftChild(i);
    int right = RightChild(i);

    int largest;
    if ((left < size) && (Heap[left] > Heap[i])) {  
      largest = left;
    }
    else {
      largest = i;
    }

    if ((right < size) && (Heap[right] > Heap[largest]))
      largest = right;

    if (largest != i) {
      Swap(Heap, i, largest);
      i = largest;
    }
    else
      break;
  }
}

void BuildHeap(int *Array, int size) {
  for (int i = floor((size - 1) / 2); i >= 0; i--) {
    Drown(Array, i, size);
  }
}

void BuildHeapIter(int *Array, int size) {
  for (int i = floor((size - 1) / 2); i >= 0; i--) {
    DrownIter(Array, i, size);
  }
}

void Heapsort(int *Array, int size) {
  int arraySize = size;
  BuildHeap(Array, arraySize);
  for (int i = size - 1; i >= 1; --i) {
    size = size - 1;
    Swap(Array, 0, i);
    Drown(Array, 0, size);
  }
}

void HeapsortIter(int *Array, int size) {
  int arraySize = size;
  BuildHeapIter(Array, arraySize);
  for (int i = size - 1; i >= 1; --i) {
    size = size - 1;
    Swap(Array, 0, i);
    DrownIter(Array, 0, size);
  }
}

void(*sortFuncInt)(int*, int);
typedef void(*PSortFuncInt)(int*, int);

// виды массивов для сортировки
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

// проверить отсортированность массива
void checkSort(int *m, int size) {
  for (int i = 0; i < size - 1; ++i) {
    if (m[i] > m[i + 1]) {
      std::cout << "ERROR SORT!!!!\n";
    }
  }

}

// подсчет времени в микросекундах
int64_t test(PSortFuncInt *f, int *data, int size) {

  __int64 elapsed_microseconds;
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  (*f)(data, size);

  end = std::chrono::system_clock::now();
  elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>
    (end - start).count();

  return elapsed_microseconds;
}

// Создать массивы размера от 20, 40, 80, 160 ... и до~100.000

// минимальное количество сортируемых данных
#define seed (20)
// максимальное количество сортируемых данных
#define ArrayMaxSize 100000
// количество серий (идем от минимума к максимуму с умножением на 2)
#define MaxSampleCount int((ceil((log2(ArrayMaxSize / double(seed)))) + 1))
// число повторов сортировки чтобы набрать статистику
#define TIMES 30 

// описание серии
struct sample_s {

  // количество чисел в серии для сортировки
  int count;

  // один элемент серии со статистикой
  struct one_sample_s {

    int64_t tmsec[TIMES];

    // матожидание по разным типам данных (рандом, 10% и тд)
    int64_t tmsec_aver = 0;

    // посчитать статистики
    void statistic() {

      // матожидание по разным типам данных (рандом, 10% и тд)
      tmsec_aver = 0;
      for (int i = 0; i < TIMES; ++i) {
        tmsec_aver += tmsec[i];
      }
      tmsec_aver /= TIMES;
    }
  };

  // eTypeBuild
  // 0 случайный
  // 1 перемешаны 10 %
  // 2 перемешаны 5 элементов
  one_sample_s dscr[tbMaxBuild];

  // посчитать статистики
  void statistic() {
    // std::cout << " count=" << count << std::endl;
    for (int i = 0; i < tbMaxBuild; ++i) {
      dscr[i].statistic();
    }
  }

};

// данные по одной функции сортировки
struct sort_func_dscr {
  // имя функции сортировки
  char name[30];
  // сортировочные серии
  sample_s *sample;
  // количество серий
  int sampleCount;
  // указатель на функцию сортировки
  PSortFuncInt p;

  // посчитать статистики
  void statistic() {
    // статистика для каждой серии
    // std::cout << name << " sampleCount=" << sampleCount << std::endl;
    for (int i = 0; i < sampleCount; ++i) {
      sample[i].statistic();
    }
  }
};

int main()
{
  sort_func_dscr sortInfo[] = {
    { "Heapsort",           new sample_s[MaxSampleCount], MaxSampleCount, &Heapsort},
    { "HeapsortIter" ,      new sample_s[MaxSampleCount], MaxSampleCount, &HeapsortIter},
  };

  std::cout << MaxSampleCount << std::endl;
  int count = seed;
  int massive[ArrayMaxSize];

  // цикл по сериям (разные количества элементов в массиве сортировки)
  for (int numSample = 0; numSample < MaxSampleCount; ++numSample) {

    // цикл по видам данных массивов
    for (int typeBuild = tbMinBuild; typeBuild < tbMaxBuild; ++typeBuild) {

      // конкретный вид массива строим заново для статистики
      for (int times = 0; times < TIMES; ++times) {

        // строим разные массивы для разнообразия
        // потом будем усреднять времена
        buildUnsort(massive, count, typeBuild);

        // цикл по алгоритмам сортировки на одних и тех же данных
        for (int func = 0; func < sizeof(sortInfo) / sizeof(sort_func_dscr); ++func) {

          // делаем копию, так как сортировка по месту и а нам нужен один и тот же массив
          // для чистоты расчетов
          int massiveCopy[ArrayMaxSize];
          memcpy(massiveCopy, massive, sizeof(massive));

          sortInfo[func].sample[numSample].count = count;
          sortInfo[func].sample[numSample].dscr[typeBuild].tmsec[times] =
            test(&sortInfo[func].p, massiveCopy, count);

          // контроль правильности сортировки
          checkSort(massiveCopy, count);
        } // все алгоритмы на одном наборе данных

      } // меняем наборы данных одного типа (рандом, 10% и тд)

    } // перебираем типы наборово данных

    // следующий тест новый размер массива (увеличиваем в 2 раза)
    count *= 2;
    if (count > ArrayMaxSize)
      count = ArrayMaxSize;
  } // варьируем размер данных

  //////////////////////////////////////////////////////////////////////
  // печать статистики 
  //////////////////////////////////////////////////////////////////////
  for (int i = 0; i < sizeof(sortInfo) / sizeof(sort_func_dscr); ++i) {

    sortInfo[i].statistic();

    std::cout << sortInfo[i].name << std::endl;

    std::cout << "N;Mrandom;M_10%;M_random5" << std::endl;
    // бежим по каждой серии
    for (int j = 0; j < sortInfo[i].sampleCount; j++) {

      // количество элементов сортировки
      std::cout << sortInfo[i].sample[j].count;

      // в пределах одной серии бежим по разным массивам данных
      for (int typeBuild = tbMinBuild; typeBuild < tbMaxBuild; ++typeBuild) {
        std::cout << ";" << sortInfo[i].sample[j].dscr[typeBuild].tmsec_aver;
      }
      std::cout << std::endl;
    }

    std::cout << std::endl;
  }
}
