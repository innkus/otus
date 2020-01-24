// 21.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <map>
#include <vector>

// Реализовать алгоритм Бойера - Мура - Хорспула
// Цель : 
// 1) Реализовать функцию FindPrefixes, которая создает таблицу префиксов(примеры работы есть в материале) : 
// 2) Реализовать алгоритм Бойера - Мура - Хорспула. 
// Функция FindPrefixes(string) возвращает массив Z такой, 
// что Z[i] содержит число, равное длине подстроки S[i], котороая при этом является и префиксом S.
// Если такого префикса нет, то Z[i] будет 0.
// 
// Есть тест - кейсы: файл preprocess_test_cases.txt.Традиционно, первая строка - количество записей, 
// первый столбец - строка, далее через пробел - записанные последовательно значения из таблицы префиксов для данной строки.
// 
// Для проверки есть тест - кейсы : string_matching_test_cases.tsv.Это.tsv без заголовка, но в первой строчке 
// общее число записей.Первый столбец - текст(содержит пробелы), после первого таба - столбец с паттерном
// (также может содержать пробелы), в последнем столбце через пробел все вхождения паттерна в текст, в том 
// числе, перекрывающиеся частично.Если вхождений нет, столбец пуст.
// 
// (для тех, кто пишет на C) : проверить работу алгоритма Бойера - Мура, реализация на С, из википедии.
// Можно на предложенных тест - кесах, можно на своих.
// 
// Критерии оценки : 1) Реализовать функцию FindPrefixes + 2 балла
// 2) Реализовать алгоритм Бойера - Мура - Хорспула. + 3 балла

void read(const std::string &testFile) {
  std::cout << "test " << testFile << std::endl;

  //std::wifstream wif(testFile);
  //wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
  //std::wstringstream wss;
  //
  //wchar_t wTest[1000];
  //
  //std::string testDir, testPath;
  //
  //wif.getline(wTest, sizeof(wTest));
  //
  //while (!wif.eof()) {
  //  wif.getline(wTest, sizeof(wTest));
  //
  //}
  char buffer[1000];
  std::ifstream infile(testFile, std::ios::in);
  while (!infile.eof()) {
    infile.getline(buffer, sizeof(buffer), ' ');
    std::cout << buffer << " ";
    infile.getline(buffer, sizeof(buffer), '\t');
    std::cout << buffer << " ";
    infile.getline(buffer, sizeof(buffer), '\n');
    std::cout << buffer << std::endl;
  }

}

int * findPrefixes(const std::string &pattern) {
  if (!pattern.size())
    return 0;
  int *result = new int[pattern.size()];
  memset(result, 0, sizeof(int)*pattern.size());

  for (unsigned i = 0; i < pattern.size(); ++i) {
    for (unsigned j = 0; i + j < pattern.size(); ++j) {
      if (pattern.at(i + j) == pattern.at(j))
        result[i]++;
      else
        break;
    }
  }
  return result;
}

/////////////////////////////////////////////////
void test_findPrefixes() {

  struct s_s {
    std::string pattern;
    std::vector<int> sample;
  };

  s_s test[] = {
    {"coconut", { 7,0,2,0,0,0,0 }},
    {"EXAMPLE", { 7,0,0,0,0,0,1 }},
    {"aaaaaaa", { 7,6,5,4,3,2,1 }},
    {"ELMAXE", { 6,0,0,0,0,1 }},
  };

  for (int i = 0; i < sizeof(test) / sizeof(s_s); ++i) {
    int * myResult = findPrefixes(test[i].pattern);
    if (memcmp(myResult, test[i].sample.data(), sizeof(int)*test[i].sample.size()) != 0)
      std::cout << (i+1) << ") " << test[i].pattern << ": failed";
    else
      std::cout << (i + 1) << ") " << test[i].pattern << ": ok";
    std::cout << std::endl;
    delete[]myResult;
  }
}
/////////////////////////////////////////////////
std::map<char,int> formOffsetTable(const std::string &pattern) {
  if (!pattern.size())
    return std::map<char, int>();
  
  std::map<char, int> table;
  int length = pattern.length();

  for (const auto &c : pattern) {
    table.insert(std::make_pair(c, length));
  }

  for (unsigned i = 0; i < pattern.size() - 1; i++) {
    table[pattern.at(i)] = length - i - 1;
  }
  return table;
}

