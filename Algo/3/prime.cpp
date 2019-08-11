#include "pch.h"
#include <iostream>
#include <chrono>
#include "array-dyna.h"
#include "array-bitset.h"

using namespace std::chrono;

// јлгоритмы поиска кол - ва простых чисел до N 
// „ерез перебор делителей
bool prime(int p) {
  int c = 0;
  for (int i = 1; i <= p; i++) {
    if (p % i == 0)
      c++;
  }
  return c == 2;
}
int search(int N) {
  int count = 0;
  for (int i = 2; i <= N; i++) {
    if (prime(i))
      count++;
  }
  return count;
}

// Ќесколько оптимизаций перебора делителей, с использованием массива
//*******************************************
// идем только по нечетным числам
bool prime2(int p) {
  int c = 0;
  for (int i = 1; i <= p; i++) {
    if (p % i == 0)
      c++;
  }
  return c == 2;
}
int search_odd(int N) {
  int count = 1;
  for (int i = 3; i <= N; i+=2) {
    if (prime2(i))
      count++;
  }
  return count;
}
//*******************************************
// идем только по нечетным числам и выходим сразу как только будет найдет делитель
bool prime3(int p) {
  
  for (int i = 2; i < p; i++) {
    if (p % i == 0)
      return false;
  }
  return true;
}
int search_odd_break(int N) {
  int count = 1;
  for (int i = 3; i <= N; i++) {
    if (prime3(i))
      count++;
  }
  return count;
}
//*******************************************
// идем только по нечетным числам и выходим сразу как только будет найдет делитель
// при проверки на делители простого числе ограничиваемс¤ корнем
bool prime4(int p) {

  for (int i = 2; i*i <= p; i++) {
    if (p % i == 0)
      return false;
  }
  return true;
}
int search_odd_break_root(int N) {
  int count = 1;
  for (int i = 3; i <= N; i++) {
    if (prime4(i))
      count++;
  }
  return count;
}
// –ешето Ёратосфена
int eratosfen(int N) {
  boolarray prime(100);
  
  prime.add(false);
  prime.add(false);
  
  for (int i = 2; i <= N; i++) {
    prime.add(true);
  }

  for (int i = 2; i *i <= N; ++i) {
    if (!prime.get(i))
      continue;
    for (int j = i * i; j <= N; j+=i) {
      if (j % i == 0)
        prime.set(false, j);
    }    
  }

  int c = 0;
  for (int i = 1; i <= N; i++) {
    if (prime.get(i))
      c++;
  }
  return c;
}

// –ешето Ёратосфена с битовой матрицей, по 32 значени¤ в одном int(по желанию) 
int eratosfen_bit(int N) {
  bitset prime;

  prime.add(false);
  prime.add(false);

  for (int i = 2; i <= N; i++) {
    prime.add(true);
  }

  for (int i = 2; i *i <= N; ++i) {
    if (!prime.get(i))
      continue;
    for (int j = i * i; j <= N; j += i) {
      if (j % i == 0)
        prime.set(false, j);
    }
  }

  int c = 0;
  for (int i = 1; i <= N; i++) {
    if (prime.get(i))
      c++;
  }
  return c;
}

// тест подсчет простых чисел
void test_prime() {
  struct temp {
    unsigned x;
    int result;
  };
  temp data[] = {
    {5, 3},
    {10, 4},
    {50, 15},
    {100, 25},
    {200, 46},
    {300, 62},
    {400, 78},
    {500, 95},
    {600, 109},
    {700, 125},
    {800, 139},
    {900, 154},
    {1000, 168},
  };

  std::cout << "********* Prime *********\n";
  int res;
  for (int i = 0; i < sizeof(data) / sizeof(temp); ++i) {

    res = search(data[i].x);
    if (res != data[i].result)      
      std::cout << "! prime=" << data[i].x << " N=" << res << " search " << std::endl;
    else
      std::cout << "+ prime=" << data[i].x << " N=" << res << " search " << std::endl;
   
    res = search_odd(data[i].x);
    if (res != data[i].result)
      std::cout << "! prime=" << data[i].x << " N=" << res << " search_odd " << std::endl;
    else
      std::cout << "+ prime=" << data[i].x << " N=" << res << " search_odd " << std::endl;

    res = search_odd_break(data[i].x);
    if (res != data[i].result)
      std::cout << "! prime=" << data[i].x << " N=" << res << " search_odd_break " << std::endl;
    else
      std::cout << "+ prime=" << data[i].x << " N=" << res << " search_odd_break " << std::endl;

    res = search_odd_break_root(data[i].x);
    if (res != data[i].result)
      std::cout << "! prime=" << data[i].x << " N=" << res << " search_odd_break_root " << std::endl;
    else
      std::cout << "+ prime=" << data[i].x << " N=" << res << " search_odd_break_root " << std::endl;

    res = eratosfen(data[i].x);
    if (res != data[i].result)
      std::cout << "! prime=" << data[i].x << " N=" << res << " eratosfen " << std::endl;
    else
      std::cout << "+ prime=" << data[i].x << " N=" << res << " eratosfen " << std::endl;

    res = eratosfen_bit(data[i].x);
    if (res != data[i].result)
      std::cout << "! prime=" << data[i].x << " N=" << res << " eratosfen_bit " << std::endl;
    else
      std::cout << "+ prime=" << data[i].x << " N=" << res << " eratosfen_bit " << std::endl;

    std::cout << "....................................\n";
  }

  int N = 100000;
  char name[30];
  for (int i = 0; i < 6; i++) {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
 
    switch (i) {
      
    case 0:
      strcpy_s(name, sizeof(name), "search");
      res = search(N); break;
    case 1:
      strcpy_s(name, sizeof(name), "search_odd");
      res = search_odd(N); break;
      
    case 2:
      strcpy_s(name, sizeof(name), "search_odd_break");
      res = search_odd_break(N); break;
      
    case 3:
      strcpy_s(name, sizeof(name), "search_odd_break_root");
      res = search_odd_break_root(N); break;

    case 4:
      strcpy_s(name, sizeof(name), "eratosfen");
      res = eratosfen(N); break;

    case 5:
      strcpy_s(name, sizeof(name), "eratosfen_bit");
      res = eratosfen_bit(N); break;

    default:
      break;
    }

    end = std::chrono::system_clock::now();

    __int64 elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
      (end - start).count();

    int64_t tms = elapsed_milliseconds;
    std::cout << "prime=" << N << " N=" << res << " " << "Tms = " << tms << " " << name << std::endl;
  }

  std::cout << "********* Prime *********\n\n";
}
