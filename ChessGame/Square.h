#pragma once
#include "Global.h"

/*
* [1]---[2]
*  |     |
*  |     |
* [0]---[3]
*/

class Point {
	int x = 0, y = 0;
public:
	Point() {};
	Point(int x, int y) : x(x), y(y) {};
	~Point() {};

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}
};

class Square {
	Point coord[4];
	int owner = Empty;
public:
	Square() {};
	~Square() {};

	int getOwner();
	int getVertexX(int i);
	int getVertexY(int i);

	void setCoordinates(std::vector<int> coord_vector);
	void draw(float r, float g, float b);
	void reDraw(float r, float g, float b);
	void setOwner(int owner);
	void resetOwner();
	void drawFigure(bool clearFigure = false);

	bool checkContainsFigure();
	bool checkBoundaries(int x, int y);
	bool checkPlayerBoundaries(int x, int y);
};