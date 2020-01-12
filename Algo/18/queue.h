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

template <typename T> 
class Queue {
public:
  //
  Queue() {
    head = tail = nullptr;
  }
  //
  Node<T> *getHead() const {
    return head;
  }
  //
  Node<T> *getTail() const {
    return tail;
  }
  //
  bool isEmpty() const {
    return head == nullptr;
  }
  //
  void enqueue(const T &item) {
    Node<T> *node = new Node<T>(item);
    if (isEmpty()) {
      head = tail = node;
    }
    else {
      tail->setNext(node);
      tail = node;
    }
  }
  // вставка после 
  bool insertAfter(Node<T> *node, const T &item) {
    if (isEmpty() && node) {
      // странно - выйдем
      return false;
    }

    if (isEmpty() || (node == tail)) {
      enqueue(item);
      return true;
    }

    if (!node) {
      // вставить в начало
      Node<T> *headNode = new Node<T>(item);
      headNode->setNext(head);
      head = headNode; 
      return true;
    }

    // проверим что есть такой элемент
    Node<T> *curr = head;
    while (curr != tail) {
      if (curr == node) {
        Node<T> *newdNode = new Node<T>(item);
        newdNode->setNext(node->getNext());
        node->setNext(newdNode);
        return true;
      }
      curr = curr->getNext();
    }
    return false;
  }
  // удаление следующего за
  bool removeAfter(Node<T> *node) {
    if (isEmpty() || (node == tail) ) {
      // странно - выйдем
      return false;
    }

    if (node == nullptr) {
      dequeue();
      return true;
    }

    // проверим что есть такой элемент
    Node<T> *curr = head;
    while (curr != tail) {
      if (curr == node) {
        Node<T> *del = node->getNext();
        node->setNext(del->getNext());
        delete del;
        return true;
      }
      curr = curr->getNext();
    }
    return false;
  }
  //
  T dequeue() {
    if (isEmpty())
      return T();
    T item = head->getItem();
    Node<T> *deletedItem = head;
    head = head->getNext();
    delete deletedItem;
    return item;
  }
  //
  void clear() {
    while (!isEmpty()) {
      dequeue();
    }
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

private:
  Node<T> *head;
  Node<T> *tail;
};