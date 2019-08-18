#include "pch.h"
#include "game.h"

//////////////////////////////////////////////////////
// ����� �������� ���������� ���������� ������� ��� �����
//////////////////////////////////////////////////////
double degree_degree2_multy(double x, int n) {
  double res = 1;
  while (n > 1) {
    if (n % 2 == 1)
      res *= x;
    x *= x;
    n /= 2;
  }
  if (n > 0)
    res *= x;
  return res;
}

// 
board_s::board_s() {
  memset(boards, 0, sizeof(boards));
}

// �������� ������������ �����
bool board_s::load(const char *placement) {

  std::string s;
  int i;
  for (i = 0; i < strlen(placement) - 1; ++i) {
    if ((placement[i] == '/') && (placement[i + 1] == '/')) {
      s += "/8";
    }
    else
    {
      s += placement[i];
    }
  }  
  s += placement[i];
  placement = s.c_str();

  // ������ ������ �������� �� ������
  const char *delim = "/";
  int len = strlen(placement) + 1;
  char *fenCpy = new char[len];
  strcpy_s(fenCpy, len, placement);

  int numLine = 7;
  char *next_token = NULL;
  char * token = strtok_s(fenCpy, delim, &next_token);
  while (token) {
    // ���������� ������
    lineToBoard(numLine--, token);
    token = strtok_s(NULL, delim, &next_token);
  }

  delete[]fenCpy;
  return true;
}

/*
P whitePawns,
N whiteKnights,
B whiteBishops,
R whiteRooks,
Q whiteQueens,
K whiteKing,

p blackPawns,
n blackKnights,
b blackBishops,
r blackRooks,
q blackQueens,
k blackKing
*/

// ��������� ������ � ��������� �� �����
void board_s::lineToBoard(int numLine, const char *token) {
  int len = strlen(token);
  int col = 0;
  for (int i = 0; i < len; ++i) {
    int numBoard = getBoardByFigure(token[i]);
    if (numBoard == -1) {
      // ������ ������� ������� ������� ����� ���� ����������
      col += token[i] - '0';
    }
    else {
      // �������� �� ����� ������
      int pos = numLine * 8 + col;
      boards[numBoard] += ((uint64_t)1 << pos);
      col++;
    }
  }
}

// �������� ����� ����� �� �������
int board_s::getBoardByFigure(char c) const {

  switch (c) {
  case 'P': // whitePawns
    return 0;
  case 'N': // whiteKnights
    return 1;
  case 'B': // whiteBishops
    return 2;
  case 'R': // whiteRooks
    return 3;
  case 'Q': // whiteQueens
    return 4;
  case 'K': // whiteKing
    return 5;

  case 'p': // blackPawns
    return 6;
  case 'n': // blackKnights
    return 7;
  case 'b': // blackBishops
    return 8;
  case 'r': // blackRooks
    return 9;
  case 'q': // blackQueens
    return 10;
  case 'k': // blackKing
    return 11;
  }
  return -1;
}

// �������� ������ �� ������ �����
char board_s::getFigureByBoard(int numBoard) const {

  switch (numBoard) {
    case 0: return 'P'; // whitePawns,
    case 1: return 'N'; // whiteKnights,
    case 2: return 'B'; // whiteBishops,
    case 3: return 'R'; // whiteRooks,
    case 4: return 'Q'; // whiteQueens,
    case 5: return 'K'; // whiteKing,
    case 6: return 'p'; // blackPawns,
    case 7: return 'n'; // blackKnights,
    case 8: return 'b'; // blackBishops,
    case 9: return 'r'; // blackRooks,
    case 10: return 'q'; // blackQueens,
    case 11: return 'k'; // blackKing
  }
  return '\0';
}

// ��������� �������� ������������ �����
std::string board_s::upload() const {
  std::string result;

  // ����� �� ��������
  for (int num = 7; num >= 0; --num) {
    
    int blankCount = 0;

    for (int col = 0; col < 8; ++col) {

      int pos = num * 8 + col;

      // ��������� ����� ������ ��� ��� �� �����-���� �� �����
      bool bFinded = false;
      int j;
      for (j = 0; j < 12; j++) {
        
        if((boards[j] >> pos) & 1) {
          // ���-�� ����� 
          bFinded = true;
          break;   
        }
      }

      // ���� ���-�� �����, �� ��������� � ������
      if (bFinded) {
        // ���� ������� ��� �� ����� ���� � ������ ����������� - ���������� �� � ������
        if(blankCount)
          result += std::to_string(blankCount);
        // ��������� ������ � ����� ��������� ��������
        result += getFigureByBoard(j);
        // ��������� ������� ������
        blankCount = 0;
      }
      else {
        // ��� ������� ������� ��
        blankCount++;      
      }
    }

    // �� ������ ���� ��������� ���������
    if(blankCount)
      result += std::to_string(blankCount);

    // ����� ����������� ���� (������ ���� ��� �� ��������� ������)
    if(num != 0)
      result += "/";
  }

  return result;
}

/////////////////////////////////////////////////////
game_s::game_s(const char *fen) {
  load(fen);
}

// ��������� ������ � ���������
bool game_s::load(const char *fen) {

  // ��������� �� �����
  if (!fen)
    return false;

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
    return false;
  }

  // ��������� ������

  // ������������ �����
  piecePlacement.load(listStr.get(0));

  // ����������� � �������� ������
  if (listStr.get(1)[0] == 'w' || listStr.get(1)[0] == 'W')
    �olor = eWhiteColor;
  else
    �olor = eBlackColor;

  // ��������� ��������� (��� ���� - ���� ������ �� �������, ��� �� ���������)
  loadCastling(listStr.get(2));

  // ������ �� ������� (��� ���� - ���� ������ �� �������, ��� �� ���������)
  strcpy_s(enPassant, sizeof(enPassant), listStr.get(3));

  // ������� ������������
  halfmoveClock = std::stoi(listStr.get(4));

  // ����� ����
  fullmoveNumber = std::stoi(listStr.get(5));
  return true;
}

// ������������� �������� ���������
void game_s::loadCastling(const char *_castling) {
  std::string str(_castling), sOut;

  std::size_t found = str.find('-');
  if (found != std::string::npos) {
    sOut = "-";
  }
  else 
  {
    char f[4] = { 'K', 'Q', 'k', 'q' };
    for (int i = 0; i < 4; ++i) {
      std::size_t found = str.find(f[i]);
      if (found != std::string::npos) 
        sOut += f[i];
    }
  }
  strcpy_s(castling, sizeof(castling), sOut.c_str());
}

// ��������� ������ � ���������
std::string game_s::upload() const {

  std::string result;

  // ������������ �����
  result += piecePlacement.upload();
  result += " ";

  // ����������� � �������� ������
  if (�olor == eWhiteColor)
    result += "w ";
  else
    result += "b ";

  // ��������� ��������� (��� ���� - ���� ������ �� �������, ��� �� ���������)
  result += castling;
  result += " ";

  // ������ �� ������� (��� ���� - ���� ������ �� �������, ��� �� ���������)
  result += enPassant;
  result += " ";

  // ������� ������������
  result += std::to_string(halfmoveClock);
  result += " ";

  // ����� ����
  result += std::to_string(fullmoveNumber);

  return result;
}

// �������� ���
void game_s::counter() {

  if (�olor == eBlackColor) {
    // ��������� ��� - �����
    // ���� ��������� �������
    fullmoveNumber++;
    // ������ �������
    �olor = eWhiteColor;
  }
  else {
    // ������ �������
    �olor = eBlackColor;
  }
}
