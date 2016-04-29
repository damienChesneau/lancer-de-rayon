#ifndef __LEVEL1__
#define __LEVEL1__

#include <g3x.h>
#include <limits.h>

int rayInterSphere(G3Xpoint pos,G3Xvector dir, G3Xpoint ri);
int rayInterTriangle(G3Xpoint pos, G3Xvector dir, G3Xpoint ri);
int rayInterCube(G3Xpoint pos, G3Xvector dir, G3Xpoint ri);
int rayInterCylindre(G3Xpoint pos,G3Xvector dir, G3Xpoint ri);
void initObjects(char* src);
char* getOpt(char* opt,int argc,char* argv[]);
void doLevel1(int argc,char* argv[]);
void doLevel2(int argc,char* argv[]);
void doLevel3(int argc,char* argv[]);
void doLevel4(int argc,char* argv[]);
void save(char * dst);
void printMat(G3Xhmat mat);
void readMat(FILE* fichier,G3Xhmat mat);
void prodMatV(G3Xhmat mat, G3Xvector v,G3Xvector r);
void prodMatP(G3Xhmat mat, G3Xpoint p, G3Xpoint r);
void prodMatM(G3Xhmat a, G3Xhmat b, G3Xhmat r);

typedef struct{
	G3Xcolor color;
	int (*intersection)(G3Xpoint,G3Xvector,G3Xpoint);
	G3Xhmat transfo;
	G3Xhmat inverse;
}Object;

#endif