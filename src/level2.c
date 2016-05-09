#include "../include/level2.h"


typedef struct boundingSphere{
	Object* object;
	float ray;
	G3Xpoint center;
	struct boundingSphere* leaves[4];
	struct boundingSphere* s1;
	struct boundingSphere* s2;
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

BoundingSphere* lvl2_initBoundingSphere(){
	return (BoundingSphere *) malloc(sizeof(BoundingSphere));
}

int lvl2_tabCpy(int* p, int* s, int start, int end){
	int i = 0;
	for(i = start; i<end; i++){
		s[i] = p[i];
	}
	return i;
}


BoundingSphere* lvl2_getNewBoundingSphere (int* s, int length, int axe){
	printf("%d\n",s[0]);
	float min = bs[s[0]].center[axe] - bs[s[0]].ray;
	float max = bs[s[0]].center[axe] + bs[s[0]].ray;

	int i = 0;
	for(i = 0; i<length; i++){
		if(min > bs[s[i]].center[axe] - bs[s[i]].ray){
			min = bs[s[i]].center[axe] - bs[s[i]].ray;
		}
		if(max < bs[s[i]].center[axe] + bs[s[i]].ray){
			max = bs[s[i]].center[axe] + bs[s[i]].ray;
		}
	}
	
	BoundingSphere* ret= lvl2_initBoundingSphere();
	
	ret->ray = (max-min)/2.0;
	
	ret->center[0] = 0;
	ret->center[1] = 0;
	ret->center[2] = 0;
	ret->center[axe] = min + ret->ray;

	ret->object = NULL;
	ret->s1 = NULL;
	ret->s2 = NULL;

	ret->leaves[0] = NULL;
	ret->leaves[1] = NULL;
	ret->leaves[2] = NULL;
	ret->leaves[3] = NULL;

	return ret;
}

void lvl2_initHierarchicalStructure(int* s, int length, BoundingSphere* father){
	if(length < 4){
		int i = 0;
		for( i = 0; i<length; i++){
			father->leaves[i] = &(bs[s[i]]);
		}
		return;
	}

	float t_min = 0;
	int i_min = 0;
	int axe_min = 0;
	int *ts1_min=NULL;
	int *ts2_min=NULL;
	int s1_length_min = 0;
	int s2_length_min = 0; 
	BoundingSphere* s1_min = NULL;
	BoundingSphere* s2_min = NULL;

	int axe= 0;
 	for (axe = 0; axe<3; axe++){
		int p[length];
		lvl2_tabCpy(s,p,0,length);
		int i = 0;
		int j = 0;
		for(i = 0; i<length; i++){
			for(j = i; j<length; j++){
				if(bs[p[i]].center[axe] > bs[p[j]].center[axe]){
					int temp = p[i];
					p[i] = p[j];
					p[j] = temp;
				}
			}
		}

		for(i = 0; i<length; i++){
			int ts1[length];
			int ts2[length];

			int s1_length = lvl2_tabCpy(p,ts1,0,i);
			int s2_length = lvl2_tabCpy(p,ts2,i+1,length);

			BoundingSphere* s1 = lvl2_getNewBoundingSphere(ts1, s1_length, axe);
			BoundingSphere* s2 = lvl2_getNewBoundingSphere(ts2, s2_length, axe);

			float a_s1 = 4 * 3.14 * s1->ray * s1->ray;
			float a_s2 = 4 * 3.14 * s2->ray * s2->ray;
			float a_s = 4 * 3.14 * father->ray * father->ray;
			float t = (1 + a_s1 / a_s * s1_length) + (1 + a_s2 / a_s *s2_length);

			if( t<t_min){
				i_min = p[i];
				t_min = t;
				ts1_min = ts1;
				ts2_min = ts2;
				s1_min = s1;
				s2_min = s2;
				axe_min = axe;
				s1_length_min = s1_length;
				s2_length_min = s2_length;
			}
		}

	}

	father->leaves[0] = &(bs[i_min]);
	father->s1 = s1_min;
	father->s2 = s2_min;

	lvl2_initHierarchicalStructure(ts1_min,s1_length_min,s1_min);
	lvl2_initHierarchicalStructure(ts2_min,s2_length_min,s2_min);
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
				bs[i].ray = lvl2_getRay(1.1,bs[i].object->transfo);
				break;
			case 1:
				/*racine_carré_de((1/4)+(1/2))*/
				bs[i].ray = lvl2_getRay(0.867,bs[i].object->transfo);
				break;
			case 2:
				bs[i].ray = lvl2_getRay(1.1,bs[i].object->transfo);
				break;	
			case 3:
				/*racine_carré_de (1²+0.5²) = 1.118*/
				bs[i].ray = lvl2_getRay(1.119,bs[i].object->transfo);
				break;			
		}
		bs[i].center[0] = bs[i].object->transfo[12]; 
		bs[i].center[1] = bs[i].object->transfo[13]; 
		bs[i].center[2] = bs[i].object->transfo[14];
		bs[i].s1= NULL;
		bs[i].s2 = NULL;
	}

}
/**************************************************/
void lvl2_do(int argc,char* argv[]){
	lvl2_initBoundingSpheres("./fractal_generator.format");
	int indexs[nbObjects];
	int i = 0;
	for(i = 0; i < nbObjects; i++){
		indexs[i] = i;
	}
	hierarchicalStructure.ray = 400;
	hierarchicalStructure.center[0] = 0;
	hierarchicalStructure.center[1] = 0;
	hierarchicalStructure.center[2] = 0;	
	lvl2_initHierarchicalStructure(indexs,nbObjects,&hierarchicalStructure);
}