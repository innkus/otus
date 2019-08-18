#include "pch.h"
#include "game.h"
#include "simpleArray.h"

std::string resultFen;

/// @param fen FEN-������
/// @return ������ �� ������ �������� � ����
const char* task_1746(const char *fen) {
  game_s game(fen);
  game.counter();
  resultFen = game.upload();
  return resultFen.c_str();
}


/// @param infile ���� �� ������� fen
int task_1746(const std::string &dir) {

  std::cout << "1746" << std::endl;

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

    // ��������������� ������ ������ ifstream ��� ����� �� �����
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


/// ��������� � ���
/// @param fen FEN-������
/// @return ������ �� ������ �������� � ���� (������ ���������� �������!)
const char* task_1746_obsolete(const char *fen) {

  if (!fen)
    return nullptr;

  SingleArray<char *> listStr;

  // ������ ������ �������� �� ������
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

  // ������ ���� 6 �����
  if (listStr.size() != 6) {
    delete[]fenCpy;
    return nullptr;
  }

  // ����� ������ 1 ��������� ��� ���
  char chSide = listStr.get(1)[0];

  // ���� ���� ����� ������ 5 � ��������� ������� 
  int counter = std::stoi(listStr.get(5));
  if (chSide == 'b') {
    // ��������� ��� - �����
    // ���� ��������� �������
    counter++;
    // ������ �������
    chSide = 'w';
  }
  else {
    // ������ �������
    chSide = 'b';
  }
  
  char bufCounter[10];
  sprintf_s(bufCounter, sizeof(bufCounter), "%d", counter);

  // ��������� ����� ������ (������� � �������, ���� �� ������� ���������, �� ����� ��)
  char *pNextFen = new char[2 * len];

  // 0 ������
  strcpy_s(pNextFen, strlen(listStr.get(0))+1, listStr.get(0));
  int pos = strlen(pNextFen);
  // ������
  pNextFen[pos++] = ' ';
  // 1 ������
  pNextFen[pos++] = chSide;
  // ������
  pNextFen[pos++] = ' ';
  // 2-3-4
  for (int i = 2; i <= 4; ++i) {
    strcpy_s(pNextFen + pos, strlen(listStr.get(i)) + 1, listStr.get(i));
    pos += strlen(listStr.get(i));
    pNextFen[pos++] = ' ';
  }
  // ��������� �������
  strcpy_s(pNextFen + pos, strlen(bufCounter) + 1, bufCounter);
  delete[]fenCpy;
  return pNextFen;
}
