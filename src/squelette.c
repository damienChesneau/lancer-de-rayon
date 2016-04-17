#include<g3x.h>

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
