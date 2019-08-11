#pragma once

#include "pch.h"
#include "iarray.h"

/************************************************************/
template <typename T>
class DynamicArray : IArray<T> {

private:
  T *array = nullptr;
  int length;
  int maxLen;
  char arrayName[20];
  int seed;

public:
  DynamicArray(int _seed) : seed(_seed) {
    array = new T[_seed];
    maxLen = _seed;
    length = 0;

    const char *tmp = "DynamicArray";
    strcpy_s(arrayName, strlen(tmp) + 1, tmp);
  }

  int size() const override {
    return length;
  }

  void add(T item) override {
    if (length >= maxLen)
      resize();
    array[length] = item;
    length++;
  }

  void resize() {
    T *newArray = new T[size() + seed];
    memcpy(newArray, array, sizeof(T)*size());
    delete[]array;
    array = newArray;
    maxLen += seed;
  }

  T get(int index) const override {
    if (index >= length)
      return T();
    else
      return array[index];
  }

  // return deleted element
  T remove(int index) override {

    if (index >= length)
      return T();

    T deletedItem = get(index);

    // сдвигаем хвост на 1 элемент
    memcpy(array + index, array + index + 1, sizeof(T)*(size() - (index + 1)));
    length--;

    //не будем урезать память до кратного seed - сэкономим на выделении

    return deletedItem;
  }

  void print() override {
    std::cout << arrayName << " (" << length << ") : ";
    for (int i = 0; i < length; ++i) {
      std::cout << get(i) << " ";
    }
    std::cout << std::endl;
  }

  const char* name() const override {
    return arrayName;
  }

  void clear() override {
      length = 0;
  }

  virtual void set(T item, int index) override {
    if (index >= length)
      return;
    array[index] = item;
  }
};
/************************************************************/

typedef DynamicArray<bool> boolarray;