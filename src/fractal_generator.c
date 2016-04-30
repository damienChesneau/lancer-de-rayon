#include "../include/level1.h"
#define MAX_OBJECTS 100

static Object objects[MAX_OBJECTS];
static int nbObjects  = 0;
static int currentColor = 0;

/*****************************************************/
void getColor(G3Xcolor color);
void setFractal(int flag, G3Xvector homothetieValue, G3Xvector translationValue);
void initNewObject(G3Xvector homothetieValue, G3Xvector translationValue);

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

void initNewObject(G3Xvector homothetieValue, G3Xvector translationValue){
	getColor(objects[nbObjects].color);

	G3Xhmat translation;
	G3Xhmat homothetie;
	G3Xvector invertedTranslationValue = {-translationValue[0], -translationValue[1], -translationValue[2]};
	G3Xvector invertedHomothetieValue = {1/homothetieValue[0], 1/homothetieValue[1], 1/homothetieValue[2]};


	g3x_MakeHomothetieV(homothetie,homothetieValue);
	g3x_MakeTranslationV(translation,translationValue);
	g3x_ProdHMat(translation,homothetie,objects[nbObjects].transfo);

	
	g3x_MakeHomothetieV(homothetie,invertedHomothetieValue);
	g3x_MakeTranslationV(translation,invertedTranslationValue);
	g3x_ProdHMat(homothetie,translation,objects[nbObjects].inverse);

	nbObjects++;
}

void setFractal(int flag, G3Xvector homothetieValue, G3Xvector translationValue){
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
			
		}
	}
}
/****************************************************/
int main(int argc, char* argv[]){
	G3Xvector initTranslation = {0,0,10};
	G3Xvector initHomothetie = {100,100,100};
	initNewObject(initHomothetie,initTranslation);
	printMat(objects[nbObjects-1].transfo);
	printMat(objects[nbObjects-1].inverse);


	setFractal(-1,initHomothetie,initTranslation);

	return 1;
}