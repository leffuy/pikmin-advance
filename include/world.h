#include "sprite.h"
#define OFFXY512(x,y) ((512*(y))+(x)) 



typedef struct{
int objPos[6];
SPRITE* aPikmin;
SPRITE* bPikmin;

}WORLD;



void changePalSet(int worldtimer, unsigned short* curPal, unsigned short* pal);
int checkXSides(SPRITE* sprite);
int checkYSides(SPRITE* sprite);
int findInc(int first, int second);
int checkCollision(SPRITE* sprite, unsigned short* map);
int checkLtCol(SPRITE* sprite, unsigned short* map);
int checkRtCol(SPRITE* sprite, unsigned short* map);
int checkDnCol(SPRITE* sprite, unsigned short* map);
int checkUpCol(SPRITE* sprite, unsigned short* map);