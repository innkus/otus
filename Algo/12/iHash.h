#pragma once
#include <string>

template <typename Key, typename T>
struct IHash {
  // вставить элемент
  virtual bool insert(const Key &key, T t) = 0;
  // найти элемент
  virtual T get(const Key &key) = 0;
  // исключить элемент
  virtual T remove(const Key &key) = 0;
  // распечатать таблицу
  virtual void print() = 0;
  // сбор статистики
  virtual std::string statistic() = 0;
};
