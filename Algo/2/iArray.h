// arrays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include "pch.h"
#define max(a,b) ((a > b) ? (a) : (b)) 

using namespace std::chrono;

class timeMesuare {
public:
  void start() {
    elapsed_milliseconds = 0;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    startTime = std::chrono::system_clock::now();
  }
  void stop() {
    endTime = std::chrono::system_clock::now();
    elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
      (endTime - startTime).count();
  }
  void print() {
    std::cout << elapsed_milliseconds;
  }
  __int64 data() {
    return elapsed_milliseconds;
  }
private:
  std::chrono::time_point<std::chrono::system_clock> startTime;
  std::chrono::time_point<std::chrono::system_clock> endTime;
  __int64 elapsed_milliseconds;
};

template <typename T>
struct IArray {
  virtual int size() const = 0;
  virtual void add(T item) = 0;
  virtual void add(T item, int index) = 0; // with shift to tail
  virtual T get(int index) const = 0;
  virtual T remove(int index) = 0; // return deleted element
  virtual void clear() = 0;
  virtual void print() = 0;
  virtual const char* name() const = 0;

  /// тестирование вставка в начало
  /// @param count 
  void test(int64_t count) {
    timeMesuare tm;

    //count;intoBegin;fromBegin;intoMiddle;fromMiddle;intoEnd;fromEnd
    std::cout << count << ";";

    tm.start();
    for (int i = 0; i < count; ++i) {
      add(i,0);
    }
    tm.stop();
    std::cout << tm.data() << ";";

    tm.start();
    for (int i = 0; i < count; ++i) {
      remove(0);
    }
    tm.stop();
    std::cout << tm.data() << ";";

    tm.start();
    for (int i = 0; i < count; ++i) {
      add(i, size()/2);
    }
    tm.stop();
    std::cout << tm.data() << ";";

    tm.start();
    while (size()) {
      remove(size() / 2);
    }
    tm.stop();
    std::cout << tm.data() << ";";

    tm.start();
    for (int i = 0; i < count; ++i) {
      add(i);
    }
    tm.stop();
    std::cout << tm.data() << ";";

    tm.start();
    while (size()) {
      remove(size()-1);
    }
    tm.stop();
    std::cout << tm.data() << ";";
  }

  /// тестирование вставка в начало
  /// @param count 
  int64_t testAdd(int64_t count) {

    clear();

    __int64 elapsed_milliseconds;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    for (int i = 0; i < count; ++i) {
      add(i);
    }

    end = std::chrono::system_clock::now();
    elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
      (end - start).count();
    std::cout << name() << "(" << size() << ") :" << " N=" << count << " Tms=" << elapsed_milliseconds << std::endl;
    return elapsed_milliseconds;
  }

