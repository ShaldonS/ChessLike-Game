#pragma once
#include "Global.h"
#include "Square.h"

class GameEngine {
	int winner = Empty;

	int enemyPositions[9];
	int playerPositions[9];
	std::vector<int> possibleNewPos = { -1, -1, -1, -1 };
	std::vector<int> possibleNewPosEnemy = { -1, -1, -1, -1 };

	std::queue<int> enemyQueue;

	bool playersMove = true;
	bool highlightBounds = false;
	bool highlightEnemyBounds = false;
	bool showWinnerDialog = false;
	bool eraseFigure = false;

	int erasePos = -1;
	int lastFigurePos = -1;

	Square grid[boardSize];
	int enemyInitialPositions[9];

	int playerPoints = 0;
public:
	GameEngine() {}
	~GameEngine() {}

	void fillGrid();
	void initialize(float r, float g, float b);
	void display(void);

	int calcNewIdx(int idx);

	bool checkWin();

	std::vector<int> getNeighbourSquaresToMove(int pos);

	void doEnemysMove();
	void doPlayerMove(int x, int y);
};