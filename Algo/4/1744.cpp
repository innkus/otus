#include "pch.h"
#include <string>
#include "simpleArray.h"
#include "game.h"

/// @param infile файл со строкой fen
int task_1744(const std::string &dir) {

  for (int i = 0; i < 10; ++i) {

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

    std::ifstream ioOutputFile(outputFile.c_str());
    if (!ioOutputFile) {
      return -2;
    }
    std::string outputFen[8];
    for (int i = 0; i < 8; i++) {
      getline(ioOutputFile, outputFen[i], '\n');
    }
      
    std::string resultFen;
    board_s boards;
    if (boards.load(inputFen.c_str())) {
      
      bool bError = false;
      for (int i = 0; i < 8; ++i) {
        if (boards.boards[i] != std::stoull(outputFen[i])) {
          bError = true;
          break;
        }
      }
      if (!bError)
        std::cout << "+ " << inputFile << std::endl;
      else
        std::cout << "! " << inputFile << std::endl;
    }
    else
    {
      std::cout << "? " << inputFile << std::endl;
    }

  } // end for

  return 0;
}
