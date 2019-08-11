#include "pch.h"
#include <iostream>
#include "matrix.h"

//////////////////////////////////////////////////////
// �������� ���������� � ������� 
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// ����������� (n ���������) 
//////////////////////////////////////////////////////
template <typename T>
T degree_iter_templ(T x, int n) {
  T res;
  res = x;
  res = 1;
  for (int i = n; i > 0; i--) {
    res = res * x;
  }
  return res;
}
// ���������� ����� � �������
double degree_iter(double x, int n) {
  return degree_iter_templ<double>(x, n);
}
// ���������� ������ � �������
matrix degree_iter(matrix x, int n) {
  return degree_iter_templ<matrix>(x, n);
}

//////////////////////////////////////////////////////
// ����� ������� ������ � �����������
//////////////////////////////////////////////////////
template <typename T>
T degree_2_multy_templ(T x, int n) {

  T res;
  if (n == 0) {
    res = x;
    res = 1;
    return res;
  }
      
  res = x;
  int i;
  for (i = 1; i * 2 <= n; i = i * 2) {
    res = res * res;
  }
  for (int j = i; j < n; ++j)
    res = res * x;
  return res;
}
// ����� ������� ������ � ����������� ��� �����
double degree_2_multy(double x, int n) {
  return degree_2_multy_templ<double>(x, n);
}
// ����� ������� ������ � ����������� ��� �������
matrix degree_2_matrix(matrix x, int n) {
  return degree_2_multy_templ<matrix>(x, n);
}

//////////////////////////////////////////////////////
// ����� �������� ���������� ���������� ������� ��� �����
//////////////////////////////////////////////////////
double degree_degree2_multy(double x, int n) {
  double res = 1;
  while (n > 1) {
    if (n % 2 == 1)
      res *= x;
    x *= x;
    n /= 2;
  }
  if (n > 0)
    res *= x;
  return res;
}

//////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////
void test_degree() {
  struct temp {
    double x;
    int n;
    double res;
  };
  temp data[] = {
    { 5, 0, 1},
    { 1, 10, 1},
    { 2, 10, 1024},
    { 3, 4, 81},
    { 1000, 3, 1000000000},
  };

  std::cout << "********* Degree *********\n";
  int res;
  for (int i = 0; i < sizeof(data) / sizeof(temp); ++i) {

    res = degree_iter(data[i].x, data[i].n);
    if (res != data[i].res)
      std::cout << "! " << data[i].x << "^" << data[i].n << " = " << res << " degree_iter" << std::endl;
    else
      std::cout << "+ " << data[i].x << "^" << data[i].n << " = " << res << " degree_iter" << std::endl;

    res = degree_2_multy(data[i].x, data[i].n);
    if (res != data[i].res)
      std::cout << "! " << data[i].x << "^" << data[i].n << " = " << res << " degree_2_multy" << std::endl;
    else
      std::cout << "+ " << data[i].x << "^" << data[i].n << " = " << res << " degree_2_multy" << std::endl;

    res = degree_degree2_multy(data[i].x, data[i].n);
    if (res != data[i].res)
      std::cout << "! " << data[i].x << "^" << data[i].n << " = " << res << " degree_degree2_multy" << std::endl;
    else
      std::cout << "+ " << data[i].x << "^" << data[i].n << " = " << res << " degree_degree2_multy" << std::endl;
    std::cout << "....................................\n";
  }

  
  std::cout << "e = " << degree_iter(1.0000001, 10000000) << " degree_iter " << std::endl;
  std::cout << "e = " << degree_2_multy(1.0000001, 10000000) << " degree_2_multy " << std::endl;
  std::cout << "e = " << degree_degree2_multy(1.0000001, 10000000) << " degree_degree2_multy " << std::endl;
  std::cout << "********* Degree *********\n\n";
}
