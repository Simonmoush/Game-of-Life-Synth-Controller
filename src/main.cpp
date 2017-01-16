#include <cstdlib>
#include <iostream>
#include "life.h"
#include <unistd.h>

char* addr = NULL;
int port = -1;
int dimx = -1;
int dimy = -1;
float density = -1;
float delay = .3;

void parseArgs(int argc, char* argv[]){
	for(int i = 1; i < argc; i++){
		if((strcmp(argv[i],"-a") == 0) && (i+1 < argc)){
			addr = new char[strlen(argv[i+1])];
			strcpy(addr, argv[i+1]);
			i++;
		}else if((strcmp(argv[i],"-p") == 0) && (i+1 < argc)){
			port = atoi(argv[i+1]);
			i++;
		}else if((strcmp(argv[i],"-x") == 0) && (i+1 < argc)){
			dimx = atoi(argv[i+1]);
			i++;
		}else if((strcmp(argv[i],"-y") == 0) && (i+1 < argc)){
			dimy = atoi(argv[i+1]);
			i++;
		}else if((strcmp(argv[i],"-t") == 0) && (i+1 < argc)){
			delay = (atof(argv[i+1]));
			i++;
		}else if((strcmp(argv[i],"-d") == 0) && (i+1 < argc)){
			density = atof(argv[i+1]);
			i++;
		}else{
			std::cout << "options:\n\t"
					<< "-a\taddress of sound generator\n\t"
					<< "-p\tport of sound generator\n\t"
					<< "-x\tx dimension\n\t"
					<< "-y\ty dimension\n\t"
					<< "-t\ttime delay\n\t"
					<< "-d\tdensity"
			<< std::endl;
			exit(0);
		}
	}
}

int main(int argc, char* argv[]){
	parseArgs(argc, argv);
	GameOfLife* L = new GameOfLife(dimx, dimy, density, addr, port, delay);
	L->showControls();
	bool loop = true;
	bool up = false;
	bool paused = false;
	float runReturn;
	clock_t start = clock();
	while(loop){
		if(((((float)(clock() - start))/CLOCKS_PER_SEC) > delay) && !paused){
			up = true;
			start = clock();
		}else{
			up = false;
		}

		runReturn = L->run(up);

		if(runReturn == -2){ // quit signal
			loop = false;
		}else if(runReturn == -1){ // pause signal
			paused = !paused;
		}else if(runReturn != 0){ // change speed
			delay = runReturn;
		}

	}
	return 0;
}
