// 24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

// Написать функцию сжатия массива по алгоритму RLE
// Написать программу для сжатия файлов
// Написать программу для распаковки файлов
// Реализовать улучшенный алгоритм RLE: сжатие и распаковку
// Сравнить работу программы с разными типами файлов: текст, фото, аудио, zip-архив
// Составить отчет сравнения разультата работы двух версий алгоритма с разными файлами
// При запуске программы без праметров она должна выводить краткую инструкцию,
// как ее запускать для сжатия/распаковки файлов

// -1 не хватает параметров запуска данных для выполнения сжатия
// -2 нет доступа к файлу
// -3 ошибка открытия файла на запись
// -4 ошибка открытия файла на чтения
// -5 неизвестный режим сжатия файла
// -6 при декомпрессии RLE ошибка в бинарных данных файла
// -7 при сравнении файлов не совпадает символ в некоторой позиции
// -8 при сравнении файлов длины файлов не равны
// -9 ошибка записи в файл
// -10 ошибка чтения данных из файла


int test_modified();
// тест
int test(const char *mode,
  const char *inputFileName,
  const char *outputFileName
);

// сформировать сжатую строку
/// 500a => 128a128a128a116a
///< valueRepeatCount количество повторов символа data
///< value повторяемый символ
///< blockSize максимальное количество повторов в сжатой строке
bool getCompressedData(
  uint64_t valueRepeatCount, 
  int8_t value, 
  uint8_t blockSize,
  int outputMaxSize, 
  int8_t &outputSize, 
  int8_t *pOutput
) {
  outputSize = 0;

  // считаем сколько блоков помещается в массив данных
  int n = valueRepeatCount / blockSize;
  int r = valueRepeatCount % blockSize;
  if (outputMaxSize < n + 1)
    return false;

  for (int i = 0; i < n; ++i) {
    pOutput[outputSize++] = blockSize;
    pOutput[outputSize++] = value;
  }
  if (r) {
    pOutput[outputSize++] = r;
    pOutput[outputSize++] = value;
  }
  return true;
}

// сформировать сжатую строку
/// 500a => 128a128a128a116a
///< valueRepeatCount количество повторов символа data
///< value повторяемый символ
///< blockSize максимальное количество повторов в сжатой строке
int writeCompressedRepeatedValue(
  uint64_t valueRepeatCount,
  int8_t value,
  unsigned blockSize,
  std::ofstream &fout,
  const char *outputFileName,
  uint64_t &len
) {
  
  // считаем сколько блоков помещается в массив данных
  int n = valueRepeatCount / blockSize;
  int r = valueRepeatCount % blockSize;
  int ret = -9;

  if (!fout.is_open()) {
    for (int i = 0; i < n; ++i) {
      std::cout << blockSize << ".";
      std::cout << value << " ";
      len += 2;
    }
    if (r) {
      std::cout << r << ".";
      std::cout << value << " ";
      len += 2;
    }
    return 0;
  }
  
  __try {
    for (int i = 0; i < n; ++i) {
      if (!fout.write((const char*)&blockSize, 1))
        __leave;
      if (!fout.write((const char*)&value, 1))
        __leave;
      len += 2;
    }
    if (r) {
      if (!fout.write((const char*)&r, 1))
        __leave;
      if (!fout.write((const char*)&value, 1))
        __leave;
      len += 2;
    }
    ret = 0;
  } 
  __finally {
    if(ret && outputFileName)
      std::cerr << "Ошибка записи в файл: " << outputFileName << std::endl;
  }
  return ret;
}

//
uint64_t file_length(const char *fileName) {
  std::ifstream file(fileName, std::ios::binary | std::ios::in);
  if (!file)
    return 0;
  file.seekg(0, std::ios_base::end);
  //Получаем текущую позицию
  uint64_t len = file.tellg();
  //
  file.close();
  return len;
}

