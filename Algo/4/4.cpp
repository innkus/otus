// 4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <sstream>
#include <fstream>
#include <codecvt>

std::string resultFen;

/// @param dir каталог с тестами, например D:\Projects\otus\Algo\4\2019-08-12_Chess-Tasks\1743.1.FEN - ASCII
int task_1743(const std::string &dir);
/// @param dir каталог с тестами
int task_1744(const std::string &dir);
/// @param dir каталог с тестами
int task_1745(const std::string &dir);
/// @param dir каталог с тестами
int task_1746(const std::string &dir);
/// @param dir каталог с тестами
int task_3694(const std::string &dir);
/// @param dir каталог с тестами
int task_3711(const std::string &dir);
/// @param dir каталог с тестами
int task_3713(const std::string &dir);
/// @param dir каталог с тестами
int task_3714(const std::string &dir);

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring &wstr) {
  if (wstr.empty()) return std::string();
  int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
  std::string strTo(size_needed, 0);
  WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
  return strTo;
}

int test(const std::string &testFile) {

  std::wifstream wif(testFile);
  wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
  std::wstringstream wss;
  wchar_t wTestPath[256], wTestDir[256];

  std::string testDir, testPath;
  wif.getline(wTestDir, sizeof(wTestDir));
  testDir = utf8_encode(wTestDir);

  while (!wif.eof()) {
    wif.getline(wTestPath, sizeof(wTestPath));
    if (!wcslen(wTestPath))
      break;
    testPath = utf8_encode(wTestPath) ;
    int testNum = std::stoi(testPath);
    std::cout << "-------------------------------------------------------------------" << std::endl;
    std::cout << testNum << " " << testPath << std::endl;
    
    std::string oneTestDir(testDir);
    oneTestDir += "/";
    oneTestDir += testPath;
    
    switch (testNum) {

    case 1743:
      task_1743(oneTestDir); break;

    case 1744:
      task_1744(oneTestDir); break;

    case 1745:
      task_1745(oneTestDir); break;

    case 1746:
      task_1746(oneTestDir); break;

    case 3694:
      task_3694(oneTestDir); break;

    case 3711:
      task_3711(oneTestDir); break;

    case 3713:
      task_3713(oneTestDir); break;

    case 3714:
      task_3714(oneTestDir); break;
    }

  }
 
  return -1;
}


int main(int argc, char *argv[])
{
  SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
  SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода

  if (argc < 2) {
    std::cout << "run: 4.exe <file with test description>" << std::endl;
    return 0;
  } 
  const std::string testFile(argv[1]);
  std::cout << "File with test description: " << testFile << std::endl;

  std::wifstream wif(testFile);
  wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
  std::wstringstream wss;
  wchar_t wTestPath[256], wTestDir[256];

  std::string testDir, testPath;
  wif.getline(wTestDir, sizeof(wTestDir));
  testDir = utf8_encode(wTestDir);

  while (!wif.eof()) {
    wif.getline(wTestPath, sizeof(wTestPath));
    if (!wcslen(wTestPath))
      break;
    testPath = utf8_encode(wTestPath);
    int testNum = std::stoi(testPath);
    std::cout << "-------------------------------------------------------------------" << std::endl;
    std::cout << testNum << " " << testPath << std::endl;

    std::string oneTestDir(testDir);
    oneTestDir += "/";
    oneTestDir += testPath;

    switch (testNum) {

    case 1743:
      task_1743(oneTestDir); break;

    case 1744:
      task_1744(oneTestDir); break;

    case 1745:
      task_1745(oneTestDir); break;

    case 1746:
      task_1746(oneTestDir); break;

    case 3694:
      task_3694(oneTestDir); break;

    case 3711:
      task_3711(oneTestDir); break;

    case 3713:
      task_3713(oneTestDir); break;

    case 3714:
      task_3714(oneTestDir); break;
    }

  }

  return -1;
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
