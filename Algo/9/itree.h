#pragma once

#include <string>

// ����������� ��� ��� ��������� ������
// ������� 1. �������� ���������� ��� - ������
// ������ � ���������� :
// smallLeft / RightRotation - ����� ����� / ������ ��������
// bigLeft / RightRotation - ������� ����� / ������ ��������, �������� ����� ����� ����� ��������
// insert
// remove
// rebalance

//
std::string& operator<< (std::string &out, const std::string &in) {  
  out  = out + in;
  return out;
}

template <typename Key, typename Value>
class TreeLeaf { 
public:
  Key key;     
  Value value;    
  TreeLeaf *parent = nullptr;    
  TreeLeaf *left = nullptr;
  TreeLeaf *right = nullptr;

  // ��� ������������������ /enum balanceType
  int balance = eBalanced;

  // ������ ����
  int height = 1;
};

// ��� ������������������ ����
enum balanceType {
  eBalanced   = 0,    // ��� ��������� ����� ���� �������
  eLeftHeavy  = -1,   // ����� ��������� �������
  eRightHeavy = +1    // ������ ��������� ������
};

// smallLeft / RightRotation - ����� ����� / ������ ��������
// bigLeft / RightRotation - ������� ����� / ������ ��������, �������� ����� ����� ����� ��������
// insert
// remove
// rebalance

template <typename Key, typename Value>
struct IBinTree {
  // ������� ����
  virtual void insert(const Key &key, const Value &value) = 0;
  // �������� ����
  virtual Value remove(const Key &key) = 0;
  // ������
  virtual void print() = 0;
  // ����� �������
  virtual Value find(const Key &key) = 0;
  // ��������� ������ �� ������
  virtual bool isEmpty() = 0;

  // �������� ������ �� ������������
  virtual bool check() = 0;
  // �������� ������ ������
  virtual TreeLeaf<Key, Value> *getRoot() = 0;
  // �������� ������� ����������������
  virtual void testBaseFunctions() = 0;
  // ��� ������
  virtual std::string name() = 0;
};

