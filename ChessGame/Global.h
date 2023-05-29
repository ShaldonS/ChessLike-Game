#pragma once
#include <GL/glut.h>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

enum { Player = 0, Enemy, Empty };

constexpr int w = 600, h = 600;
constexpr int boardSize = 64, rowsNum = 8, numberOfFigures = 9;

/*
enum { Player = 0, Enemy, Empty };

constexpr int w = 600, h = 600;
extern int winner;
constexpr int boardSize = 64, rowsNum = 8, numberOfFigures = 9;

extern int enemyPositions[9];
extern int playerPositions[9];
extern std::vector<int> possibleNewPos;
extern std::vector<int> possibleNewPosEnemy;

extern std::queue<int> enemyQueue;

extern bool playersMove;
extern bool highlightBounds;
extern bool highlightEnemyBounds;
extern bool showWinnerDialog;
extern bool eraseFigure;

extern int erasePos;
extern int lastFigurePos;*/