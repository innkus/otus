#pragma once

#include "pch.h"

template <typename T>
class Node {
public:
  Node(const T &item, const Node<T> *next) {
    this->item = item;
    this->next = next;
  }
  Node(T item) {
    this->item = item;
    this->next = nullptr;
  }
  void setNext(Node<T> *next) {
    this->next = next;
  }
  T getItem() const {
    return item;
  }
  Node<T> *getNext() const {
    return next;
  }

private:
  T item;
  Node<T> *next;
};

