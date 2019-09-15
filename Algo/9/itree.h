#pragma once

#include <string>

// Реализовать АВЛ или Декартово дерево
// Вариант 1. Написать реализацию АВЛ - дерева
// Методы к реализации :
// smallLeft / RightRotation - малое левое / правое вращение
// bigLeft / RightRotation - большое левое / правое вращение, написать через вызов малых вращений
// insert
// remove
// rebalance

//
std::string& operator<< (std::string &out, const std::string &in) {  
  out  = out + in;
  return out;
}

template <typename Key, typename Value>
class TreeLeaf { 
public:
  Key key;     
  Value value;
  TreeLeaf *parent = nullptr;
  TreeLeaf *left = nullptr;
  TreeLeaf *right = nullptr;

  // тип сбалансированности /enum balanceType
  int balance = eBalanced;

  // высота узла
  int height = 1;
};

// тип сбалансированности узла
enum balanceType {
  eBalanced   = 0,    // оба поддерева имеют одну глубину
  eLeftHeavy  = -1,   // левое поддерево длиннее
  eRightHeavy = +1    // правое поддерево длинее
};

// smallLeft / RightRotation - малое левое / правое вращение
// bigLeft / RightRotation - большое левое / правое вращение, написать через вызов малых вращений
// insert
// remove
// rebalance

template <typename Key, typename Value>
struct IBinTree {
  // вставка узла
  virtual void insert(const Key &key, const Value &value) = 0;
  // удаление узла
  virtual Value remove(const Key &key) = 0;
  // печать
  virtual void print() = 0;
  // найти элемент
  virtual Value find(const Key &key) = 0;
  // проверить пустое ли дерево
  virtual bool isEmpty() = 0;

  // проверка дерева на корректность
  virtual bool check() = 0;
  // получить корень дерева
  virtual TreeLeaf<Key, Value> *getRoot() = 0;
  // проверка базовой функциональности
  virtual void testBaseFunctions() = 0;
  // имя дерева
  virtual std::string name() = 0;
};

