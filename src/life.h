#ifndef LIFE_H
#define LIFE_H

#include <iostream>
#include <cstdlib>
#include "transmitter.h"
#include <ncurses.h>
#include <algorithm>

#define NORM 1
#define MARK 2
#define CURS 3
#define PAINT 4
#define KEY 5
#define ERASE 6
#define DEAD 12


#define TMARK 7
#define TPAINT 8
#define TKEY 9
#define TERASE 10
#define TCURS 11
#define TDEAD 13

class GameOfLife{
	private:
	//data
	int worldX, worldY;
	bool** world;
	bool** oldWorld;
	bool** selection;
	bool** keySquares;
	bool** death;
	int population;

	Transmitter* trans;

	//dysplay info
	int screenX, screenY;
	float delay;

	// selection info
	int selX, selY;
	bool paintMark;
	bool paintKey;
	bool paintErase;
	bool paintDead;
	bool paintX;

	// interaction
	float action;

	//methods
	bool** makeWorld();
	void deleteWorld(bool **b);
	bool check(int i, int j);
	int countNeighbors(int i, int j);
	void update();
	void draw();
	void randInit(float density);
	void sendNotes();
	float ctrl1(int i);
	float ctrl2(int j);
	float ctrl3();
	float handleInput();
	void put(int i, int j, bool value);

	
	public:
	GameOfLife(int dimx, int dimy, float density, char* addr, int port, float del);
	void showControls();
	float run(bool up);
};

#endif
