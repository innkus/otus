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

// для построения дерева используем величину баланса
template <typename Key, typename Value>
class AvlTreeBal : public IBinTree<Key, Value> {
public:

  /// https://en.ppt-online.org/13500
  /// http ://www.mkurnosov.net/teaching/uploads/DSA/dsa-fall-lecture3.pdf
  /// @param bGrow признак того, что высота дерева увеличилась
  void insert(TreeLeaf<Key, Value> *&parent, const Key &key, const Value &value, bool &bGrew) {
    
    if (!parent) {
      // это корень
      parent = new TreeLeaf<Key, Value>();
      parent->key = key;
      // в целях отладки значение равно ключу 
      parent->value = key;
      parent->balance = eBalanced;
      bGrew = true;
      return;
    }

    if (key <= parent->key) {

      // идем влево
      insert(parent->left, key, value, bGrew);

      // проверяем изменилась ли длина поддерева
      if (!bGrew)
        return;
      switch (parent->balance) {
        case eBalanced  : // оба поддерева имеют одну глубину 
        { 
          // теперь лево удлиннилось на 1
          parent->balance = eLeftHeavy;
          bGrew = true;
          break;
        }
        case eLeftHeavy : // левое поддерево длиннее
        {
          // левое поддерево и так было длинное а стало еще длинее
          // катастрофа - придется перебалансировать его
          parent = rebalanceLeft(parent);
          // после балансировки уровень уйдет так что длина вернется
          // к тому что было
          bGrew = false;
          break;
        }
        case eRightHeavy: // правое поддерево длинее
        {
          // добавляли в левое поддерево и оно выросло и догнало правое поддерево
          parent->balance = eBalanced;
          bGrew = false;
          break;
        }
      } // end switch

    } // конец вставки в левое поддерево
    else 
    {
      // вставляем в правое поддерево
      insert(parent->right, key, value, bGrew);
      // проверяем изменилась ли длина поддерева
      if (!bGrew)
        return;
      switch (parent->balance) {
        case eBalanced: // оба поддерева имеют одну глубину 
        {
          // теперь право удлинилось на 1
          parent->balance = eRightHeavy;
          bGrew = true;
          break;
        }
        case eRightHeavy: // правое поддерево длиннее
        {
          // правое поддерево и так было длинное а стало еще длинее
          // катастрофа - придется перебалансировать его
          parent = rebalanceRight(parent);
          // после балансировки уровень уйдет так что длина вернется
          // к тому что было
          bGrew = false;
          break;
        }
        case eLeftHeavy: // левое поддерево длинее
        {
          // добавляли в правое поддерево и оно выросло и догнало левое поддерево
          parent->balance = eBalanced;
          bGrew = false;
          break;
        }
      } // end switch

    } // конец вставки в правое поддерево
  }

  // левое вращение возвращается новый корень L
  // правый потомок становится корнем
  TreeLeaf<Key, Value> *leftRotation(TreeLeaf<Key, Value> *parent) {
    // баланс parent нарушен
    TreeLeaf<Key, Value> *child = parent->right;
    parent->right = child->left;
    child->left = parent;    
    parent = child;
    return parent;
  }

  // правое вращение возвращается новый корень R
  // левый потомок становится корнем
  TreeLeaf<Key, Value> *rightRotation(TreeLeaf<Key, Value> *parent) {
    // баланс parent нарушен
    TreeLeaf<Key, Value> *child = parent->left;
    parent->left = child->right;
    child->right = parent;
    parent = child;
    return parent;
  }

  // балансировка правого поддерева (лево или право-лево) RL
  // возвращается новый parent
  TreeLeaf<Key, Value> *rebalanceRight(TreeLeaf<Key, Value> *parent) {

    TreeLeaf<Key, Value> *child = parent->right;
    TreeLeaf<Key, Value> *X = parent;
    TreeLeaf<Key, Value> *B = parent->right;
    TreeLeaf<Key, Value> *D = B->left;

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

    if (B->balance == eRightHeavy) {      
      // правое поддерево длиннее - просто повернем налево
      parent = leftRotation(parent);      
      B->balance = eBalanced;
      X->balance = eBalanced;
      return parent;
    } 

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

    // в левом поддереве B неполадки
    // повернем вправо относительно 
    parent->right = rightRotation(parent->right);
    parent = leftRotation(parent);

    // выставим баланс
    if (D->balance == eRightHeavy) {
      // случай 1 (t3 удлинили)
      B->balance = eBalanced;
      X->balance = eLeftHeavy;
    }
    if (D->balance == eLeftHeavy) {
      // случай 2 (t2 удлинили)
      B->balance = eRightHeavy;
      X->balance = eBalanced;
    }
    if (D->balance == eBalanced) {
      B->balance = eBalanced;
      X->balance = eBalanced;
    }
    parent->balance = eBalanced;
    return parent;
  }

