#pragma once
#include "GameEngine.h"

GameEngine game;

void display(void) {
	game.display();
}

void mouseClick(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON) {
		game.doPlayerMove(x, y);
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ChessLike Game");

	game.fillGrid();
	game.initialize(0.5, 0.5, 0.5);

	glutDisplayFunc(display);
	glutMouseFunc(mouseClick);

	glutMainLoop();
	return 0;
}