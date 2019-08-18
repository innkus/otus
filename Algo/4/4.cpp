// 4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>

int task_1743(const std::string &dir);
int task_1744(const std::string &dir);
int task_1745(const std::string &dir);
int task_1746(const std::string &dir);

int main(int argc, char *argv[])
{
  SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
  SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода

  if (argc > 2) {
    int numTask = std::stoi(argv[1]);
    switch (numTask) {

    case 1743:
      return task_1743(std::string(argv[2]));

    case 1744:
      return task_1744(std::string(argv[2]));

    case 1745:
      return task_1745(std::string(argv[2]));

    case 1746:
      /// @param infile файл со срокой fen
      task_1746(std::string(argv[2]));
      break;

    }
  }
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
