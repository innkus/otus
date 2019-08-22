#include "pch.h"
#include "game.h"

//////////////////////////////////////////////////////
// через двоичное разложение показателя степени для числа
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

#define boardByPos(pos)  ((uint64_t)1 << pos)

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
int board_s::getBoardByFigure(char c)  {

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

// получить имя фигуры по номеру доски
char board_s::getFigureByBoard(int numBoard) {

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

// построить доску по положению фигуры
uint64_t board_s::buildBoardByPos(char col, char row) {
  int pos = (row - 1) * 8 + (col - 'a');
  return boardByPos(pos);
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
    color = eWhiteColor;
  else
    color = eBlackColor;

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
  if (color == eWhiteColor)
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

  if (color == eBlackColor) {
    // следующий ход - белых
    // надо увеличить счетчик
    fullmoveNumber++;
    // мен¤ем сторону
    color = eWhiteColor;
  }
  else {
    // мен¤ем сторону
    color = eBlackColor;
  }
}

// разобрать текущий ход из входных данных
bool game_s::readCurrentMove(const char *move) {
  
  // сбрасываем фигуру превращения
  symTransformFigure = 0;

  int strlenMove = 0;
  if (!currentMove.init(move, strlenMove))
    return false;

  currentMove.sourceBoard = board_s::buildBoardByPos(move[0], move[1]);
  currentMove.targetBoard = board_s::buildBoardByPos(move[2], move[3]);

  switch (strlenMove) {
    case 4: {
      // обычный ход
      return true;
    }
    case 5: {
      // это должна быть пешка с превращением
      // проверим это точно пешка
      // ищем номер доски доски, которая соответствует текущему ходу 
      // с учетом текущего активного цвета
      int numBoard = getNumBoardByCurrentSourceMoveAndColor();
      if ((numBoard != numWhitePawns) && (numBoard != numBlackPawns)) {
        // что-то пошло не так
        return false;
      }
      // фиксируем фигуру превращения
      symTransformFigure = (color == eWhiteColor) ? toupper(move[4]) : tolower(move[4]);
      return true;
    } // case

  }
  
  return false;
}

// Нужно посчитать этот ход, передать право хода другой стороне,
// а также увеличить или сбросить счётчик полуходов для правила 50 ходов.
// Счётчик сбрасывается, если было взятие или ход пешкой.
// В остальных случаях счётчик увеличивается на 1 после каждого полухода.
void game_s::halfMoveClock() {

  // накидываем полуход
  // если есть взятие или пешка то он все равно сбросится
  halfmoveClock++;

  // проверяем если ходит пешка то сбрасываем счетчик
  // возмем пешечные доски и сверимся
  int numBoard = (color == eWhiteColor) ? 0 : 6;
  uint64_t board = piecePlacement.boards[numBoard];
  if (board & currentMove.sourceBoard) {
    // это пешка !!!
    halfmoveClock = 0;
  }
  else {

    // ходит не пешка
    // смотрим может было взятие фигур другого цвета ?
    // пробежим по остальным доскам и проверим может кого-то побили
    int numBoard = (color == eWhiteColor) ? 6 : 0;
    for (int k = numBoard; k < numBoard + 6; ++k) {
      uint64_t board = piecePlacement.boards[k];
      if (board & currentMove.targetBoard) {
        // кого-то побили
        // сбрасываем счетчик и закругляемся
        halfmoveClock = 0;
        break;
      }
    } // end for

  }
}

// Это обычных ход или взятие.
// Не рокировка, не взятие на проходе, не превращение ферзя, а обычный, самый простой ход со взятием или без.
void game_s::doSimpleMove() {
    
  int numBoard = (color == eWhiteColor) ? 0 : 6;
  int numSourceBoard = -1, numTargetBoard = -1;

  // ищем номер доски доски, которая соответствует текущему ходу
  for (int k = numBoard; k < numBoard + 6; ++k) {
    uint64_t board = piecePlacement.boards[k];
    if (board & currentMove.sourceBoard) {
      numSourceBoard = k;
      break;
    }
  }

  numBoard = (color != eWhiteColor) ? 0 : 6;
  for (int k = numBoard; k < numBoard + 6; ++k) {
    uint64_t board = piecePlacement.boards[k];
    if (board & currentMove.targetBoard) {
      numTargetBoard = k;
      break;
    }
  }

  if (numSourceBoard == -1) {
    // что-то пошло не так
    return;
  }

  // разбираемся с тем кто ходит
  // сбрасываем исходную позицию
  piecePlacement.boards[numSourceBoard] &= ~currentMove.sourceBoard;
  // ставим новую позицию
  piecePlacement.boards[numSourceBoard] |= currentMove.targetBoard;

  // разбираемся с соперником
  if (numTargetBoard == -1) {
    // ставится на чистое поле - доски соперника не трогаем
  }
  else {
    // если съели надо убрать с доски фигуру
    piecePlacement.boards[numTargetBoard] &= ~currentMove.targetBoard;
  }
 
  // проверим если это пешка и был скачок на два хода и есть рядом пешка
  // то поставим битовое поле
  if (numSourceBoard == numBlackPawns || numSourceBoard == numWhitePawns) {

    if ((labs(currentMove.source.row - currentMove.target.row) == 2) &&
      (currentMove.source.col == currentMove.target.col))
    {
      // смотрим есть ли рядом с целевым полем пешки другого цвета ?      
      int numPawnBoard = board_s::getBoardByFigure((color != eWhiteColor) ? symWhitePawns : symBlackPawns);
      if (numPawnBoard == -1)
        return;
      uint64_t pawnBoard = piecePlacement.boards[numPawnBoard];

      // посчитаем позиции соседних полей
      bool bAddBitField = false;
      if (currentMove.target.col != 'a') {
        uint64_t boardLeft = board_s::buildBoardByPos(currentMove.target.col - 1, currentMove.target.row);
        if (pawnBoard & boardLeft)
          bAddBitField = true;
      }
      if (currentMove.target.col != 'f') {
        uint64_t boardRight = board_s::buildBoardByPos(currentMove.target.col + 1, currentMove.target.row);
        if (pawnBoard & boardRight)
          bAddBitField = true;
      }
      if (bAddBitField) {
        // формируем признак битового поля
        char row = (currentMove.target.row + currentMove.source.row) / 2;
        enPassant[0] = currentMove.target.col;
        enPassant[1] = row;
        enPassant[2] = '\0';
      }
      else {
        strcpy_s(enPassant, sizeof(enPassant), "-");
      }

      return;
    }
    // обработка взятия на проходе
    // смотрим если битовое поле совпадает с целевым полем значит 
    // было взятие на проходе и надо убрать целевой ход с доски
    if (strlen(enPassant) == 2) {
      pos_s passant(enPassant[0], enPassant[1]);
      if (passant == currentMove.target) {
        // это взятие на проходе - конструируем положение противной фигуры
        // которую надо снять
        pos_s bit;
        bit.col = passant.col;
        bit.row = currentMove.source.row;
        uint64_t passantBoard = board_s::buildBoardByPos(bit.col, bit.row);
        // снимаем
        int numPawnBoard = board_s::getBoardByFigure((color != eWhiteColor) ? symWhitePawns : symBlackPawns);
        if (numPawnBoard == -1)
          return;
        piecePlacement.boards[numPawnBoard] &=~passantBoard;
      }
    }

  }

  strcpy_s(enPassant, sizeof(enPassant), "-");
}

// проверить является ли ход превращением пешки
bool game_s::isPawnTransform() {
  return symTransformFigure != 0;
}

// Превращение пешки и передача хода.
// Проверка на возможность хода не делается.  
void game_s::doTransformMove() {
  // делаем простой ход
  doSimpleMove();

  // после простого хода пешка стоит на новом месте
  // меняем пешку на фигуру превращения

  // найти номер доски пешки
  int numPawnBoard = board_s::getBoardByFigure( (color == eWhiteColor) ? symWhitePawns : symBlackPawns);
  if (numPawnBoard == -1)
    return;
  
  // сбросить целевую позицию
  piecePlacement.boards[numPawnBoard] &= ~currentMove.targetBoard;

  // найти доску активного цвета по фигуре превращения
  int numTransformBoard = board_s::getBoardByFigure(symTransformFigure);
  if (numTransformBoard == -1)
    return;

  // поставить фигуру превращения на доску
  piecePlacement.boards[numTransformBoard] |= currentMove.targetBoard;
}

// ищем номер доски доски, которая соответствует текущему ходу 
// с учетом текущего активного цвета
int game_s::getNumBoardByCurrentSourceMoveAndColor() {

  int numBoard = (color == eWhiteColor) ? 0 : 6;
  int numSourceBoard = -1;

  for (int k = numBoard; k < numBoard + 6; ++k) {
    uint64_t board = piecePlacement.boards[k];
    if (board & currentMove.sourceBoard) {
      numSourceBoard = k;
      break;
    }
  }
  return numSourceBoard;
}