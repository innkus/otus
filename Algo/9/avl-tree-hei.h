#pragma once

#include "itree.h"
#include <iostream>
#include <string>

// Реализовать АВЛ или Декартово дерево
// Вариант 1. Написать реализацию АВЛ - дерева
// Методы к реализации :
// smallLeft / RightRotation - малое левое / правое вращение
// bigLeft / LeftRotation - большое левое / правое вращение, написать через вызов малых вращений
// insert
// remove
// rebalance

template <typename Key, typename Value>
class AvlTreeHeight : public IBinTree<Key, Value> {
public:

  /// https://habr.com/ru/post/150732/

// балансировка правого поддерева (лево или право-лево) RL
// возвращается новый parent
// Исходный случай 0
// 4 |          ______X+______
//   |         /              \
// 3 |        /             ___B+__
//   |       /             /       \    
// 2 |      T1            /         \   
//   |                   /           \
// 1 |                  T2           (t3)
//   |                                 \
// 0 |                                 T3 (добавили элемент в t3 и стало T3)
    
// Шаг 1. Левый поворот для узла X (меняем местами X и B)
// 3 |                  ____B=____
//   |                 /          \    
// 2 |                X=           \   
//   |               / \            \
// 1 |              /   \           (t3)
//   |             /     \            \
// 0 |            T1      T2          T3 

// Исходный случай 1
// 5 |          ______X+______________
//   |         /                      \
// 4 |        /                   _____B-__
//   |       /                   /         \    
// 3 |      /               ____D+___       \
//   |     /               /         \       \
// 2 |    T1              /           \       \
//   |                   /             \       \
// 1 |                  T2             (t3)    T4
//   |                                   \ 
// 0 |                                   T3 (добавили элемент в t3 и стало T3)
    
// Исходный случай 2
// 5 |          ______X+______________
//   |         /                      \
// 4 |        /                   _____B-__
//   |       /                   /         \    
// 3 |      /               ____D-___       \
//   |     /               /         \       \
// 2 |    T1              /           \       \
//   |                   /             \       \
// 1 |                 (t2)            T3      T4
//   |                 /                  
// 0 |                T2 (добавили элемент в t2 и стало T2)

// Шаг 1. Правый поворот для узла B (меняем местами D и B)
// 5 |          ______X______________
//   |         /                     \
// 4 |        /                   ____D____
//   |       /                   /         \    
// 3 |      /                   /           B
//   |     /                   /           / \
// 2 |    T1                 (t2)         /   \
//   |                       /           /     \
// 1 |                      T2         (t3)     \
//   |                                 /         \
// 0 |                                T3         T4

// Шаг 2. Левый поворот для узла X (меняем местами X и D)
// 5 |               ____________D__________
//   |              /                       \    
// 4 |             X                         B
//   |            / \                       / \
// 3 |           /   \                     /   \
//   |          /     \                   /     \
// 2 |         /      (t2)              (t3)     \
//   |        /         \               /         \
// 1 |       T1         T2             T3         T4
//   |
// 0 |


// балансировка левого поддерева (право или лево-право) LR
// возвращается новый parent

// Исходный случай 0
// 4 |                 ______X-______    
//   |                /              \    
// 3 |            ___B-__             \    
//   |           /       \             \   
// 2 |          /         \            T3 
//   |         /           \
// 1 |       (t1)          T2
//   |       /                
// 0 |      T1 (добавили элемент в t1 и стало T1)

// Шаг 1. Правый поворот для узла X (меняем местами X и B)
// 3 |                  ____B=____
//   |                 /          \    
// 2 |                /            X=       
//   |               /            / \     
// 1 |             (t1)          /   \    
//   |             /            /     \   
// 0 |            T1           T2      T3 

// TODO!!!!
// Исходный случай 1
// 5 |                 ______X-__________
//   |                /                  \     
// 4 |        _______B+________           \
//   |       /                 \           \    
// 3 |      /               ____D+___       \
//   |     /               /         \       \
// 2 |    /               /           \       T4
//   |   /               /             \       
// 1 |  T1              T2             (t3)    
//   |                                   \ 
// 0 |                                   T3 (добавили элемент в t3 и стало T3)

// Исходный случай 2
// 5 |                ______X-__________
//   |               /                  \     
// 4 |        ______B+________           \
//   |       /                \           \    
// 3 |      /              ____D-___       \
//   |     /              /         \       \
// 2 |    /              /           \       T4
//   |   /              /             \       
// 1 |  T1            (t2)             T3    
//   |                /                   
// 0 |               T2  (добавили элемент в t2 и стало T2)

// Шаг 1. Левый поворот для узла B (меняем местами D и B)
// 5 |                  ______X______
//   |                 /             \     
// 4 |            ____D____           \
//   |           /          \          \    
// 3 |        __B__          \          \
//   |       /     \          \          \
// 2 |      /       \        (t3)        T4
//   |     /         \          \               
// 1 |    /          (t2)        T3        
//   |   /             \            
// 0 |  T1             T2                   

// Шаг 2. Правый поворот для узла X (меняем местами X и D)
// 5 |               ____________D__________
//   |              /                       \    
// 4 |             B                         X
//   |            / \                       / \
// 3 |           /   \                     /   \
//   |          /     \                   /     \
// 2 |         /      (t2)              (t3)     \
//   |        /         \               /         \
// 1 |       T1         T2             T3         T4
//   |
// 0 |
  
