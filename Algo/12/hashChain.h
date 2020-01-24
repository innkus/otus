#pragma once
#include "iHash.h"
#include "hashChainItem.h"

template <typename Key, typename T>
class hashChain 
  : public IHash<Key,T>
  , public IGoroundTree<Key,T>
{
public:
  //
  hashChain(int initialCapacity, float loadFactor, int seed) {    
    m_iBaseInitialCapacity = initialCapacity;
    m_iInitialCapacity = m_iBaseInitialCapacity <= 0 ? 1 : m_iBaseInitialCapacity;
    m_fLoadFactor = fabs(loadFactor);
    m_pTable = new hashChainItem<Key,T>[m_iInitialCapacity];
    m_iBusyCapacity = 0;
  }
  //
  virtual ~hashChain() {
    delete []m_pTable;
  }

  // IHash
  // вставить элемент
  virtual bool insert(const Key &key, T t) override {
    // проверить на возможность перестройки таблицы
    tryRehashTable();
    return __insert(key, t);
  }
  // найти элемент
  virtual T get(const Key &key) override {
    // считаем ключ
    int hashKey = hashFunction(key);
    if (!isValidHashKey(hashKey))
      return T();
    else {
      T t;
      if (m_pTable[hashKey].get(key, t))
        return t;
      else
        return T();
    }
  }
  // исключить элемент
  virtual T remove(const Key &key) override {
    // считаем ключ
    int hashKey = hashFunction(key);
    if (!isValidHashKey(hashKey))
      return false;
    
    T t;
    if (m_pTable[hashKey].remove(key, t)) {
      m_iBusyCapacity--;
      return t;
    }
    else
      return T();
  }
  // распечатать таблицу
  virtual void print() override {
    std::cout << "Hash-table: capacity=" << m_iInitialCapacity << " loadFactor=" << m_fLoadFactor
      << " threshold=" << threshold() << " busyCapacity=" << m_iBusyCapacity << std::endl;
      for (int i = 0; i < m_iInitialCapacity; ++i) {
        std::cout << i + 1 << ") ";
        m_pTable[i].print();
      }
  }
  // сбор статистики
  virtual std::string statistic() override {
    // считаем среднее по количеству элементов в каждой ячейке
    int m = 0;
    double mean = m_iBusyCapacity / double(m_iInitialCapacity);
    double deviation = 0;
    for (int i = 0; i < m_iInitialCapacity; ++i) {
      double dD = (mean - m_pTable[i].size());
      deviation += dD * dD;
    }
    // потом считаем среднеквадратичное отклонение
    deviation = sqrt(deviation / (m_iInitialCapacity - 1));
    
    std::string result;
    result       
      << std::to_string(m_fLoadFactor) << ";"
      << std::to_string(m_iBaseInitialCapacity) << ";"
      << std::to_string(m_iInitialCapacity) << ";"
      << std::to_string(m_iBusyCapacity) << ";"
      << std::to_string(mean) << ";" 
      << std::to_string(deviation);
    return result;
  }

protected:
  // порог
  int threshold() {
    return int(m_iInitialCapacity*m_fLoadFactor);
  }
  // вставить элемент
  bool __insert(const Key &key, T t){
    int hashKey = hashFunction(key);
    if (!isValidHashKey(hashKey))
      return false;

    // вставка
    m_pTable[hashKey].insert(key, t);
    m_iBusyCapacity++;
    return true;
  }
  //
  bool isValidHashKey(int hashKey) const {
    return (hashKey >= 0) && (hashKey < m_iInitialCapacity);
  }
  // перестроить таблицу
  // возвращается false если при перестройке произошла ошибка добавления
  bool tryRehashTable() {
    // проверить предел заполняемости таблицы
    if (m_iBusyCapacity < threshold())
      return true;
    // сохраняем старое
    hashChainItem<Key,T> *oldTable = m_pTable;
    int oldInitialCapacity = m_iInitialCapacity;
      
    // ставим новые значения
    m_iInitialCapacity = (m_iInitialCapacity * 2) + 1;
    m_iBusyCapacity = 0;
    m_pTable = new hashChainItem<Key,T>[m_iInitialCapacity];
    for (int i = 0; i < oldInitialCapacity; ++i) {
      oldTable[i].replaceItems(this);
    }
    delete []oldTable;
    return true;
  }
  //
  int hashFunction(Key key) {
    return key % (m_iInitialCapacity-1) + 1;
  }
  ////////////////////////////////////////////////////////////
  // интерфейс для заполнения новой таблицы значениями из старой таблицы
  virtual void onNode(const Key &key, const T &t) override {
    __insert(key, t);
  }
  //
  virtual void onStartNode() override {}
  //
  virtual void onStopNode() override {}
  ////////////////////////////////////////////////////////////

private:
  // таблица 
  hashChainItem<Key,T> *m_pTable = nullptr;
  // коэффициент заполняемости
  float m_fLoadFactor;
  // емкость текущая
  int m_iInitialCapacity;
  // емкость начальная
  int m_iBaseInitialCapacity;      
  // количество занятых элементов
  int m_iBusyCapacity;
};