// сравнение двух файлов
int compare_files(const char *fileName1, const char *fileName2) {

  std::ifstream file1(fileName1, std::ios::binary | std::ios::in);
  if (!file1) {
    std::cerr << "Сравнение: ошибка открытия файла " << fileName1 << std::endl;
    return -4;
  }

  std::ifstream file2(fileName2, std::ios::binary | std::ios::in);
  if (!file2) {
    std::cerr << "Сравнение: ошибка открытия файла " << fileName2 << std::endl;
    return -4;
  }

  if (file_length(fileName1) != file_length(fileName2)) {
    std::cerr << "Сравнение: не совпадают размеры файлов " << fileName1 << " & " << fileName2 << std::endl;
    return -8;
  }

  char sym1, sym2;
  while (file1 && file2) {
    if (!file1.read(&sym1, 1) || !file2.read(&sym2, 1)) {
      break;
    }
    if (sym1 != sym2) {
      std::cerr << "Сравнение: ошибка сравнения данных " << fileName1 << " & " << fileName2 << std::endl;
      return -7;
    }
  }
  std::cout << "Сравнение" << std::endl
    << "файл1: " << fileName1 << std::endl
    << "файл2: " << fileName2 << std::endl
    << "*** Успешно" << std::endl << std::endl;
  return 0;
}

// сформировать разжатый файл для алгоритма RLE
int decompress_simple(
  std::ifstream &in_file, 
  std::ofstream &out_file,
  const char *inputFileName, 
  const char *outputFileName
) 
{
  uint64_t writtenBytes = 0;
  while (in_file) { 
    uint8_t repeat, sym;
    if (in_file.read((char*)&repeat, 1) && in_file.read((char*)&sym, 1)) {
      uint8_t syms[255];
      memset(syms, sym, repeat);
      out_file.write((const char*)&syms, repeat);
      writtenBytes += repeat;
    }
    else
      break;
  } 

  std::cout << "RLE сжатие" << std::endl
    << "вход: " << inputFileName << " (" << file_length(inputFileName) <<")" << std::endl
    << "выход: " << outputFileName << "(" << writtenBytes << ")" << std::endl
    << "*** Успешно" << std::endl << std::endl;

  return 0;
}

// сформировать разжатый файл для улучшенного алгоритма RLE
int decompress_modified(
  std::ifstream &in_file,
  std::ofstream &out_file,
  const char *inputFileName,
  const char *outputFileName
)
{
  uint64_t writtenBytes = 0;
  int8_t syms[255];
  int8_t repeat, sym;

  while (in_file) {    
    if (!in_file.read((char*)&repeat, 1))
      break;
    if (repeat > 0) {
      
      if (!in_file.read((char*)&sym, 1))
        return -10;

      // продублируем и запишем      
      memset(syms, sym, repeat);
      out_file.write((const char*)&syms, repeat);
      writtenBytes += repeat;
    }
    else {
      // надо считать блок одиночных
      repeat = -repeat;
      if (!in_file.read((char*)syms, repeat))
        return -10;
      out_file.write((const char*)&syms, repeat);
      writtenBytes += repeat;
    }
  }

  std::cout 
    << "RLE распаковка" << std::endl
    << "вход: " << inputFileName << " (" << file_length(inputFileName) << ")" << std::endl
    << "выход: " << outputFileName << "(" << writtenBytes << ")" << std::endl
    << "*** Успешно" << std::endl << std::endl;

  return 0;
}

// разжать
int decompress(const char *inputFileName, const char *outputFileName) {

  // что разжимаем
  std::ifstream in_file(inputFileName, std::ios::binary | std::ios::in);
  if (!in_file) {
    std::cerr << "Ошибка открытия файла: " << inputFileName << std::endl;
    return -4;
  }

  // куда разжимаем
  std::ofstream out_file(outputFileName, std::ios::binary | std::ios::out);
  if (!out_file) {
    std::cerr << "Ошибка открытия файла: " << outputFileName << std::endl;
    return -3;
  }

  char mode;
  int ret = -5;
  in_file.read(&mode, 1);
  switch (mode) {
    case 's': {
      ret = decompress_simple(in_file, out_file, inputFileName, outputFileName);
      break;
    }    
    case 'm': {
      ret = decompress_modified(in_file, out_file, inputFileName, outputFileName);
      break;
    }
  }

  in_file.close();
  out_file.close();
  return ret;
}

