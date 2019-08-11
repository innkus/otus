#pragma once

#include "math.h"
#include "array-dyna.h"

// да, реализация квадртаной матрицы - так себе
// квадратная матрица
template <typename T>
struct squareMatrix : DynamicArray<T> {
  //
  squareMatrix(int _N) : DynamicArray<T>(4), N(_N) {
    for (int i = 0; i < N*N; ++i) {
      DynamicArray<T>::add(0);
    }
  }

  //
  squareMatrix() : DynamicArray<T>(4), N(0) {
  }

  // формирование диагональной матрица 
  squareMatrix & operator=(T diagValue) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (i == j)
          DynamicArray<T>::set(diagValue, i*N + j);
        else
          DynamicArray<T>::set(0, i*N + j);
      }
    }
    return *this;
  }

  // оператор умножения
  squareMatrix & operator*(const squareMatrix &m) {
    squareMatrix res(m.N);
    int n = m.N < N ? m.N : N;
    for (int row = 0; row < n; ++row) {
      for (int col = 0; col < n; ++col) {
        T sum = 0;
        for (int k = 0; k < N; ++k) {
          T a = get2(row, k);
          T b = m.get2(k, col);
          sum += get2(row, k) * m.get2(k, col);
        }        
        res.set2(sum, row, col);
      }      
    }
    *this = res;
    return *this;
  }

  // оператор умножения
  squareMatrix & operator=(const squareMatrix &m) {
    if (this == &m)
      return *this;
    DynamicArray<T>::clear();
    N = m.N;
    for (int i = 0; i < N*N; ++i) {
      DynamicArray<T>::add(m.get(i));
    }
    return *this;
  }

  // оператор умножения
  bool operator==(const squareMatrix &m) {
    if (N != m.N)
      return false;
    for (int i = 0; i < N*N; ++i) {
      if (DynamicArray<T>::get(i) != m.get(i))
        return false;
    }
    return true;
  }

  // оператор умножения
  bool operator==(const T &value) {
    for (int i = 0; i < N*N; ++i) {
      if (DynamicArray<T>::get(i) != value)
        return false;
    }
    return true;
  }

  // получить элемент m[i,j]
  T get2(int i, int j) const {
    return DynamicArray<T>::get(i*N + j);
  }
  // установить элемент m[i,j]
  void set2(T value, int i, int j) {
    return DynamicArray<T>::set(value, i*N + j);
  }
  //
  void print() {
    for (int row = 0; row < N; ++row) {
      for (int col = 0; col < N; ++col) {
        std::cout << get2(row, col) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  // размер матрицы
  int N;
};

typedef squareMatrix<int64_t> matrix;