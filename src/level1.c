#include "../include/level1.h"

static Object camera;
static Object objects[MAX_OBJECTS];
static int nbObjects = 0;
static G3Xcolor image[IMAGE_SIZE][IMAGE_SIZE];
/*****************************************************************************************************/

void lvl1_setPixel(G3Xcolor toSet,int i,int j){
	image[i][j][0] = toSet[0];
	image[i][j][1] = toSet[1];
	image[i][j][2] = toSet[2];
}

Object* lvl1_getObjects(){
	return objects;
}

Object lvl1_getCamera(){
	return camera;
}

int lvl1_getNbObjects(){
	return nbObjects;
}

char* lvl1_getOpt(char* opt,int argc,char* argv[]){
	int i = 0;	
	for(i = 1; i<argc; i++){
		if(strcmp(argv[i],opt) == 0){
			return argv[i+1];
		}
	}
}

void lvl1_readMat(FILE* fichier,G3Xhmat mat){
	if(fscanf(fichier,"%lf %lf %lf %lf\n", &(mat[0]), &(mat[4]), &(mat[8]), &(mat[12])));
	if(fscanf(fichier,"%lf %lf %lf %lf\n", &(mat[1]), &(mat[5]), &(mat[9]), &(mat[13])));
	if(fscanf(fichier,"%lf %lf %lf %lf\n", &(mat[2]), &(mat[6]), &(mat[10]), &(mat[14])));
	if(fscanf(fichier,"%lf %lf %lf %lf\n", &(mat[3]), &(mat[7]), &(mat[11]), &(mat[15])));
}

void lvl1_printMat(G3Xhmat mat){
	printf("%lf\t%lf\t%lf\t%lf\n", mat[0], mat[4], mat[8], mat[12]);
	printf("%lf\t%lf\t%lf\t%lf\n", mat[1], mat[5], mat[9], mat[13]);
	printf("%lf\t%lf\t%lf\t%lf\n", mat[2], mat[6], mat[10], mat[14]);
	printf("%lf\t%lf\t%lf\t%lf\n", mat[3], mat[7], mat[11], mat[15]);
}

void lvl1_save(char * dst){
	FILE* fichier = fopen(dst,"w");
	fprintf(fichier, "P3\n");
	fprintf(fichier, "%d %d\n",IMAGE_SIZE,IMAGE_SIZE);
	fprintf(fichier, "255\n");	
	int i = 0;
	int j = 0;
	for(i = 0; i<IMAGE_SIZE; i++){
		for(j = 0; j<IMAGE_SIZE -1; j++){
			fprintf(fichier, "%d %d %d\t",(int)image[i][j][0],(int)image[i][j][1],(int)image[i][j][2] );
		}
		fprintf(fichier, "%d %d %d\n",(int)image[i][j][0],(int)image[i][j][1],(int)image[i][j][2] );
	}
	fclose(fichier);
}

void lvl1_initObjects(char* src){
	FILE* fichier = fopen(src,"r");
	if (fichier != NULL){
		while(fscanf(fichier,"%d ",&(objects[nbObjects].type)) != EOF){
			if(objects[nbObjects].type != 4){
				switch(objects[nbObjects].type){
					case 0 :
						objects[nbObjects].intersection = lvl1_rayInterSphere;
						break;
					case 1 :
						objects[nbObjects].intersection = lvl1_rayInterCube;
						break;
					case 2 :
						objects[nbObjects].intersection = lvl1_rayInterTriangle;
						break;
					case 3 :
						objects[nbObjects].intersection = lvl1_rayInterCylindre;
						break;
				}
				if(fscanf(fichier,"%f %f %f\n\n", &(objects[nbObjects].color[0]), &(objects[nbObjects].color[1]), &(objects[nbObjects].color[2]) ));

				lvl1_readMat(fichier,objects[nbObjects].transfo);

				if(fscanf(fichier,"\n"));
				
				lvl1_readMat(fichier,objects[nbObjects].inverse);

				if(fscanf(fichier,"\n"));
				nbObjects++;


			}else{
				if(fscanf(fichier,"\n\n"));

				lvl1_readMat(fichier,camera.transfo);

				if(fscanf(fichier,"\n"));
				
				lvl1_readMat(fichier,camera.inverse);

				if(fscanf(fichier,"\n"));
			}
		}
	}
	fclose(fichier);
}

