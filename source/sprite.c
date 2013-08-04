#include "sprite.h"
#include "dma.h"

void shiftState(SPRITE* sprite){
	sprite->curStatePtr = &(sprite->startPtr[(sprite->curState-1)*2048]);
}


void placeIntoBuffer(SPRITE sprite, SPRITE* table){
	table[sprite.bufferSlot] = sprite;
}
	
void streamBuffer(SPRITE* table){
		int i;
		for(i = 0; i<TABLE_SIZE; i++){
			dmaTransfer((unsigned short*)SBANK(i), table[i].curStatePtr, 2048, 3, (DMA_ON | DMA_NOW));
		}
}

void checkCycles(SPRITE* sprite){
	sprite->curCycle += 1;
	if(sprite->curCycle >= sprite->cycleTime){
		sprite->curCycle = 0;
		if(sprite->curState >= sprite->totalStates){
			sprite->curState = 0;}
		sprite->curState += 1;}
}

void buildOAM(int block, unsigned short att0, unsigned short att1, unsigned short att2){
	unsigned short atts[3];
	atts[0] = att0;
	atts[1] = att1;
	atts[2] = att2;
	dmaTransfer((unsigned short*)OAMBLOCK(block), atts, 3, 3, (DMA_ON | DMA_NOW));
}

