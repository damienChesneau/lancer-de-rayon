#include "../include/level3.h"


int main(int argc, char* argv[]){
	int level = atoi(lvl1_getOpt("-n",argc,argv));
	switch(level){
		case 1:	
			lvl1_do(argc,argv);
			break;


		case 2:	
			lvl2_do(argc,argv);
			break;


		case 3:	
			lvl3_do(argc,argv);
			break;

		default:
			printf("Unknown level.\n");
			break;
	}
	return 1;
}