﻿// 27.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>

using namespace std;

// Начальные данные : одна строка в виде a / b + c / d.
// Строка записана именно в таком формате, вместо букв записаны числа, каждое число от 1 до 10000.
// Вывод результата : ответ в виде x / y, представляющих собой несократимую дробь.
// Максимальное значение дроби 1 / 1.
//13:48 -14:07

int gcd(int x, int y) {
  if (x == y)
    return x;
  if(x==0)
    return y;
  if (y == 0)
    return x;
  int dx = x % 2;
  int dy = y % 2;
  if (!dx && !dy)
    return 2 * gcd(x / 2, y / 2);
  if(!dx && dy)
    return  gcd(x / 2, y);
  if (dx && !dy)
    return gcd(x, y/2);
  if(x>y)
    return gcd((x-y)/2, y);
  else
    return gcd(x, (y-x)/2);
}

int main() {
  int a, b, c, d;
  unsigned char symb;
  cin >> a >> symb >> b >> symb >> c >> symb >> d;
  int x = a * d + b * c, y = b * d;

  int result = gcd(x, y);
  x /= result;
  y /= result;
  cout << x << "/" << y;
  return 0;
}

// Входные данные
// В первой строке вводится натуральное число N не больше 100 - размер квадратной матрицы.
// В следующих N строках задаются элементы матрицы, по N чисел через пробел на каждой.
// 
// Выходные данные
// Вывести единственное число - количество островов.
// 12:55 - 13:55 (-5мин)

int land[100][100];
int NN;
int island(int y, int x) {
  if (y < 0 || y >= NN)
    return 0;
  if (x < 0 || x >= NN)
    return 0;
  if (land[y][x]) {
    land[y][x] = 0;
    island(y + 1, x);
    island(y - 1, x);
    island(y, x + 1);
    island(y, x - 1);
    return 1;
  }
  return 0;
}

int island()
{  
  cin >> NN;
  int S = 0;
  for (int y = 0; y < NN; ++y) {
    for (int x = 0; x < NN; ++x) {
      cin >> land[y][x];
    }
  }

  int count = 0;

  for (int y = 0; y < NN; ++y) {
    for (int x = 0; x < NN; ++x) {
      if (island(y, x))
        count++;
    }
  }
  cout << count;
  return 0;
}

// 5x8 12:32-12:50

// Новогодняя ёлочка программиста состоит из цифр.
// На первой строке записана высота ёлочки -
// натуральное число от 1 до 100.
// Далее на N строчках зарисована сама ёлочка,
// сначала 1 цифра, потом 2, на последней строке - N цифр через пробел.
// В начале и в конце каждой строки могут быть пробелы, чтобы ёлочка получилась красивой.
// 
// Гирляндой называется последовательность цифр, которую можно получить спускаясь от верхушки
// ёлки до её подножья, каждый раз смещаясь либо влево - вниз, либо вправо вниз.
// 
// Найти гирлянду с максимальной суммой цифр.
// 
// Вывести одно число - максимальную сумму найденной гирлянды.

// 0:57-1:29
int firTree()
{
  int x[100][100];
  int N;
  cin >> N;
  for (int row = 1; row <= N; ++row)
    for (int col = 0; col < row; ++col) {
      cin >> x[row-1][col];
    }

  int result[100];
  for (int col = 0; col < N; ++col) {
    result[col] = x[N - 1][col];
  }

  for (int row = N - 2; row >= 0; --row) {
    for (int col = 0; col < row + 1; ++col) {
      int left, right;
      left = x[row + 1][col];
      right = x[row + 1][col + 1];
      result[col] = x[row][col] + ((left > right) ? left : right);
    }
    for (int col = 0; col < row + 1; ++col)
      x[row][col] = result[col];
  }
  cout << x[0][0];
  return 0;
}


int16_t saraij[1000][1000];
int16_t saraijLen[1000][1000];
int16_t N, M, T;

// Начальные данные : Вводится матрица размера N × M из 0 и 1.
// 1 соответствует постройке, 0 - пустой клетке.
// На первой строке вводится размер матрицы N M(через пробел) от 1 до 30.
// Элементы матрицы вводятся на M строках по N элементов через пробел.
// Вывод результата : 1 число, соответствующее максимальной площади сарая(количество ячеек).
// 0:03-0:42
// проверить пуст ли прямоугольник на позиции верх/лево + высота + ширина
bool isEmptyRect(uint16_t yy, uint16_t xx, uint16_t y, uint16_t x) {
  for (int j = yy; j < yy + y; ++j) {
    for (int i = xx; i < xx + x; ++i) {    
      if (saraij[j][i] == 1)
        return false;
    }
  }
  return true;
}

