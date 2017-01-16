#include "life.h"

// constructor
GameOfLife::GameOfLife(int dimx, int dimy, float density, char* addr, int port, float del){
	worldX = (dimx != -1) ? dimx : 40;
	worldY = (dimy != -1) ? dimy : 40;

	oldWorld = NULL;
	world = makeWorld();
	selection = makeWorld();
	keySquares = makeWorld();
	death = makeWorld();

	selX = worldX/2;
	selY = worldY/2;
	
	paintMark = false;
	paintKey = false;
	paintErase = false;
	paintDead = false;
	paintX = false;

	(density != -1) ? randInit(density) : randInit(.5);

	trans = new Transmitter(addr, port);
	delay = del;

	// screen setup
	initscr();
	getmaxyx(stdscr, screenY, screenX);

	start_color();
	use_default_colors();
	init_pair(NORM, COLOR_WHITE, -1);
	init_pair(MARK, COLOR_WHITE, COLOR_RED);
	init_pair(CURS, COLOR_WHITE, COLOR_GREEN);
	init_pair(PAINT, COLOR_WHITE, COLOR_BLUE);
	init_pair(KEY, COLOR_WHITE, COLOR_YELLOW);
	init_pair(ERASE, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(DEAD, COLOR_WHITE, COLOR_BLACK);

	init_pair(TMARK, COLOR_RED, -1);
	init_pair(TPAINT, COLOR_BLUE, -1);
	init_pair(TKEY, COLOR_YELLOW, -1);
	init_pair(TERASE, COLOR_MAGENTA, -1);
	init_pair(TCURS, COLOR_GREEN, -1);
	init_pair(TDEAD, COLOR_BLACK, -1);

	noecho();
	curs_set(0);
	raw();
	keypad(stdscr, TRUE);
	nodelay(stdscr, true);
}

bool** GameOfLife::makeWorld(){
	bool **newWorld = new bool*[worldX];

	for (int i = 0; i < worldX; i++){
		newWorld[i] = new bool[worldY];
	}
	for(int i = 0; i < worldX; i++){
		for(int j = 0; j < worldY; j++){
			newWorld[i][j] = false;
		}
	}
	return newWorld;
}

void GameOfLife::deleteWorld(bool ** b){
	for (int j = 0; j < worldX; j++){
		delete[] b[j];
	}
	delete[] b;
}

void GameOfLife::put(int i, int j, bool value){
	int putX, putY;
	if(i < 0){
		putX = i+worldX;
	}else if(i >= worldX){
		putX = i%worldX;
	}else{
		putX = i;
	}
	if(j < 0){
		putY = j+worldY;
	}else if(j >= worldY){
		putY = j%worldY;
	}else{
		putY = j;
	}
	world[putX][putY] = value;
}

bool GameOfLife::check(int i, int j){
	int checkX, checkY;
	if(i < 0){
		checkX = i+worldX;
	}else if(i >= worldX){
		checkX = i%worldX;
	}else{
		checkX = i;
	}
	if(j < 0){
		checkY = j+worldY;
	}else if(j >= worldY){
		checkY = j%worldY;
	}else{
		checkY = j;
	}
	return world[checkX][checkY];
}

int GameOfLife::countNeighbors(int i, int j){
	int neighbors = 0;

	// check north
	if(check(i, j-1)) neighbors++;
	// check northeast
	if(check(i+1, j-1)) neighbors++;
	// check east
	if(check(i+1, j)) neighbors++;
	// check southeast
	if(check(i+1, j+1)) neighbors++;
	// check south
	if(check(i, j+1)) neighbors++;
	// check southwest
	if(check(i-1, j+1)) neighbors++;
	// check west
	if(check(i-1, j)) neighbors++;
	// check northwest
	if(check(i-1, j-1)) neighbors++;
	
	return neighbors;
}

void GameOfLife::update(){
	int neighbors;
	bool **newWorld = makeWorld();
	for(int i = 0; i < worldX; i++){
		for(int j = 0; j < worldY; j++){
			if(death[i][j]){
				newWorld[i][j] = false;
			}else{
				neighbors = countNeighbors(i, j);
				if(world[i][j]){ // if alive
					// 2 or 3 neighbors life, all else dies
					if(neighbors == 2 || neighbors == 3){
						newWorld[i][j] = true;
					}else{
						newWorld[i][j] = false;
					}
				}else{
					// 3 neighbors live, all else dies
					if(neighbors == 3){
						newWorld[i][j] = true;
					}else{
						newWorld[i][j] = false;
					}
				}
			}
		}
	}

	if(oldWorld != NULL){ deleteWorld(oldWorld); }

	oldWorld = world;
	world = newWorld;
}

void GameOfLife::draw(){
	char square = ' ';
	population = 0;
	for(int j = 0; j < worldY; j++){
		for(int i = 0; i < worldX; i++){
			// check the world
			if(world[i][j]){
				square = 'X';
				population++;
			}else{
				square  = ' ';
			}

			// color the cursor square
			if(i == selX && j == selY){
				if(paintX){
					attron(COLOR_PAIR(PAINT));
				}else if(paintMark){
					attron(COLOR_PAIR(MARK));
				}else if(paintKey){
					attron(COLOR_PAIR(KEY));
				}else if(paintErase){
					attron(COLOR_PAIR(ERASE));
				}else if(paintDead){
					attron(COLOR_PAIR(DEAD));
				}else{
					attron(COLOR_PAIR(CURS));
				}
			}

			// color other squares
			else if(selection[i][j]){
				attron(COLOR_PAIR(MARK));
			}else if(keySquares[i][j]){
				attron(COLOR_PAIR(KEY));
			}else if(death[i][j]){
				attron(COLOR_PAIR(DEAD));
			}else{
				attron(COLOR_PAIR(NORM));
			}

			mvprintw((screenY/2)-(worldY/2)+j, (screenX/2)-(worldX)+(i*2), " %c",square);

			if(i == selX && j == selY){
				if(paintX){
					attroff(COLOR_PAIR(PAINT));
				}else if(paintMark){
					attroff(COLOR_PAIR(MARK));
				}else if(paintKey){
					attroff(COLOR_PAIR(KEY));
				}else if(paintErase){
					attroff(COLOR_PAIR(ERASE));
				}else if(paintDead){
					attroff(COLOR_PAIR(DEAD));
				}else{
					attroff(COLOR_PAIR(CURS));
				}
			}
			else if(selection[i][j]){
				attroff(COLOR_PAIR(MARK));
			}else if(keySquares[i][j]){
				attroff(COLOR_PAIR(KEY));
			}else if(death[i][j]){
				attroff(COLOR_PAIR(DEAD));
			}else{
				attroff(COLOR_PAIR(NORM));
			}
		}
	}
	refresh();
}

void GameOfLife::randInit(float density){
	std::cout << "density: " << density << std::endl;
	int prob = (int) (density*100);
	std::cout << "probability: " << prob << std::endl;
	for(int i = 0; i < worldX; i++){
		for(int j = 0; j < worldY; j++){
			if((std::rand() % 100) < prob){
				world[i][j] = true;
			}else{
				world[i][j] = false;
			}
		}
	}
}


// each note will be the duration of one cycle
void GameOfLife::sendNotes(){
	bool key;
	for(int i = 0; i < worldX; i++){
		for(int j = 0; j < worldX; j++){
			if(world[i][j]){
				key = keySquares[i][j];
				if(!key){ // non keysquares are always sent
					trans->send("life", worldY-j, delay*1000, ctrl1(i), ctrl2(j), ctrl3());
				}else if(world[i][j] != oldWorld[i][j]){ //keysquares are only sent if they are new
					trans->send("key", worldY-j, delay*10000, ctrl1(i), ctrl2(j), ctrl3());
				}
			}
		}
	}
}

float GameOfLife::ctrl1(int i){
	return ((float) i)/((float) worldX);
}

float GameOfLife::ctrl2(int j){
	return ((float) j)/((float) worldY);
}

float GameOfLife::ctrl3(){
	return ((float)population)/((float)(worldX*worldY)/2);
}


float GameOfLife::handleInput(){
	int key = getch();

	// handle moving
	if(key == 259){
		//up
		selY = std::max(0, selY-1);
	}else if(key == 261){
		//right
		selX = std::min(worldX-1, selX+1);
	}else if(key == 258){
		//down
		selY = std::min(worldY-1, selY+1);
	}else if(key == 260){
		//left
		selX = std::max(0, selX-1);
	}

	// handle placing x
	if(key == 120){
		//x
		world[selX][selY] = true;
	}

	// handle painting x
	if(key == 88){
		//X
		paintX = !paintX;
	}
	if(paintX){
		world[selX][selY] = true;
	}

	// handle marking
	if(key == 109){
		//m
		selection[selX][selY] = !selection[selX][selY];
	}

	// handle mark painting
	if(key == 77){
		//M
		paintMark = !paintMark;
	}

	if(paintMark){
		selection[selX][selY] = true;
	}

	// handle keySquares
	if(key == 107){
		//k
		keySquares[selX][selY] = !keySquares[selX][selY];
	}
	
	// handle keysquare painting
	if(key == 75){
		//K
		paintKey = !paintKey;
	}
	if(paintKey){
		keySquares[selX][selY] = true;
	}

	// handle death
	if(key == 100){
		//d
		death[selX][selY] = !death[selX][selY];
	}

	// handle death painting
	if(key == 68){
		//D
		paintDead = !paintDead;
	}
	if(paintDead){
		death[selX][selY] = true;
		keySquares[selX][selY] = false;
		selection[selX][selY] = false;
	}

	// handle erasing
	if(key == 101){
		//e
		paintErase = !paintErase;
	}
	if(paintErase){
		keySquares[selX][selY] = false;
		selection[selX][selY] = false;
		death[selX][selY] = false;
	}

	// handle stamps
	if(key == 103){
		//g -> glider
		put(selX-1, selY-1, false);	// NW
		put(selX, selY-1, false);	// N
		put(selX+1, selY-1, true);	// NE
		put(selX-1, selY, true);	// E
		put(selX, selY, false);		// self
		put(selX+1, selY, true);	// W
		put(selX-1, selY+1, false);	// SW
		put(selX, selY+1, true);	// S
		put(selX+1, selY+1, true);	// SE
	}

	// handle applying
	if(key == 10){
		//enter
		for(int i = 0; i < worldX; i++){
			for(int j = 0; j < worldY; j++){
				if(selection[i][j]){
					world[i][j] = true;
				}
				selection[i][j] = false;
			}
		}
	}

	// handle speed up
	if(key == 83){
		//S
		delay *= .9;
		return delay;
	}

	// handle slow down
	if(key == 115){
		//s
		delay *= 1.1;
		return delay;
	}


	// handle freezing
	if(key == 102){
		//f
		return -1; //freeze
	}

	// handle exit
	if(key == 3){
		return -2; //exit
	}

	return 0; //ok
}

void GameOfLife::showControls(){
	move(0, 0);
	printw("arrow keys - move ");
	attron(COLOR_PAIR(TCURS));
	printw("cursor\n");
	attroff(COLOR_PAIR(TCURS));

	printw("x - place x\n");

	printw("X - ");
	attron(COLOR_PAIR(TPAINT));
	printw("paint");
	attroff(COLOR_PAIR(TPAINT));
	printw(" x's\n");

	printw("m - place ");
	attron(COLOR_PAIR(TMARK));
	printw("mark\n");
	attroff(COLOR_PAIR(TMARK));

	printw("M - paint ");
	attron(COLOR_PAIR(TMARK));
	printw("marks\n");
	attroff(COLOR_PAIR(TMARK));

	printw("enter - apply marks\n");

	printw("k - place ");
	attron(COLOR_PAIR(TKEY));
	printw("key square\n");
	attroff(COLOR_PAIR(TKEY));

	printw("K - paint ");
	attron(COLOR_PAIR(TKEY));
	printw("key squares\n");
	attroff(COLOR_PAIR(TKEY));

	printw("d - place ");
	attron(COLOR_PAIR(TDEAD));
	printw("dead square\n");
	attroff(COLOR_PAIR(TDEAD));

	printw("D - paint ");
	attron(COLOR_PAIR(TDEAD));
	printw("dead squares\n");
	attroff(COLOR_PAIR(TDEAD));

	printw("e - ");
	attron(COLOR_PAIR(TERASE));
	printw("erase\n");
	attroff(COLOR_PAIR(TERASE));

	printw("g - make glider\n");
	printw("f - freeze\n");
	printw("S - speed up\n");
	printw("s - slow down\n");
	printw("ctrl-c - quit\n");
}

float GameOfLife::run(bool up){
	if(up){
		update();
	}
	action = handleInput();
	draw();
	if(up){ // do I want to send notes even when there's no update?
		sendNotes();
	}
	if(action == -2) endwin();
	return action;
}