  // балансировка левого поддерева (право или лево-право) LR
  // возвращается новый parent
  TreeLeaf<Key, Value> *rebalanceLeft(TreeLeaf<Key, Value> *parent) {

    TreeLeaf<Key, Value> *X = parent;
    TreeLeaf<Key, Value> *B = parent->left;
    TreeLeaf<Key, Value> *D = B->right;

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

    if (B->balance == eLeftHeavy) {
      // левое поддерево длиннее - просто повернем направо
      parent = rightRotation(parent);      
      B->balance = eBalanced;
      X->balance = eBalanced;
      return parent;
    }

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

    // в правом поддереве B неполадки
    // повернем влево относительно 
    parent->left = leftRotation(parent->left);
    parent = rightRotation(parent);

    // выставим баланс
    if (D->balance == eLeftHeavy) {
      // случай 2 (t2 удлинили)
      B->balance = eBalanced;
      X->balance = eRightHeavy;
    }
    if (D->balance == eRightHeavy) {
      // случай 1 (t3 удлинили)
      B->balance = eLeftHeavy;
      X->balance = eBalanced;
    }
    if (D->balance == eBalanced) {
      //
      B->balance = eBalanced;
      X->balance = eBalanced;
    }
    parent->balance = eBalanced;
    return parent;
  }


  // проверка дерева на корректность
  virtual bool check()  override {
    const TreeLeaf<Key, Value> *errorNode = checkNode(root);
    return !errorNode;
  }
  
  // вставка несбалансированная
  virtual void insert(const Key &key, const Value &value = 0) override {
    /// @param bGrow признак того, что высота дерева увеличилась
    bool bGrew = false;
    insert(root, key, value, bGrew);
  }
  
  // сбалансировать дерево
  virtual TreeLeaf<Key, Value> *rebalance(TreeLeaf<Key, Value> *node) override {
    return nullptr;
  }

  //
  virtual void print() override {
    printLevel();
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
    // TODO
    return Value();
  }

  // получить корень дерева
  virtual TreeLeaf<Key, Value> *getRoot() override {
    return root;
  }

  //
  virtual std::string name() override {
    return "Tree with balance";
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
    std::cout << node->key << "(" << node->balance << ")\n";
    printLevel(node->left, level + 1, "L ");
    printLevel(node->right, level + 1, "R ");
  }

  // проверка дерева на корректность
  const TreeLeaf<Key, Value> *checkNode(const TreeLeaf<Key, Value> *node) const {
    if (!node)
      return nullptr;

    // баланс должен быть 1 -1 0
    if (labs(node->balance) > 1) {
      std::cout << "check balance error: key=" << node->key << " balance=" << node->balance << std::endl;
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

private:
  TreeLeaf<Key,Value> *root = nullptr;
  int maxBalance = 2;

public:
  // тест обычной вставки + тест поиска
  void testBaseFunctions() {

    {
      AvlTreeBal<int, int> tb;
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
      AvlTreeBal<int, int> tb;
      tb.insert(30); tb.check(); tb.printLevel();
      tb.insert(15); tb.check(); tb.printLevel();
      tb.insert(40); tb.check(); tb.printLevel();
      tb.insert(10); tb.check(); tb.printLevel();
      tb.insert(20); tb.check(); tb.printLevel();
      tb.insert(5); tb.check(); tb.printLevel();
    }

    {
      AvlTreeBal<int, int> tb;
      tb.insert(5); tb.check(); tb.printLevel();
      tb.insert(4); tb.check(); tb.printLevel();
      tb.insert(3); tb.check(); tb.printLevel();
      tb.insert(2); tb.check(); tb.printLevel();
      tb.insert(1); tb.check(); tb.printLevel();
    }
    { 
      AvlTreeBal<int, int> tb;
      tb.insert(5); tb.check(); tb.printLevel();      
      tb.insert(7); tb.check(); tb.printLevel();
      tb.insert(6); tb.check(); tb.printLevel();
    }

    { //RL
      AvlTreeBal<int, int> tb;
      tb.insert(10); tb.check(); tb.printLevel();
      tb.insert(9);  tb.check(); tb.printLevel();
      tb.insert(17); tb.check(); tb.printLevel();
      tb.insert(20); tb.check(); tb.printLevel();
      tb.insert(12); tb.check(); tb.printLevel();
      tb.insert(14); tb.check(); tb.printLevel();
    }

    { // R
      AvlTreeBal<int, int> tb;
      tb.insert(10); tb.check(); tb.printLevel();
      tb.insert(8); tb.check(); tb.printLevel();
      tb.insert(6); tb.check(); tb.printLevel();
    }

    { // L
      AvlTreeBal<int, int> tb;
      tb.insert(1); tb.check(); tb.printLevel();
      tb.insert(2); tb.check(); tb.printLevel();
      tb.insert(3); tb.check(); tb.printLevel();
    }

    { // 
      AvlTreeBal<int, int> tb;
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
