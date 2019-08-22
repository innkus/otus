#pragma once

#include "pch.h"
#include "simpleArray.h"

// rnbqkbnr / pppppppp / 8 / 8 / 8 / 8 / PPPPPPPP / RNBQKBNR w KQkq - 0 1

enum eNumBoards {
  numWhitePawns   = 0 ,
  numWhiteKnights = 1 ,
  numWhiteBishops = 2 ,
  numWhiteRooks   = 3 ,
  numWhiteQueens  = 4 ,
  numWhiteKing    = 5 ,
  numBlackPawns   = 6 ,
  numBlackKnights = 7 ,
  numBlackBishops = 8 ,
  numBlackRooks   = 9 ,
  numBlackQueens  = 10,
  numBlackKing    = 11,
};

enum eSymBoards {
  symWhitePawns   =  'P', 
  symWhiteKnights =  'N', 
  symWhiteBishops =  'B', 
  symWhiteRooks   =  'R', 
  symWhiteQueens  =  'Q', 
  symWhiteKing    =  'K', 
  symBlackPawns   =  'p', 
  symBlackKnights =  'n', 
  symBlackBishops =  'b', 
  symBlackRooks   =  'r', 
  symBlackQueens  =  'q', 
  symBlackKing    =  'k', 
};

// активный цвет
enum activeColor : int {
  eWhiteColor = 0,  // белый цвет
  eBlackColor = 1,  // черный цвет
};

struct pos_s {
  char row;
  char col;
  pos_s() : row(0), col(0) {}
  pos_s(char _col, char _row) : row(_row), col(_col) {}
  bool operator==(const pos_s &p) const{
    return row == p.row && col == p.col;
  }
};

struct move_s {
  //
  bool init(const char *move, int &len) {
    len = strlen(move);
    if (len < 4)
      return false;
    source = pos_s(move[0], move[1]);
    target = pos_s(move[2], move[3]);
    return true;
  }

  pos_s     source;
  pos_s     target;

  // доски, соответствующие позициям
  uint64_t  sourceBoard;
  uint64_t  targetBoard;
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
  static int getBoardByFigure(char c);

  // получить имя фигуры по номеру доски
  static char getFigureByBoard(int numBoard);

  // построить доску по положению фигуры
  static uint64_t buildBoardByPos(char col, char row);

  // выгрузить описание расположения фигур
  std::string upload() const;
};

// game
struct game_s {
  ///
  game_s() {};
  ///
  game_s(const char *fen);

  // загрузить строку с описанием
  bool load(const char *fen);

  // выгрузить строку с описанием
  std::string upload() const;

  // изменить ход
  // смена стороны!!!
  void counter();

  // корректировка описания рокировки
  void loadCastling(const char *castling);

  // Нужно посчитать этот ход, передать право хода другой стороне,
  // а также увеличить или сбросить счётчик полуходов для правила 50 ходов.
  // Счётчик сбрасывается, если было взятие или ход пешкой.
  // В остальных случаях счётчик увеличивается на 1 после каждого полухода.
  // Анализ досок!!!
  void halfMoveClock();

  // разобрать текущий ход из входных данных
  bool readCurrentMove(const char *move);

  // Это обычных ход или взятие.
  // Не рокировка, не взятие на проходе, не превращение ферзя, а обычный, самый простой ход со взятием или без.
  // Меняются доски!!!
  void doSimpleMove();

  // проверить является ли ход превращением пешки
  bool isPawnTransform();

  // Превращение пешки и передача хода.
  // Проверка на возможность хода не делается.  
  void doTransformMove();

  // ищем номер доски доски, которая соответствует текущему ходу 
  // с учетом текущего активного цвета
  // @return номер доски или -1
  int getNumBoardByCurrentSourceMoveAndColor();

  board_s piecePlacement;

  // описание рокировки
  char castling[5];

  // взятие на проходе
  char enPassant[5];

  // счетчик полупроходов
  int halfmoveClock;

  // номер хода
  int fullmoveNumber;

  // кто ходит
  activeColor color;

  // текущий ход
  move_s currentMove;

  // текущая фигура превращения
  char symTransformFigure;
};
