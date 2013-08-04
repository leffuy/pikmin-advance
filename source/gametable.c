#include "gametable.h"

void processGame(GAMEFLOW* gTable){
	gTable->loopFlag = 0;
	if(gTable->breakState == 1){
		gTable->loopFlag = 255;
		gTable->breakState = 0;
		if(gTable->state == gTable->maxState){
		gTable->state = 0;}
		gTable->state += 1;
	}
}