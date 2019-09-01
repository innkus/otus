#pragma once

#include "pch.h"
#include "iArray.h"
#include "queue.h"

/************************************************************/
template <typename T>
class LinkedArray : public IArray<T> {

private:
  Queue<T> queue;
  int length;
  char arrayName[20];

public:
  LinkedArray() {
    length = 0;
    const char *tmp = "LinkedArray";
    strcpy_s(arrayName, strlen(tmp) + 1, tmp);
  }

  int size() const override {
    return length;
  }

  void add(T item) override {
    queue.enqueue(item);
    length++;
  }

  T get(int index) const override {
    if (index >= size())
      return T();
    Node<T> *curr = queue.getHead();
    for (int j = 0; j < index; ++j) {
      curr = curr->getNext();
    }
    return curr->getItem();
  }

  Node<T> * getNode(int index) const  {
    if (index >= size() || index < 0)
      return nullptr;
    Node<T> *curr = queue.getHead();
    for (int j = 0; j < index; ++j) {
      curr = curr->getNext();
    }
    return curr;
  }

  // return deleted element
  T remove(int index) override {

    if (index >= size() || index < 0)
      return T();

    if (index == 0) {
      // первый элемент
      length--;
      return queue.dequeue();
    }

    Node<T> *prevDel = getNode(index - 1);
    T deletedItem = prevDel->getNext()->getItem();

    if (queue.removeAfter(prevDel)) {
      length--;
      return deletedItem;
    }

    return T();
  }

  // with shift to tail
  void add(T item, int index) override {

    if (index <= length) {
      Node<T> *prev = getNode(index - 1);
      if (queue.insertAfter(prev, item))
        length++;
      return;
    }

    while (size() != index) {
      add(T());
    }
    add(item);
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
    queue.clear();
    length = 0;
  }
};
/************************************************************/

