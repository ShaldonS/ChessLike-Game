#pragma once
#include "GameEngine.h"

void GameEngine::fillGrid() {
	std::vector<int> coord_vector(8, 0);

	int start_offset = 100;
	int x = start_offset, y = start_offset;
	int cnt = 0;
	int summand = 7;
	for (size_t i(0); i < boardSize; ++i) {
		Square sq;
		int idx = 0;

		if (i % 8 == 0 && i != 0) {
			y = start_offset + 50 * (++cnt);
			x = start_offset;
		}

		// 1 vertex
		coord_vector[idx++] = x;
		coord_vector[idx++] = y;

		y += 50;
		// 2 vertex
		coord_vector[idx++] = x;
		coord_vector[idx++] = y;

		x += 50;
		// 3 vertex
		coord_vector[idx++] = x;
		coord_vector[idx++] = y;

		y -= 50;
		// 4 vertex
		coord_vector[idx++] = x;
		coord_vector[idx++] = y;

		sq.setCoordinates(coord_vector);

		grid[i] = sq;
	}

	// 
	int pos = 0;
	for (size_t i(0); i < 9; ++i, ++pos) {
		if (pos == 3) pos = 8;
		if (pos == 11) pos = 16;
		enemyPositions[i] = pos;
		enemyInitialPositions[i] = pos;
		enemyQueue.push(pos);
	}

	pos = 45;
	for (size_t i(0); i < 9; ++i, ++pos) {
		if (pos == 48) pos = 53;
		if (pos == 56) pos = 61;
		playerPositions[i] = pos;
	}

	for (size_t i(0); i < 9; ++i) {
		grid[enemyPositions[i]].setOwner(Enemy);
		grid[playerPositions[i]].setOwner(Player);
	}
}

