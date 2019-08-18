#pragma once

#include "pch.h"
#include "simpleArray.h"

// rnbqkbnr / pppppppp / 8 / 8 / 8 / 8 / PPPPPPPP / RNBQKBNR w KQkq - 0 1

// �������� ����
enum activeColor : int {
  eWhiteColor = 0,  // ����� ����
  eBlackColor = 1,  // ������ ����
};

// �����
struct board_s {
  uint64_t boards[12];

  // 
  board_s();

  // �������� ������������ �����
  bool load(const char *placement);

  // ��������� ������ � ��������� �� �����
  void lineToBoard(int numLine, const char *token);

  // �������� ����� ����� �� �������
  int getBoardByFigure(char c) const;

  // �������� ����� ����� �� �������
  char getFigureByBoard(int numBoard) const;

  // ��������� �������� ������������ �����
  std::string upload() const;
};

// game
struct game_s {
  ///
  game_s() {}
  ///
  game_s(const char *fen);

  // ��������� ������ � ���������
  bool load(const char *fen);

  // ��������� ������ � ���������
  std::string upload() const;

  // �������� ���
  void counter();

  // ������������� �������� ���������
  void loadCastling(const char *castling);

  board_s piecePlacement;

  // ��� �����
  activeColor �olor;

  // �������� ���������
  char castling[5];

  // ������ �� �������
  char enPassant[5];

  // ������� ������������
  int halfmoveClock;

  // ����� ����
  int fullmoveNumber;

};
