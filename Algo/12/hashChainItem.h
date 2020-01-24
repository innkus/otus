#pragma once
#include "vectorArray.h"
#include "bin-tree.h"

// максимальный размер элементов при котором выполняется замена линейного хранения
// элементов на контейнер с двоичным деревом
#define MAX_COUNT_OF_LIST_ELEMENTS (32)

//class hashChain;

template <typename Key, typename T>
struct pair {
  pair() : key(0), t(T()) {}
  pair(Key _key, T _t) : key(_key), t(_t) {}
  Key key;
  T t;
};

template <typename Key, typename T>
struct hashChainItem {
  
  //
  hashChainItem(int seed = 5) {
    m_pArray = new VectorArray<pair<Key,T>>(seed);
  }
  //
  virtual ~hashChainItem() {
    if(m_pArray)
      delete m_pArray;
    if (m_pTree)
      delete m_pTree;
  }
  //
  void print() {
    if (m_pArray) {
      m_pArray->print();
      return;
    }
    if (m_pTree) {
      m_pTree->print();
      return;
    }
    std::cout << "X\n";
  }
  //
  bool insert(const Key &key, T t) {
    if (m_pArray)
    {
      m_pArray->add(pair<Key,T>(key,t));
      if (m_pArray->size() <= MAX_COUNT_OF_LIST_ELEMENTS)
        return true;

      // переходим на дерево
      createBinTree();
      return true;
    }

    if(m_pTree)
    {
      m_pTree->insert(key, t);
      return true;
    }
    return false;
  }
  //
  bool remove(const Key &key, T &t) {        
    // ищем в массиве
    if (m_pArray) {
      for (int i = 0; i < m_pArray->size(); ++i) {
        pair<Key, T> p = m_pArray->get(i);
        if (p.key == key) {
          m_pArray->remove(i);
          return true;
        }
      }
      return false;
    }
    // ищем в дереве
    if (m_pTree) {
      T t = m_pTree->remove(key);
      return t != T();
    }
    // негде искать печаль
    return false;
  }
  //
  bool get(const Key &key, T &t) {
    // ищем в массиве
    if (m_pArray) {
      for (int i = 0; i < m_pArray->size(); ++i) {
        pair<Key, T> p = m_pArray->get(i);
        if (p.key == key) {
          t = p.t;
          return true;
        }
      }
      return false;
    }
    // ищем в дереве
    if (m_pTree) {
      T t = m_pTree->find(key);
      return t != T();
    }
    // негде искать печаль
    return false;
  }

  // переходим на дерево
  void createBinTree() {
    if (!m_pArray)
      return;

    if (!m_pTree)
      m_pTree = new BinTree<Key, T>();

    for (int i = 0; i < m_pArray->size(); ++i) {
      pair<Key, T> p = m_pArray->get(i);
      m_pTree->insert(p.key, p.t);
    }
    delete m_pArray;
    m_pArray = nullptr;
  }
  
  // пробежать по элементам и добавить их в новую таблицу
  void replaceItems(IGoroundTree<Key,T> *pMain) {

    // элементы из массива
    if (m_pArray && m_pArray->size()) {
      for (int i = 0; i < m_pArray->size(); ++i) {
        pair<Key, T> p = m_pArray->get(i);
        pMain->onNode(p.key, p.t);
      }
      return;
    }    
    // заполнение из бинарного дерева
    if (m_pTree && !m_pTree->isEmpty()) {
      m_pTree->makeGoround(pMain);
    }
  }
  
  // количество элементов в ячейке
  int size() {
    if (m_pArray)
      return m_pArray->size();
    if (m_pTree)
      return m_pTree->size();
    return 0;
  }

  // массив
  VectorArray<pair<Key,T>> *m_pArray = nullptr;
  IBinTree<Key, T> *m_pTree = nullptr;
};

