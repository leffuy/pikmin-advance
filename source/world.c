#include "world.h"
#include "dma.h"








void changePalSet(int worldtimer, unsigned short* curPal, unsigned short* pal){
	unsigned short* palmem = (unsigned short*)0x5000000;
	unsigned short* spal = (unsigned short*)0x5000200;
	int i;
	static int change = 0;
	static int timer = 0;
	if(worldtimer == 3500){
		change = 1;}
	if(change == 1 && worldtimer%26 == 0){
		for(i = 0; i < 16; i++){palmem[i] += COLOR(0,0,1);
		palmem[i] -= COLOR(1,0,0);
		spal[i] += COLOR(0,0,1);
		spal[i] -= COLOR(0,0,0);}
		if(timer > 3){change = 0;}
		timer++;}
}
		

int checkXSides(SPRITE* sprite){
	if(sprite->x < 32*2 && sprite->x > 0){
		return 1;}
	if(sprite->x > 256*4-32*6){
		return 1;}
	return 0;}

int checkYSides(SPRITE* sprite){
	if(sprite->y < 32*2 && sprite->y >0){
		return 1;}
	if(sprite->y > 256*4-32*3){
		return 1;}
	return 0;}
	
int findInc(int first, int second){
	int dT = first - second;
	if(dT < 0){dT = dT*(-1);}
	return dT;}


int checkCollision(SPRITE* sprite, unsigned short* map){
	int x1, x2, x3, x4, y1, y2, y3, y4;
	x1 = sprite->x;
	y1 = sprite->y;
	x2 = sprite->x + sprite->width;
	y2 = sprite->y;
	x3 = sprite->x + sprite->width;
	y3 = sprite->y + sprite->height;
	x4 = sprite->x;
	y4 = sprite->y + sprite->height;
	if((map[OFFXY512(x1,y1)] == 0) || (map[OFFXY512(x2,y2)] == 0) || (map[OFFXY512(x3,y3)] == 0) || (map[OFFXY512(x4,y4)] == 0)){
		return 1;
	}
	return 0;
}

int checkRtCol(SPRITE* sprite, unsigned short* map){
	int x1, x2, y1, y2;
	x1 = sprite->x + sprite->width;
	y1 = sprite->y;
	x2 = sprite->x + sprite->width;
	y2 = sprite->y + sprite->height;
	//int t = 0;
	if((map[OFFXY512(x1,y1)] == 0 || map[OFFXY512(x2,y2)] == 0) && (map[OFFXY512(x1,y1+1)] == 0 || map[OFFXY512(x2,y2+1)] == 0)){
		return 1;
		//up no more
	}
	return 0;
}

int checkDnCol(SPRITE* sprite, unsigned short* map){
	int x1, x2, y1, y2;
	x1 = sprite->x;
	y1 = sprite->y + sprite->height;
	x2 = sprite->x + sprite->width;
	y2 = sprite->y + sprite->height;
	if((map[OFFXY512(x1,y1)] == 0 || map[OFFXY512(x2,y2)] == 0) && (map[OFFXY512(x1+1,y1)] == 0 || map[OFFXY512(x2-1,y2)] == 0)){
		return 1;
	}
	return 0;
}

int checkLtCol(SPRITE* sprite, unsigned short* map){
	int x1, x2, y1, y2;
	x1 = sprite->x;
	y1 = sprite->y;
	x2 = sprite->x;
	y2 = sprite->y + sprite->height;
	if((map[OFFXY512(x1,y1)] == 0 || map[OFFXY512(x2,y2)] == 0) && (map[OFFXY512(x1,y1+1)] == 0 || map[OFFXY512(x2,y2-1)] == 0)){
		return 1;
	}
	return 0;
}

int checkUpCol(SPRITE* sprite, unsigned short* map){
	int x1, x2, y1, y2;
	x1 = sprite->x;
	y1 = sprite->y;
	x2 = sprite->x + sprite->width;
	y2 = sprite->y;
	if((map[OFFXY512(x1,y1)] == 0 || map[OFFXY512(x2,y2)] == 0) && (map[OFFXY512(x1+1,y1)] == 0 || map[OFFXY512(x2-1,y2)] == 0)){
		return 1;
	}
	return 0;
}
	