int lvl1_rayInterSphere(G3Xpoint pos,G3Xvector dir, G3Xpoint ri){
	double ps = G3Xprodscal(pos,dir);
	if(ps>= 0 ){
		return 0;
	}

	double p2 = (ps*ps)/G3Xsqrvnorm(dir);
	double d2 = G3Xsqrvnorm(pos) - p2;

	if(d2 >= 1){
		return 0;
	}

	double t = -ps - sqrt(1-d2);

	ri[0] = pos[0] + t * dir[0];
	ri[1] = pos[1] + t * dir[1];
	ri[2] = pos[2] + t * dir[2];

	return 1;
}

int lvl1_rayInterCylindre(G3Xpoint pos,G3Xvector dir, G3Xpoint ri){
	if(G3Xprodscal(pos,dir)>=0){
		return 0;
	}

	double ps = pos[0]*dir[0] + pos[1]*dir[1];
	double p2 = ps*ps/(dir[0]*dir[0]+dir[1]*dir[1]);
	double d2 = (pos[0]*pos[0]+pos[1]*pos[1]) - p2;

	if(d2 <= 1){
		double t = -ps - sqrt(1-d2);
		ri[0] = pos[0] + t * dir[0];
		ri[1] = pos[1] + t * dir[1];
		ri[2] = pos[2] + t * dir[2];

		if(ri[2]<=0.5 && ri[2] >= -0.5){
			return 1;
		}
	}

	G3Xvector normal[2] = {
		{1,0,0},
		{-1,0,0}
	};

	int normalIndex = -1;
	double psf = G3Xprodscal(normal[0],dir);

	if(psf>0){
			normalIndex = 1;
	}else{
		if(psf != 0){
			normalIndex = 0;
		}
	}

	double k = 0;
	switch(normalIndex){
		case 0:
			k = (0.5-pos[2])/dir[2];
			ri[0] = k * dir[0] + pos[0]; 
			ri[1] = k * dir[1] + pos[1]; 
			ri[2] = 0.5;


			if( ri[0]<=1 &&
				ri[1]<= 1 &&
				(ri[0]*ri[0])+(ri[1]*ri[1]) <= 1){
				return 1;
			}
			break;
		case 1:
			k = (-0.5-pos[2])/dir[2];
			ri[0] = k * dir[0] + pos[0]; 
			ri[1] = k * dir[1] + pos[1]; 
			ri[2] = -0.5;


			if( ri[0]<=1 &&
				ri[1]<= 1 &&
				(ri[0]*ri[0])+(ri[1]*ri[1]) <= 1){
				return 1;
			}
			break;
	}
	
	return 0;

}

int lvl1_rayInterTriangle(G3Xpoint pos, G3Xvector dir, G3Xpoint ri){
	G3Xvector normal = {1,0,0};
	if(G3Xprodscal(normal,dir)>=0){
		return 0;
	}
	double k = (0-pos[0])/dir[0];

	ri[0] = 0;
	ri[1] = k * dir[1] + pos[1];
	ri[2] = k * dir[2] + pos[2];

	if( ri[1] < 0 || 
		ri[2] < 0 || 
		ri[1] + ri[2] > 1){

		return 0;
	}
	return 1;
}