/*
Хеш - функция:

814 : private int hash(Object key)
815 : {
  816:     // Note: Inline Math.abs here, for less method overhead, and to avoid
  817 :     // a bootstrap dependency, since Math relies on native methods.
    818 : int hash = key.hashCode() % buckets.length;
  819:     return hash < 0 ? -hash : hash;
  820:   }
Пример.hashCode() для String :

1065 : public int hashCode()
1066 : {
  1067:     if (cachedHashCode != 0)
    1068 : return cachedHashCode;
  1069:
  1070 :     // Compute the hash code using a local variable to be reentrant.
    1071 : int hashCode = 0;
  1072:     int limit = count + offset;
  1073:     for (int i = offset; i < limit; i++)
    1074 : hashCode = hashCode * 31 + value[i];
  1075:     return cachedHashCode = hashCode;
  1076:   }
Поиск в хеш - таблице по ключу :

390 :   *
391:    Return the value in this Hashtable associated with the supplied key,
392:    or null if the key maps to nothing.
393:    
394:    @param key the key for which to fetch an associated value
395:    @return what the key maps to, if present
396:    @throws NullPointerException if key is null
397:    @see #put(Object, Object)
398:    @see #containsKey(Object)
399:    
400:   public synchronized V get(Object key)
401 : {
  402:     int idx = hash(key);
  403:     HashEntry e = buckets[idx];
  404:     while (e != null)
    405 : {
    406:         if (e.key.equals(key))
      407 : return e.value;
    408:         e = e.next;
    409:       }
  410 : return null;
  411:   }
Ресайзинг и перезапись хешей :

874 :   
875:    Increases the size of the Hashtable and rehashes all keys to new array
876:    indices; this is called when the addition of a new value would cause
877:    size() > threshold. Note that the existing Entry objects are reused in
878:    the new hash table.
879:    

880:    
881:    This is not specified, but the new size is twice the current size plus
882:    one; this number is not always prime, unfortunately. This implementation
883:    is not synchronized, as it is only invoked from synchronized methods.
884:    
885:   protected void rehash()
886 : {
  887:     HashEntry[] oldBuckets = buckets;
  888:
  889 : int newcapacity = (buckets.length * 2) + 1;
  890:     threshold = (int)(newcapacity * loadFactor);
  891:     buckets = (HashEntry[]) new HashEntry[newcapacity];
  892:
  893 : for (int i = oldBuckets.length - 1; i >= 0; i--)
    894 : {
    895:         HashEntry e = oldBuckets[i];
    896:         while (e != null)
      897 : {
      898:             int idx = hash(e.key);
      899:             HashEntry dest = buckets[idx];
      900:
      901 : if (dest != null)
        902 : {
        903:                 HashEntry next = dest.next;
        904:                 while (next != null)
          905 : {
          906:                     dest = next;
          907:                     next = dest.next;
          908:                   }
        909 : dest.next = e;
        910:               }
      911:             else
        912:               {
        913:                 buckets[idx] = e;
        914:               }
      915 :
        916 : HashEntry next = e.next;
      917:             e.next = null;
      918:             e = next;
      919:           }
    920:       }
  921:   }

*/