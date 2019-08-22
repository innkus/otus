// 6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

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
    //print(Array, size);
  }
}

void BuildHeapIter(int *Array, int size) {
  for (int i = floor((size - 1) / 2); i >= 0; i--) {
    DrownIter(Array, i, size);
    //print(Array, size);
  }
}

void Heapsort(int *Array, int size) {
  std::cout << "Recursive Heapsort\n";
  int arraySize = size;
  BuildHeap(Array, arraySize);
  //print(Array, arraySize);
  for (int i = size - 1; i >= 1; --i) {
    size = size - 1;
    Swap(Array, 0, i);
    //print(Array, arraySize);
    Drown(Array, 0, size);
    //print(Array, arraySize);
  }
}

void HeapsortIter(int *Array, int size) {
  std::cout << "Iteration Heapsort\n";
  int arraySize = size;
  BuildHeapIter(Array, arraySize);
  //print(Array, arraySize);
  for (int i = size - 1; i >= 1; --i) {
    size = size - 1;
    Swap(Array, 0, i);
    //print(Array, arraySize);
    DrownIter(Array, 0, size);
    //print(Array, arraySize);
  }
}

void test(bool bIter) {

  {
    int ArrayIn[] = { 9, 5, 10, 8, 2, 1, 0, 3, 11, 4, 6, 7 };
    int ArrayOut[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
    int size = sizeof(ArrayIn) / sizeof(int);
    
    bIter ? HeapsortIter(ArrayIn, size) : Heapsort(ArrayIn, size);
    bool bError = false;
    for (int i = 0; i < size; ++i)
    {
      if (ArrayIn[i] != ArrayOut[i]) {
        std::cout << "Error\n";
        std::cout << "out: "; print(ArrayOut, size);
        std::cout << "res: "; print(ArrayIn, size);
        bError = true;
        break;
      }
    }
    if (!bError) {
      std::cout << "+\n";
    }
  }

  {
    int ArrayIn[] = { 1, 2, 3, 4, 5, 6, 7};
    int ArrayOut[] = { 1, 2, 3, 4, 5, 6, 7 };
    int size = sizeof(ArrayIn) / sizeof(int);

    bIter ? HeapsortIter(ArrayIn, size) : Heapsort(ArrayIn, size);
    bool bError = false;
    for (int i = 0; i < size; ++i)
    {
      if (ArrayIn[i] != ArrayOut[i]) {
        std::cout << "Error\n";
        std::cout << "out: "; print(ArrayOut, size);
        std::cout << "res: "; print(ArrayIn, size);
        bError = true;
        break;
      }
    }
    if (!bError) {
      std::cout << "+\n";
    }
  }

  {
    int ArrayIn[] = { 7, 6, 5, 4, 3, 2, 1 };
    int ArrayOut[] = { 1, 2, 3, 4, 5, 6, 7 };
    int size = sizeof(ArrayIn) / sizeof(int);

    bIter ? HeapsortIter(ArrayIn, size) : Heapsort(ArrayIn, size);
    bool bError = false;
    for (int i = 0; i < size; ++i)
    {
      if (ArrayIn[i] != ArrayOut[i]) {
        std::cout << "Error\n";
        std::cout << "out: "; print(ArrayOut, size);
        std::cout << "res: "; print(ArrayIn, size);
        bError = true;
        break;
      }
    }
    if (!bError) {
      std::cout << "+\n";
    }
  }

  {
    int ArrayIn[] = { 19, 13, 1, 14, 9, 10, 6, 17, 3, 11, 20, 7, 16, 4, 15, 2, 8, 18, 5, 12};
    int ArrayOut[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    int size = sizeof(ArrayIn) / sizeof(int);

    bIter ? HeapsortIter(ArrayIn, size) : Heapsort(ArrayIn, size);
    bool bError = false;
    for (int i = 0; i < size; ++i)
    {
      if (ArrayIn[i] != ArrayOut[i]) {
        std::cout << "Error\n";
        std::cout << "out: "; print(ArrayOut, size);
        std::cout << "res: "; print(ArrayIn, size);
        bError = true;
        break;
      }
    }
    if (!bError) {
      std::cout << "+\n";
    }
  }
  

  {
    int ArrayIn[] = { 5, 5, 45, 45, 1, 1};
    int ArrayOut[] = { 1, 1, 5, 5, 45, 45 };
    int size = sizeof(ArrayIn) / sizeof(int);

    bIter ? HeapsortIter(ArrayIn, size) : Heapsort(ArrayIn, size);
    bool bError = false;
    for (int i = 0; i < size; ++i)
    {
      if (ArrayIn[i] != ArrayOut[i]) {
        std::cout << "Error\n";
        std::cout << "out: "; print(ArrayOut, size);
        std::cout << "res: "; print(ArrayIn, size);
        bError = true;
        break;
      }
    }
    if (!bError) {
      std::cout << "+\n";
    }
  }

  {
    int ArrayIn[] = { 1 };
    int ArrayOut[] = { 1 };
    int size = sizeof(ArrayIn) / sizeof(int);

    bIter ? HeapsortIter(ArrayIn, size) : Heapsort(ArrayIn, size);
    bool bError = false;
    for (int i = 0; i < size; ++i)
    {
      if (ArrayIn[i] != ArrayOut[i]) {
        std::cout << "Error\n";
        std::cout << "out: "; print(ArrayOut, size);
        std::cout << "res: "; print(ArrayIn, size);
        bError = true;
        break;
      }
    }
    if (!bError) {
      std::cout << "+\n";
    }
  }

  {
    int ArrayIn[] = { 1, 0 };
    int ArrayOut[] = { 0, 1 };
    int size = sizeof(ArrayIn) / sizeof(int);

    bIter ? HeapsortIter(ArrayIn, size) : Heapsort(ArrayIn, size);
    bool bError = false;
    for (int i = 0; i < size; ++i)
    {
      if (ArrayIn[i] != ArrayOut[i]) {
        std::cout << "Error\n";
        std::cout << "out: "; print(ArrayOut, size);
        std::cout << "res: "; print(ArrayIn, size);
        bError = true;
        break;
      }
    }
    if (!bError) {
      std::cout << "+\n";
    }
  }

}


int main()
{
  // рекурсивно
  test(false);
  // итеративно
  test(true);
}