void GameEngine::initialize(float r, float g, float b)
{
	glClearColor(r, g, b, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

void GameEngine::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (size_t i(0); i < boardSize; ++i) {
		grid[i].draw(0.0f, 0.0f, 0.0f);
	}

	for (size_t i(0); i < 9; ++i) {
		glColor3f(0.0, 0.0, 0.0);
		grid[calcNewIdx(enemyPositions[i])].drawFigure();
		glColor3f(1.0, 1.0, 1.0);
		grid[calcNewIdx(playerPositions[i])].drawFigure();
	}

	// highlight Player bounds
	if (highlightBounds) {
		for (size_t j(0); j < 4; ++j) {
			if (possibleNewPos[j] != -1) {
				if (!grid[possibleNewPos[j]].checkContainsFigure()) {
					grid[calcNewIdx(possibleNewPos[j])].reDraw(0.0f, 1.0f, 0.0f);
				}
			}
		}
		highlightBounds = false;
	}

	// highlight Enemy bounds
	if (highlightEnemyBounds) {
		for (size_t j(0); j < possibleNewPosEnemy.size(); ++j) {
			if (possibleNewPosEnemy[j] != -1) {
				grid[calcNewIdx(possibleNewPosEnemy[j])].reDraw(1.0f, 0.0f, 0.0f);
			}
		}
		highlightEnemyBounds = false;
	}

	if (eraseFigure) {
		grid[calcNewIdx(erasePos)].drawFigure(true);
		eraseFigure = false;
	}

	if (showWinnerDialog) {
		std::string winnerStr = "";
		if (winner == Player) winnerStr = "Player wins";
		else if (winner == Player) winnerStr = "AI wins";
	}

	glFlush();
}

int GameEngine::calcNewIdx(int idx) {
	int newIdx = idx;

	int pos = idx % 8;

	int summator = 7;
	for (; pos > 0; --pos) {
		summator -= 2;
	}

	newIdx += summator;

	return newIdx;
}

bool GameEngine::checkWin() {
	int enemyPoints = 0;
	int _playerPoints = 0;
	for (size_t i(0); i < boardSize; ++i) {
		for (size_t j(0); j < 9; ++j) {
			int owner = grid[i].getOwner();
			if (owner == Player && i == enemyInitialPositions[j]) {
				_playerPoints++;
			}
			else if (owner == Enemy && i == playerPositions[j]) {
				enemyPoints++;
			}
		}
	}
	playerPoints = _playerPoints;
	if (_playerPoints == 9) winner = Player;
	else if (enemyPoints == 9)  winner = Enemy;

	return winner == Empty ? false : true;
}

std::vector<int> GameEngine::getNeighbourSquaresToMove(int pos) {
	int cnt = 0;
	std::vector<int> vec(4, -1);

	int newPos = -1;

	// down
	newPos = pos + 1;
	if (newPos < boardSize && !((pos + 1) % 8 == 0)) {
		vec[cnt] = newPos;
	}
	cnt++;

	// right
	newPos = pos + rowsNum;
	if (newPos < boardSize) {
		vec[cnt] = newPos;
	}
	cnt++;

	// left
	newPos = pos - rowsNum;
	if (newPos >= 0) {
		vec[cnt] = newPos;
	}
	cnt++;

	// up
	newPos = pos - 1;
	if (newPos >= 0 && !(pos % 8 == 0)) {
		vec[cnt] = newPos;
	}

	return vec;
}

void GameEngine::doEnemysMove() {
	possibleNewPosEnemy = { -1, -1, -1, -1 };
	std::vector<int> newPosEnemy, lastPositions;
	int lastPos = -1, newPos = -1;

	bool check = false;

	//std::random_shuffle(std::begin(enemyPositions), std::end(enemyPositions));
	std::sort(std::begin(enemyPositions), std::end(enemyPositions), std::greater<int>());
	//for(int i : enemyPositions) {
	for (size_t i(0); i < enemyQueue.size(); ++i) {
		int idx = enemyQueue.front();
		enemyQueue.pop();
		possibleNewPosEnemy = getNeighbourSquaresToMove(idx);
		for (size_t j(0); j < 4; ++j) {
			int _pos = possibleNewPosEnemy[j]; // 0 - up, 1 - down, 2 - left, 3 = right
			if (_pos != -1 && !grid[_pos].checkContainsFigure()) {

				newPosEnemy.push_back(_pos);
				lastPositions.push_back(idx);
				check = true;
			}
		}
		if (check) {
			break;
		}
	}
	int size = newPosEnemy.size();
	playersMove = true;



	if (size == 0) {
		if (enemyQueue.size() == 0) {
			for (size_t j(0); j < 9; ++j) {
				enemyQueue.push(enemyPositions[j]);
			}
		}
		return;
	}

	possibleNewPosEnemy = newPosEnemy;

	//int randIdx = rand() % size;
	lastPos = lastPositions[0];
	newPos = newPosEnemy[0];

	for (size_t j(0); j < numberOfFigures; ++j) {
		if (lastPos == enemyPositions[j]) {
			enemyPositions[j] = newPos;
			break;
		}
	}

	if (enemyQueue.size() == 0) {
		for (size_t j(0); j < 9; ++j) {
			enemyQueue.push(enemyPositions[j]);
		}
	}

	if (newPos == 10) {
		int a = 5;
	}

	grid[newPos].setOwner(Enemy);
	grid[lastPos].resetOwner();

	if (newPos == 10) {
		int a = 5;
	}

	eraseFigure = true;
	erasePos = lastPos;
	highlightEnemyBounds = true;
	if (!checkWin()) playersMove = true;
	else {
		glutSetWindowTitle("Winner - AI!");
		playersMove = false;
	}
}

void GameEngine::doPlayerMove(int x, int y) {
	if (!playersMove) {
		return;
	}
	for (size_t i(0); i < boardSize; ++i) {
		if (i == boardSize - 1 && playerPoints == 8) {
			glutSetWindowTitle("No way to move!");
			playersMove = false;
		}
		else if (grid[i].getOwner() == Player) {
			if (grid[i].checkBoundaries(x, y)) {
				possibleNewPos = getNeighbourSquaresToMove(i);

				std::string str = std::to_string(i) + ": ";
				for (size_t j(0); j < 4; ++j) {
					if (!grid[possibleNewPos[j]].checkContainsFigure()) {
						str += std::to_string(possibleNewPos[j]) + ", ";
					}
				}

				glutSetWindowTitle(str.c_str());
				highlightBounds = true;
				lastFigurePos = i;
				break;
			}
		}
		else if (grid[i].getOwner() == Empty) {
			if (!grid[i].checkBoundaries(x, y)) continue;

			int pos = -1;
			for (size_t j(0); j < 4; ++j) {
				if (i == possibleNewPos[j]) {
					pos = i;
					break;
				}
			}
			if (pos == -1) continue;
			possibleNewPos = { -1, -1, -1, -1 };

			playersMove = false;
			grid[pos].setOwner(Player);
			grid[lastFigurePos].resetOwner();

			eraseFigure = true;
			erasePos = lastFigurePos;

			for (size_t j(0); j < 9; ++j) {
				if (lastFigurePos == playerPositions[j]) {
					playerPositions[j] = pos;
					break;
				}
			}
			lastFigurePos = -1;

			if (checkWin()) {
				glutSetWindowTitle("Winner - Player!");
				playersMove = false;
				break;
			}
			doEnemysMove();
			break;
		}
	}
}