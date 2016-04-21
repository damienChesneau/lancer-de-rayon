#include <g3x.h>
#include <limits.h>
#define IMAGE_SIZE 64

int rayInterCercle(G3Xpoint pos,G3Xvector dir, G3Xpoint ri);
int rayInterTriangle(G3Xpoint pos, G3Xvector dir, G3Xpoint ri);
int rayInterRectangle(G3Xpoint pos, G3Xvector dir, G3Xpoint ri);
void initObjects(char* src);
char* getOpt(char* opt,int argc,char* argv[]);
void doLevel1(int argc,char* argv[]);
void doLevel2(int argc,char* argv[]);
void doLevel3(int argc,char* argv[]);
void doLevel4(int argc,char* argv[]);
void save(char * dst);
void printMat(G3Xhmat mat);

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
		while(fscanf(fichier,"%d\n",&type) != EOF){
			if(type != 4){
				switch(type){
					case 0 :
						objects[nbObjects].intersection = rayInterCercle;
						objects[nbObjects].color[0] = 255;
						objects[nbObjects].color[1] = 0;
						objects[nbObjects].color[2] = 0 ;
						break;
					case 1 :
						objects[nbObjects].intersection = rayInterRectangle;
						objects[nbObjects].color[0] = 0;
						objects[nbObjects].color[1] = 255;
						objects[nbObjects].color[2] = 0;
						break;
					case 2 :
						objects[nbObjects].intersection = rayInterTriangle;
						objects[nbObjects].color[0] = 0;
						objects[nbObjects].color[1] = 0;
						objects[nbObjects].color[2] = 255;
						break;
					case 3 :
						objects[nbObjects].intersection = NULL;
						objects[nbObjects].color[0] = 255;
						objects[nbObjects].color[1] = 255;
						objects[nbObjects].color[2] = 0;
						break;
				}

				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(objects[nbObjects].transfo[0]), &(objects[nbObjects].transfo[4]), &(objects[nbObjects].transfo[8]), &(objects[nbObjects].transfo[12])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(objects[nbObjects].transfo[1]), &(objects[nbObjects].transfo[5]), &(objects[nbObjects].transfo[9]), &(objects[nbObjects].transfo[13])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(objects[nbObjects].transfo[2]), &(objects[nbObjects].transfo[6]), &(objects[nbObjects].transfo[10]), &(objects[nbObjects].transfo[14])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(objects[nbObjects].transfo[3]), &(objects[nbObjects].transfo[7]), &(objects[nbObjects].transfo[11]), &(objects[nbObjects].transfo[15])));

				if(fscanf(fichier,"\n"));

				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(objects[nbObjects].inverse[0]), &(objects[nbObjects].inverse[4]), &(objects[nbObjects].inverse[8]), &(objects[nbObjects].inverse[12])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(objects[nbObjects].inverse[1]), &(objects[nbObjects].inverse[5]), &(objects[nbObjects].inverse[9]), &(objects[nbObjects].inverse[13])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(objects[nbObjects].inverse[2]), &(objects[nbObjects].inverse[6]), &(objects[nbObjects].inverse[10]), &(objects[nbObjects].inverse[14])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(objects[nbObjects].inverse[3]), &(objects[nbObjects].inverse[7]), &(objects[nbObjects].inverse[11]), &(objects[nbObjects].inverse[15])));

				if(fscanf(fichier,"\n"));
				nbObjects++;


			}else{

				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(camera.transfo[0]), &(camera.transfo[4]), &(camera.transfo[8]), &(camera.transfo[12])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(camera.transfo[1]), &(camera.transfo[5]), &(camera.transfo[9]), &(camera.transfo[13])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(camera.transfo[2]), &(camera.transfo[6]), &(camera.transfo[10]), &(camera.transfo[14])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(camera.transfo[3]), &(camera.transfo[7]), &(camera.transfo[11]), &(camera.transfo[15])));

				if(fscanf(fichier,"\n"));

				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(camera.inverse[0]), &(camera.inverse[4]), &(camera.inverse[8]), &(camera.inverse[12])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(camera.inverse[1]), &(camera.inverse[5]), &(camera.inverse[9]), &(camera.inverse[13])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(camera.inverse[2]), &(camera.inverse[6]), &(camera.inverse[10]), &(camera.inverse[14])));
				if(fscanf(fichier,"%lf %lf %lf %lf\n", &(camera.inverse[3]), &(camera.inverse[7]), &(camera.inverse[11]), &(camera.inverse[15])));

				if(fscanf(fichier,"\n"));
			}
		}
	}
	fclose(fichier);
}

