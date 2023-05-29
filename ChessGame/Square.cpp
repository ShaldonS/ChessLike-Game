#pragma once
#include "Square.h"

int Square::getOwner() {
	return owner;
}

int Square::getVertexX(int i) {
	return coord[i].getX();
}

int Square::getVertexY(int i) {
	return coord[i].getY();
}

void Square::setCoordinates(std::vector<int> coord_vector) {
	if (coord_vector.size() != rowsNum) {
		return;
	}
	int j(0);
	for (size_t i(0); i < 4; ++i) {
		coord[i] = Point(coord_vector[j++], coord_vector[j++]);
	}
}

void Square::draw(float r, float g, float b) {
	glColor3f(r, g, b);

	glBegin(GL_LINES);

	for (size_t i(0); i < 3; ++i) {
		glVertex2i(coord[i].getX(), coord[i].getY());
		glVertex2i(coord[i + 1].getX(), coord[i + 1].getY());
	}
	glVertex2i(coord[0].getX(), coord[0].getY());
	glVertex2i(coord[3].getX(), coord[3].getY());

	glEnd();
}

void Square::reDraw(float r, float g, float b) {
	this->draw(r, g, b);
}

void Square::setOwner(int owner) {
	this->owner = owner;
}

void Square::resetOwner() {
	this->owner = Empty;
}

void Square::drawFigure(bool clearFigure) {
	int mid_y = coord[0].getX() + (coord[1].getX() - coord[0].getX()) / 2;
	int mid_x = coord[1].getY() + (coord[2].getY() - coord[1].getY()) / 2;

	glPointSize(20);
	if (clearFigure) {
		glColor3f(0.5f, 0.5f, 0.5f);
		glPointSize(0);
	}
	glBegin(GL_POINTS);
	glVertex2f((GLfloat)mid_y, (GLfloat)mid_x);
	glEnd();
}

bool Square::checkContainsFigure() {
	return owner == Empty ? false : true;
}

bool Square::checkBoundaries(int x, int y) {
	if (x > coord[0].getX() && y > coord[0].getY())
	{
		if (x < coord[2].getX() && y < coord[2].getY())
		{
			return true;
		}
	}
	return false;
}

bool Square::checkPlayerBoundaries(int x, int y) {
	if (x > coord[0].getX() && y > coord[0].getY())
	{
		if (x < coord[2].getX() && y < coord[2].getY())
		{
			return owner == Player ? true : false;
		}
	}
	return false;
}