#include "../include/level1.h"
#define MAX_OBJECTS 100

static Object objects[MAX_OBJECTS];
static int nbObjects  = 0;
static int currentColor = 0;

/*****************************************************/
void getColor(G3Xcolor color);
void setFractal();

/*****************************************************/

void getColor(G3Xcolor color){
	switch (currentColor){
		case 0:
			color[0] = 255;
			color[1] = 0;
			color[2] = 0;
			break;
		case 1:
			color[0] = 0;
			color[1] = 255;
			color[2] = 0;
			break;
		case 2:
			color[0] = 0;
			color[1] = 0;
			color[2] = 255;
			break;		
	}
	if(currentColor == 2){
		currentColor = 0;
	}else{
		currentColor++;
	}
}

void setFractal(int flag, G3Xhmat transfo, G3Xhmat inverse){
	G3Xvector normal[6] = {
		{1,0,0},
		{-1,0,0},
		{0,1,0},
		{0,-1,0},
		{0,0,1},
		{0,0,-1}
	};
	int i = 0;
	for(i = 0; i<6; i++){
		if(i!=flag){
			G3Xhmat operation;
			g3x_MakeIdentity(operation);
		
		}
	}
}
/****************************************************/
int main(int argc, char* argv[]){
	getColor(objects[nbObjects].color);
	g3x_MakeIdentity(objects[nbObjects].transfo);
	g3x_MakeIdentity(objects[nbObjects].inverse);
	g3x_MakeHomothetieXYZ(objects[nbObjects].transfo,100,100,100);
	g3x_MakeHomothetieXYZ(objects[nbObjects].inverse,0.01,0.01,0.01);
	printMat(objects[nbObjects].transfo);
	printMat(objects[nbObjects].inverse);
	nbObjects++;

	setFractal(-1,objects[nbObjects-1].transfo,objects[nbObjects-1].inverse);

	return 1;
}