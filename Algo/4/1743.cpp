#include "pch.h"

//   +-----------------+
// 8 | r n b q k b n r |
// 7 | p p p p p p p p |
// 6 | . . . . . . . . |
// 5 | . . . . . . . . |
// 4 | . . . . . . . . |
// 3 | . . . . . . . . |
// 2 | P P P P P P P P |
// 1 | R N B Q K B N R |
//   +-----------------+
//     a b c d e f g h
// rnbqkbnr / pppppppp / 8 / 8 / 8 / 8 / PPPPPPPP / RNBQKBNR w KQkq - 0 1

/// @param fen FEN-строка
/// @outfile поток выходного файла
void task_1743(const const std::string &fen, std::string &outFen) {

  std::ostringstream outStream;
  char delimiter = '/';
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(fen.c_str());
  
  outStream << "  +-----------------+" << std::endl;

  int line = 8;
  while (std::getline(tokenStream, token, delimiter))
  {
    outStream << line-- << " | ";

    for (int i = 0; i < token.size(); ++i) {
      if (token.c_str()[i] == ' ')
        break;
      if (token.c_str()[i] >= '1' && token.c_str()[i] <= '8') {
        int num_blank = token.c_str()[i] - '0';
        for (int k = 0; k < num_blank; ++k) {
          outStream << ". ";
        }
      }
      else {
        outStream << token.c_str()[i] << ' ';
      }
    }
    outStream << "|\n";
    outFen = outStream.str();
  }

  outStream << "  +-----------------+" << std::endl;
  outStream << "    a b c d e f g h  " << std::endl << std::endl;
  outFen = outStream.str();
}


/// @param infile файл со срокой fen
int task_1743(const std::string &dir) {

  std::cout << "1743" << std::endl;

  for (int i = 0; i < 10; ++i) {
    std::cout << "----------------------------------------------------------\n";
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
    std::string outputFen, str;
    while (std::getline(ioOutputFile, str)) {
      outputFen += str;
      outputFen += "\n";
    }

    // вызываем функцию
    std::string resultFen;
    task_1743(inputFen, resultFen);

    std::cout << inputFile << std::endl;
    std::cout << outputFile << std::endl;
    std::cout << inputFen << std::endl;
    std::cout << "Sample: " << std::endl;
    std::cout << outputFen << std::endl;
    std::cout << "Result: " << std::endl;
    std::cout << resultFen << std::endl;

    if (outputFen == resultFen) {
      std::cout << "+ result==sample" << std::endl << std::endl;
    }
    else {
      std::cout << "! result!=sample" << std::endl << std::endl;
    }

  } // end for

  return 0;
}
