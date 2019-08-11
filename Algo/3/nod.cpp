#include "pch.h"
#include <iostream>

// НОД через деление
int nod_devide(unsigned x, unsigned y) {
  while ((x != 0) && (y != 0)) {
    if (x > y)
      x = x % y;
    else
      y = y % x;
  }
  return x + y;
}

// НОД через вычитание
int nod_substruct(unsigned x, unsigned y) {
  while (x != y) {
    if (x > y)
      x -= y;
    else
      y -= x;

  }
  return x;
}

// тест
void test_nod() {
  struct temp {
    int x, y, nod;
  };
  temp nod[] = {
    { 5, 5, 5},
    { 15, 5, 5},
    { 15, 25, 5},
    { 2 * 3 * 7 * 11, 5 * 3 * 7 * 13, 3 * 7},
    { 5 * 3 * 7 * 13, 2 * 3 * 7 * 11, 3 * 7},
  };

  std::cout << "********* NOD *********\n";
  int res;
  for (int i = 0; i < sizeof(nod) / sizeof(temp); ++i) {
      
    res = nod_devide(nod[i].x, nod[i].y);    
    if(res != nod[i].nod)
      std::cout << "! nod (" << nod[i].x << ", " << nod[i].y << ") = " << res << " nod_devide " << std::endl;
    else
      std::cout << "+ nod (" << nod[i].x << ", " << nod[i].y << ") = " << res << " nod_devide " << std::endl;

    res = nod_substruct(nod[i].x, nod[i].y);
    if (res != nod[i].nod)
      std::cout << "! nod (" << nod[i].x << ", " << nod[i].y << ") = " << res << " nod_substruct " << std::endl;
    else
      std::cout << "+ nod (" << nod[i].x << ", " << nod[i].y << ") = " << res << " nod_substruct " << std::endl;
    std::cout << "....................................\n";
  }

  std::cout << "********* NOD *********\n\n";
}