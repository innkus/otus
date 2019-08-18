#pragma once

#include "pch.h"
#include "simpleArray.h"

// rnbqkbnr / pppppppp / 8 / 8 / 8 / 8 / PPPPPPPP / RNBQKBNR w KQkq - 0 1

// активный цвет
enum activeColor : int {
  eWhiteColor = 0,  // белый цвет
  eBlackColor = 1,  // черный цвет
};

// доска
struct board_s {
  uint64_t boards[12];

  // 
  board_s();

  // описание расположения фигур
  bool load(const char *placement);

  // разобрать строку и поставить на доску
  void lineToBoard(int numLine, const char *token);

  // получить номер доски по символу
  int getBoardByFigure(char c) const;

  // получить номер доски по символу
  char getFigureByBoard(int numBoard) const;

  // выгрузить описание расположения фигур
  std::string upload() const;
};

// game
struct game_s {
  ///
  game_s() {}
  ///
  game_s(const char *fen);

  // загрузить строку с описанием
  bool load(const char *fen);

  // выгрузить строку с описанием
  std::string upload() const;

  // изменить ход
  void counter();

  // корректировка описания рокировки
  void loadCastling(const char *castling);

  board_s piecePlacement;

  // кто ходит
  activeColor сolor;

  // описание рокировки
  char castling[5];

  // взятие на проходе
  char enPassant[5];

  // счетчик полупроходов
  int halfmoveClock;

  // номер хода
  int fullmoveNumber;

};