int small_saraij(int argc, char *argv[]) {

  cin >> N >> M;
  int S = 0;
  for (int y = 0; y < M; ++y) {
    for (int x = 0; x < N; ++x) {    
      cin >> saraij[y][x];
    }
  }

  for (int y = 1; y <= M; ++y) {
    for (int x = 1; x <= N; ++x) {

      for (int yy = 0; yy < M-y; yy++) {
        for (int xx = 0; xx < N-x; xx++) {
          
          if (isEmptyRect(yy, xx, y, x)) {
            int s = y * x;
            if (s > S)
              S = s;
          }            
        }
      }

    }
  }
  cout << S << endl;

  return 0;
}

// считаем длину сверху вниз
void  calcLength() {

  // проставляем начальные условия  
  for (int x = 0; x < N; ++x) {
    saraijLen[0][x] = saraij[0][x] == 1 ? 0 : 1;
  }

  for (int y = 1; y < M; ++y) {
    for (int x = 0; x < N; ++x) {
      if (saraij[y][x] == 0)
        saraijLen[y][x] = saraijLen[y - 1][x] + 1;
      else
        saraijLen[y][x] = 0;
    }
  }
}

// Начальные данные :
// На первой строке вводится размер матрицы N M(через пробел) от 1 до 1000.
// На второй строке вводится количество построек T(от 0 до 10000).
// Далее на T строчках вводится координаты построек
// по два числа X Y, где 0 <= X < N; 0 <= Y < M.
// Вывод результата : 1 число, соответствующее максимальной площади сарая(количество ячеек).
// 23:09 - 23:29
int greate_saraij(int argc, char *argv[]) {

  cin >> N >> M >> T;

  for (int x = 0; x < N; ++x) {
    for (int y = 0; y < M; ++y) {
      saraij[y][x] = 0;
    }
  }

  for (int k = 0; k < T; ++k) {
    int x, y;
    cin >> x >> y;
    saraij[y][x] = 1;
  }

  calcLength();

  int S = 0;
  uint16_t L, R;

  for (int y = 0; y < M; ++y) {

    for (int x = 0; x < N; ++x) {
      L = R = x;

      for (int k = x - 1; k >= 0; --k) {
        if (saraijLen[y][k] >= saraijLen[y][x])
          L = k;
        else
          break;
      }

      for (int k = x + 1; k < N; ++k) {
        if (saraijLen[y][x] <= saraijLen[y][k])
          R++;
        else
          break;
      }

      // считаем макс ширину и сразу же вычисляем площадь
      int width = R - L + 1;
      int s = width * saraijLen[y][x];
      if (s > S)
        S = s;
    }
  }
  cout << S;

  return 0;
}

// Начальные данные : массив A.
// На первой строке записано число N от 1 до 10000.
// Далее на N строчках записаны элементы массива A, каждое от 0 до 10000.
// 
// Вывод результата : массив L и R.
// Построить две новые последовательности чисел по следующему правилу.
// 
// Массив L.L[j] = x.
// Для каждого элемента A[j] определить индекс x наиболее отдаленного элемента слева, который больше или равен A[j].
// 
// И то же самое на в другую сторону :
// 
// Массив R.R[j] = x.
// Для каждого элемента А[j] определить индекс x наиболее отдаленного элемента справа, который больше или равен А[j].
// 
// Индексы элементов начинаются с 0.
// Вывести каждый массив на отдельной строчке, разделяя числа пробелами.
// В конце строк пробелов быть не должно.
// 22:12 - 22:39
int width(int argc, char *argv[])
{
  int N;
  uint16_t cell[10000];
  uint16_t L[10000], R[10000];

  if (argc == 2) {
    ifstream ss(argv[1]);
    if (ss.is_open()) {

      ss >> N;
      for (int i = 0; i < N; i++) {
        ss >> cell[i];
      }

    }
  }
  else {
    cin >> N;
    for (int i = 0; i < N; i++) {
      cin >> cell[i];
    }
  }
   
  for (int i = 0; i < N; ++i) {
    L[i] = R[i] = i;

    for (int k = i - 1; k >= 0; --k) {
      if (cell[k] >= cell[i])
        L[i] = k;
      else
        break;
    }

    for (int k = i + 1; k < N; ++k) {
      if (cell[i] <= cell[k])
        R[i]++;
      else
        break;
    }
  }

  for (int i = 0; i < N-1; ++i) {
    cout << L[i] << " ";
  }
  cout << L[N-1] << endl;
  for (int i = 0; i < N - 1; ++i) {
    cout << R[i] << " ";
  }
  cout << R[N - 1] << endl;
  return 0;
}

// Начальные данные :
// На первой строке вводится размер матрицы N M(через пробел) от 1 до 1000.
// На второй строке вводится количество построек T(от 0 до 10000) (занятых клеток).
// Далее на T строчках вводится координаты построек
// по два числа X Y, где 0 <= X < N; 0 <= Y < M для каждой занятой клетки.
// Вывод результата : Матрица N x M из чисел -
// сколько "выше" есть свободных клеток подряд, начиная с указанной клетки поля.
// Всего M строк, по N чисел на каждой, записанных через пробел.
// В конце строк лишних пробелов быть не должно!