  void testBaseFunctions() {
    std::cout << name() << std::endl;
    {
      clear();
      // вставка в конец
      int in[] = { 0,1,2,3,4,5,6,7 };
      int out[] = { 0,1,2,3,4,5,6,7 };
      for (int i = 0; i < sizeof(in) / sizeof(int); ++i) {
        add(in[i]);
        if (i+1 != size())
          std::cout << "SIZE!!! insert to end\n";
        //print();
      }
      for (int i = 0; i < size(); ++i) {
        if (get(i) != out[i]) {
          std::cout << "ERROR!!! insert to end\n";
        }
      }

      // удаление из начала
      int i = 0;
      int ssss = sizeof(out) / sizeof(int);
      while (size()) {
        if (i != remove(0))
          std::cout << "ERROR!!! remove from start\n";
        i++;
        //print();
        if (ssss - 1 != size())
          std::cout << "SIZE!!! remove from start\n";
        ssss--;
      }
    }

    {
      clear();
      // вставка в начало
      int in[] = { 0,1,2,3,4,5,6,7 };
      int out[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
      for (int i = 0; i < sizeof(in) / sizeof(int); ++i) {
        add(in[i], 0);
        if (i + 1 != size())
          std::cout << "SIZE!!! insert to begin\n";
        //print();
      }
      for (int i = 0; i < size(); ++i) {
        if (get(i) != out[i]) {
          std::cout << "ERROR!!! insert to begin\n";
        }
      }

      // удаление с конца
      int i = 0;
      int ssss = sizeof(out) / sizeof(int);
      while (size()) {
        if (i != remove(size() - 1))
          std::cout << "ERROR!!! remove from end\n";
        i++;
        //print();
        if (ssss - 1 != size())
          std::cout << "SIZE!!! remove from end\n";
        ssss--;
      }
    }

    {
      clear();
      // вставка в середину
      int in[] = { 30,40,50,60,70 };
      int out[] = { 0,1,2,30,40,50,60,70,3,4 };

      add(0); 
      //print();
      add(1);  
      //print();
      add(2); 
      //print();
      add(3); 
      //print();
      add(4);
      //print();
      for (int i = 0; i < sizeof(in) / sizeof(int); ++i) {
        add(in[i], 3 + i);
        if (i + 5 + 1 != size())
          std::cout << "SIZE!!! insert to midlle\n";
        //print();
      }
      for (int i = 0; i < size(); ++i) {
        if (get(i) != out[i]) {
          std::cout << "ERROR!!! insert to midlle\n";
        }
      }

      // удаление из середины                   
      int out1[] = { 50,40,60,30,70,2,3,1,4,0 };
      int i = 0;
      int ssss = sizeof(out1) / sizeof(int);
      while (size()) {
        int index = size() / 2;
        if (out1[i] != remove(index))
          std::cout << "ERROR!!! remove from midlle\n";
        i++;
        //print();
        if (ssss - 1 != size())
          std::cout << "SIZE!!! remove from midlle\n";
        ssss--;
      }

    }

    {
       clear();
      // вставка далеко вперед
      int in[] = { 30,40 };
      int out[] = { 0,1,2,3,4, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40 };

      add(0); add(1);  add(2); add(3); add(4);
      //print();
       for (int i = 0; i < sizeof(in) / sizeof(int); ++i) {
        add(in[i], (i + 1) * 10);
        //print();
      }
      for (int i = 0; i < size(); ++i) {
        if (get(i) != out[i]) {
          std::cout << "ERROR!!! insert to forward\n";
        }
      }

      // удаление
      if (0 != remove(-100))
        std::cout << "ERROR!!! -100\n";
      //print();
      for (int i = 0; i < sizeof(out) / sizeof(int); ++i) {
        if (get(i) != out[i]) {
          std::cout << "ERROR!!! -100\n";
        }
      }

      if (0 != remove(100))
        std::cout << "ERROR!!! 100\n";
      //print();
      for (int i = 0; i < sizeof(out) / sizeof(int); ++i) {
        if (get(i) != out[i]) {
          std::cout << "ERROR!!! 100\n";
        }
      }

      remove(0);
      //print();
      int out1[] = { 1,2, 3,4, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40 };
      for (int i = 0; i < sizeof(out1) / sizeof(int); ++i) {
        if (get(i) != out1[i]) {
          std::cout << "ERROR!!! remove(0)\n";
        }
      }

      remove(size() - 1);
      int out2[] = { 1,2,3,4, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      for (int i = 0; i < sizeof(out2) / sizeof(int); ++i) {
        if (get(i) != out2[i]) {
          std::cout << "ERROR!!! remove(size()-1)\n";
        }
      }

      remove(3);
      add(-30, 2);
      //print();
      int out3[] = { 1,2,-30, 3, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      for (int i = 0; i < sizeof(out3) / sizeof(int); ++i) {
        if (get(i) != out3[i]) {
          std::cout << "ERROR!!! remove(3) add(-30, 2)\n";
        }
      }

    }
    
  }

};

