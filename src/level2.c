#include <g3x.h>
#include <limits.h>
#define IMAGE_SIZE 512

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

Object camera;
Object objects[100];
int nbObjects = 0;
G3Xcolor image[IMAGE_SIZE][IMAGE_SIZE];
/*****************************************************************************************************/

void prodMatV(G3Xhmat mat, G3Xvector v, G3Xvector r){
	r[0] = v[0]*mat[0] + v[1]*mat[4] + v[2]*mat[8];
	r[1] = v[0]*mat[1] + v[1]*mat[5] + v[2]*mat[9];
	r[2] = v[0]*mat[2] + v[1]*mat[6] + v[2]*mat[10];
}

void prodMatP(G3Xhmat mat, G3Xpoint p, G3Xpoint r){
	r[0] = p[0]*mat[0] + p[1]*mat[4] + p[2]*mat[8] + mat[12];
	r[1] = p[0]*mat[1] + p[1]*mat[5] + p[2]*mat[9] + mat[13];
	r[2] = p[0]*mat[2] + p[1]*mat[6] + p[2]*mat[10] + mat[14];
}

void prodMatM(G3Xhmat a, G3Xhmat b, G3Xhmat r){
	/*TO DO*/
}

void readMat(FILE* fichier,G3Xhmat mat){
	if(fscanf(fichier,"%lf %lf %lf %lf\n", &(mat[0]), &(mat[4]), &(mat[8]), &(mat[12])));
	if(fscanf(fichier,"%lf %lf %lf %lf\n", &(mat[1]), &(mat[5]), &(mat[9]), &(mat[13])));
	if(fscanf(fichier,"%lf %lf %lf %lf\n", &(mat[2]), &(mat[6]), &(mat[10]), &(mat[14])));
	if(fscanf(fichier,"%lf %lf %lf %lf\n", &(mat[3]), &(mat[7]), &(mat[11]), &(mat[15])));
}

void printMat(G3Xhmat mat){
	printf("%lf\t%lf\t%lf\t%lf\n", mat[0], mat[4], mat[8], mat[12]);
	printf("%lf\t%lf\t%lf\t%lf\n", mat[1], mat[5], mat[9], mat[13]);
	printf("%lf\t%lf\t%lf\t%lf\n", mat[2], mat[6], mat[10], mat[14]);
	printf("%lf\t%lf\t%lf\t%lf\n", mat[3], mat[7], mat[11], mat[15]);
}

void save(char * dst){
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

void initObjects(char* src){
	FILE* fichier = fopen(src,"r");
	if (fichier != NULL){
		int type = 0;
		while(fscanf(fichier,"%d ",&type) != EOF){
			if(type != 4){
				switch(type){
					case 0 :
						objects[nbObjects].intersection = rayInterSphere;
						break;
					case 1 :
						objects[nbObjects].intersection = rayInterCube;
						break;
					case 2 :
						objects[nbObjects].intersection = rayInterTriangle;
						break;
					case 3 :
						objects[nbObjects].intersection = rayInterCylindre;
						break;
				}
				if(fscanf(fichier,"%f %f %f\n", &(objects[nbObjects].color[0]), &(objects[nbObjects].color[1]), &(objects[nbObjects].color[2]) ));

				readMat(fichier,objects[nbObjects].transfo);

				if(fscanf(fichier,"\n"));
				
				readMat(fichier,objects[nbObjects].inverse);

				if(fscanf(fichier,"\n"));
				nbObjects++;


			}else{

				readMat(fichier,camera.transfo);

				if(fscanf(fichier,"\n"));
				
				readMat(fichier,camera.inverse);

				if(fscanf(fichier,"\n"));
			}
		}
	}
	fclose(fichier);
}

int rayInterSphere(G3Xpoint pos,G3Xvector dir, G3Xpoint ri){
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

int rayInterCylindre(G3Xpoint pos,G3Xvector dir, G3Xpoint ri){
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

int rayInterTriangle(G3Xpoint pos, G3Xvector dir, G3Xpoint ri){
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

int rayInterCube(G3Xpoint pos, G3Xvector dir, G3Xpoint ri){
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
void doLevel1(int argc,char* argv[]){
	int i = 0;
	int j = 0;
	char * src = getOpt("-i",argc,argv);
	printf("Option OK\n");
	initObjects(src);
	printf("Init Object OK\n");
	G3Xpoint canFocale = {1,0,0};
	for(i = 0; i<IMAGE_SIZE; i++){
		for(j = 0; j<IMAGE_SIZE; j++){
			G3Xpoint canPixel = {0,(i-(IMAGE_SIZE/2.0))/IMAGE_SIZE ,(j-(IMAGE_SIZE/2.0))/IMAGE_SIZE};
			G3Xvector canRay;
			G3Xsetvct(canRay,canFocale,canPixel);
			
			G3Xvector ray;
			prodMatV(camera.transfo,canRay,ray);
			
			G3Xpoint pixel;
			prodMatP(camera.transfo,canPixel,pixel);
			
			double lastInter = INT_MAX;

			G3Xcolor toSet = {0,0,0};
			
			int k = 0;
			for(k = 0; k<nbObjects;k++){
				G3Xvector inObjectRay;
				G3Xpoint inObjectPixel;
				prodMatV(objects[k].inverse,ray,inObjectRay);
				prodMatP(objects[k].inverse,pixel,inObjectPixel);
				G3Xpoint inObjectRi;
				if(objects[k].intersection(inObjectPixel,inObjectRay,inObjectRi) == 1){
					G3Xpoint ri;
					prodMatP(objects[k].transfo,inObjectRi, ri);
					G3Xvector rayInter;
					G3Xsetvct(pixel,ri,rayInter);
					
					if(G3Xsqrvnorm(rayInter)<lastInter){
						toSet[0] = objects[k].color[0];
						toSet[1] = objects[k].color[1];
						toSet[2] = objects[k].color[2];
					}
				}
				image[i][j][0] = toSet[0];
				image[i][j][1] = toSet[1];
				image[i][j][2] = toSet[2];

			}

		}
	}
	save(getOpt("-o",argc,argv));
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