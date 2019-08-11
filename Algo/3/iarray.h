#pragma once

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

using namespace std::chrono;

template <typename T>
struct IArray {
  virtual int size() const = 0;
  virtual void add(T item) = 0;
  virtual T get(int index) const = 0;
  virtual void set(T item, int index) = 0;
  virtual T remove(int index) = 0; // return deleted element
  virtual void clear() = 0;
  virtual void print() = 0;
  virtual const char* name() const = 0;

  /// тестирование вставка в начало
  /// @param count 
  int64_t testAdd(int64_t count) {

    clear();

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    for (int i = 0; i < count; ++i) {
      add(i);
    }

    end = std::chrono::system_clock::now();

    __int64 elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
      (end - start).count();

    int64_t tms = elapsed_milliseconds;
    std::cout << name() << "(" << size() << ") :" << " N=" << count << " Tms=" << tms << std::endl;
    return tms;
  }
};

struct IBitArray {
  virtual int size() const = 0;
  virtual void add(bool item) = 0;
  virtual bool get(int index) const = 0;
  virtual void set(bool item, int index) = 0;
  virtual bool remove(int index) = 0; // return deleted element
  virtual void clear() = 0;
  virtual void print() = 0;
  virtual const char* name() const = 0;

  /// тестирование вставка в начало
  /// @param count 
  int64_t testAdd(int64_t count) {

    clear();

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    for (int i = 0; i < count; ++i) {
      add(i);
    }

    end = std::chrono::system_clock::now();

    __int64 elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
      (end - start).count();

    int64_t tms = elapsed_milliseconds;
    std::cout << name() << "(" << size() << ") :" << " N=" << count << " Tms=" << tms << std::endl;
    return tms;
  }
};
