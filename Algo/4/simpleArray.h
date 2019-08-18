#pragma once

#include "pch.h"
#include "iarray.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

/************************************************************/
template <typename T>
class SingleArray : IArray<T> {

private:
  T *array = nullptr;
  int length = 0;
  char arrayName[20];

public:
  SingleArray() {
    const char *tmp = "SingleArray";
    strcpy_s(arrayName, strlen(tmp) + 1, tmp);
  }

  int size() const override {
    return length;
  }

  void add(T item) override {
    resize();
    array[size() - 1] = item;
  }

  void resize() {
    T *newArray = new T[size() + 1];
    memcpy(newArray, array, sizeof(T)*size());
    delete[]array;
    array = newArray;
    length++;
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

    // пересоздаем массив
    T *newArray = new T[size() - 1];
    memcpy(newArray, array, sizeof(T)*size());
    length--;
    return deletedItem;
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

  virtual void set(T item, int index) override {
    // TODO
  }

  virtual void print() override {
    // TODO
  }

};


/************************************************************/

