#include <g3x.h>


void prodMatV(G3Xhmat mat, G3Xvector v,G3Xvector r);
void prodMatP(G3Xhmat mat, G3Xpoint p, G3Xpoint r);
void prodMatM(G3Xhmat a, G3Xhmat b, G3Xhmat r);


typedef struct{
	G3Xcolor color;
	int (*intersection)(G3Xpoint,G3Xvector,G3Xpoint);
	G3Xhmat transfo;
	G3Xhmat inverse;
}Object;