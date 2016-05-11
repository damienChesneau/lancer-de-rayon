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


int lvl2_tabCpy(int* dst, int* src, int start, int end);
BoundingSphere* lvl2_getNewBoundingSphere (int* s, int length);
void lvl2_initHierarchicalStructure(int* s, int length, BoundingSphere* father);
void lvl2_initBoundingSpheres(char *src);
void lvl2_isTouched(BoundingSphere* s, G3Xpoint pixel, G3Xvector ray);
int lvl2_intersection(G3Xpoint pos, G3Xvector dir, float ray, G3Xvector center);
void lvl2_freeHierarchicalStructure(BoundingSphere* s);
void lvl2_do(int argc,char* argv[]);
#endif