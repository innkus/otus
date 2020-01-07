#pragma once

#include "node.h"

template <typename T> 
class Stack {
public:
  //
  Stack() {
    head = tail = nullptr;
  }
  //
  T front() {
    if (isEmpty())
      return T();
    else
      return head->getItem();
  }
  //
  void push(const T &item) {
    Node<T> *node = new Node<T>(item);
    if (isEmpty()) {
      head = tail = node;
    }
    else {
      node->setNext(head);
      head = node;
    }
  }
  //
  T pop() {
    if (isEmpty())
      return T();
    T item = head->getItem();
    Node<T> *deletedItem = head;
    head = head->getNext();
    delete deletedItem;
    return item;
  }
  //
  int size() const {
    int s = 0;
    Node<T> *curr = head;
    while (curr) {
      s++;
      curr = curr->getNext();
    }
    return s;
  }
  //
  bool isEmpty() const {
    return head == nullptr;
  }
  //
  void print() const {
    std::cout << "stack" << " (" << size() << ") :";
    Node<T> *curr = head;
    while (curr) {
      std::cout << curr->getItem() << " ";
      curr = curr->getNext();
    }
    std::cout << "\n";
  }
  //
  void clear() {
    while (!isEmpty()) {
      pop();
    }
  }

private:
  Node<T> *getHead() const {
    return head;
  }
  //
  Node<T> *getTail() const {
    return tail;
  }

private:
  Node<T> *head;
  Node<T> *tail;
};