#include "../include/level2.h"


typedef struct boundingSphere{
	Object* object;
	float ray;
	G3Xpoint center;
}BoundingSphere;

/**************************************************/
static Object camera;
static BoundingSphere bs[MAX_OBJECTS];
static int nbObjects = 0;
static BoundingSphere hierarchicalStructure;
/*****************************************************/

void lvl2_do(int argc,char* argv[]);
float lvl2_getRay(float ray, G3Xhmat mat); 
/****************************************************/

void lvl2_initHierarchicalStructure(BoundingSphere[] s, int length){
	int i = 0;
	for (i = 0; i<3; i++){
		BoundingSphere p[length];
	}
} 
float lvl2_getRay(float ray, G3Xhmat mat){
	double max = mat[0];

	int i = 0;
	for(i = 5; i<11; i+=5){
		if(mat[i] > max){
			max = mat[i];
		}
	}
	return ray * max;
}


void lvl2_initBoundingSpheres(char *src){
	lvl1_initObjects(src);
	nbObjects = lvl1_getNbObjects();
	camera = lvl1_getCamera();
	Object* objects = lvl1_getObjects();
	int i = 0;

	for(i = 0; i<nbObjects;i++){
		bs[i].object = &(objects[i]);
		switch(bs[i].object->type){
			case 0:
				bs[i].ray = getRay(1.1,bs[i].object->transfo);
				break;
			case 1:
				/*racine_carré_de((1/4)+(1/2))*/
				bs[i].ray = getRay(0.867,bs[i].object->transfo);
				break;
			case 2:
				bs[i].ray = getRay(1.1,bs[i].object->transfo);
				break;	
			case 3:
				/*racine_carré_de (1²+0.5²) = 1.118*/
				bs[i].ray = getRay(1.119,bs[i].object->transfo);
				break;			
		}
		bs[i].center[0] = bs[i].object->transfo[12]; 
		bs[i].center[1] = bs[i].object->transfo[13]; 
		bs[i].center[2] = bs[i].object->transfo[14];
		bs[i].left = NULL;
		bs[i].right = NULL;
	}

}
/**************************************************/
void lvl2_do(int argc,char* argv[]){
	printf ("Unimplemented yet");
}