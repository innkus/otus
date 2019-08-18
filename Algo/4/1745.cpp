#include "pch.h"
#include "simpleArray.h"
#include "game.h"

/// @param infile файл со строкой fen
int task_1745(const std::string &dir) {

  std::cout << "1745" << std::endl;

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
    std::string outputFen;
    getline(ioOutputFile, outputFen, '\n');

    std::string resultFen;
    game_s game;
    if (game.load(inputFen.c_str())) {
      resultFen = game.upload();

      if (resultFen == outputFen)
        std::cout << "+ " << inputFile << std::endl;
      else
        std::cout << "! " << inputFile << "\n " << resultFen << std::endl;
    }
    else
    {
      std::cout << "? " << inputFile << std::endl;
    }

  } // end for

  return 0;
}
