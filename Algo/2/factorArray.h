// arrays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "iArray.h"

/************************************************************/
template <typename T>
class FactorArray : public IArray<T> {

protected:
  T *array;
  int length;
  int maxLen;
  char arrayName[20];
  int factor;
  int startCount;

public:
  FactorArray(int _factor = 100, int _startCount = 10) : 
    factor(_factor), startCount(_startCount) 
  {
    array = new T[startCount];
    maxLen = startCount;
    length = 0;

    const char *tmp = "FactorArray";
    strcpy_s(arrayName, strlen(tmp) + 1, tmp);
  }

  int size() const override {
    return length;
  }

  void add(T item) override {
    add(item, size());
  }

  T get(int index) const override {
    if (index >= size())
      return T();
    else
      return array[index];
  }

  // return deleted element
  T remove(int index) override {

    if (index >= size() || index < 0)
      return T();
    T deletedItem = get(index);

    // сдвигаем хвост на 1 элемент
    if (size() - (index + 1))
      memcpy(array + index, array + index + 1, sizeof(T)*(size() - (index + 1)));
    length--;

    //не будем урезать память до кратного seed - сэкономим на выделении

    return deletedItem;
  }

  // with shift to tail
  // не рационально и по времени немного теряем, зато более очевидно
  void add(T item, int index) override {

    if (size() >= maxLen || (index >= maxLen)) {
      // если не хватает длины выделяем и делаем рекурсивный вызов

      // это защита от слишком малых factor 
      int offset = size()*(1 + int(factor / 100.));
      if (offset == size())
        offset += factor;
      int newLen = max(index + 1, offset);

      T *newArray = new T[newLen];
      memset(newArray, 0, sizeof(T) * newLen);
      if (size()) {
        memcpy(newArray, array, sizeof(T) * size());
        delete[]array;
      }
      array = newArray;
      maxLen = newLen;
      add(item, index);
      return;
    }

    // по максимальной длине точно проходим
    if (index < size()) {
      // вставка в серединку
      // сдвигаем хвост
      memcpy(array + index + 1, array + index, sizeof(T) * (size() - index));
      array[index] = item;
      length++;
    }
    else {
      // добавление за пределы исходного массива
      array[index] = item;
      length = index + 1;
    }
  }

  void print() override {
    std::cout << arrayName << " (" << size() << "/" << maxLen << ") : ";
    for (int i = 0; i < size(); ++i) {
      std::cout << get(i) << " ";
    }
    std::cout << std::endl;
  }

  const char* name() const override {
    return arrayName;
  }

  void clear() override {
    if (array) {
      delete[]array;
      array = new T[startCount];
      maxLen = startCount;
      length = 0;
    }
  }
};

