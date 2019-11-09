// 25.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;

// Разработка программы, реализующей идею XOR шифрования
// Цель : Результатом является опыт разработки программы для шифрования - дешифрования, чтобы на опыте усвоить базовую функциональность механизма XOR как варианта полиалфавитного шифра.С другой стороны, XOR это основа принципа сцепления блоков шифротекста.Практика работы с этой операцией даст навык использования этой стороны шифрования.
//   Написать программу, в которой реализованы два метода :
// 1. шифрование XOR
// 2. дешифрование XOR
// Программа должна на вход получать открытый текст и ключ.
// На выходе метода шифрования должен получаться шифротекст, который затем используется как входной параметр для метода дешифрования.
// Программа должна формировать в отдельных файлах :
// 1. открытый текст и ключ
// 2. шифротекст
// 
// ВАЖНО!При размещении ответа укажите, на каком языке вы выполнили ДЗ.Это поможет нам ускорить его проверку.
// Критерии оценки : Факт отправки сделанного задания + 1 балл
// Работающий алгоритм шифрования - дешифрования + 1 балл.
// За выполнение задания до начала следующего урока : +1 балл.
// Рекомендуем сдать до : 14.11.2019

// печать заголовка
void printHelp(int argc, char *argv[]) {
  cout
    << "+----------------------------------------------------------------------------+" << std::endl
    << "+                                 XOR operation                              +" << std::endl
    << "+----------------------------------------------------------------------------+" << std::endl
    << std::endl
    ;
  if (argc >= 4)
    return;

  cout << "Запуск: 25.exe key inputFile outputFile" << endl;
  cout << "key        - ключ шифрования/расшифровки" << endl;
  cout << "inputFile  - путь к файлу с открытым текстом/шифротекстом" << endl;
  cout << "outputFile - путь к файлу с шифротекстом/открытым текстом" << endl;
}

// применить xor
int toDoXOR(const string &keyFile, const string &inputFile, const string &outputFile) {

  cout << "Файл с ключом: " << keyFile << endl;
  cout << "Входной файл: "  << inputFile << endl;
  cout << "Выходной файл: " << outputFile << endl;

  // считать файл с ключом
  ifstream keyStream(keyFile);
  if(!keyStream.is_open()) {
    cout << "Ошибка открытия файла с ключом " << keyFile.c_str() << endl;
    return -1;
  }

  stringstream ss;
  ss << keyStream.rdbuf();
  string key = ss.str();
  keyStream.close();

  if (!key.length()) {
    cout << "Не считан ключ из файла " << keyFile.c_str() << endl;
    return -1;
  }
  cout << "Ключ: " << key << endl;

  struct tmpStruct {
    // входной файл
    ifstream _in;
    // выходной поток
    ofstream _out;
    //
    virtual ~tmpStruct() {
      if (_in.is_open())
        _in.close();
      if (_out.is_open())
        _out.close();
    }
  };

  tmpStruct dscr;

  dscr._in.open(inputFile, std::ios::binary | std::ios::in);
  if (!dscr._in.is_open()) {
    cout << "Ошибка открытия на чтение файла " << inputFile.c_str() << endl;
    return -1;
  }
  
  dscr._out.open(outputFile, std::ios::binary | std::ios::out);
  if (!dscr._out.is_open()) {
    cout << "Ошибка открытия на запись файла " << outputFile.c_str() << endl;
    return -1;
  }  

  char readSym;
  int64_t number = 0;
  while (dscr._in.read(&readSym, 1)) {
    char writtenSym = readSym ^ key[number % key.length()];
    number++;
    if (!dscr._out.write(&writtenSym, 1)) {
      cout << "Ошибка записи в файл " << outputFile.c_str() << endl;
      return -1;
    }
  }

  cout << "Обработка XOR: успешно" << endl;
  return 0;
}

int main(int argc, char *argv[])
{
  setlocale(LC_CTYPE, "rus");
  printHelp(argc, argv);
  if (argc < 4) {    
    return 0;
  }

  return toDoXOR(argv[1], argv[2], argv[3]);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
 