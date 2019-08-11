#include "pch.h"
#include "array-bitset.h"

//////////////////////////////////////////////////////
// тест
//////////////////////////////////////////////////////
void test_array_bitset() {

  bitset m;

  m.add(1);
  m.print();

  m.add(1);
  m.add(0);
  m.add(1);
  // 1101
  m.print();

  m.add(1);
  m.add(0);
  m.add(0);
  m.add(1);
  // 11011001
  m.print();

  m.add(1);
  // 110110011
  m.print();

  m.add(0);
  // 1101100110
  m.print();

  for (int i = 0; i < m.size();++i) {
    bool x = m.get(i);
    m.set(!x, i);
  }
  m.print();

}


