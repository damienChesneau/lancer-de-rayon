#include "../include/level2.h"
/**************************************************/
static Object camera;
static BoundingSphere bs[MAX_OBJECTS];
static int nbObjects = 0;
static BoundingSphere* hierarchicalStructure;
static double lastInter;
static G3Xcolor toSet = {0,0,0};
static G3Xpoint ri;
/****************************************************/


int lvl2_tabCpy(int* dst, int* src, int start, int end){
	int i = 0;
	for(i = start; i<end; i++){
		dst[i-start] = src[i];
	}
	return i-start;
}


BoundingSphere* lvl2_getNewBoundingSphere (int* s, int length){
	float min[3];
	float max[3];

	min[0] = INT_MAX;
	min[1] = INT_MAX;
	min[2] = INT_MAX;

	max[0] = INT_MIN;
	max[1] = INT_MIN;
	max[2] = INT_MIN;

	int i = 0;
	for(i = 0; i<length; i++){
		int axe;
		for(axe = 0; axe<3; axe++){
			if(min[axe] > bs[s[i]].center[axe] - bs[s[i]].ray){
				min[axe] = bs[s[i]].center[axe] - bs[s[i]].ray;
			}
			if(max[axe] < bs[s[i]].center[axe] + bs[s[i]].ray){
				max[axe] = bs[s[i]].center[axe] + bs[s[i]].ray;
			}
		}
	}
	
	BoundingSphere* ret= (BoundingSphere *) malloc(sizeof(BoundingSphere));
	
	ret->center[0] = (min[0]+max[0])/2;
	ret->center[1] = (min[1]+max[1])/2;
	ret->center[2] = (min[2]+max[2])/2;

	ret->ray = sqrt( ((max[0]-ret->center[0]) * (max[0]-ret->center[0])) + ((max[1]-ret->center[1]) * (max[1]-ret->center[1])) + ((max[2]-ret->center[2]) * (max[2]-ret->center[2])) );

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

	float t_min = INT_MAX;
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
		int *p= (int *)malloc(sizeof(int)*length);
		lvl2_tabCpy(p,s,0,length);
		int i;
		int j;
		for(i = 1; i<length; i++){
			for(j = 0; j<length-i; j++){
				if(bs[p[j]].center[axe] > bs[p[j+1]].center[axe]){
					int temp = p[i];
					p[i] = p[j];
					p[j] = temp;
				}
			}
		}

		for(i = 1; i<length-1; i++){
			int *ts1 = (int *)malloc(sizeof(int)*length);
			int *ts2 = (int *)malloc(sizeof(int)*length);

			int s1_length = lvl2_tabCpy(ts1,p,0,i);
			int s2_length = lvl2_tabCpy(ts2,p,i+1,length);

			BoundingSphere* s1 = lvl2_getNewBoundingSphere(ts1, s1_length);
			BoundingSphere* s2 = lvl2_getNewBoundingSphere(ts2, s2_length);

			float a_s1 = 4 * 3.14 * s1->ray * s1->ray;
			float a_s2 = 4 * 3.14 * s2->ray * s2->ray;
			float a_s = 4 * 3.14 * father->ray * father->ray;
			float t = (1 + a_s1 / a_s * s1_length) + (1 + a_s2 / a_s *s2_length);

			if( t < t_min){
				i_min = p[i];

				t_min = t;

				free(ts1_min);
				free(ts2_min);
				ts1_min = ts1;
				ts2_min = ts2;

				free(s1_min);
				free(s2_min);
				s1_min = s1;
				s2_min = s2;

				axe_min = axe;

				s1_length_min = s1_length;
				s2_length_min = s2_length;
			}else{
				free(ts1);
				free(ts2);
			}
		}
		free(p);
		p = NULL;
	}

	father->leaves[0] = &(bs[i_min]);
	father->s1 = s1_min;
	father->s2 = s2_min;
	lvl2_initHierarchicalStructure(ts1_min,s1_length_min,s1_min);
	lvl2_initHierarchicalStructure(ts2_min,s2_length_min,s2_min);
} 


void lvl2_initBoundingSpheres(char *src){
	lvl1_initObjects(src);
	nbObjects = lvl1_getNbObjects();
	camera = lvl1_getCamera();
	Object* objects = lvl1_getObjects();
	int i = 0;
	float r;
	for(i = 0; i<nbObjects;i++){
		bs[i].object = &(objects[i]);
		switch(bs[i].object->type){
			case 0:
				r = 1.0;
				break;
			case 1:
				/*racine_carré_de((1/4)+(1/2))*/
				r = 0.867;
				break;
			case 2:
				r =1.0;
				break;	
			case 3:
				/*racine_carré_de (1²+0.5²) = 1.118*/
				r =1.119;
				break;			
		}
		double max = bs[i].object->transfo[0];
		int j = 0;
		for(j = 5; j<11; j+=5){
			if(bs[i].object->transfo[j] > max){
				max = bs[i].object->transfo[j];
			}
		}

		bs[i].ray = r * max;
		bs[i].center[0] = bs[i].object->transfo[12]; 
		bs[i].center[1] = bs[i].object->transfo[13]; 
		bs[i].center[2] = bs[i].object->transfo[14];
		bs[i].s1= NULL;
		bs[i].s2 = NULL;
	}

}

