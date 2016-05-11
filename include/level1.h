#ifndef __LEVEL1__
#define __LEVEL1__

#include <g3x.h>
#include <limits.h>
#include <time.h>
#define IMAGE_SIZE 512
#define MAX_OBJECTS 300
#define MIN_OBJECTS 100

typedef struct{
	int type;
	G3Xcolor color;
	int (*intersection)(G3Xpoint,G3Xvector,G3Xpoint);
	G3Xhmat transfo;
	G3Xhmat inverse;
}Object;

void lvl1_setPixel(G3Xcolor toSet,int i,int j);
Object* lvl1_getObjects();
Object lvl1_getCamera();
int lvl1_getNbObjects();
int lvl1_rayInterSphere(G3Xpoint pos,G3Xvector dir, G3Xpoint ri);
int lvl1_rayInterTriangle(G3Xpoint pos, G3Xvector dir, G3Xpoint ri);
int lvl1_rayInterCube(G3Xpoint pos, G3Xvector dir, G3Xpoint ri);
int lvl1_rayInterCylindre(G3Xpoint pos,G3Xvector dir, G3Xpoint ri);
void lvl1_initObjects(char* src);
char* lvl1_getOpt(char* opt,int argc,char* argv[]);
void lvl1_do(int argc,char* argv[]);
void lvl1_save(char * dst);
void lvl1_printMat(G3Xhmat mat);
void lvl1_readMat(FILE* fichier,G3Xhmat mat);

#endif