int rayInterCercle(G3Xpoint pos,G3Xvector dir, G3Xpoint ri){
	double ps = G3Xprodscal(pos,dir);
	if(ps>= 0 ){
		return 0;
	}
	double d2 = G3Xsqrvnorm(pos) - ps*ps;
	if(d2>= 1){
		return 0;
	}
	double t = -ps - sqrt(1-d2);
	/*j'ai mon contact*/
	ri[0] = pos[0] + t * dir[0];
	ri[1] = pos[1] + t * dir[1];
	ri[2] = pos[2] + t * dir[2];

	return 1;
}


int rayInterTriangle(G3Xpoint pos, G3Xvector dir, G3Xpoint ri){
	G3Xvector normal = {1,0,0};
	double ps = G3Xprodscal(normal,dir);
	if(ps>=0){
		return 0;
	}
	double k = (0-pos[0])/dir[0];

	G3Xpoint proj =  {
		0, 
		k * dir[1] + pos[1] , 
		k * dir[2] + pos[2]};
	if(proj[0] != 0 || 
		proj[1] < 0 || 
		proj[2] < 0 || 
		proj[1] + proj[2] > 1){

		return 0;
	}
	ri = proj;
	return 1;
}

int rayInterRectangle(G3Xpoint pos, G3Xvector dir, G3Xpoint ri){
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
			G3Xpoint proj = {0,0,0};
			switch(i){
				case 0:
					k = (0.5-pos[0])/dir[0];
					proj[0] = 0.5; 
					proj[1] = k * dir[1] + pos[1]; 
					proj[2] = k * dir[2] + pos[2];
					break;
				case 1:
					k = (-0.5-pos[0])/dir[0];

					proj[0] = -0.5; 
					proj[1] = k * dir[1] + pos[1]; 
					proj[2] = k * dir[2] + pos[2];
					break;
				case 2:
					k = (0.5-pos[1])/dir[1];

					proj[0] = k * dir[0] + pos[0];
					proj[1] = 0.5;  
					proj[2] = k * dir[2] + pos[2];
					break;
				case 3:
					k = (-0.5-pos[1])/dir[1];

					proj[0] = k * dir[0] + pos[0];
					proj[1] = -0.5; 
					proj[2] = k * dir[2] + pos[2];
					break;
				case 4:
					k = (0.5-pos[2])/dir[2];

					proj[0] = k * dir[0] + pos[0];
					proj[1] = k * dir[1] + pos[1];
					proj[2] = 0.5;
					break;
				case 5:
					k = (-0.5-pos[2])/dir[2];

					proj[0] = k * dir[0] + pos[0];
					proj[1] = k * dir[1] + pos[1];
					proj[2] = -0.5;
					break;
			}

			if( proj[0] < -0.5 ||
				proj[0] > 0,5 ||
				proj[1] < -0.5 ||
				proj[1] > 0,5 ||
				proj[2] < -0.5 ||
				proj[2] > 0,5 ){
				return 0;
			}
			ri = proj;
		}
	}

	return 1;

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
			G3Xpoint canPixel = {0,i-IMAGE_SIZE/2 ,j-IMAGE_SIZE/2};
			G3Xpoint pixel;
			G3Xvector canRay;
			G3Xvector ray;
			G3Xsetvct(canRay,canFocale,canPixel);
			g3x_ProdHMatVector(camera.transfo,canRay,ray);
			g3x_ProdHMatPoint(camera.transfo,canPixel,pixel);
			int lastInter = INT_MAX;
			G3Xcolor toSet = {0,0,0};
			int k = 0;

			for(k = 0; k<nbObjects;k++){
				G3Xvector inObjectRay;
				G3Xpoint inObjectPixel;
				g3x_ProdHMatVector(objects[k].inverse,ray,inObjectRay);
				g3x_ProdHMatPoint(objects[k].inverse,pixel,inObjectPixel);
				G3Xpoint inObjectRi;
				G3Xpoint ri;
				if(objects[k].intersection(inObjectPixel,inObjectRay,inObjectRi) == 1){
					g3x_ProdHMatPoint(objects[k].transfo,inObjectRi, ri);
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
	for(i = 0; i<nbObjects;i++){
		printMat(objects[i].inverse);
		printf("\n");
		printMat(objects[i].transfo);
	}
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