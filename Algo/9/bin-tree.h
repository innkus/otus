#pragma once

#include "itree.h"
#include <iostream>
#include <string>
#include "avl-tree-hei.h"

// Реализовать АВЛ или Декартово дерево
// Вариант 1. Написать реализацию АВЛ - дерева
// Методы к реализации :
// smallLeft / RightRotation - малое левое / правое вращение
// bigLeft / RightRotation - большое левое / правое вращение, написать через вызов малых вращений
// insert
// remove
// rebalance

template <typename Key, typename Value>
class BinTree : public IBinTree<Key, Value> {
public:

  // проверка дерева на корректность
  virtual bool check() override {
    const TreeLeaf<Key, Value> *errorNode = checkNode(root);
    if (errorNode) {
      std::cout << "check tree structure error: key=" << errorNode->key << std::endl;
    }
    return !errorNode;
  }

  // вставка несбалансированная
  virtual void insert(const Key &key, const Value &value = 0) override {
    TreeLeaf<Key, Value> *pNew = new TreeLeaf<Key, Value>();
    pNew->key = key;
    /* в целях отладки значение равно ключу */
    pNew->value = key;
    if (!root) {
      root = pNew;
    }
    else {
      insertNode(root, pNew);
    }
    check();
  }

  //
  virtual void print() override {
    std::string out;
    printNode(root, out);
    std::cout << out << std::endl;
  }
  //
  std::string sprint() {
    std::string out;
    printNode(root, out);
    return out;
  }
  //
  virtual Value find(const Key &key) override {
    TreeLeaf<Key, Value> * finded = find(key, root);
    return finded ? finded->value : Value();
  }
  //
  virtual bool isEmpty() override {
    return !root;
  }
  // удаление несбалансированнове
  virtual Value remove(const Key &key) override {
    TreeLeaf<Key, Value> *removed = find(key, root);
    if (!removed)
      return Value();

    if (!removed->parent) {
      // это корень     

      if (removed->left) {
        root = removed->left;
        root->parent = nullptr;
        if (removed->right) 
          insertNode(root, removed->right);
      }
      else {
        // левого узла нет
        // просто перекидываем корень направо
        root = removed->right;
        if(root)
          root->parent = nullptr;
      }
    }
    else {
      // где-то в серединке
      // у родителя убираем ссылку
      TreeLeaf<Key, Value> *parent = removed->parent;

      // отрываем ссылку у родителя
      parent->left == removed ? 
        parent->left = nullptr : parent->right = nullptr;

      // разбираемся с поддеревьями
      if (removed->left)
        insertNode(parent, removed->left);
      if (removed->right)
        insertNode(parent, removed->right);
    }

    Value result = removed->value;
    delete removed;
    check();
    return result;
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


  // тест обычной вставки + тест поиска
  void testBaseFunctions() {
    {
      // дерево влево
      BinTree<int, int> tb;
      std::string checkout = "K5 L(K4 L(K3 L(K2 L(K1))))";
      int node[] = { 5,4,3,2,1 };
      for (int i = 0; i < sizeof(node) / sizeof(int); ++i) {
        tb.insert(node[i]);        
      }
      if (tb.sprint() != checkout) {
        std::cout << "ERROR INSERT: \n" << checkout << " !=  \n" << tb.sprint() << std::endl;
      }
      else {
        std::cout << "+Insert " << tb.sprint() << std::endl;
      }

      // поиск всех элементов
      for (int i = 0; i < sizeof(node) / sizeof(int); ++i) {
        if (node[i] != tb.find(node[i])) {
          std::cout << "ERROR FIND: \n" << checkout << " !=  \n" << tb.sprint() << std::endl;
        }
      }

      // удаление
      for (int i = 0; i < sizeof(node) / sizeof(int); ++i) {
        if (node[i] != tb.remove(node[i])) {
          std::cout << "ERROR REMOVE: " << node[i] << std::endl;
        }
        std::cout << "+removed " << node[i] << ": " << tb.sprint() << std::endl;
      }
      if (!tb.isEmpty()) {
        std::cout << "ERROR EMPTY!\n";
      }

    }

    {
      // дерево вправо
      BinTree<int, int> tb;
      std::string checkout = "K1 R(K2 R(K3 R(K4 R(K5))))";
      // дерево влево
      int node[] = { 1,2,3,4,5 };
      for (int i = 0; i < sizeof(node) / sizeof(int); ++i) {
        tb.insert(node[i]);
      }
      if (tb.sprint() != checkout) {
        std::cout << "ERROR: \n" << checkout << " !=  \n" << tb.sprint() << std::endl;
      }
      else {
        std::cout << "+Insert " << tb.sprint() << std::endl;
      }

      // поиск всех элементов
      for (int i = 0; i < sizeof(node) / sizeof(int); ++i) {
        if (node[i] != tb.find(node[i])) {
          std::cout << "ERROR FIND: \n" << checkout << " !=  \n" << tb.sprint() << std::endl;
        }
      }

      // удаление
      for (int i = 0; i < sizeof(node) / sizeof(int); ++i) {
        if (node[i] != tb.remove(node[i])) {
          std::cout << "ERROR REMOVE: " << node[i] << std::endl;
        }
        std::cout << "+removed " << node[i] << ": " << tb.sprint() << std::endl;
      }
      if (!tb.isEmpty()) {
        std::cout << "ERROR EMPTY!\n";
      }

    }

    {
      // дерево разнообразное
      BinTree<int, int> tb;
      std::string checkout = "K1 R(K8 L(K5 L(K4) R(K6 R(K7))) R(K9))";
      // дерево влево
      int node[] = { 1, 8, 5, 6, 7, 4, 9 };
      for (int i = 0; i < sizeof(node) / sizeof(int); ++i) {
        tb.insert(node[i]);
      }
      if (tb.sprint() != checkout) {
        std::cout << "ERROR: test != result \n" << checkout << " !=  \n" << tb.sprint() << std::endl;
      }
      else {
        std::cout << "+Insert " << tb.sprint() << std::endl;
      }

      // поиск всех элементов
      for (int i = 0; i < sizeof(node) / sizeof(int); ++i) {
        if (node[i] != tb.find(node[i])) {
          std::cout << "ERROR FIND: \n" << checkout << " !=  \n" << tb.sprint() << std::endl;
        }
      }

      // удаление
      for (int i = 0; i < sizeof(node) / sizeof(int); ++i) {
        if (node[i] != tb.remove(node[i])) {
          std::cout << "ERROR REMOVE: " << node[i] << std::endl;
        }
        std::cout << "+removed " << node[i] << ": " << tb.sprint() << std::endl;
      }
      if (!tb.isEmpty()) {
        std::cout << "ERROR EMPTY!\n";
      }

    }
  }

  //
  virtual std::string name() override {
    return "Bin Tree";
  }

private:

  // вставка несбалансированная
  virtual void insertNode(TreeLeaf<Key, Value> *localRoot, TreeLeaf<Key, Value> *pNew) {
    TreeLeaf<Key, Value> **ppChild = nullptr;
    if (pNew->key < localRoot->key) {
      // добавление влево
      ppChild = &localRoot->left;
    }
    else {
      // добавление вправо
      ppChild = &localRoot->right;
    }

    if (*ppChild)
      insertNode(*ppChild, pNew);
    else {
      pNew->parent = localRoot;
      *ppChild = pNew;
    }
  }

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
  //
  void printNode(TreeLeaf<Key, Value> *node, std::string &out) {

    if (node) {
      out << "K" << std::to_string(node->key);

      if (node->left) {
        out << " L(";
        printNode(node->left, out);
        out << ")";
      }

      if (node->right) {
        out << " R(";
        printNode(node->right, out);
        out << ")";
      }
    }
    else {
      out << "";
    }
  }

  // проверка дерева на корректность
  const TreeLeaf<Key, Value> *checkNode(const TreeLeaf<Key, Value> *root) const {
    if (!root)
      return nullptr;

    // проверяем чтобы левый потомок был меньше корня
    if (root->left)
    {
      if (root->left->key < root->value)
        return checkNode(root->left);
      else
        return root;
    }

    // проверяем чтобы правый потомок был больше корня
    if (root->right)
    {
      if (root->right->key >= root->value)
        return checkNode(root->right);
      else
        return root;
    }

    return nullptr;
  }

private:
  TreeLeaf<Key,Value> *root = nullptr;
};