// запись со сжатием по алгоритму RLE
int compress_simple(const char *inputFileName, 
  const char *outputFileName,
  const char *statisticFileName
) {

  std::ifstream in_file(inputFileName, std::ios::binary | std::ios::in);
  if (!in_file) {
    std::cerr << "Ошибка открытия файла: " << inputFileName << std::endl;
    return -4;
  }

  std::ofstream out_file(outputFileName, std::ios::binary | std::ios::out);
  if (!out_file) {
    std::cerr << "Ошибка открытия файла: " << outputFileName << std::endl;
    return -4;
  }

  int blockSize = 255;
  char symCurrent;
  uint64_t valueRepeatCount = 0;
  bool bStart = true;
  char symNew;
  uint64_t writtenBytes = 0, readBytes = 0;

  // запишем признак режима сжатия - простое сжатие RLE
  char mode = 's';
  if (!out_file.write(&mode, 1)) {
    // что-то пошло не так
    std::cerr << "Ошибка записи в файл: " << outputFileName << std::endl;
    return -9;
  }
  writtenBytes = 1;
  int ret; 

  while (in_file.read(&symNew, 1)) {    
    // std::cout << "Read bt: " << in_file.tellg() << std::endl;
    readBytes++;
    
    if (bStart) {
      valueRepeatCount = 1;
      symCurrent = symNew;
      bStart = !bStart;
      continue;
    }

    if (symCurrent == symNew) {
      valueRepeatCount++;
    }
    else 
    {
       ret = writeCompressedRepeatedValue(
        valueRepeatCount, symCurrent, blockSize, 
        out_file, outputFileName, writtenBytes
      );
      if (ret) 
        return ret;

      valueRepeatCount = 1;
      symCurrent = symNew;
    }
  }

  ret = writeCompressedRepeatedValue(
    valueRepeatCount, symCurrent, blockSize, 
    out_file, outputFileName, writtenBytes
  );
  if (ret)
    return ret;

  std::cout << "RLE сжатие" << std::endl
    << "вход: " << inputFileName << "(" << readBytes << ")" << std::endl
    << "выход: " << outputFileName << "(" << writtenBytes << ")" << std::endl
    << "*** Успешно" << std::endl << std::endl;

  in_file.close();
  out_file.close();

  if (statisticFileName) {
    std::ofstream stat_file(statisticFileName, std::ios::out | std::ios::app);
    if (stat_file.is_open()) {
      stat_file << "s " << readBytes << " " << writtenBytes << std::endl;
      stat_file.close();
    }
  }

  return 0;
}

// состояние обработки считанных символов при разборе файла для сжатия
enum eSTATE {
  S0 = 0, // стартовое состояние ничего нет
  S1 = 1, // в наличии один символ
  S2 = 2, // в наличии блок одиночных символов
  S3 = 3, // в наличии серия повторяющихся символов  
};

//                                   Таблица переходов
//
//        Условие перехода                        / Действие 
// S0->S1 безусловный переход                     / фиксируем новый элемент как текущий
//                                                / счетчик блока = 0
//                                                / счетчик повторов = 0
// S0->S2 нет перехода
// S0->S3 нет перехода

