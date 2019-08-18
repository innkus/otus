#include "pch.h"
#include "game.h"

//////////////////////////////////////////////////////
// „ерез двоичное разложение показател¤ степени дл¤ числа
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

// описание расположени¤ фигур
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

  // строим массив подстрок по строке
  const char *delim = "/";
  int len = strlen(placement) + 1;
  char *fenCpy = new char[len];
  strcpy_s(fenCpy, len, placement);

  int numLine = 7;
  char *next_token = NULL;
  char * token = strtok_s(fenCpy, delim, &next_token);
  while (token) {
    // обработаем строку
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

// разобрать строку и поставить на доску
void board_s::lineToBoard(int numLine, const char *token) {
  int len = strlen(token);
  int col = 0;
  for (int i = 0; i < len; ++i) {
    int numBoard = getBoardByFigure(token[i]);
    if (numBoard == -1) {
      // пуста¤ позици¤ смотрим сколько ¤чеек надо пропустить
      col += token[i] - '0';
    }
    else {
      // поставим на доску фигуру
      int pos = numLine * 8 + col;
      boards[numBoard] += ((uint64_t)1 << pos);
      col++;
    }
  }
}

// получить номер доски по символу
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

// получить символ по номеру доски
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

// выгрузить описание расположени¤ фигур
std::string board_s::upload() const {
  std::string result;

  // бежим по строчкам
  for (int num = 7; num >= 0; --num) {
    
    int blankCount = 0;

    for (int col = 0; col < 8; ++col) {

      int pos = num * 8 + col;

      // провер¤ем стоит фигура или нет на какой-либо из досок
      bool bFinded = false;
      int j;
      for (j = 0; j < 12; j++) {
        
        if((boards[j] >> pos) & 1) {
          // кто-то стоит 
          bFinded = true;
          break;   
        }
      }

      // если где-то стоит, то добавл¤ем в строку
      if (bFinded) {
        // если пустоты уже до этого были а теперь закончились - дописываем их в строку
        if(blankCount)
          result += std::to_string(blankCount);
        // формируем строку с нашим найденным символом
        result += getFigureByBoard(j);
        // скидываем счетчик пустот
        blankCount = 0;
      }
      else {
        // это пустота считаем ее
        blankCount++;      
      }
    }

    // на случай если закончили пустотами
    if(blankCount)
      result += std::to_string(blankCount);

    // пишем завершающий слеш (только если это не последн¤¤ строка)
    if(num != 0)
      result += "/";
  }

  return result;
}

/////////////////////////////////////////////////////
game_s::game_s(const char *fen) {
  load(fen);
}

// загрузить строку с описанием
bool game_s::load(const char *fen) {

  // разбиваем на части
  if (!fen)
    return false;

  SingleArray<char *> listStr;

  // строим массив подстрок по строке
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

  // должно быть 6 строк
  if (listStr.size() != 6) {
    delete[]fenCpy;
    return false;
  }

  // разбираем данные

  // расположение фигур
  piecePlacement.load(listStr.get(0));

  // разбираемс¤ с активным цветом
  if (listStr.get(1)[0] == 'w' || listStr.get(1)[0] == 'W')
    сolor = eWhiteColor;
  else
    сolor = eBlackColor;

  // описываем рокировку (как есть - пока толком не пон¤тно, как ее разбирать)
  loadCastling(listStr.get(2));

  // вз¤тие на проходе (как есть - пока толком не пон¤тно, как ее разбирать)
  strcpy_s(enPassant, sizeof(enPassant), listStr.get(3));

  // счетчик полупроходов
  halfmoveClock = std::stoi(listStr.get(4));

  // номер хода
  fullmoveNumber = std::stoi(listStr.get(5));
  return true;
}

// корректировка описани¤ рокировки
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

// выгрузить строку с описанием
std::string game_s::upload() const {

  std::string result;

  // расположение фигур
  result += piecePlacement.upload();
  result += " ";

  // разбираемс¤ с активным цветом
  if (сolor == eWhiteColor)
    result += "w ";
  else
    result += "b ";

  // описываем рокировку (как есть - пока толком не пон¤тно, как ее разбирать)
  result += castling;
  result += " ";

  // вз¤тие на проходе (как есть - пока толком не пон¤тно, как ее разбирать)
  result += enPassant;
  result += " ";

  // счетчик полупроходов
  result += std::to_string(halfmoveClock);
  result += " ";

  // номер хода
  result += std::to_string(fullmoveNumber);

  return result;
}

// изменить ход
void game_s::counter() {

  if (сolor == eBlackColor) {
    // следующий ход - белых
    // надо увеличить счетчик
    fullmoveNumber++;
    // мен¤ем сторону
    сolor = eWhiteColor;
  }
  else {
    // мен¤ем сторону
    сolor = eBlackColor;
  }
}