int lvl1_rayInterCube(G3Xpoint pos, G3Xvector dir, G3Xpoint ri){
	if(G3Xprodscal(pos,dir)>=0){
		return 0;
	}
	G3Xvector normal[6] = {
		{1,0,0},
		{-1,0,0},
		{0,1,0},
		{0,-1,0},
		{0,0,1},
		{0,0,-1}
	};
	int flags[6] = {0,0,0,0,0,0};
	double ps[3] = {0,0,0};
	ps[0] = G3Xprodscal(normal[0],dir);
	ps[1] = G3Xprodscal(normal[2],dir);
	ps[2] = G3Xprodscal(normal[4],dir);
	
	int i;
	for(i = 0; i<3; i++){
		int flag = i*2;
		if(ps[i]>0){
			flags[flag+1] = 1;
		}else{
			if(ps[i] != 0){
				flags[flag] = 1;
			}
		}

	}

	for(i = 0; i<6; i++){
		if(flags[i] == 1){
			double k = 0;

			switch(i){
				case 0:
					k = (0.5-pos[0])/dir[0];
					ri[0] = 0.5; 
					ri[1] = k * dir[1] + pos[1]; 
					ri[2] = k * dir[2] + pos[2];


					if( ri[1] >= -0.5 &&
						ri[1] <= 0.5 &&
						ri[2] >= -0.5 &&
						ri[2] <= 0.5 ){
						return 1;
					}
					break;
				case 1:
					k = (-0.5-pos[0])/dir[0];

					ri[0] = -0.5; 
					ri[1] = k * dir[1] + pos[1]; 
					ri[2] = k * dir[2] + pos[2];


					if( ri[1] >= -0.5 &&
						ri[1] <= 0.5 &&
						ri[2] >= -0.5 &&
						ri[2] <= 0.5 ){
						return 1;
					}
					break;
				case 2:
					k = (0.5-pos[1])/dir[1];

					ri[0] = k * dir[0] + pos[0];
					ri[1] = 0.5;  
					ri[2] = k * dir[2] + pos[2];


					if( ri[0] >= -0.5 &&
						ri[0] <= 0.5 &&
						ri[2] >= -0.5 &&
						ri[2] <= 0.5 ){
						return 1;
					}
					break;
				case 3:
					k = (-0.5-pos[1])/dir[1];

					ri[0] = k * dir[0] + pos[0];
					ri[1] = -0.5; 
					ri[2] = k * dir[2] + pos[2];


					if( ri[0] >= -0.5 &&
						ri[0] <= 0.5 &&
						ri[2] >= -0.5 &&
						ri[2] <= 0.5 ){
						return 1;
					}
					break;
				case 4:
					k = (0.5-pos[2])/dir[2];

					ri[0] = k * dir[0] + pos[0];
					ri[1] = k * dir[1] + pos[1];
					ri[2] = 0.5;


					if( ri[0] >= -0.5 &&
						ri[0] <= 0.5 &&
						ri[1] >= -0.5 &&
						ri[1] <= 0.5 ){
						return 1;
					}
					break;
				case 5:
					k = (-0.5-pos[2])/dir[2];

					ri[0] = k * dir[0] + pos[0];
					ri[1] = k * dir[1] + pos[1];
					ri[2] = -0.5;


					if( ri[0] >= -0.5 &&
						ri[0] <= 0.5 &&
						ri[1] >= -0.5 &&
						ri[1] <= 0.5 ){
						return 1;
					}
					break;
			}
		}
	}

	return 0;

}

/****************************************************************************************************/
void lvl1_do(int argc,char* argv[]){
	int i = 0;
	int j = 0;
	lvl1_initObjects(lvl1_getOpt("-i",argc,argv));
	G3Xpoint canFocale = {1,0,0};
	clock_t start,finish;
	start = clock();
	for(i = 0; i<IMAGE_SIZE; i++){
		for(j = 0; j<IMAGE_SIZE; j++){
			G3Xpoint canPixel = {0,(i-(IMAGE_SIZE/2.0))/IMAGE_SIZE ,(j-(IMAGE_SIZE/2.0))/IMAGE_SIZE};
			G3Xvector canRay;
			G3Xsetvct(canRay,canFocale,canPixel);
			
			G3Xvector ray;
			g3x_ProdHMatVector(camera.transfo,canRay,ray);
			
			G3Xpoint pixel;
			g3x_ProdHMatPoint(camera.transfo,canPixel,pixel);
			

			double lastInter = INT_MIN;

			G3Xcolor toSet = {0,0,0};
			
			int k = 0;
			for(k = 0; k<nbObjects;k++){
				G3Xvector inObjectRay;
				G3Xpoint inObjectPixel;
				g3x_ProdHMatVector(objects[k].inverse,ray,inObjectRay);
				g3x_ProdHMatPoint(objects[k].inverse,pixel,inObjectPixel);
				G3Xpoint inObjectRi;
				if(objects[k].intersection(inObjectPixel,inObjectRay,inObjectRi) == 1){
					G3Xpoint ri;
					G3Xpoint tempRi;
					g3x_ProdHMatPoint(objects[k].transfo,inObjectRi, tempRi);
					g3x_ProdHMatPoint(camera.inverse,tempRi, ri);
					double inter = ri[0];
					if(inter > lastInter){
						toSet[0] = objects[k].color[0];
						toSet[1] = objects[k].color[1];
						toSet[2] = objects[k].color[2];
						lastInter = inter;
					}
				}
				image[i][j][0] = toSet[0];
				image[i][j][1] = toSet[1];
				image[i][j][2] = toSet[2];

			}

		}
	}
	finish = clock();
	double duration = (double)(finish - start)/CLOCKS_PER_SEC;
	printf("%lfsecondes\n",duration);
	lvl1_save(lvl1_getOpt("-o",argc,argv));
}