//        Условие перехода                        / Действие 
// S1->S1 нет перехода
// S1->S2 новый элемент не совпадает с текущим    / формируем блок из двух одиночных символов, счетчик блока = 2 
//                                                / фиксируем новый элемент как текущий
// S1->S3 новый элемент совпадает с текущим       / взводим счетчик повторов в 2 
// 
//        Условие перехода                        / Действие
// S2->S1 новый элемент не совпадает с текущим    / вывести блок, счетчик блока в 0
//        количество элементов в блоке одиночных  / фиксируем новый элемент как текущий
//        ==127
// S2->S2 новый элемент не совпадает с текущим    / добавляем новый элемент в блок, счетчик блока +1
//        количество элементов в блоке одиночных  / фиксируем новый элемент как текущий
//        <=126
// S2->S3 новый элемент совпадает с текущим    / взводим счетчик повторов в 2, выводим блок одиночных без последнего
//                                             / символа, фиксируем новый элемент как текущий
//                                             / сбрасываем счетчик блока для верности
// 
//        Условие перехода                     / Действие
// S3->S1 новый элемент не совпадает с текущим / вывести серию повторов, сбросить счетчик повторов в 0
//                                             / фиксируем новый элемент как текущий
// S3->S2 нет перехода
// S3->S3 новый элемент совпадает с текущим    / счетчик повторов +1

// максимальное число повторов в серии
#define maxRepeatCount 127
// максимальный размер блока одиночных символов
#define maxSingleCount 127

// машина состояний
struct stateMachine {
  // текущее состояние
  eSTATE state = S0;
  // текущий элемент
  uint8_t currentSym;
  // блок одиночных символов
  int8_t singleBlock[maxSingleCount];
  int8_t singleCount;
  // серия повторов
  uint64_t repeatCount;
  // новый (только что считанный) элемент
  uint8_t newSym;
  // количество записанных байт
  uint64_t writtenBytes = 0;
  // количество байт для считывания
  uint64_t readBytes = 0;
  // выходной поток
  std::ofstream out_file;
  // входной файл
  std::ifstream in_file;
  // строка для тестирования
  std::string testString;

  void toCompress() {
    
    state = S0;
    
    if (testString.size()) {
      // для тестирования - символы из тестовой строки
      int len = testString.size(), pos = 0;
      while (pos != len) {
        newSym = testString.at(pos++);
        toDo();
      }
    }
    else {
      // основной цикл - символы из потока

      const int block_size = 2*4096;
      char buffer[block_size];
      int n = readBytes / block_size;
      int r = readBytes % block_size;
      
      for (int i = 0; i < n; i++) {
        if (!in_file)
          break;
        
        if (in_file.read((char *)buffer, block_size)) {          
          for (int j = 0; j < block_size; ++j) {
            newSym = buffer[j];
            toDo();
          }          
        }
      }

      if (in_file && r) {
        if (in_file.read((char *)buffer, r)) {
          for (int j = 0; j < r; ++j) {
            newSym = buffer[j];
            toDo();
          }
        }
      }

    }

    // обход закончился но у нас остался незаписанный хвост
    switch (state) {
      case S1: {
        singleBlock[0] = currentSym;
        singleCount = 1;
      }
      case S2: {
        writeSingleBlock(singleCount);
        break;
      }
      case S3: {
        uint64_t len = 0;
        writeCompressedRepeatedValue(
          repeatCount,
          currentSym,
          maxRepeatCount,
          out_file,
          nullptr,
          writtenBytes);
        break;
      }
    }    
  }