void test_offsetTable() {
  {
    std::string test = "abbad";
    std::map<char, int> m = formOffsetTable(test);
    
    int   result1[] = { 1, 2, 5 };
    char  result2[] = { 'a', 'b', 'd' };
    int res = true;
    if (sizeof(result1) / sizeof(int) != m.size()) {
      std::cout << test << " : offsetTable failed" << std::endl;
      res = false;
    }
    else {
      for (int i = 0; i < sizeof(result1) / sizeof(int); ++i) {
        std::map<char, int>::const_iterator it = m.find(result2[i]);
        if (m.end() == it) {
          std::cout << test << "(" << result2[i] << "): offsetTable failed" << std::endl;
          res = false;
        }
        else if (it->first != result2[i] || it->second != result1[i]) {
          std::cout << test << "(" << result2[i] << "): offsetTable failed" << std::endl;
          res = false;
        }
      }
    }

    if(res)
      std::cout << test << " : offsetTable success" << std::endl;
  }
}

// алгоритм Бойера Мура Хорспула для отдельной строки
std::vector<int> boyer_mur_horspul_for_string(const std::string &text, 
  const std::string &pattern) {
  
  std::vector<int> result;
  std::map<char, int> table = formOffsetTable(pattern);
  
  // бежим по тексту
  int len_t = text.length();
  int len_p = pattern.length();
  int offset = 0;
  while (offset + len_p <= len_t) {
    if (memcmp(text.c_str() + offset, pattern.c_str(), len_p) == 0) {
      result.push_back(offset);
      offset += len_p;
      continue;
    }
    // берем последний символ
    char c = text.at(offset + len_p - 1);
    
    // ищем символ в таблице смещений
    std::map<char, int>::const_iterator it = table.find(c);
    if (it == table.cend())
      offset += len_p;
    else
      offset += it->second;
  }
  return result;
}

// abcdxfhjkasabcd abcd 0,11
// abcmassdfggabcdg abcd 11
// abbdxxxabcdyyabcdyab abcd 7,13

void test_boyer_mur_horspul() {

  struct s_s {
    std::string text;
    std::string pattern;
    std::vector<int> result;
  };

  s_s test[] = {
    {"abcdxfhjkasabcd", "abcd", {0,11}},
    {"abcmassdfggabcdg", "abcd", {11}},
    {"abbdxxxabcdyyabcdyab", "abcd", {7,13}},
    //01234567890123
    {"abcd abcd abcd", "abcd", {0,5,10}},
    //01234567890123
    {"cd abcd abcd", "abcd", {3,8}},
    //01234567890123
    {"cd abcd ab", "abcd", {3}},
    //01234567890123
    {"cd abcd cd", "abcd", {3}},
    //01234567890123
    {" abcd ", "abcd", {1}},
    //01234567890123
    {"ab abcd ab", "abcd", {3}},
  };

  for (int i = 0; i < sizeof(test) / sizeof(s_s); ++i) {
    std::vector<int> out = boyer_mur_horspul_for_string(test[i].text, test[i].pattern);

    if (out.size() != test[i].result.size()) {
      std::cout << (i+1) << ") " <<  test[i].text << " : BMH failed" << std::endl;
    }
    else {
      int num = 0;
      bool bFailed = false;
      for (const auto &pos : out) {
        if (pos != test[i].result[num++]) {
          std::cout << (i + 1) << ") " << test[i].text << " pos=" << pos << " : BMH failed" << std::endl;
          bFailed = true;
        }
      }
      if (!bFailed)
        std::cout << (i + 1) << ") " << test[i].text << " : BMH ok" << std::endl;
    }
  }

}

int main()
{
  test_boyer_mur_horspul();
  test_offsetTable();
  test_findPrefixes();
 // read("D:/Projects/otus/Algo/21/test_cases.tsv");
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
