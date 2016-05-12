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

/*Place la couleur toSet à la pixel (i,j) de l'image finale*/
void lvl1_setPixel(G3Xcolor toSet,int i,int j);

/*renoive la liste d'objets chargées*/
Object* lvl1_getObjects();

/*renvoie la caméra*/
Object lvl1_getCamera();

/*renvoie le nombre d'objets dans la scènre*/
int lvl1_getNbObjects();

/*Vérifie l'intersection d'un rayon avec une sphère*/
int lvl1_rayInterSphere(G3Xpoint pos,G3Xvector dir, G3Xpoint ri);


/*Vérifie l'intersection d'un rayon avec une triange*/
int lvl1_rayInterTriangle(G3Xpoint pos, G3Xvector dir, G3Xpoint ri);


/*Vérifie l'intersection d'un rayon avec une cube*/
int lvl1_rayInterCube(G3Xpoint pos, G3Xvector dir, G3Xpoint ri);


/*Vérifie l'intersection d'un rayon avec une cylindre*/
int lvl1_rayInterCylindre(G3Xpoint pos,G3Xvector dir, G3Xpoint ri);

/*Initialise la scène avec les objets contenues dans le fichier src*/
void lvl1_initObjects(char* src);

/*Récupère l'option voulu dans les arguments du programme*/
char* lvl1_getOpt(char* opt,int argc,char* argv[]);

/*Execute le niveau 1*/
void lvl1_do(int argc,char* argv[]);

/*Sauvegarde l'image de la scène*/
void lvl1_save(char * dst);

/*Affiche dans STDOUT une matrice (debug)*/
void lvl1_printMat(G3Xhmat mat);

/*Lit une matrice dans un fichier*/
void lvl1_readMat(FILE* fichier,G3Xhmat mat);

#endif