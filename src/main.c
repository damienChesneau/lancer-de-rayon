#include <g3x.h>

typedef struct{
	G3Xpoint a;
	G3Xpoint b;
	G3Xpoint c;
}Triangle;

typedef struct{
	G3Xpoint centre;
	double rayon;
}Sphere;

typedef struct{
	G3Xpoint depart;
	G3Xvector longueur;
	G3Xvector largeur;
	G3Xvector hauteur;
}Rectangle;

typedef struct{
	G3Xpoint centre;
	double rayon;
	G3Xvector hauteur;
}Cylindre;

typedef struct{
	G3Xpoint pos;
	G3Xvector dir;
}Rayon;

typedef struct{
	void *this;
	G3Xhmat transform;
	G3Xhmat inverse;
	int (*intersection)(G3Xpoint,G3Xvector,G3Xpoint);
}Object;


Triangle triangle = {	
	{0,0,0}, 
	{1,0,0}, 
	{0,1,0}	
};

Sphere sphere = {
	{0,0,0},
	1
};

Rectangle rectangle = {
	{0,0,0},
	{1,0,0},
	{0,1,0},
	{0,0,1},
};


Cylindre cylindre = {
	{0,0,0},
	1,
	{0,0,1}
};

G3Xcamera camera;
Object objects[100];
int nbObjects = 0;
float image[32][32];
/*****************************************************************************************************/



/****************************************************************************************************/
void doLevel1(int argc,char* argv[]){
	int i = 0;
	int j = 0;
	for(i = 0; i<32; i++){
		for(j = 0; j<32; j++){

		}
	}
}

void doLevel2(int argc,char* argv[]){
	printf ("Unimplemented yet");
}

void doLevel3(int argc,char* argv[]){
	printf ("Unimplemented yet");
}

void doLevel4(int argc,char* argv[]){
	printf ("Unimplemented yet");
}

char* getOpt(char* opt,int argc,char* argv[]){
	int i = 0;	
	for(i = 1; i<argc; i++){
		if(strcmp(argv[i],opt) == 0){
			return argv[i+1];
		}
	}
}

int main(int argc, char* argv[]){
	int level = atoi(getOpt("-n",argc,argv));
	switch(level){
		case 1:	
			doLevel1(argc,argv);
			break;


		case 2:	
			doLevel2(argc,argv);
			break;


		case 3:	
			doLevel3(argc,argv);
			break;


		case 4:	
			doLevel4(argc,argv);
			break;

		default:
			printf("Unknown level.\n");
			break;
	}
	return 1;
}