  // вставка несбалансированная
  virtual void insert(const Key &key, const Value &value = 0) override {    
    root = insert(root, key, value);
  }
  
  //
  virtual Value find(const Key &key) override {
    TreeLeaf<Key, Value> *finded = find(key, root);
    return finded ? finded->value : Value();
  }
  
  // удаление несбалансированнове
  virtual Value remove(const Key &key) override {
    TreeLeaf<Key, Value> *removed = nullptr;
    root = removeNode(root, key, &removed);
    if (removed) {
      Value result = removed->value;
      delete removed;
      return result;
    }
    else {
      return Value();
    }
  }
  
  // проверка дерева на корректность
  virtual bool check()  override {
    const TreeLeaf<Key, Value> *errorNode = checkNode(root);
    return !errorNode;
  }

  //
  virtual bool isEmpty() override {
    return !root;
  }

  //
  virtual void print() override {
    printLevel();
  }

  // получить корень дерева
  virtual TreeLeaf<Key, Value> *getRoot() override {
    return root;
  }

  //
  void printLevel(const TreeLeaf<Key, Value> *node = 0, int level = -1, const char *c = nullptr) {
    if (node == 0 && level == -1) {
      std::cout << "-----------------------------------------\n";
      printLevel(root, 0);
    }
    if (node == nullptr)
      return;
    for (int i = 0; i < level; i++)
      printf("          ");
    if (c)
      std::cout << c;
    std::cout << node->key << "(" << node->height << ")\n";
    printLevel(node->left, level + 1, "L ");
    printLevel(node->right, level + 1, "R ");
  }

private:

  //
  TreeLeaf<Key, Value> *find(const Key &key, TreeLeaf<Key, Value> *node) {
    if (!node)
      return nullptr;
    if (node->key == key)
      return node;
    if (key < node->key)
      return find(key, node->left);
    else
      return find(key, node->right);
  }
  
  // проверка дерева на корректность
  const TreeLeaf<Key, Value> *checkNode(const TreeLeaf<Key, Value> *node) const {
    if (!node)
      return nullptr;

    // баланс должен быть меньше 2    
    int ball = getBalance(node);
    if (labs(ball) > maxBalance) {
      std::cout << "check balance error: key=" << node->key << " balance=" << ball << std::endl;
      return node;
    }

    // проверяем чтобы левый потомок был меньше корня
    if (node->left)
    {
      if (node->left->key < node->key)
        return checkNode(node->left);
      else {
        std::cout << "check tree structure error: key=" << node->key << std::endl;
        return root;
      }
    }

    // проверяем чтобы правый потомок был больше корня
    if (node->right)
    {
      if (node->right->key > node->key)
        return checkNode(node->right);
      else {
        std::cout << "check tree structure error: key=" << node->key << std::endl;
        return node;
      }
    }

    return nullptr;
  }

  // высота узла
  int height(const TreeLeaf<Key, Value> *node) const {
    return node ? node->height : 0;
  }
  
  // баланс узла
  int getBalance(const TreeLeaf<Key, Value> *node) const {
    return height(node->right) - height(node->left);
  }

  // вычисление высоты узла по правильным потомкам
  void setHeight(TreeLeaf<Key, Value> *node) {
    int hLeft   = height(node->left);
    int hRight  = height(node->right);
    node->height = max(hLeft, hRight) + 1;
  }

