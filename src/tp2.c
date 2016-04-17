#include<g3x.h>



int rayInterCo(G3Xpoint a,G3Xvector u, G3Xpoint i){
	double ps = G3Xprodscal((G3Xvector)a,u);
	if(ps>= 0 ){
		return 0;
	}
	double d2 = G3Xsqrvnorm((G3Xvector)a) - ps*ps;
	if(d2>= 1){
		return 0;
	}
	double t = -ps - sqrt(1-d2);
	/*j'ai mon contact*/
	i[0] = a[0] + t * u[0];
	i[1] = a[1] + t * u[1];
	i[2] = a[2] + t * u[2];
	return 1;
}




void init (void){

}

void draw(void){

}

void anim(void){

}

void quit(void){

}

int main (int argc, char* argv[]){
  g3x_InitWindow(*argv,500,500);
  g3x_SetInitFunction(init);
  g3x_SetExitFunction(quit);
  g3x_SetDrawFunction(draw);
  //g3x_SetAnimFunction(anim);
  
  return g3x_MainStart();
}
