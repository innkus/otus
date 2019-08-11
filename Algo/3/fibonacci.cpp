#include "pch.h"
#include <iostream>
#include <chrono>
#include "array-dyna.h"
#include "matrix.h"

// Через двоичное разложение показателя степени
double degree_2_multy(double x, int n);
// Через степень двойки с домножением
matrix degree_2_matrix(matrix x, int n);
// Cтепень итеративно
matrix degree_iter(matrix x, int n);

// Алгоритм поиска чисел Фибоначчи 
// Через рекурсию
int fibonacci_recursive(int N) {
  if (N == 0 || N == 1)
    return N;
  else
    return fibonacci_recursive(N - 1) + fibonacci_recursive(N - 2);
}

// Через итерацию
int fibonacci_iter(int N) {

  if (N == 0 || N == 1)
    return N;

  int res = 0;
  int a = 0;
  int b = 1;
  while (N > 1) {
    res = a + b;
    a = b;
    b = res;
    N--;
  }
  return res;
}

// По формуле золотого сечения
// Fn = [g^n/sqrt(5) + 0.5]; g = (1+sqrt(5))/2
int fibonacci_gold(int N) {
  double root5 = sqrt(5);
  double g = (1 + root5) / 2;
  // g^n
  double g_degre_n = degree_2_multy(g, N);
  return int(g_degre_n/root5 + 0.5);
}

matrix degree_iter2(matrix x, int n) {
  matrix res(x.N);
  res = 1;
  for (int i = n; i > 0; i--) {
    res = res * x;
  }
  return res;
}

// Через умножение матриц(по желанию) + 1 балл
int fibonacci_matrix(int N) {
  
  matrix matrix_n(2);
  if (N == 0 || N == 1)
    matrix_n = N;
  else {
    matrix_n.set2(1, 0, 0);
    matrix_n.set2(1, 0, 1);
    matrix_n.set2(1, 1, 0);
    matrix_n.set2(0, 1, 1);

    matrix_n = degree_2_matrix(matrix_n, N - 1);
  }
  return int(matrix_n.get(0));
}

// тест поиска чисел Фибоначчи 
void test_fibonacci() {
  struct temp {
    unsigned x;
    int result;
  };
  temp data[] = {
    {0,   0},
    {1,   1},
    {2,   1},
    {3,   2},
    {4,   3},
    {5,   5},
    {6,   8},
    {7,   13},
    {8,   21},
    {9,   34},
    {10,  55},
    {11,  89},
    {12,  144},
    {13,  233},
    {14,  377},
    {15,  610},
    {16,  987},
    {17,  1597},
  };

  std::cout << "********* Fibonacci *********\n";
  int res;
  for (int i = 0; i < sizeof(data) / sizeof(temp); ++i) {

    res = fibonacci_recursive(data[i].x);
    if (res != data[i].result)
      std::cout << "! F(" << data[i].x << ") = " << res << " fibonacci_recursive" << std::endl;
    else
      std::cout << "+ F(" << data[i].x << ") = " << res << " fibonacci_recursive" << std::endl;

    res = fibonacci_iter(data[i].x);
    if (res != data[i].result)
      std::cout << "! F(" << data[i].x << ") = " << res << " fibonacci_iter" << std::endl;
    else
      std::cout << "+ F(" << data[i].x << ") = " << res << " fibonacci_iter" << std::endl;

    res = fibonacci_gold(data[i].x);
    if (res != data[i].result)
      std::cout << "! F(" << data[i].x << ") = " << res << " fibonacci_gold" << std::endl;
    else
      std::cout << "+ F(" << data[i].x << ") = " << res << " fibonacci_gold" << std::endl;

    res = fibonacci_matrix(data[i].x);
    if (res != data[i].result)
      std::cout << "! F(" << data[i].x << ") = " << res << " fibonacci_matrix" << std::endl;
    else
      std::cout << "+ F(" << data[i].x << ") = " << res << " fibonacci_matrix" << std::endl;

    std::cout << "....................................\n";
  }
  
  int N = 28;
  int times = 50;
  char name[100];
  for (int i = 0; i < 4; i++) {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    for (int k = 0; k < times; k++) {

      switch (i) {

      case 0:
        strcpy_s(name, sizeof(name), "fibonacci_recursive");
        res = fibonacci_recursive(N); break;
      case 1:
        strcpy_s(name, sizeof(name), "fibonacci_iter");
        res = fibonacci_iter(N); break;
      case 2:
        strcpy_s(name, sizeof(name), "fibonacci_gold");
        res = fibonacci_gold(N); break;

      case 3:
        strcpy_s(name, sizeof(name), "fibonacci_matrix");
        res = fibonacci_matrix(N); break;
      default:
        break;
      }
    }

    end = std::chrono::system_clock::now();

    __int64 elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
      (end - start).count();

    int64_t tms = elapsed_milliseconds;
    std::cout << times << " times: F(" << N << ")=" << res << " " << "Tms = " << tms << " " << name << std::endl;
  }

  std::cout << "********* Fibonacci *********\n\n";
}
