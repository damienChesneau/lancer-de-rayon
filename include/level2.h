#ifndef __LEVEL2__
#define __LEVEL2__
#include "level1.h"

typedef struct boundingSphere{
	Object* object;
	float ray;
	G3Xpoint center;
	struct boundingSphere* leaves[4];
	struct boundingSphere* s1;
	struct boundingSphere* s2;
}BoundingSphere;

/*Copy les éléments du tableau src dans le tableau dst en commencant par start et finissant par end*/
int lvl2_tabCpy(int* dst, int* src, int start, int end);

/*Renvoie une sphère englobant la totalitée des éléments dans s*/
BoundingSphere* lvl2_getNewBoundingSphere (int* s, int length);

/*Initialise la structure hierarchique*/
void lvl2_initHierarchicalStructure(int* s, int length, BoundingSphere* father);

/*Initialise les objets dans une liste de sphère englobantes*/
void lvl2_initBoundingSpheres(char *src);

/*Vérifie si un rayon touche un objet. Si oui sa couleur et son point d'intersection sont stockés.*/
void lvl2_isTouched(BoundingSphere* s, G3Xpoint pixel, G3Xvector ray);

/*Vérifie l'intersection d'une sphère englobante avec un rayon*/
int lvl2_intersection(G3Xpoint pos, G3Xvector dir, float ray, G3Xvector center);

/*Libère l'espace mémoire de la sructure hierarchique*/
void lvl2_freeHierarchicalStructure(BoundingSphere* s);

/*Exectute le niveau 2*/
void lvl2_do(int argc,char* argv[]);
#endif