void lvl2_isTouched(BoundingSphere* s, G3Xpoint pixel, G3Xvector ray){
	if(s == NULL){	
		return;
	}
	if(s->object !=NULL){
		G3Xvector inObjectRay;
		G3Xpoint inObjectPixel;

		g3x_ProdHMatVector(s->object->inverse,ray,inObjectRay);
		g3x_ProdHMatPoint(s->object->inverse,pixel,inObjectPixel);

		G3Xpoint inObjectRi;
		if(s->object->intersection(inObjectPixel,inObjectRay,inObjectRi) == 1){
			G3Xpoint tempRi;
			g3x_ProdHMatPoint(s->object->transfo,inObjectRi, tempRi);
			G3Xvector rayInter;
			G3Xsetvct(tempRi,pixel,rayInter);
			double inter = G3Xsqrvnorm(rayInter);
			if(inter < lastInter){
				toSet[0] = s->object->color[0];
				toSet[1] = s->object->color[1];
				toSet[2] = s->object->color[2];
				ri[0] = tempRi[0];
				ri[1] = tempRi[1];
				ri[2] = tempRi[2];
				lastInter = inter;
			}
		}
		return;
	}
	if(s->s1 == NULL && s->s2 == NULL){
		int i = 0;
		for (i = 0; i<4 && s->leaves[i] != NULL; i++){
			lvl2_isTouched(s->leaves[i],pixel,ray);
		}
		return;
	}
	if(lvl2_intersection(pixel,ray,s->s1->ray,s->s1->center) == 1){
		lvl2_isTouched(s->s1,pixel,ray);
	}
	if(lvl2_intersection(pixel,ray,s->s2->ray,s->s2->center) == 1){
		lvl2_isTouched(s->s2,pixel,ray);
	}
	lvl2_isTouched(s->leaves[0],pixel,ray);

}

int lvl2_intersection(G3Xpoint pos, G3Xvector dir, float ray, G3Xvector center){
	G3Xvector centerPos;
	G3Xsetvct(centerPos,center,pos);
	double ps = G3Xprodscal(centerPos,dir);
	if(ps>= 0 ){
		return 0;
	}

	double p2 = (ps*ps)/G3Xsqrvnorm(dir);
	double d2 = G3Xsqrvnorm(centerPos) - p2;

	if(d2 >= (ray*ray)){
		return 0;
	}

	return 1;
}

void lvl2_freeHierarchicalStructure(BoundingSphere* s){
	if(s == NULL){
		return;
	}
	lvl2_freeHierarchicalStructure(s->s1);
	lvl2_freeHierarchicalStructure(s->s2);
	free(s);
	s = NULL;
}
/**************************************************/
void lvl2_do(int argc,char* argv[]){
	lvl2_initBoundingSpheres(lvl1_getOpt("-i",argc,argv));
	int nbSample = atoi(lvl1_getOpt("-ps",argc,argv));
	int *indexs = (int *)malloc(sizeof(int)*nbObjects);
	int i;
	for(i = 0; i < nbObjects; i++){
		indexs[i] = i;
	}
	hierarchicalStructure = lvl2_getNewBoundingSphere(indexs,nbObjects);
	lvl2_initHierarchicalStructure(indexs,nbObjects,hierarchicalStructure);
	int j;
	int k;
	G3Xpoint canFocale = {1,0,0};
	clock_t start,finish;
	start = clock();
	G3Xcolor toRet = {0,0,0};
	for(i = 0; i<IMAGE_SIZE; i++){
		for(j = 0; j<IMAGE_SIZE; j++){
			for(k = 0; k<nbSample; k++){
				double r = rand()%10;
				double sample_x = r / 10;
				r = rand()%10;
				double sample_y = r /10;

				double x = (i+sample_x-(IMAGE_SIZE/2.0))/IMAGE_SIZE;
				double y = (j+sample_y-(IMAGE_SIZE/2.0))/IMAGE_SIZE;

				G3Xpoint canPixel = {0,x,y};
				G3Xvector canRay;
				G3Xsetvct(canRay,canFocale,canPixel);
				
				G3Xvector ray;
				g3x_ProdHMatVector(camera.transfo,canRay,ray);
				
				G3Xpoint pixel;
				g3x_ProdHMatPoint(camera.transfo,canPixel,pixel);
				
				lastInter = INT_MAX;

				toSet[0] = 0;
				toSet[1] = 0;
				toSet[2] = 0;
				
				lvl2_isTouched(hierarchicalStructure,pixel,ray);

				toRet[0]+= toSet[0];
				toRet[1]+= toSet[1];
				toRet[2]+= toSet[2];
			}
			toRet[0] /= nbSample;
			toRet[1] /= nbSample;
			toRet[2] /= nbSample;
			lvl1_setPixel(toRet,i,j);
		}
	}
	finish = clock();
	double duration = (double)(finish - start)/CLOCKS_PER_SEC;
	printf("%lfsecondes\n",duration);
	lvl1_save(lvl1_getOpt("-o",argc,argv));
	free(indexs);
	lvl2_freeHierarchicalStructure(hierarchicalStructure);
}