  // считан элемент разбираемся в зависимости от текущего состояния
  // куда надо перейти
  // выполняем действие, меняем состояние
  void toDo() {

    if (state == S0) {
      S0_to_S1_toDo();
      return;
    }

    // фиксируем совпадает новый элемент с предыдущим или нет
    bool bEqualSyms = (currentSym == newSym);

    if (state == S1) {
      // S1->S1 нет перехода
      // S1->S2 новый элемент не совпадает с текущим    / формируем блок из двух одиночных символов, счетчик блока = 2 
      //                                                / фиксируем новый элемент как текущий
      // S1->S3 новый элемент совпадает с текущим       / взводим счетчик повторов в 2 
      if (!bEqualSyms)
        S1_to_S2_toDo();
      else
        S1_to_S3_toDo();
      return;
    }

    if (state == S2) {
      // S2->S1 новый элемент не совпадает с текущим    / вывести блок, счетчик блока в 0
      //        количество элементов в блоке одиночных  / фиксируем новый элемент как текущий
      //        ==127
      // S2->S2 новый элемент не совпадает с текущим    / добавляем новый элемент в блок, счетчик блока +1
      //        количество элементов в блоке одиночных  / фиксируем новый элемент как текущий
      //        <=126
      if (!bEqualSyms) {
        if (singleCount == 127)
          S2_to_S1_toDo();
        else
          S2_to_S2_toDo();
      }
      // S2->S3 новый элемент совпадает с текущим    / взводим счетчик повторов в 2, выводим блок одиночных без последнего
      //                                             / символа, фиксируем новый элемент как текущий
      //                                             / сбрасываем счетчик блока для верности
      else {
        S2_to_S3_toDo();
      }      

      return;
    }

    if (state == S3) {
      // S3->S1 новый элемент не совпадает с текущим / вывести серию повторов, сбросить счетчик повторов в 0
      //                                             / фиксируем новый элемент как текущий
      // S3->S2 нет перехода
      // S3->S3 новый элемент совпадает с текущим    / счетчик повторов +1
      if (!bEqualSyms)
        S3_to_S1_toDo();
      else
        S3_to_S3_toDo();
      return;
    }

  }

  // S0->S1 безусловный переход                     / фиксируем новый элемент как текущий
  //                                                / счетчик блока = 0
  //                                                / счетчик повторов = 0
  void S0_to_S1_toDo() {
    currentSym = newSym;
    singleCount = 0;
    repeatCount = 0;
    state = S1;
  }

  // S1->S2 новый элемент не совпадает с текущим / формируем блок из двух одиночных символов, счетчик блока = 2 
  //                                             / фиксируем новый элемент как текущий
  void S1_to_S2_toDo() {
    singleBlock[0] = currentSym;
    singleBlock[1] = newSym;
    singleCount = 2;
    currentSym = newSym;
    state = S2;
  }

  // S1->S3 новый элемент совпадает с текущим    / взводим счетчик повторов в 2 
  void S1_to_S3_toDo() {
    repeatCount = 2;
    currentSym = newSym;
    state = S3;
  }
  
  // S2->S1 новый элемент не совпадает с текущим    / вывести блок, счетчик блока в 0
  //        количество элементов в блоке одиночных  / фиксируем новый элемент как текущий
  //        ==127
  void S2_to_S1_toDo() {
    writeSingleBlock(singleCount);
    singleCount = 0;
    currentSym = newSym;
    state = S1;
  }

  // S2->S2 новый элемент не совпадает с текущим    / добавляем новый элемент в блок, счетчик блока +1
  //        количество элементов в блоке одиночных  / фиксируем новый элемент как текущий
  //        <=126
  void S2_to_S2_toDo() {
    singleBlock[singleCount++] = newSym;
    currentSym = newSym;
    state = S2;
  }

  // S2->S3 новый элемент совпадает с текущим    / взводим счетчик повторов в 2, выводим блок одиночных без последнего
  //                                             / символа, фиксируем новый элемент как текущий
  //                                             / сбрасываем счетчик блока для верности
  void S2_to_S3_toDo() {
    repeatCount = 2;
    writeSingleBlock(singleCount - 1);
    singleCount = 0;
    currentSym = newSym;
    state = S3;
  }

  // S3->S1 новый элемент не совпадает с текущим / вывести серию повторов, сбросить счетчик повторов в 0
  //                                             / фиксируем новый элемент как текущий
  void S3_to_S1_toDo() {
    uint64_t len = 0;
    writeCompressedRepeatedValue(
      repeatCount,
      currentSym,
      maxRepeatCount,
      out_file,
      nullptr,
      writtenBytes);
    repeatCount = 0;
    currentSym = newSym;
    state = S1;
  }

