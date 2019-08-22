#include "pch.h"
#include "game.h"

extern std::string resultFen;

// Дана FEN - позиция и ход фигурой.
// Нужно посчитать этот ход, передать право хода другой стороне,
// а также увеличить или сбросить счётчик полуходов для правила 50 ходов.
// Счётчик сбрасывается, если было взятие или ход пешкой.
// В остальных случаях счётчик увеличивается на 1 после каждого полухода.
// ФИГУРЫ ПЕРЕМЕЩАТЬ НЕ НУЖНО

/// @param fen FEN-строка
/// @param move ход
/// @return строка со сменой счетчика и хода
const char* task_3694(const char *fen, const char *move) {
  
  game_s game(fen);

  resultFen.clear();

  if (game.readCurrentMove(move)) {
    game.halfMoveClock();
    game.counter();
  }

  resultFen = game.upload();
  return resultFen.c_str();
}

/// @param infile файл со строкой fen
int task_3694(const std::string &dir) {

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

    std::string inputFen, move;
    getline(ioInputFile, inputFen, '\n');
    getline(ioInputFile, move, '\n');

    std::ifstream ioSampleFile(outputFile.c_str());
    if (!ioSampleFile) {
      return -2;
    }
    std::string outputFen;
    getline(ioSampleFile, outputFen, '\n');

    const char* p = task_3694(inputFen.c_str(), move.c_str());
    if (strcmp(p, outputFen.c_str()) == 0)
      std::cout << "+ " << inputFile << std::endl;
    else {
      std::cout << "! " << inputFile << std::endl;
      std::cout << "test: " << outputFen << std::endl;
      std::cout << "prog: " << p << std::endl;
    }

  } // end for

  return 0;
}

