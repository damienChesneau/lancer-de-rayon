#include <g3x.h>


int rayInterCercle(G3Xpoint pos,G3Xvector dir, G3Xpoint ri);
int rayInterTriangle(G3Xpoint pos, G3Xvector dir, G3Xpoint ri);
int rayInterRectangle(G3Xpoint pos, G3Xvector dir, G3Xpoint ri);
void initObjects(char* src);
char* getOpt(char* opt,int argc,char* argv[]);
void doLevel1(int argc,char* argv[]);
void doLevel2(int argc,char* argv[]);
void doLevel3(int argc,char* argv[]);
void doLevel4(int argc,char* argv[]);

typedef struct{
	G3Xpoint a;
	G3Xpoint b;
	G3Xpoint c;
	int (*intersection)(G3Xpoint,G3Xvector,G3Xpoint);
}Triangle;

typedef struct{
	G3Xpoint centre;
	double rayon;
	int (*intersection)(G3Xpoint,G3Xvector,G3Xpoint);
}Sphere;

typedef struct{
	G3Xpoint depart;
	G3Xvector longueur;
	G3Xvector largeur;
	G3Xvector hauteur;
	int (*intersection)(G3Xpoint,G3Xvector,G3Xpoint);
}Rectangle;

typedef struct{
	G3Xpoint centre;
	double rayon;
	G3Xvector hauteur;
	int (*intersection)(G3Xpoint,G3Xvector,G3Xpoint);
}Cylindre;

typedef struct{
	G3Xpoint pos;
	G3Xvector dir;
}Rayon;

typedef struct{
	int this;
	G3Xhmat transfo;
	G3Xhmat inverse;
}Object;


Triangle triangle = {	
	{0,0,0}, 
	{1,0,0}, 
	{0,1,0},	
	rayInterTriangle
};

Sphere sphere = {
	{0,0,0},
	1,
	rayInterCercle
};

Rectangle rectangle = {
	{0,0,0},
	{1,0,0},
	{0,1,0},
	{0,0,1},
	rayInterRectangle
};


Cylindre cylindre = {
	{0,0,0},
	1,
	{0,0,1},
	NULL
};



Object camera;
Object objects[100];
int nbObjects = 0;
float image[32][32];
/*****************************************************************************************************/

void initObjects(char* src){
	FILE* fichier = fopen(src,"r");
	if (fichier != NULL){
		int type = 0;
		while(fscanf(fichier,"%d\t",&type)){
			int i = 0;
			if(type != 4){
				objects[nbObjects].this = type;
				for(i = 0; i<16; i++){
					if(fscanf(fichier,"%lf ",&(objects[nbObjects].transfo[i])));
				}
				for(i = 0; i<16; i++){
					if(fscanf(fichier,"%lf ",&(objects[nbObjects].inverse[i])));
				}
				nbObjects++;
			}else{
				for(i = 0; i<16; i++){
					if(fscanf(fichier,"%lf ",&(camera.transfo[i])));
				}
				for(i = 0; i<16; i++){
					if(fscanf(fichier,"%lf ",&(camera.inverse[i])));
				}
			}
			fscanf(fichier, "\n");
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
			G3Xvector proj = {0,0,0};
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
		}
	}

	return 1;

}

/****************************************************************************************************/
void doLevel1(int argc,char* argv[]){
	int i = 0;
	int j = 0;
	char * src = getOpt("-i",argc,argv);
	initObjects(src);
	for(i = 0; i<32; i++){
		for(j = 0; j<32; j++){
			
		}
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