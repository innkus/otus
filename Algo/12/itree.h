#pragma once

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

// интерфейс для получения элементов при обходе дерева
template <typename Key, typename T>
struct IGoroundTree {
  //
  virtual void onStartNode() = 0;
  //
  virtual void onStopNode() = 0;
  //
  virtual void onNode(const Key &key, const T &t) = 0;
};

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
  // имя дерева
  virtual std::string name() = 0;
  //
  virtual void makeGoround(IGoroundTree<Key,Value> *) = 0;
  // количество элементов
  virtual int size() = 0;
};