  // S3->S2 нет перехода

  // S3->S3 новый элемент совпадает с текущим    / счетчик повторов +1
  void S3_to_S3_toDo() {
    repeatCount++;
    currentSym = newSym;
    state = S3;
  }

  // записать блок одиночных символов заданного количества
  void writeSingleBlock(int count) {    
    int minusCount = -count;
    if (out_file.is_open()) {
      out_file.write((const char*)&minusCount, 1);
      out_file.write((const char*)singleBlock, count);
    }
    else {
      std::cout << minusCount << ".";
      for (int i = 0; i < count; ++i) {
        std::cout << singleBlock[i];
      }
      std::cout << " ";
    }
    ++writtenBytes;
    writtenBytes += count;
  }

};

// запись со сжатием по улучшенному алгоритму RLE
int compress_modified(
  const char *inputFileName, 
  const char *outputFileName,
  const char *statisticFileName
) {
  
  stateMachine context;

  context.in_file.open(inputFileName, std::ios::binary | std::ios::in);
  if (!context.in_file.is_open()) {
    std::cerr << "Ошибка открытия файла: " << inputFileName << std::endl;
    return -4;
  }

  context.out_file.open(outputFileName, std::ios::binary | std::ios::out);
  if (!context.out_file.is_open()) {
    std::cerr << "Ошибка открытия файла: " << outputFileName << std::endl;
    return -4;
  }

  context.readBytes = file_length(inputFileName);

  // запишем признак режима сжатия - простое сжатие RLE
  char mode = 'm';
  
  if (context.out_file.is_open()) {
    if (!context.out_file.write(&mode, 1)) {
      // что-то пошло не так
      std::cerr << "Ошибка записи в файл: " << outputFileName << std::endl;
      return -9;
    }
    context.writtenBytes = 1;
  }  
  context.toCompress();

  std::cout << "Модифицированное RLE сжатие" << std::endl
    << "вход: " << inputFileName << "(" << context.readBytes << ")" << std::endl
    << "выход: " << outputFileName << "(" << context.writtenBytes << ")" << std::endl
    << "*** Успешно" << std::endl << std::endl;

  context.in_file.close();
  context.out_file.close();

  if (statisticFileName) {
    std::ofstream stat_file(statisticFileName, std::ios::out | std::ios::app);
    if (stat_file.is_open()) {
      stat_file << "m " << context.readBytes << " " << context.writtenBytes << std::endl;
      stat_file.close();
    }
  }

  return 0;
}

// описание режимов работы
std::string getCompressMode(const char *mode) {
  if (strlen(mode) == 0)
    return "?";
  switch (mode[0]) {
  case 's': return "сжатие RLE              файл1 --> файл2";
  case 'm': return "модиф.сжатие RLE        файл1 --> файл2";
  case 'd': return "распаковка              файл1 --> файл2";
  case 'r': return "сравнение двух файлов   файл1 & файл2";
  case '1': return "тест проверки сжатия RLE\n\t\t/упаковка, распаковка, сравнение/   файл1 --> файл2";
  case '2': return "тест проверки модиф.сжатия RLE\n\t\t/упаковка, распаковка, сравнение/   файл1 --> файл2";
  }
  return "?";
}

// печать заголовка
void printHeader(int argc, char *argv[]) {
  std::cout     
<< "+----------------------------------------------------------------------------+" << std::endl
<< "+                          RLE compress/decompress                           +" << std::endl
<< "+----------------------------------------------------------------------------+" << std::endl
    ;
  std::cout << std::endl;
  if (argc>1) {
    if (argc > 1)
      std::cout << "Режим: " << getCompressMode(argv[1]) << std::endl;
    if (argc > 2)
      std::cout << "Входной файл: " << argv[2] << std::endl;
    if (argc > 3)
      std::cout << "Выходной файл: " << argv[3] << std::endl;
    std::cout << std::endl;
  }
}

