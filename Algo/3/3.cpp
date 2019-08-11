// 3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

// тест НОД
void test_nod();
// тест возведения в степень
void test_degree();
// тест подсчет простых чисел
void test_prime();
// тест поиска чисел Фибоначчи 
void test_fibonacci();
// тестирование матриц
void test_matrix();
// битовый массив
void test_array_bitset();

int main()
{
  test_nod();
  test_degree();  
  test_prime();
  test_fibonacci();
  
  return 0;
}
