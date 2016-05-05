#include "../include/level1.h"
#define MAX_OBJECTS 300
#define MIN_OBJECTS 100

static Object objects[MAX_OBJECTS];
static int nbObjects  = 0;
static int currentColor = 0;
static Object camera;


/*****************************************************/
void getColor(G3Xcolor color);
void setFractal(int nbRecursion, int flag, G3Xvector homothetieValue, G3Xvector translationValue);
void initNewObject(G3Xvector homothetieValue, G3Xvector translationValue);
int getNbRecursion(int min);
void saveFormat();
void fprintMat(FILE * fichier,G3Xhmat mat);
void fprintObject(FILE* fichier, Object object, int type);

/*****************************************************/

void saveFormat(){
	FILE * fichier = fopen("fractal_generator.format","w");
	fprintObject(fichier,camera,4);
	int i = 0;
	for(i = 0; i<nbObjects; i++){
		fprintObject(fichier,objects[i],1);
	}
}

void fprintObject(FILE* fichier, Object object, int type){
	if(type == 4){
		fprintf(fichier,"%d\n\n",type);
	}else{
		fprintf(fichier, "%d %f %f %f\n\n", type, object.color[0], object.color[1], object.color[2]);
	}
	fprintMat(fichier,object.transfo);
	fprintf(fichier,"\n");
	fprintMat(fichier,object.inverse);
	fprintf(fichier,"\n");
}

void fprintMat(FILE * fichier,G3Xhmat mat){
	fprintf(fichier,"%lf %lf %lf %lf\n", mat[0], mat[4], mat[8], mat[12]);
	fprintf(fichier,"%lf %lf %lf %lf\n", mat[1], mat[5], mat[9], mat[13]);
	fprintf(fichier,"%lf %lf %lf %lf\n", mat[2], mat[6], mat[10], mat[14]);
	fprintf(fichier,"%lf %lf %lf %lf\n", mat[3], mat[7], mat[11], mat[15]);
}

int getNbRecursion(int min){
	int i = 0;
	int sum  = 1;
	int value = sum;
	while(sum<min){
		if(sum > MAX_OBJECTS){
			return -1;
		}
		value *= 5;
		sum+= value;
		i+=1;
	}
	return i;
}
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

void setFractal(int nbRecursion, int flag, G3Xvector homothetieValue, G3Xvector translationValue){
	if(nbRecursion == 0){
		return;
	}
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
			G3Xvector newHomothetieValue = {homothetieValue[0]/2.0, homothetieValue[1]/2.0, homothetieValue[2]/2.0};
			G3Xvector newTranslationValue = {
				translationValue[0] + ( normal[i][0] * ((homothetieValue[0] + newHomothetieValue[0]) / 2)),
				translationValue[1] + ( normal[i][1] * ((homothetieValue[1] + newHomothetieValue[1]) / 2)),
				translationValue[2] + ( normal[i][2] * ((homothetieValue[2] + newHomothetieValue[2]) / 2))
			};
			int newFlag = i+1;
			if( (i & 1) == 1){
				newFlag = i-1;
			}
			initNewObject(newHomothetieValue, newTranslationValue);
			setFractal(nbRecursion-1, newFlag, newHomothetieValue, newTranslationValue);
		}
	}
}
/****************************************************/
int main(int argc, char* argv[]){
	G3Xvector initTranslation = {0,0,10};
	G3Xvector initHomothetie = {100,100,100};
	initNewObject(initHomothetie,initTranslation);
	int nbRecursion = getNbRecursion(MIN_OBJECTS);

	if(nbRecursion == -1){
		printf("Number of elements to hight");
		return 0;
	}

	setFractal(nbRecursion,1,initHomothetie,initTranslation);
	g3x_MakeTranslationXYZ(camera.transfo,300,0,0);
	g3x_MakeTranslationXYZ(camera.inverse,-300,0,0);

	saveFormat();
	return 1;
}