// справочная информация
int showHelp() {
  std::cout << "Запуск: 24.exe (s|m|d|r|1|2) путь_к_файлу1 путь_к_файлу2 {путь_к_файлу_статистики}" << std::endl;
  std::cout << "s: " << getCompressMode("s") << std::endl;
  std::cout << "m: " << getCompressMode("m") << std::endl;
  std::cout << "d: " << getCompressMode("d") << std::endl;
  std::cout << "r: " << getCompressMode("r") << std::endl;
  std::cout << "1: " << getCompressMode("1") << std::endl;
  std::cout << "2: " << getCompressMode("2") << std::endl;
  std::cout << "путь_к_файлу_статистики: для режимов s/m записывается коэффициент сжатия" << std::endl;
  return 0;
}

// список всех действий
int toDo(int argc, char *argv[]) {
  
  const char *statisticFileName = nullptr;
  if (argc > 4)
    statisticFileName = argv[4];

  // сравнить 2 файла
  if ((strcmp(argv[1], "r") == 0) && (argc >= 4)) {
    return compare_files(argv[2], argv[3]);
  }

  if ((strcmp(argv[1], "s") == 0) && (argc >= 4)) {
    return compress_simple(argv[2], argv[3], statisticFileName);
  }

  if ((strcmp(argv[1], "m") == 0) && (argc >= 4)) {
    return compress_modified(argv[2], argv[3], statisticFileName);
  }

  if ((strcmp(argv[1], "d") == 0) && (argc >= 4)) {
    return decompress(argv[2], argv[3]);
  }

  if ((strcmp(argv[1], "1") == 0) && (argc >= 4)) {
    return test("s", argv[2], argv[3]);
  }

  if ((strcmp(argv[1], "2") == 0) && (argc >= 4)) {
    return test("m", argv[2], argv[3]);
  }

  if ((strcmp(argv[1], "3") == 0)) {
    return test_modified();
  }

  return 0;
}

// тест
int test(const char *mode, 
  const char *inputFileName, 
  const char *outputFileName
) {

  std::string fn = inputFileName;
  std::string ext = fn.substr(fn.find_last_of(".") + 1);
  if (!ext.size())
    ext = "~";
  std::string output = outputFileName;
  output.append(".");
  output.append(ext);

  {
    const char *argv[] = {
      "", mode, inputFileName, outputFileName
    };
    toDo(sizeof(argv) / sizeof(const char *), (char **)argv);
  }

  {
    const char *argv[] = {
      "", "d", outputFileName, output.c_str()
    };
    toDo(sizeof(argv) / sizeof(const char *), (char **)argv);
  }

  {
    const char *argv[] = {
      "", "r", inputFileName, output.c_str()
    };
    toDo(sizeof(argv) / sizeof(const char *), (char **)argv);
  }
  return 0;
}

int test_modified() {

#if 0
  {
    stateMachine context;

    context.testString =
      "12223444566678889000"
      ;
    // 128.1 22.1 -127.02... -23... 128.1 22.1
    context.toCompress();


    context.testString =
      "111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
      "023456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
      "111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
      ;
    // 128.1 22.1 -127.02... -23... 128.1 22.1
    context.toCompress();

    context.testString =
      "023456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
      "111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
      "023456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
      "111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
      "023456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
      ;
    std::cout << std::endl << std::endl;
    // -127.02... -23... 128.1 22.1 -127.02... -23... 128.1 22.1 -127.02... -23... 
    context.toCompress();

  }
#endif

  return 0;
}

int main(int argc, char *argv[])
{    
  setlocale(LC_CTYPE, "rus");
  printHeader(argc, argv);
 
  if (argc <= 1) {
    // 
    return showHelp();
  }

  if (argc <= 2) {
    std::cout << "Недостаточно параметров для работы программы." << std::endl << std::endl;
    return showHelp();
  }

  // отрабатываем ключ
  return toDo(argc, argv);
}

