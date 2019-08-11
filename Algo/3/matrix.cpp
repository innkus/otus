#include "pch.h"
#include "matrix.h"

// тестирование матриц
void test_matrix() {
  {
    matrix m(2);
    m = 1;
    m.print();
  }
  {
    matrix m(2);
    m.set2(1, 0, 0);
    m.set2(1, 0, 1);
    m.set2(1, 1, 0);
    m.set2(0, 1, 1);
    m.print();
    
    m = m * m;
    m.print();
    
    for (int i = 0; i < 5; i++) {
      m = m * m;
      m.print();
    }
    
  }
}