  // правое вращение возвращается новый корень R
  // левый потомок становится корнем
  // возвращается новый корень
  TreeLeaf<Key, Value> *smallRight(TreeLeaf<Key, Value> *parent) {
    // баланс parent нарушен
    TreeLeaf<Key, Value> *child = parent->left;
    parent->left = child->right;
    child->right = parent;
    // коректируем высоты узлов
    // сначала parent - он ниже
    setHeight(parent);
    // потом child - он ушел наверх
    setHeight(child);
    return child;
  }

  // левое вращение возвращается новый корень L
  // правый потомок становится корнем
  TreeLeaf<Key, Value> *smallLeft(TreeLeaf<Key, Value> *parent) {
    // баланс parent нарушен
    TreeLeaf<Key, Value> *child = parent->right;
    parent->right = child->left;
    child->left = parent;
    // коректируем высоты узлов
    // сначала parent - он ниже
    setHeight(parent);
    // потом child - он ушел наверх
    setHeight(child);
    return child;
  }

  // большое левое вращение
  TreeLeaf<Key, Value> *bigLeft(TreeLeaf<Key, Value> *parent) {
    parent->right = smallRight(parent->right);
    return smallLeft(parent);
  }

  // большое правое вращение
  TreeLeaf<Key, Value> *bigRight(TreeLeaf<Key, Value> *parent) {
    parent->left = smallLeft(parent->left);
    return smallRight(parent);
  }

  //
  TreeLeaf<Key, Value> *insert(TreeLeaf<Key, Value> *parent, 
    const Key &key, const Value &value) {    
    if (!parent) {      
      parent = new TreeLeaf<Key, Value>;
      parent->key = key;
      parent->value = value;
      return parent;
    }
    if (key < parent->key)
      parent->left = insert(parent->left, key, value);
    else
      parent->right = insert(parent->right, key, value);
    return balance(parent);
  }

  // балансировка 
  TreeLeaf<Key, Value> *balance(TreeLeaf<Key, Value> *parent) {
    // считаем высоту 
    setHeight(parent);
    // считаем баланс
    int ball = getBalance(parent);

    switch (ball) {
      case 2: {
        if (getBalance(parent->right) < 0)
          return bigLeft(parent);
        else
          return smallLeft(parent);
      }
      case -2: {
        if (getBalance(parent->left) > 0)
          return bigRight(parent);
        else
          return smallRight(parent);
      }
    }
    return parent; // балансировка не нужна
  }

  // поиск узла с минимальным ключом в поддереве
  TreeLeaf<Key, Value> *findMinNode(TreeLeaf<Key, Value> *node) {
    // всегда идем влево по построению
    return node->left ? findMinNode(node->left) : node;
  }

  // если это минимальный элемент, то по построению справа или один лист или вообще пусто (чтобы баланс был 0/1)
  // надо вернуть указатель на правый узел
  // при возвращении из рекурсии (поднимаеся наверх) выполняетсяь балансировка
  // удаление узла с минимальным ключом из поддерева
  TreeLeaf<Key, Value> *removeMinNode(TreeLeaf<Key, Value>* node) {
    if (node->left == 0)
      return node->right;
    node->left = removeMinNode(node->left);
    return balance(node);
  }

  // находим узел для удаления как обычно
  // разделяем поддерево на лево и право
  // если правое поддерево пустое, возвращаем указатель на левое поддерево
  // если правое поддерево не пустое, то находим там минимальный элемент min
  // тянем мин элемент наверх  и балансируем его
  // удаление 
  TreeLeaf<Key, Value> *removeNode(TreeLeaf<Key, Value> *node, const Key &key, TreeLeaf<Key, Value> **ppRemovedNode) {
    // 
    if (!node) 
      return 0;
    if (key < node->key) {
      node->left = removeNode(node->left, key, ppRemovedNode);
    }
    else if (key > node->key) {
      node->right = removeNode(node->right, key, ppRemovedNode);
    }
    else {
      // key == node->key 
      // нашли узел

      // сохранили потомков
      TreeLeaf<Key, Value> *leftChild = node->left;
      TreeLeaf<Key, Value> *rightChild = node->right;
      *ppRemovedNode = node;
      
      // правого потомка нет - и замечательно - надо просто перевесить левого потомка повыше
      if (!rightChild) 
        return leftChild;

      // есть правый потомок - найдем у него минимум
      TreeLeaf<Key, Value> *minNode = findMinNode(rightChild);
      // удаляем минимум со своего места и перевешиваем ссылки
      minNode->right = removeMinNode(rightChild);
      minNode->left = leftChild;
      //  minNode->right сбалансировали
      //  minNode->left уже раньше был сбалансирован
      return balance(minNode);
    }

    return balance(node);
  }