// 19:37-20:47
int length(int argc, char *argv[])
{
  int16_t N, M, count;
  int16_t sarn[500][500];

  if (argc == 2) {
    ifstream ss(argv[1]);
    if (ss.is_open()) {

      ss >> N >> M >> count;

      for (int x = 0; x < N; ++x) {
        for (int y = 0; y < M; ++y) {
          sarn[y][x] = 0;
        }
      }

      for (int k = 0; k < count; ++k) {
        int x, y;
        ss >> x >> y;
        sarn[y][x] = 1;
      }

    }
  }
  else {

    cin >> N >> M >> count;

    for (int x = 0; x < N; ++x) {
      for (int y = 0; y < M; ++y) {
        sarn[y][x] = 0;
      }
    }

    for (int k = 0; k < count; ++k) {
      int x, y;
      cin >> x >> y;
      sarn[y][x] = 1;
    }

  }

  int16_t sarnLen[500];

  // проставляем начальные условия  
  for (int x = 0; x < N; ++x) {
    sarnLen[x] = sarn[0][x] == 1 ? 0 : 1;
    cout << sarnLen[x];
    if (x != (N - 1))
      cout << " ";
  }
  cout << endl;

  for (int y = 1; y < M; ++y) {
    for (int x = 0; x < N; ++x) {
      int xx;
      if (sarn[y][x] == 0)
        xx = sarnLen[x] + 1;
      else
        xx = 0;
      cout << xx;
      sarnLen[x] = xx;
      if (x != (N - 1))
        cout << " ";
    }
    cout << endl;
  }

  return 0;
}


// Задачи на динамическое программирование
// Цель : Четыре маленькие задачи. + 4 балла Одна большая задача про сарай + 6 баллов
//   1 часть.Четыре маленькие задачи. + 4 балла.
//   Решить 4 задачи по материалам вебинара,
//   желательно на разных языках программирования.
// 
//   Можно использовать авторскую систему тестирования
//   для проверки правильности решения, страница авторизации :
// https://www.robotsharp.info/otus/
// 
// 1. Раз / два горох + 1 балл
// https ://www.robotsharp.info/index.php?page=TaskInfo&taskId=3704
// 
// 2. Цифровая ёлочка + 1 балл
// https ://www.robotsharp.info/index.php?page=TaskInfo&taskId=3707
// 
// 3. Пятью - восемь + 1 балл
// https ://www.robotsharp.info/index.php?page=TaskInfo&taskId=3706
// 
// 4. Острова + 1 балл.
// https ://www.robotsharp.info/index.php?page=TaskInfo&taskId=3708
// 
//   Укажите в отчёте, сколько времени ушло на решение этих задач.
// 
//   2 часть.Большой и маленький сарай. + 6 баллов.
//   Фермер хочет построить на своей земле как можно больший по площади сарай.
//   Но на его участке есть деревья и хозяйственные постройки, которые он не хочет никуда переносить.
//   Для удобства представим ферму сеткой размера N × M.
//   Каждое из деревьев и построек размещается в одном или нескольких узлах сетки.
//   Найти максимально возможную площадь сарая и где он может размещаться.
//   Начальные данные : Вводится матрица размера N × M из 0 и 1.
//   1 соответствует постройке, 0 - пустой клетке.
// 
//   Начальные данные :
// На первой строке вводится размер матрицы N M(через пробел) от 1 до 1000.
// На второй строке вводится количество построек T(от 0 до 10000).
// Далее на T строчках вводится координаты построек по два числа X Y, где 0 <= X < N; 0 <= Y < M.
// 
//   Вывод результата :
// Число, соответствующее максимальной площади сарая(количество ячеек).
// 
// Решение задачи разделено на 4 этапа :
//   1. Маленький сарай.Решение задачи простым перебором, +1 балл.
//   https ://www.robotsharp.info/index.php?page=TaskInfo&taskId=1285
//   Укажите в отчёте, сколько времени ушло на решение этой задачи.
// 
//   2. Длина сарая.Вычисление максимальной длины сарая для каждой клетки, +2 балла
//   https ://www.robotsharp.info/index.php?page=TaskInfo&taskId=1739
// Укажите в отчёте, сколько времени ушло на решение этой задачи.
// 
// 3. Ширина сарая.Вычисление максимально доступной ширины для выбранной длины. + 2 балла.
// https ://www.robotsharp.info/index.php?page=TaskInfo&taskId=1741
//   Укажите в отчёте, сколько времени ушло на решение этой задачи.
// 
//   4. Большой сарай.Совмещение всех решений в одну программу. + 1 балл.
//   https ://www.robotsharp.info/index.php?page=TaskInfo&taskId=1738
//   Укажите в отчёте, сколько времени ушло на решение этой задачи.
//   Критерии оценки : Указано в условии.
//   Рекомендуем сдать до : 28.11.2019