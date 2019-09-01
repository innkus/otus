#pragma once

#include "pch.h"
#include "queue.h"

struct para {
  int priority;
  int data;
};

template <typename T>
struct PNode {
  int priority;
  Queue<T> queue;
};

template <typename T>
class PriorityQueue {
private:
  Queue<PNode<T>*> listQueue;
  int length = 0;
public:

  // Методы к реализации :
  // поместить элемент в очередь
  void enqueue(int priority, T item) {
    // найти список с заданным приоритетом
    Node <PNode<T>*> *curr = listQueue.getHead();
    Node <PNode<T>*> *prev = nullptr;
    while (curr) {
      PNode<T>* p = curr->getItem();
      if (p->priority == priority) {
        p->queue.enqueue(item);
        length++;
        return;
      }
      if (p->priority < priority)
        prev = curr;
      curr = curr->getNext();
    }

    // если мы здесь то не нашли нужного приоритета
    PNode<T>* pNew = new PNode<T>;
    pNew->priority = priority;
    pNew->queue.enqueue(item);
    if (listQueue.insertAfter(prev, pNew))
      length++;
  }

  // выбрать элемент из очереди
  T dequeue() {
    // найти список с этим приоритетом
    if (listQueue.isEmpty())
      return T();

    Node <PNode<T>*> *head = listQueue.getHead();
    PNode<T>* p = head->getItem();
    T deletedItem = p->queue.dequeue();
    length--;
    if (p->queue.isEmpty()) {
      listQueue.removeAfter(nullptr);
    }
    return deletedItem;
  }

  //
  void print() {
    std::cout << "PriorityQueue" << " (" << listQueue.size() << ") elemCount=" << length << " >>\n";

    int i = 1;
    Node <PNode<T>*> *curr = listQueue.getHead();
    while (curr) {
      PNode<T>* p = curr->getItem();
      std::cout << i++ << ") pri=" << p->priority << " >> ";
      p->queue.print();
      curr = curr->getNext();
    }
  }

  void clear() {
    while (!listQueue.isEmpty()) {
      PNode<T>* p = listQueue.dequeue();
      p->queue.clear();
      delete p;
    }
  }

  void testBaseFunctions() {
    std::cout << "PriorityQueue" << std::endl;
    {
      // добавлять новые приоритеты в начало
      // добавить несколько элементов а существующий приоритет

      clear();
      para in[] = { {10, 10}, {10, 100}, {9,9}, {9,90}, {8,8}, {8,80}, {7,7}, {7,70} };
      para out[] = { {7,7}, {7,70}, {8,8}, {8,80}, {9,9}, {9,90}, {10, 10}, {10, 100} };
      for (int i = 0; i < sizeof(in) / sizeof(para); ++i) {
        enqueue(in[i].priority, in[i].data);
        //print();
      }
      int i = 0;
      while (!listQueue.isEmpty()) {
        T data = dequeue();
        if (data != out[i].data) {
          std::cout << "ERROR!!! remove from start\n";
        }
        i++;
        //print();
      }
    }

    {
      // добавлять новые приоритеты в конец
      // добавить несколько элементов а существующий приоритет

      clear();
      para in[] = { {10, 10}, {10, 100}, {11,111}, {11,112}, {10,222}, {11,222} };
      para out[] = { {10, 10}, {10, 100}, {10,222}, {11,111}, {11,112}, {11,222} };
      for (int i = 0; i < sizeof(in) / sizeof(para); ++i) {
        enqueue(in[i].priority, in[i].data);
        //print();
      }
      int i = 0;
      while (!listQueue.isEmpty()) {
        T data = dequeue();
        if (data != out[i].data) {
          std::cout << "ERROR!!! remove from start\n";
        }
        i++;
        //print();
      }
    }

    {
      // добавлять новые приоритеты в середину
      // добавить несколько элементов а существующий приоритет

      clear();
      para in[] = { {10, 10}, {10, 100}, {90,9}, {90,90}, {45,45}, {40,40}, {60,60}, {45,4545} };
      para out[] = { {10, 10}, {10, 100}, {40,40}, {45,45}, {45,4545}, {60,60}, {90,9}, {90,90}};
      for (int i = 0; i < sizeof(in) / sizeof(para); ++i) {
        enqueue(in[i].priority, in[i].data);
        //print();
      }
      int i = 0;
      while (!listQueue.isEmpty()) {
        T data = dequeue();
        if (data != out[i].data) {
          std::cout << "ERROR!!! remove from start\n";
        }
        i++;
        //print();
      }
    }
 
  }
};
