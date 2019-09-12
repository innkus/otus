// arrays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "iArray.h"

/************************************************************/
template <typename T>
class SingleArray : public IArray<T> {

private:
  T *array;
  int length;
  char arrayName[20];

public:
  SingleArray() {
    array = nullptr;
    length = 0;
    const char *tmp = "SingleArray";
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

    if (length == 1) {
      length--;
      delete[]array;
      array = nullptr;
      return deletedItem;
    }

    // сдвигаем хвост на 1 элемент
    memcpy(array + index, array + index + 1, sizeof(T)*(size() - (index + 1)));

    // пересоздаем массив
    length--;
    T *newArray = new T[length];
    memcpy(newArray, array, sizeof(T)*size());
    delete[]array;
    array = newArray;
    return deletedItem;
  }

  // with shift to tail
  void add(T item, int index) override {
    // определяем размер массива
    int newLen;
    if (index < size())
      newLen = size() + 1;
    else
      newLen = index + 1;

    T *newArray = new T[newLen];
    memset(newArray, 0, sizeof(T) * newLen);

    if (index < size()) {
      // вставка в серединку
      // перекопируем часть до вставки
      if (index)
        memcpy(newArray, array, sizeof(T) * index);
      newArray[index] = item;
      // перекопируем хвост
      memcpy(newArray + index + 1, array + index, sizeof(T) * (size() - index));
    }
    else {
      // добавление за пределы исходного массива
      // просто перекопируем и вставим куда надо
      memcpy(newArray, array, sizeof(T) * size());
      newArray[index] = item;
    }

    delete[]array;
    array = newArray;
    length = newLen;
  }

  void print() override {
    std::cout << arrayName << " (" << size() << ") : ";
    for (int i = 0; i < size(); ++i) {
      std::cout << get(i) << " ";
    }
    std::cout << std::endl << std::endl;
  }

  const char* name() const override {
    return arrayName;
  }

  void clear() override {
    if (array) {
      delete[]array;
      array = nullptr;
      length = 0;
    }
  }
};
/************************************************************/
