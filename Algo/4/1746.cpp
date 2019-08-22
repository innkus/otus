#include "pch.h"
#include "game.h"
#include "simpleArray.h"

extern std::string resultFen;

/// @param fen FEN-строка
/// @return строка со сменой счетчика и хода
const char* task_1746(const char *fen) {
  game_s game(fen);
  game.counter();
  resultFen = game.upload();
  return resultFen.c_str();
}


/// @param infile файл со строкой fen
int task_1746(const std::string &dir) {

  for (int i = 0; i < 5; ++i) {

    std::string file(dir);
    file.append("/test.");

    char buf[10];
    sprintf_s(buf, sizeof(buf), "%d", i);

    file.append(buf);

    std::string inputFile(file);
    inputFile.append(".in");

    std::string outputFile(file);
    outputFile.append(".out");

    // сконструировать объект класса ifstream для ввода из файла
    std::ifstream ioInputFile(inputFile.c_str());
    if (!ioInputFile) {
      return -1;
    }

    std::string inputFen;
    getline(ioInputFile, inputFen, '\n');

    std::ifstream ioSampleFile(outputFile.c_str());
    if (!ioSampleFile) {
      return -2;
    }
    std::string outputFen;
    getline(ioSampleFile, outputFen, '\n');

    const char* p = task_1746(inputFen.c_str());
    if (strcmp(p, outputFen.c_str()) == 0)
      std::cout << "+ " << inputFile << std::endl;
    else
      std::cout << "! " << inputFile << " : " << outputFen << std::endl;

  } // end for

  return 0;
}


/// пересчеты в лоб
/// @param fen FEN-строка
/// @return строка со сменой счетчика и хода (строку освободить снаружи!)
const char* task_1746_obsolete(const char *fen) {

  if (!fen)
    return nullptr;

  SingleArray<char *> listStr;

  // строим массив подстрок по строке
  const char *delim = " ";
  int len = strlen(fen) + 1;
  char *fenCpy = new char[len];
  strcpy_s(fenCpy, len, fen);

  char *next_token = NULL;
  char * token = strtok_s(fenCpy, delim, &next_token);
  while (token) {
    listStr.add(token);
    token = strtok_s(NULL, delim, &next_token);
  }

  // должно быть 6 строк
  if (listStr.size() != 6) {
    delete[]fenCpy;
    return nullptr;
  }

  // берем строку 1 считываем чей ход
  char chSide = listStr.get(1)[0];

  // если надо берем строку 5 и считываем счетчик 
  int counter = std::stoi(listStr.get(5));
  if (chSide == 'b') {
    // следующий ход - белых
    // надо увеличить счетчик
    counter++;
    // меняем сторону
    chSide = 'w';
  }
  else {
    // меняем сторону
    chSide = 'b';
  }
  
  char bufCounter[10];
  sprintf_s(bufCounter, sizeof(bufCounter), "%d", counter);

  // формируем новую строку (выделим с запасом, надо бы конечно посчитать, но ладно уж)
  char *pNextFen = new char[2 * len];

  // 0 строка
  strcpy_s(pNextFen, strlen(listStr.get(0))+1, listStr.get(0));
  int pos = strlen(pNextFen);
  // пробел
  pNextFen[pos++] = ' ';
  // 1 строка
  pNextFen[pos++] = chSide;
  // пробел
  pNextFen[pos++] = ' ';
  // 2-3-4
  for (int i = 2; i <= 4; ++i) {
    strcpy_s(pNextFen + pos, strlen(listStr.get(i)) + 1, listStr.get(i));
    pos += strlen(listStr.get(i));
    pNextFen[pos++] = ' ';
  }
  // вставляем счетчик
  strcpy_s(pNextFen + pos, strlen(bufCounter) + 1, bufCounter);
  delete[]fenCpy;
  return pNextFen;
}
