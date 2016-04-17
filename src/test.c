#include<g3x.h>

typedef struct{
	G3Xpoint a;
	G3Xpoint u;
}Ray;



void init (void){

}

void draw(void){
	/*glPushMatrix();
		glTranslatef(5.,-3,2);
		glRotated(30.,0,1.,0);
		glScalef(2.,1,0.5);*/
		glutSolidSphere(1.,40,40);
	/*glPopMatrix();*/
	glDisable(GL_LIGHTING);
		glColor3f(1.,0.,0.);
		glBegin(GL_LINES	);
			glVertex3dv(A);
			glVertex3d(A[0] + u[0],A[1] + u[1], A[2] + u[2]);
		glEnd();
	glEnable(GL_LIGHTING);
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
  /*g3x_SetAnimFunction(anim);*/
  
  return g3x_MainStart();
}
