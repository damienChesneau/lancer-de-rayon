#include "../include/level3.h"


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

		default:
			printf("Unknown level.\n");
			break;
	}
	return 1;
}