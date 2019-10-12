// arrays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include "pch.h"
#define max(a,b) ((a > b) ? (a) : (b)) 

template <typename T>
struct IArray {
  virtual int size() const = 0;
  virtual void add(T item) = 0;
  virtual void add(T item, int index) = 0; // with shift to tail
  virtual T get(int index) const = 0;
  virtual T remove(int index) = 0; // return deleted element
  virtual void clear() = 0;
  virtual const char* name() const = 0;
  virtual void set(T item, int index) = 0; 
  virtual const T *data() const = 0;
};

