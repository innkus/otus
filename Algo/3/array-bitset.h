#pragma once

#include "pch.h"
#include "iarray.h"

template <typename T>
class BitsetArray : IArray<T> {

private:
  uint32_t *array;
  int count_bit;
  int maxLen32;
  char arrayName[20];
  int seed = 1;

public:

  BitsetArray() {
    count_bit = 0;
    maxLen32 = seed;
    array = new uint32_t[seed];
    memset(array, 0, sizeof(uint32_t)*(seed));
    const char *tmp = "BitArray";
    strcpy_s((char*)arrayName, strlen(tmp) + 1, tmp);
  }

  virtual ~BitsetArray() {
    delete[]array;
    count_bit = 0;
    maxLen32 = 0;
  }

  virtual int size() const override {
    return count_bit;
  }

  int maxSizeByte() const {
    return maxLen32 * sizeof(uint32_t);
  }

  virtual void add(T item) override {
    if (count_bit >= maxSizeByte() * 8)
      resize();
    count_bit++;
    set(item, count_bit - 1);
  }

  void resize() {
    uint32_t *newArray = new uint32_t[maxLen32 + seed];
    memset(newArray, 0, sizeof(uint32_t)*(maxLen32 + seed));
    memcpy(newArray, array, sizeof(uint32_t)*maxLen32);

    delete[]array;
    array = newArray;
    maxLen32 += seed;
  }

  virtual T get(int index) const override {
    if (index < count_bit) {
      // номер элемента массива
      int num = index / 32;
      // позиция в элементе
      int pos = index % 32;
      uint32_t x = array[num] >> pos;
      return T(x & 1);
    }
    return T(0);
  }

  virtual void set(T item, int index) override {
    if (index >= count_bit)
      return;
    // номер элемента массива
    int num = index / 32;
    // позиция в элементе
    int pos = index % 32;
    uint32_t x = 1 << pos;
    if (item) {
      array[num] |= x;
    }
    else {
      x = ~x;
      array[num] &= x;
    }
  }

  virtual T remove(int index) override {
    // TODO ...
    return false;
  }

  virtual void clear() {
    count_bit = 0;
    maxLen32 = 0;
  }

  void print() override {
    std::cout << arrayName << " (" << count_bit << ") : ";
    for (int i = 0; i < count_bit; ++i) {
      std::cout << get(i) << " ";
    }
    std::cout << std::endl;
  }

  const char* name() const override {
    return arrayName;
  }
};

typedef BitsetArray<bool> bitset;
