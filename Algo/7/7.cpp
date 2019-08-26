// 7.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

// минимальное количество сортируемых данных
#define seed (20)
// максимальное количество сортируемых данных
#define ArrayMaxSize 30000
// количество серий (идем от минимума к максимуму с умножением на 2)
#define MaxSampleCount int((ceil((log2(ArrayMaxSize / double(seed)))) + 1))
// число повторов сортировки чтобы набрать статистику
#define TIMES 30 

// Простой обмен !!!!
void swap(int *m, int i, int j) {
  int x = m[i];
  m[i] = m[j];
  m[j] = x;
}

// сортировка вставкой 
void sortInsertInt(int *m, int size) {
  if (size < 2)
    return;

  for (int i = 1; i < size; ++i) {
    int x = m[i];
    int j = i - 1;
    while (j >= 0 and m[j] > x) {
      // меняем
      m[j + 1] = m[j]; // "сдвиг" вправо
      j = j - 1;
      m[j + 1] = x; // Вставка x в отсортированную часть
    }
  }
}

// создание копии массива
int *copyArray(int *m, int size) {
  int *copyM = new int[size];
  memcpy(copyM, m, size * sizeof(int));
  return copyM;
}

// сливаем две отсортированные части одного массива в другой массив Copy
void Merge(const int *Array, int begin, int middle, int end, int *Copy) {
  int fst = begin;
  int snd = middle;

  for (int ptr = begin; ptr < end; ++ptr) {
    if ((fst < middle) && ( (snd >= end) || (Array[fst] <= Array[snd])) ) {
      Copy[ptr] = Array[fst];
      fst += 1;
    }
    else {
      Copy[ptr] = Array[snd];
      snd += 1;
    }
  }
}

// @param begin первый элемент подмассива
// @param end начало следующего подмассива
void SplitMerge(int *copyMassive, int begin, int end, int *massive) {
  if (end - begin < 2)
    return;
  int middle = (end + begin) / 2;
  // расщепляем massive на [begin;middle-1] в copyMassive
  SplitMerge(massive, begin, middle, copyMassive);
  // расщепляем massive на [middle;end-1] в copyMassive
  SplitMerge(massive, middle, end, copyMassive);
  //
  Merge(copyMassive, begin, middle, end, massive);
}

// сортировка слиянием
void MergeSort(int *massive, int size) {
  int *copyMassive = copyArray(massive, size);
  SplitMerge(copyMassive, 0, size, massive);
  delete[]copyMassive;
}

//////////////////////////////////////////////////////////
// стержневой элемент в конце
int partition_increasing(int *massive, int begin, int end) {
  int pivotData = massive[end - 1];
  int i = begin - 1;
  for (int j = begin; j <= end - 2; ++j) {
    if (massive[j] <= pivotData) {
      i += 1;
      swap(massive, i, j);
    }
  }
  swap(massive, i + 1, end - 1);
  return i + 1;
}

void quicksort(int *massive, int begin, int end) {
  if (begin < end) {
    int pivot = partition_increasing(massive, begin, end);
    quicksort(massive, begin, pivot);
    quicksort(massive, pivot + 1, end);
  }
}
void Quicksort(int *massive, int size) {
  quicksort(massive, 0, size);
}

//////////////////////////////////////////////////////////
// Оригинальная схема упорядочения значений, предложенная Хоаром.
int PartitionHoare(int *massive, int lo, int hi) {
  int pivot = massive[lo + (hi - lo) / 2];
  int i  = lo - 1;
  int j = hi + 1;
  while (true) {
    do {
      j -= 1;
    } while (massive[j] > pivot);

    do {
      i += 1;
    } while (massive[i] < pivot);

    if (i >= j)
      return j; // Здесь мы выходим из бесконечного цикла
    swap(massive, i, j);
  }
}
void quicksortHoare(int *massive, int begin, int end) {
  if (begin < end) {
    int pivot = PartitionHoare(massive, begin, end);
    quicksortHoare(massive, begin, pivot);
    quicksortHoare(massive, pivot + 1, end);
  }
}
void QuicksortHoare(int *massive, int size) {
  quicksortHoare(massive, 0, size-1);
}

std::mt19937 gen(time(0));
//////////////////////////////////////////////////////////
// рандомизированный выбор стержневого элемента
int PartitionRand(int *massive, int begin, int end) {  
  std::uniform_int_distribution<> uid(begin, end-1);
  int i = uid(gen);
  swap(massive, i, end-1);
  return partition_increasing(massive, begin, end);
}
void quicksortRand(int *massive, int begin, int end) {
  if (begin < end) {    
    int pivot = PartitionRand(massive, begin, end);
    quicksortRand(massive, begin, pivot);
    quicksortRand(massive, pivot + 1, end);
  }
}
void QuicksortRand(int *massive, int size) {
  quicksortRand(massive, 0, size);
}

//////////////////////////////////////////////////////////
// при размере подмассива меньше insertSortConst выполняется insertSort
void quicksort_with_insertsort(int *massive, int begin, int end, int insertSortConst) {
  if (begin < end) {
    int pivot = partition_increasing(massive, begin, end);
    
    if (pivot - begin + 1 < insertSortConst) {
      sortInsertInt(massive + begin, pivot - begin + 1);      
    }
    else
      quicksort_with_insertsort(massive, begin, pivot, insertSortConst);

    if (end - (pivot + 1) < insertSortConst)
      sortInsertInt(massive + pivot + 1, end - (pivot + 1));
    else
      quicksort_with_insertsort(massive, pivot + 1, end, insertSortConst);
  }
}
// быстрая сортировка с сортировкой вставкой подмассивов размером меньше 32 
void QuicksortInsertsort32(int *massive, int size) {
  quicksort_with_insertsort(massive, 0, size, 32);
}
// быстрая сортировка с сортировкой вставкой подмассивов размером меньше 64 
void QuicksortInsertsort64(int *massive, int size) {
  quicksort_with_insertsort(massive, 0, size, 64);
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
    { "MergeSort",              new sample_s[MaxSampleCount], MaxSampleCount, &MergeSort},
    { "Quicksort",              new sample_s[MaxSampleCount], MaxSampleCount, &Quicksort},
    { "QuicksortHoare",         new sample_s[MaxSampleCount], MaxSampleCount, &QuicksortHoare},
    { "QuicksortRand",          new sample_s[MaxSampleCount], MaxSampleCount, &QuicksortRand},
    { "QuicksortInsertsort32",  new sample_s[MaxSampleCount], MaxSampleCount, &QuicksortInsertsort32},
    { "QuicksortInsertsort64",  new sample_s[MaxSampleCount], MaxSampleCount, &QuicksortInsertsort64}
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