  //
  virtual std::string name() override {
    return "Avl(with H)";
  }

private:
  TreeLeaf<Key,Value> *root = nullptr;
  int maxBalance = 2;

public:
  // тест обычной вставки + тест поиска
  void testBaseFunctions() {


    {
      AvlTreeHeight<int, int> tb;
      int value[] = { 10, 20, 5, 1, 6};
      for (int i = 0; i < sizeof(value) / sizeof(int);++i) {
        tb.insert(value[i], value[i]); tb.check(); tb.printLevel();
      }
      for (int i = 0; i < sizeof(value) / sizeof(int);++i) {
        if (value[i] != tb.remove(value[i]))
          std::cout << "REMOVE ERROR: " << value[i] << std::endl;
        tb.check(); tb.printLevel();
      }
    }

    {
      AvlTreeHeight<int, int> tb;
      tb.insert(4); tb.check(); tb.printLevel();
      tb.insert(5); tb.check(); tb.printLevel();
      tb.insert(8); tb.check(); tb.printLevel();
    }

    {
      AvlTreeHeight<int, int> tb;
      tb.insert(20); tb.check(); tb.printLevel();
      tb.insert(30); tb.check(); tb.printLevel();
      tb.insert(80); tb.check(); tb.printLevel();
      tb.insert(40); tb.check(); tb.printLevel();
      tb.insert(10); tb.check(); tb.printLevel();
      tb.insert(60); tb.check(); tb.printLevel();
      tb.insert(50); tb.check(); tb.printLevel();
      tb.insert(70); tb.check(); tb.printLevel();
    }

    {
      AvlTreeHeight<int, int> tb;
      tb.insert(30); tb.check(); tb.printLevel();
      tb.insert(15); tb.check(); tb.printLevel();
      tb.insert(40); tb.check(); tb.printLevel();
      tb.insert(10); tb.check(); tb.printLevel();
      tb.insert(20); tb.check(); tb.printLevel();
      tb.insert(5); tb.check(); tb.printLevel();
    }

    {
      AvlTreeHeight<int, int> tb;
      tb.insert(5); tb.check(); tb.printLevel();
      tb.insert(4); tb.check(); tb.printLevel();
      tb.insert(3); tb.check(); tb.printLevel();
      tb.insert(2); tb.check(); tb.printLevel();
      tb.insert(1); tb.check(); tb.printLevel();
    }
    { 
      AvlTreeHeight<int, int> tb;
      tb.insert(5); tb.check(); tb.printLevel();      
      tb.insert(7); tb.check(); tb.printLevel();
      tb.insert(6); tb.check(); tb.printLevel();
    }

    { //RL
      AvlTreeHeight<int, int> tb;
      tb.insert(10); tb.check(); tb.printLevel();
      tb.insert(9);  tb.check(); tb.printLevel();
      tb.insert(17); tb.check(); tb.printLevel();
      tb.insert(20); tb.check(); tb.printLevel();
      tb.insert(12); tb.check(); tb.printLevel();
      tb.insert(14); tb.check(); tb.printLevel();
    }

    { // R
      AvlTreeHeight<int, int> tb;
      tb.insert(10); tb.check(); tb.printLevel();
      tb.insert(8); tb.check(); tb.printLevel();
      tb.insert(6); tb.check(); tb.printLevel();
    }

    { // L
      AvlTreeHeight<int, int> tb;
      tb.insert(1); tb.check(); tb.printLevel();
      tb.insert(2); tb.check(); tb.printLevel();
      tb.insert(3); tb.check(); tb.printLevel();
    }

    { // 
      AvlTreeHeight<int, int> tb;
      tb.insert(4); tb.check(); tb.printLevel();
      tb.insert(5); tb.check(); tb.printLevel();
      // RR
      tb.insert(7); tb.check(); tb.printLevel();
      tb.insert(2); tb.check(); tb.printLevel();
      // LL
      tb.insert(1); tb.check(); tb.printLevel();
      // LR
      tb.insert(3); tb.check(); tb.printLevel();
      // RL
      tb.insert(6); tb.check(); tb.printLevel();
    }

  }

};
