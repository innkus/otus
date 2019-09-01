// arrays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "iArray.h"

/************************************************************/
template <typename T>
class MatrixArray : public IArray<T> {

protected:
  SingleArray<VectorArray<T>*> *array;
  int length;
  int seed;
  char arrayName[20];
  
public:
  MatrixArray(int _seed = 5) : seed(_seed) {    
    array = new SingleArray<VectorArray<T>*>;
    array->add(new VectorArray<T>(seed));
    length = 0;
    const char *tmp = "MatrixArray";
    strcpy_s(arrayName, strlen(tmp) + 1, tmp);
    //print();
  }

  int size() const override {
    return length;
  }

  void add(T item) override {
    if(!array->size())
      array->add(new VectorArray<T>(seed));

    int maxLen = seed * array->size();
    if (length == maxLen) {
      array->add(new VectorArray<T>(seed));
    }
    // просто добавл¤ем в конец
    array->get(array->size() - 1)->add(item);
    length++;
  }

  // with shift to tail
  // не рационально и по времени немного тер¤ем, зато более очевидно
  void add(T item, int index) override {

    if (!array->size())
      array->add(new VectorArray<T>(seed));

    if (index > length) {
      // вставка вперед
      for (int i = length; i < index; ++i)
        add(T());
      add(item);
      return;
    }

    int maxLen = seed * array->size();
    if (length == maxLen) {
      array->add(new VectorArray<T>(seed));
    }

    // определить в какой строке 
    int row = index / seed;
    int col = index % seed;

    // вставка в середину
    // удал¤ем с конца строки элемента
    T deletedItem = array->get(row)->remove(seed - 1);
    // добавл¤ем то что нам надо куда надо
    array->get(row)->add(item, col);

    T deletedNextItem;

    for (int r = row + 1; r < array->size(); r++) {
      deletedNextItem = array->get(r)->remove(seed - 1);
      array->get(r)->add(deletedItem, 0);
      deletedItem = deletedNextItem;
    }
    length++;


    return;
  }


  T get(int index) const override {
    if (index >= size())
      return T();
    else
      return array->get(index / seed)->get(index % seed);
  }

  // return deleted element
  T remove(int index) override {

    if (index >= size() || index < 0)
      return T();

    // определить в какой строке идет удаление
    int row = index / seed;
    int col = index % seed;

    // исключаем сам элемент
    T deletedItem = array->get(row)->remove(col);

    //теперь в этой строке не хватает одного элемента
    //из каждой следующей строки берем первый элемент и перекидываем его
    //в последнюю позицию предыдущей строки
    for (int r = row + 1; r < array->size(); ++r) {
      //вз¤ть первый элемент
      T item = array->get(r)->remove(0);
      //перекинуть вверх в конец
      array->get(r-1)->add(item, seed - 1);
    }
    length--;

    // провер¤ем может быть последн¤¤ строка совсем пуста¤ (если в ней болталс¤
    // перед удалением один элемент), тогда удал¤ем строку
    if (array->get(array->size() - 1)->size() == 0) {
      array->remove(array->size() - 1);
    }

    return deletedItem;
  }

  void print() override {
    std::cout << arrayName << " len=" << size() << " / seed=" << seed << " / vCount=" << array->size() << " =>> " << std::endl;
    for (int i = 0; i < array->size(); ++i) {
      array->get(i)->print();
    }
    std::cout << std::endl;
  }

  const char* name() const override {
    return arrayName;
  }

  void clear() override {
    while (array->size()) {
      array->remove(0);
    }
    length = 0;
  }
};
/************************************************************/
