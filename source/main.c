#include "video.h"
#include "types.h"
#include "register.h"
#include "input.h"
#include "dma.h"
#include "pikminS1.h"
#include "gametable.h"
#include "sprite.h"
#include "test.h"
#include "pikminSplash2.h"
#include "pathfinding.h"
#include "timer.h"
#include "sound.h"
#include "up.h" 
#include "piklvl01_bin.h"
#include "Pikmin2_bin.h"
//#include "pikm.h"
#include "interrupts.h"
#include "marble.h"
#include "olipik.h"
#include "tmap.h"
#include "Whistle_bin.h"
#include "Dismiss_bin.h"
#include "Throw_bin.h"
#include "pik22_bin.h"
#include "level1.h"
#include "blend.h"
#include "forest_bin.h"
#include "endGame.h"
#include "end_bin.h"

#define XMASK 0x01FF
#define YMASK 0x00FF
#define OFFSET512(x,y) ((512*(y))+(x)) 

#define TILEBLOCK(i) ((i)*128)

#define REG_DISPSTAT  (*(unsigned short*)0x4000004)

#define REG_BG0CNT  (*(u16 *) 0x4000008) //BG 0 control
#define REG_BG0HOFS (*(u16 *) 0x4000010) //BG 0 Horizontal Offset
#define REG_BG0VOFS (*(u16 *) 0x4000012) //BG 0 Vertical Offset
#define REG_BG1CNT  (*(u16 *) 0x400000A) //BG 1 control
#define REG_BG1HOFS (*(u16 *) 0x4000014) //BG 1 Horizontal Offset
#define REG_BG1VOFS (*(u16 *) 0x4000016) //BG 1 Vertical Offset
#define REG_BG2CNT  (*(u16 *) 0x400000C) //BG 2 control
#define REG_BG2HOFS (*(u16 *) 0x4000018) //BG 2 Horizontal Offset
#define REG_BG2VOFS (*(u16 *) 0x400001A) //BG 2 Vertical Offset
#define REG_BG3CNT  (*(u16 *) 0x400000E) //BG 3 control
#define REG_BG3HOFS (*(u16 *) 0x400001C) //BG 3 Horizontal Offset
#define REG_BG3VOFS (*(u16 *) 0x400001E) //BG 3 Vertical Offset

#define BG_4BPP (0 << 7)
#define BG_8BPP (1 << 7)
#define BG_REG_32x32 (0 << 14)
#define BG_REG_64x32 (1 << 14)
#define BG_REG_32x64 (2 << 14)
#define BG_REG_64x64 (3 << 14)

#define CBB(i) ((i) << 2) //place this in the BG CNT register to define what Character block the data is in
#define SBB(i) ((i) << 8)

int airtime = 0;
int gdX = 0;
int	gdY = 0;
unsigned short* curPal;
int wTime = 0;
static int gCycle = 1;
int vOff = 0;
int hOff = 0;
int pikH = 0;
int pikV = 0;
int calling = 0;
int staying = 0;
int throwing = 0;
int start = 1;
int firstTime = 1;
int nokey;
unsigned short sampleLengthA;
unsigned short samplePositionA;
unsigned short sampleRateA;
unsigned short numVblanksA;
unsigned short sampleLengthB;
unsigned short sampleRateB;
unsigned short samplePositionB;
unsigned short numVblanksB;
unsigned short* sprMap = (unsigned short*)0xE00AAAA;
//defines for game based functions
void gameInit(void);
void gameSplash(void);
void gameMain(void);
void userInputSplash(void);
void userInputMain(void);
void update(void);
void render(void);
void gameInit(void);
void splashes(int splash);
void processPikmin(SPRITE* pikmin);
void pikminMove(SPRITE* pikmin);
void findPikminScreen(SPRITE* pikmin);
void buildIRQ(void);
void interruptHandler(void);
void playASound(void *sound, unsigned short length, unsigned short rate);
void playBSound(void *sound, unsigned short length, unsigned short rate);
void updatePikminDist(SPRITE* pikmin);
unsigned short* tempOAM = (unsigned short*)0x7000000;
int called(SPRITE* pikmin);
int stay(SPRITE* pikmin);
int thrown(SPRITE* pikmin);
void olimarCall(void);
void olimarStay(void);
void olimarThrow(void);
void resetVar(void);
void pikminThrow(int startX, int startY, int endX, int endY, SPRITE* pikmin);
void stopSound(int* cycles, int thresh, int* stop);
void calibratePikmin(SPRITE* pikmin1, SPRITE* pikmin2);
void adjustpikminThrow(SPRITE* sprite);
void gameEnd(void);

//sprite table and sprites
WPAL testpal;
SPRITE table[TABLE_SIZE];
SPRITE pikminF; //sprite entry 1
SPRITE olimarF;
SPRITE pikminB;
SPRITE olimarB;	//sprite entry 2
SPRITE pikminR;
SPRITE olimarR;
SPRITE pikminL;
SPRITE olimarL;
SPRITE pikminTL;
SPRITE pikminTR;
SPRITE aOlimar;
SPRITE aObject;
SPRITE bObject;
SPRITE cObject;
SPRITE dObject;
SPRITE aPikmin;
SPRITE bPikmin;
PATH4  testpath;
PATH4  path2;
PATH4  path3;
PATH4* pathTable[3];
GAMEFLOW gameTable;
int cycleRestart = 0;



void gameEnd(void){
	static int timer = 0;
	REG_DISPCTL = MODE_3 | BG2_ENABLE;
	dmaTransfer(0,0,0,1,0);
	dmaTransfer((unsigned short*)VRAM_BASE, endGameBitmap, endGameBitmapLen/2, 3, (DMA_ON | DMA_NOW));
	playASound(end_bin, 4000128, 11025);
	while(1){
	if(timer >= 20000*62){
		dmaTransfer(0,0,0,1,0);}
	if(keysPressed() & KEY_START){
		return;}
	timer++;
	}
}
//SOUNDCLIP pikTheme;
//main function
int main(void){
	//add to game init

	gameInit();
	while(1){
	gameSplash();
	gameMain();
	gameEnd();
	}
return 0;
}
//game splash screen control
void gameSplash(void){
	REG_DISPCTL = MODE_3 | BG2_ENABLE;
	//playASound(Pikmin2_bin, 1, 11025);
	REG_SND_STAT = SND_STAT_ON;
    REG_SND_DSCNT = SND_DSCNT_ARESET|SND_DSCNT_A50|SND_DSCNT_AL|SND_DSCNT_ATMR0;
    REG_SND_DSCNT |= SND_DSCNT_BRESET|SND_DSCNT_B50|SND_DSCNT_BR|SND_DSCNT_BTMR1;
	//playASound(Pikmin2_bin, 19408, 11025);
	while(gameTable.loopFlag != 255){
	//if(airtime >= 100){
	//stopSound(&airtime, 200);
	splashes(3);
	userInputSplash();
	processGame(&gameTable);
	waitForVBlank();
	airtime+=1;
	}
}
//main game loop
void gameMain(void){
	unsigned short white = 0xFFFF;
	REG_DISPCTL = 0;
	REG_DISPCTL = MODE_0 | OBJ_ENABLE | OBJ_1D | BG0_ENABLE;
	//place into own code state	
	dmaTransfer((unsigned short*)VRAM_BASE, level1Tiles, level1TilesLen/2, 3, (DMA_ON | DMA_NOW));
	dmaTransfer((unsigned short*)0x5000000, level1Pal, 256, 3, (DMA_ON | DMA_NOW));
	dmaTransfer((unsigned short*)(VRAM_BASE+(0x800*28)), level1Map, level1MapLen/2, 3, (DMA_ON | DMA_NOW));
	REG_BG0CNT = 3 | BG_4BPP | BG_REG_64x64 | CBB(0) | SBB(28);
//	REG_BG1CNT = 3 | BG_8BPP | BG_REG_32x32 | CBB(6) | SBB(27;
	//endchange
	while(1){
	//dmaTransfer(0,0,0,1,0);
	changePalSet(wTime, curPal, level1Pal);
	if(firstTime == 1){playASound(forest_bin, 7402960, 11025);
    firstTime = 0;}
	calibratePikmin(&aPikmin, &bPikmin);
	calibratePikmin(&bPikmin, &aPikmin);
	userInputMain();
	findPikminScreen(&aPikmin);
	findPikminScreen(&bPikmin);
	findPikminScreen(&aObject);
	findPikminScreen(&bObject);
	findPikminScreen(&cObject);
	processPikmin(&aPikmin);
	processPikmin(&bPikmin);
//	pikminMove(&bPikmin);
	processGame(&gameTable);
	render();
	waitForVBlank();
	airtime+=1;
	wTime++;
	if(wTime >= 5000){
		break;}
	}
}

//game init
void gameInit(void){
	//sprite building
	aObject.totalStates = 1;
	aObject.curState = 1;
	aObject.cycleTime = 1;
	aObject.startPtr = &olipikTiles[2048*51];
	aObject.x = 100;
	aObject.y = 300;
	//aObject.screenX = 90;
	//aObject.screenY = 90;
	aObject.bufferSlot = 3;
	aObject.id = 0; 
	bObject.totalStates = 1;
	bObject.curState = 1;
	bObject.cycleTime = 1;
	bObject.startPtr = &olipikTiles[2048*51];
	bObject.x = 100;
	bObject.y = 500;
	bObject.bufferSlot = 4;
	bObject.id = 1;
	cObject.totalStates = 1;
	cObject.curState = 1;
	cObject.cycleTime = 1;
	cObject.startPtr = &olipikTiles[2048*52];
	cObject.x = 300;
	cObject.y = 355;
	cObject.bufferSlot = 5;
	cObject.id = 2;
	olimarF.totalStates = 4;
	olimarF.curState = 1;
	olimarF.cycleTime = 6;
	olimarF.curCycle = 1;
	olimarF.startPtr = &olipikTiles[2048*1];
	olimarB.startPtr = &olipikTiles[2048*5];
	olimarR.startPtr = &olipikTiles[2048*9];
	olimarL.startPtr = &olipikTiles[2048*13];
	pikminF.totalStates = 5;
	pikminF.curState = 1;
	pikminF.cycleTime = 4;
	pikminF.curCycle = 1;
	pikminF.startPtr = &olipikTiles[2048*17]; 
	pikminB.startPtr = &olipikTiles[2048*23];
	pikminR.startPtr = &olipikTiles[2048*28];
	pikminL.startPtr = &olipikTiles[2048*32];
	pikminR.totalStates = 4;
	pikminTL.totalStates = 8;
	pikminTL.curState = 1;
	pikminTL.cycleTime = 2;
	pikminTL.curCycle = 1;
	pikminTL.startPtr = &olipikTiles[2048*34];
	pikminTR.totalStates = 8;
	pikminTR.curState = 1;
	pikminTR.cycleTime = 2;
	pikminTR.startPtr = &olipikTiles[2048*42];
	aPikmin = pikminF;
	aOlimar = olimarF;
	aOlimar.width = 64;
	aOlimar.height = 64;
	aOlimar.x = 50;
	aOlimar.y = 50;
	aOlimar.screenX = 50;
	aOlimar.screenY = 50;
	aOlimar.att2 = ATTR2_PRIO_2;
	aOlimar.att0 = DEFATTR0;
	aOlimar.bufferSlot = 0;
	aPikmin.width = 64;
	aPikmin.height = 64;
	aPikmin.x = 100;
	aPikmin.y = 20;
	aPikmin.screenX = 100;
	aPikmin.screenY = 20;
	aPikmin.bufferSlot = 1;
	aPikmin.process = 1;
	bPikmin = pikminF;
	bPikmin.x = 90;
	bPikmin.y = 90;
	bPikmin.screenX = 90;
	bPikmin.screenY = 90;
	bPikmin.bufferSlot = 2;
	bPikmin.process = 0;
	placeIntoBuffer(aOlimar, table);
	placeIntoBuffer(aPikmin, table);
	placeIntoBuffer(bPikmin, table);
	placeIntoBuffer(aObject, table);
	placeIntoBuffer(bObject, table);
	placeIntoBuffer(cObject, table);
	dmaTransfer((unsigned short*)SPRITEPAL, olipikPal, 256, 3, (DMA_ON | DMA_NOW));
	
	
	//path init
	testpath.x1 = 75;
	testpath.y1 = 250;
	
	testpath.x2 = 50;
	testpath.y2 = 100;
	
	testpath.x3 = 50;
	testpath.y3 = 75;
	
	testpath.x4 = 50;
	testpath.y4 = 50;
	
	path2.x1 = 30;
	path2.y1 = 485;
	
	path2.x2 = 10;
	path2.y2 = 400;
	
	path2.x3 = 50;
	path2.y3 = 400;
	
	path2.x4 = 50;
	path2.y4 = 50;
	
	path3.x1 = 85;
	path3.y1 = 255;
	
	path3.x2 = 50;
	path3.y2 = 255;
	
	path3.x3 = 50;
	path3.x4 = 150;
	
	path3.x4 = 50;
	path3.x4 = 50;
	
	pathTable[0] = &testpath;
	pathTable[1] = &path2;
	pathTable[2] = &path3;
	
	//palette shit
	
	testpal.morningPal = level1Pal;
}
	
//////////////////////////////////
///Interactive engine pieces
//////////////////////////////

//splash user input
//////////////////////

void splashes(int splash){
	unsigned short yellow = 0x0ABA;
	unsigned short white = 0xFFFF;
	switch(splash){
		case 1:
			dmaTransfer((unsigned short*)VRAM_BASE, &yellow, 240*160, 3, (DMA_ON | DMA_SOURCE_FIXED | DMA_NOW));
			break;
		case 2:
			dmaTransfer((unsigned short*)VRAM_BASE, &white, 240*160, 3, (DMA_ON | DMA_SOURCE_FIXED | DMA_NOW));
			break;
		case 3:
			dmaTransfer((unsigned short*)VRAM_BASE, pikminSplash2Bitmap, 240*160, 3, (DMA_ON | DMA_NOW));
			break;
	}
}

void userInputSplash(void){
	//receive key data
	scanKeys();
	if(keysPressed() & KEY_START){ //do this block for when start button is pressed every cycle
		gameTable.breakState = 1;
		}
}

//main user input
//
void userInputMain(void){
	static int soundPlaying = 0;
	static int soundFirstPlayed = 1;
	static int playtime = 0;
	static int disableUP;
	static int disableDN;
	static int disableRT;
	static int disableLT;
	scanKeys();
	
	if(checkRtCol(&aOlimar, tmapBitmap) == 1){disableRT = 1;}
	else{disableRT = 0;}
	if(checkLtCol(&aOlimar, tmapBitmap) == 1){disableLT = 1;}
	else{disableLT = 0;}
	if(checkUpCol(&aOlimar, tmapBitmap) == 1){disableUP = 1;}
	else{disableUP = 0;}
	if(checkDnCol(&aOlimar, tmapBitmap) == 1){disableDN = 1;}
	else{disableDN = 0;}
						
	if((keysPressed() & KEY_A)){
		olimarCall();
		playBSound(Whistle_bin, 54000, 11025);
		soundPlaying = 1;}
	if(keysReleased() & KEY_A){
		resetVars();}
		
	if(keysPressed() & KEY_B){
		olimarStay();
		playBSound(Dismiss_bin, 54000, 11025);
		soundPlaying = 1;}
	if(keysReleased() & KEY_B){
		resetVars();}
		
	if(keysHeld() & KEY_R){
		olimarThrow();
		soundPlaying = 1;}
	if(keysReleased() & KEY_R){
		resetVars();}
	

	
	if((keysHeld() & KEY_DOWN) && aOlimar.y < 256*4-64 && disableDN == 0){
		aOlimar.direction = 1;
		aOlimar.startPtr = olimarF.startPtr;
		aOlimar.y++;
		vOff++;
		pikV--;
		checkCycles(&aOlimar);
		if((checkYSides(&aOlimar) == 1)){
			aOlimar.screenY++;
			vOff--;
			pikV++;
	}
}
		if(keysHeld() & KEY_UP && aOlimar.y > 0 && disableUP == 0){
		aOlimar.direction = 2;
		aOlimar.startPtr = olimarB.startPtr;
		aOlimar.att2 = ATTR2_PRIO_1;
		aOlimar.y--;
		vOff--;
		pikV++;
		checkCycles(&aOlimar);
		if((checkYSides(&aOlimar) == 1)){
			aOlimar.screenY--;
			vOff++;
			pikV--;
	}
}
	if(keysHeld() & KEY_LEFT && aOlimar.x > 0 && disableLT == 0){
		aOlimar.direction = 3;
		aOlimar.startPtr = olimarR.startPtr;
		aOlimar.x--;
		hOff--;
		pikH++;
		checkCycles(&aOlimar);
		if((checkXSides(&aOlimar) == 1)){
			aOlimar.screenX--;
			hOff++;
			pikH--;
	}
}
		if(keysHeld() & KEY_RIGHT && aOlimar.x < 256*4-64 && disableRT == 0){
		aOlimar.direction = 4;
		aOlimar.startPtr = olimarL.startPtr;
		aOlimar.x++;
		hOff++;
		pikH--;
		checkCycles(&aOlimar);
		if((checkXSides(&aOlimar) == 1)){
			aOlimar.screenX++;
			hOff--;
			pikH++;
	}
}
		if(soundPlaying == 1){
		stopSound(&playtime, 70, &soundPlaying);
		playtime += 1;}
		
}	

//Master Render control
void render(void){
	buildOAM(0, (aOlimar.att0 | aOlimar.screenY), (ATTR1_SIZE_64x64 | aOlimar.screenX), (aOlimar.att2 | TILEBLOCK(0)));
	buildOAM(1, (aPikmin.att0 | aPikmin.screenY), (ATTR1_SIZE_64x64 | aPikmin.screenX), (aPikmin.att2 | TILEBLOCK(1)));
	buildOAM(2, (bPikmin.att0 | bPikmin.screenY), (ATTR1_SIZE_64x64 | bPikmin.screenX), (bPikmin.att2 | TILEBLOCK(2)));
	buildOAM(3, (aObject.att0 | aObject.screenY), (ATTR1_SIZE_64x64 | aObject.screenX), (aObject.att2 | TILEBLOCK(3)));
	buildOAM(4, (bObject.att0 | bObject.screenY), (ATTR1_SIZE_64x64 | bObject.screenX), (bObject.att2 | TILEBLOCK(4)));
	buildOAM(5, (cObject.att0 | cObject.screenY), (ATTR1_SIZE_64x64 | cObject.screenX), (cObject.att2 | TILEBLOCK(5)));
	
	shiftState(&aOlimar);
	shiftState(&aPikmin);
	shiftState(&bPikmin);
	shiftState(&aObject);
	shiftState(&bObject);
	shiftState(&cObject);
	placeIntoBuffer(aOlimar, table);
	placeIntoBuffer(aPikmin, table);
	placeIntoBuffer(bPikmin, table);
	placeIntoBuffer(aObject, table);
	placeIntoBuffer(bObject, table);
	placeIntoBuffer(cObject, table);
	streamBuffer(table);
	REG_BG0HOFS = hOff;
	REG_BG0VOFS = vOff;
}

void resetVars(void){
	calling = 0;
	staying = 0;
	throwing = 0;}

void olimarCall(void){
	calling = 10;}
void olimarStay(void){
	staying = 10;}
void olimarThrow(void){
	throwing = 10;}

int called(SPRITE* pikmin){
	updatePikminDist(pikmin);
	if(pikmin->distolimar < 4400 && calling == 10){
		return 1;}
	return 0;
}
int stay(SPRITE* pikmin){
	updatePikminDist(pikmin);
	if(pikmin->distolimar < 4400 && staying == 10){
		return 1;}
	return 0;
}
int thrown(SPRITE* pikmin){
	updatePikminDist(pikmin);
	if(pikmin->distolimar < 100 && throwing == 10){
		playBSound(Throw_bin, 54000, 11025);
		return 1;}
	return 0;
}

void processPikmin(SPRITE* pikmin){
	updatePikminDist(pikmin);
	if(pikmin->process == 7){
		return;}
	if(pikmin->process == 0){
		pikminIdle(pikmin);}
	if(pikmin->process == 1){
		pikminMove(pikmin);}
	if(pikmin->process == 3){
		
		updatePikminDist(pikmin);
		if(aOlimar.direction == 4){
			pikmin->totalStates = pikminTR.totalStates;
			pikmin->startPtr = pikminTR.startPtr;
			pikmin->cycleTime = pikminTR.cycleTime;
			pikminThrow(aOlimar.x, aOlimar.y, aOlimar.x+30, aOlimar.y, pikmin);
			adjustPikminThrow(pikmin);}
		if(aOlimar.direction == 1){pikminThrow(aOlimar.x, aOlimar.y, aOlimar.x, aOlimar.y+30, pikmin);}
		if(aOlimar.direction == 2){pikminThrow(aOlimar.x, aOlimar.y, aOlimar.x, aOlimar.y-30, pikmin);}
		if(aOlimar.direction == 3){
			pikmin->totalStates = pikminTL.totalStates;
			pikmin->startPtr = pikminTL.startPtr;
			pikmin->cycleTime = pikminTL.cycleTime;
			pikminThrow(aOlimar.x, aOlimar.y, aOlimar.x-30, aOlimar.y, pikmin);
			adjustPikminThrow(pikmin);}
		if(pikmin->aODist < 1000){pikmin->process = 4;
		pikmin->object = &aObject; }
		if(pikmin->bODist < 1000){pikmin->process = 4;
		pikmin->object = &bObject; }
		if(pikmin->cODist < 1000){pikmin->process = 4;
		pikmin->object = &cObject; }
		//if(pikmin->dODist < 1000){pikmin->process = 4;
		//pikmin->object = &dObject; }
	}
	if(pikmin->process == 4){pikminReturn(pikmin);
	pikminFollow(pikmin);}
}

void pikminIdle(SPRITE* pikmin){
	pikmin->curState = 2;
	gCycle = 0;
	shiftState(pikmin);
	if(called(pikmin) == 1){
		pikmin->process = 1;}
	if(thrown(pikmin) == 1){
		pikmin->process = 3;}
}

void findPikminScreen(SPRITE* pikmin){
	unsigned short white = 0xFFFF;
	unsigned short black = 0;
	int j;
	int k;
	int x = pikmin->x;
	int y = pikmin->y;
	int sX = hOff;
	int sY = vOff;
	int nvX = x - sX;
	int nvY = y - sY;
	pikmin->screenX = XMASK & nvX;
	pikmin->screenY = YMASK & nvY;
	if(nvX < -64 || nvY < -64 || nvX > 240 || nvY > 160){
	pikmin->att0 = DEFATTR0 | ATTR0_HIDE;}
	if((nvX > -64 && nvY > -64) && (nvX < 240 && nvY < 160)){
	pikmin->att0 = DEFATTR0;} 
}


//Pikmin pathfinding run per pikmin
//
void updatePikminDist(SPRITE* pikmin){
	//dist to olimar

	int dX = findInc(pikmin->x, aOlimar.x);
	int dY = findInc(pikmin->y, aOlimar.y);
	int dDistance = (dX*dX) + (dY*dY);
	pikmin->distolimar = dDistance;

	//dist to object A
	dX = findInc(pikmin->x, aObject.x);
	dY = findInc(pikmin->y, aObject.y);
	dDistance = (dX*dX) + (dY*dY);
	pikmin->aODist = dDistance;
	
	
	//dist to object B
	dX = findInc(pikmin->x, bObject.x);
	dY = findInc(pikmin->y, bObject.y);
	dDistance = (dX*dX) + (dY*dY);
	pikmin->bODist = dDistance;
	
	//dist to object C
	dX = findInc(pikmin->x, cObject.x);
	dY = findInc(pikmin->y, cObject.y);
	dDistance = (dX*dX) + (dY*dY);
	pikmin->cODist = dDistance;
	
	/*
	//dist to object D
	dX = findInc(pikmin->x, dObject.x);
	dY = findInc(pikmin->y, dObject.y);
	dDistance = (dX*dX) + (dY*dY);
	pikmin->dODist = dDistance;*/
	
}




void adjustPikminThrow(SPRITE* pikmin){
	int dX = findInc(pikmin->x, aOlimar.x);
	dX = dX*dX;
	if(dX <= (25*25)){pikmin->y -= 2;}
	if(dX > (25*25)){pikmin->y +=2;}
}
		

void pikminThrow(int startX, int startY, int endX, int endY, SPRITE* pikmin){
	static int timer;
	static int firstTime = 1;
	static int moveU;
	checkCycles(pikmin);
	gdX = endX - startX;
	gdY = endY - startY;
	pikmin->x += gdX/10;
	pikmin->y += gdY/10;
	updatePikminDist(pikmin);
	if(pikmin->distolimar > 2500){
		pikmin->cycleTime = pikminF.cycleTime;
		pikmin->curState = 1;
		pikmin->process = 0;}
	if(checkCollision(pikmin, tmapBitmap) == 1){
		pikmin->cycleTime = pikminF.cycleTime;
		pikmin->x -= gdX/10;
		pikmin->process = 0;}
	timer += 1;
}
	

			
		
void pikminMove(SPRITE* pikmin){
	updatePikminDist(pikmin);
	int i;
	int dx, dy;
	int px, py;
	int krx[2];
	int kry[2];
	int tSpot = pikmin->bufferSlot;
	dx = aOlimar.x - pikmin->x;
	dy = aOlimar.y - pikmin->y;
	px = dx/10;
	py = dy/10;
	if(thrown(pikmin) == 1){
		pikmin->process = 3;}
	if(stay(pikmin) == 1){
		pikmin->process = 0;}
	//if(pikmin->distolimar<=900){
	//	pikmin->curCycle = 1;}
	if(pikmin->distolimar>=1200){
	if(checkCollision(pikmin, tmapBitmap) == 0){
		pikmin->x += px;
		pikmin->y += py;}
	if(cycleRestart >= 0  && cycleRestart < 10 && checkCollision(pikmin, tmapBitmap) != 0){
		pikmin->x -= px;
		pikmin->y -= py;
		cycleRestart++;
		if(cycleRestart > 8){pikmin->process = 0;}
	}
	if((dy > dx) && (dy > 0 && dx > 0)){
		pikmin->startPtr = pikminF.startPtr;
		pikmin->totalStates = pikminF.totalStates;
		pikmin->att2 = ATTR2_PRIO_1;
		checkCycles(pikmin);}
	if((dy < dx) && (dy > 0 && dx > 0)){
		pikmin->startPtr = pikminR.startPtr;
		pikmin->totalStates = pikminR.totalStates;
		checkCycles(pikmin);}
	if((dy > (dx*(-1))) && (dy > 0 && dx < 0)){
		pikmin->startPtr = pikminF.startPtr;
		pikmin->totalStates = pikminF.totalStates;
		pikmin->att2 = ATTR2_PRIO_1;
		checkCycles(pikmin);}
	if((dy < (dx*(-1))) && (dy > 0 && dx < 0)){
		pikmin->startPtr = pikminL.startPtr;
		pikmin->totalStates = pikminR.totalStates;
		checkCycles(pikmin);}
	if(((dy*(-1)) < (dx*(-1))) && (dy < 0 && dx < 0)){
		pikmin->startPtr = pikminL.startPtr;
		pikmin->totalStates = pikminR.totalStates;
		checkCycles(pikmin);}
	if(((dy*(-1)) > (dx*(-1))) && (dy < 0 && dx < 0)){
		pikmin->startPtr = pikminB.startPtr;
		pikmin->totalStates = pikminF.totalStates;
		pikmin->att2 = ATTR2_PRIO_0;
		checkCycles(pikmin);}
	if(((dy*(-1)) > dx) && (dy < 0 && dx > 0)){
		pikmin->startPtr = pikminB.startPtr;
		pikmin->totalStates = pikminF.totalStates;
		pikmin->att2 = ATTR2_PRIO_0;
		checkCycles(pikmin);}
	if(((dy*(-1)) < dx) && (dy < 0 && dx > 0)){
		pikmin->startPtr = pikminR.startPtr;
		pikmin->totalStates = pikminR.totalStates;
		checkCycles(pikmin);}
	}


}


void stopSound(int* cycles, int thresh, int* stop){
	if(*cycles >= thresh){
	dmaTransfer(0,0,0,2,0);
	*cycles = 0;
	*stop = 0;
	}
}

void pikminFollow(SPRITE* pikmin){
	int i;
	int dx, dy;
	int px, py;
	int krx[2];
	int kry[2];
	SPRITE* obj;
	obj = (SPRITE*)(pikmin->object);
	int tSpot = pikmin->bufferSlot;
	dx = obj->x - pikmin->x;
	dy = obj->y - pikmin->y;
	px = dx/10;
	py = dy/10;
	if(thrown(pikmin) == 1){
		pikmin->process = 3;}
	if(stay(pikmin) == 1){
		pikmin->process = 0;}
	
	if(checkCollision(pikmin, tmapBitmap) == 0){
		pikmin->x += px;
		pikmin->y += py;}
	if(cycleRestart >= 0  && cycleRestart < 10 && checkCollision(pikmin, tmapBitmap) != 0){
		pikmin->x -= px;
		pikmin->y -= py;
		cycleRestart++;
		if(cycleRestart > 8){pikmin->process = 0;}
	}
	if((dy > dx) && (dy > 0 && dx > 0)){
		pikmin->startPtr = pikminF.startPtr;
		pikmin->totalStates = pikminF.totalStates;
		pikmin->att2 = ATTR2_PRIO_1;
		checkCycles(pikmin);}
	if((dy < dx) && (dy > 0 && dx > 0)){
		pikmin->startPtr = pikminR.startPtr;
		pikmin->totalStates = pikminR.totalStates;
		checkCycles(pikmin);}
	if((dy > (dx*(-1))) && (dy > 0 && dx < 0)){
		pikmin->startPtr = pikminF.startPtr;
		pikmin->totalStates = pikminF.totalStates;
		pikmin->att2 = ATTR2_PRIO_1;
		checkCycles(pikmin);}
	if((dy < (dx*(-1))) && (dy > 0 && dx < 0)){
		pikmin->startPtr = pikminL.startPtr;
		pikmin->totalStates = pikminR.totalStates;
		checkCycles(pikmin);}
	if(((dy*(-1)) < (dx*(-1))) && (dy < 0 && dx < 0)){
		pikmin->startPtr = pikminL.startPtr;
		pikmin->totalStates = pikminR.totalStates;
		checkCycles(pikmin);}
	if(((dy*(-1)) > (dx*(-1))) && (dy < 0 && dx < 0)){
		pikmin->startPtr = pikminB.startPtr;
		pikmin->totalStates = pikminF.totalStates;
		pikmin->att2 = ATTR2_PRIO_0;
		checkCycles(pikmin);}
	if(((dy*(-1)) > dx) && (dy < 0 && dx > 0)){
		pikmin->startPtr = pikminB.startPtr;
		pikmin->totalStates = pikminF.totalStates;
		pikmin->att2 = ATTR2_PRIO_0;
		checkCycles(pikmin);}
	if(((dy*(-1)) < dx) && (dy < 0 && dx > 0)){
		pikmin->startPtr = pikminR.startPtr;
		pikmin->totalStates = pikminR.totalStates;
		checkCycles(pikmin);}



}



//this uses a basic pathfinding algorithm to help pikmin find their way back to 
//the ship however since pikmin don't know anything except following something or 
//being thrown the object moves while the pikmin follow it to the ship object
//identifiers are used in the path to determine which stage each object in
//Note: objects also have predetermined paths depending on their location also determined
//by their pathID and no active uID
void pikminReturn(SPRITE* pikmin){
	static int cycleT[3];
	static int paths[3];
	static int init = 1;
	if(init == 1){
		cycleT[0] = 0;
		cycleT[1] = 0;
		cycleT[2] = 0;
		paths[0] = 1;
		paths[1] = 1;
		paths[2] = 1;
		init = 0;
	}
	SPRITE* obj = (SPRITE*)(pikmin->object);
	if(paths[obj->id] == 1 && cycleT[obj->id]%3==0){
		if(obj->y > pathTable[obj->id]->y1){obj->y--;}
		if(obj->x > pathTable[obj->id]->x1){obj->x--;}
		if(obj->y < pathTable[obj->id]->y1){obj->y++;}
		if(obj->x < pathTable[obj->id]->x1){obj->x++;}
		if(obj->x == pathTable[obj->id]->x1 && obj->y == pathTable[obj->id]->y1){paths[obj->id] = 2;}
	}
	if(paths[obj->id] == 2 && cycleT[obj->id]%3==0){
		if(obj->y > pathTable[obj->id]->y2){obj->y--;}
		if(obj->x > pathTable[obj->id]->x2){obj->x--;}
		if(obj->y < pathTable[obj->id]->y2){obj->y++;}
		if(obj->x < pathTable[obj->id]->x2){obj->x++;}
		if(obj->x == pathTable[obj->id]->x2 && obj->y == pathTable[obj->id]->y2){paths[obj->id] = 3;}
	}
	if(paths[obj->id] == 3 && cycleT[obj->id]%3==0){
		if(obj->y > pathTable[obj->id]->y3){obj->y--;}
		if(obj->x > pathTable[obj->id]->x3){obj->x--;}
		if(obj->y < pathTable[obj->id]->y3){obj->y++;}
		if(obj->x < pathTable[obj->id]->x3){obj->x++;}
		if(obj->x == pathTable[obj->id]->x3 && obj->y == pathTable[obj->id]->y3){paths[obj->id] = 4;}
	}
	if(paths[obj->id] == 4 && cycleT[obj->id]%3==0){
		if(obj->y > testpath.y4){obj->y--;}
		if(obj->x > testpath.x4){obj->x--;}
		if(obj->y < testpath.y4){obj->y++;}
		if(obj->x < testpath.x4){obj->x++;}
		if(obj->x == pathTable[obj->id]->x4 && obj->y == pathTable[obj->id]->y4){paths[obj->id] = 5;}
	}
	if(paths[obj->id] == 5){
		pikmin->process = 0;
		cycleT[obj->id] = 0;
		paths[obj->id] = 1;}
	cycleT[obj->id]++;
}

void playASound(void *sound, unsigned short length, unsigned short rate)
//Takes in a sound variable, it's length (in samples, get this from the
//actual size of the binary file you made when you right click it and),
//check its properties. NOT THE SIZE ON DISK), and the rate (so 11025
//for all of my sounds, but whatever you converted them to)
{

    sampleLengthA = length; //Storing the length; declare this in main
                            //and scope it globally.
    sampleRateA = rate; //Storing the rate; declare this in main
                        //and scope it globally.
    numVblanksA = ((sampleLengthA*60)/(sampleRateA));
    //This calculates how many vblanks the sound plays over, so
    //that my interrupt handler can use vblanks for disabling
    //the sound channel's FIFO queue. Scope it globally.
    REG_TM0CNT = 0;
    //This sets timer zero's controller to 0.
    REG_SND_DSCNT |= SND_DSCNT_ARESET;
    //Resetting the FIFO queue for channel A
    REG_TM0D = 65535-(16777216/rate);
    //This tells the timer its starting value, and sets it to fire
    //just fast enough for the sound to play properly.
    REG_TM0CNT = TIMER_ENABLE;
    //Turns the timer on.
       dmaTransfer(REG_FIFO_A ,(sound), 0, 1, DMA_AT_REFRESH|DMA_32|DMA_REPEAT|DMA_ON);
    //Turns the DMA on, for channel 1 (the A sound channel), and tells
    //it to fire automatically, at refresh (hence the 0 transfer size;
    //that size doesn't seem to matter)

}

void playBSound(void *sound, unsigned short length, unsigned short rate)
{
    sampleLengthB = length;
    sampleRateB = rate;
    numVblanksB = ((sampleLengthB*60)/(sampleRateB));
    REG_TM1CNT = 0;
    REG_SND_DSCNT |= SND_DSCNT_BRESET;
    REG_TM1D = 65535-(16777216/rate);
    REG_TM1CNT = TIMER_ENABLE;

       dmaTransfer(REG_FIFO_B ,(sound), 0, 2,
DMA_AT_REFRESH|DMA_32|DMA_REPEAT|DMA_ON);
}


void interruptHandler()
{
    unsigned short flag;
    REG_IME = 0x00;
    //Disable interrupts
    flag = REG_IF;
    //Store the flags that have been thrown.
    if((REG_IF & IRQ_VBLANK) == IRQ_VBLANK)
    //Check to see if we have a vblank interrupt
    {
        if(sampleLengthA)
        //Check to see if we have a sound that needs playing on
        //channel A.
        {
           samplePositionA++;
           //I had this scoped globally, but it doesn't need to be.
           //Initialize it to zero, it's just keeping track of how many
           //vblanks have gone by so far since the sound on channel A
           //started playing.
           
		   if(samplePositionA > numVblanksA)
           //If more vblanks have gone by then the sound requires...
           {
             REG_TM0CNT = 0; //Disable the timer controlling channel A
             samplePositionA = 0; //Reset the number of vBlanks that
                                  //have gone by to 0.
             sampleLengthA = 0; //Reset the length to 0 (may not be
			
                                //needed)
             numVblanksA = 0; //Reset the number of vblanks needed to 0.
           }

        }
        if(sampleLengthB)
        {
           samplePositionB++;
           if(samplePositionB > numVblanksB)
           {
             REG_TM1CNT = 0;
             samplePositionB = 0;
             sampleLengthB = 0;
             numVblanksB = 0;
       //Everything here is the same as for A, except relying on
        //sound channel B, and, importantly, resetting timer 1, not 0
           }

        }

    }
    REG_IF = flag;
    //Resetting the flags that have fired. I think this is what
    //Aaron was talking about when he was babbling about voltages,
    //and setting REG_IF = REG_IF, but this is what I did first, it
    //worked, and I think when I tried changing it, things broke. So
    //whatever.
    REG_IME = 0x01;
    //Re-enabling interrupts.
}


//this is a god function that keeps pikming from getting too close to each other and fucking
//everything else up
void calibratePikmin(SPRITE* pikmin1, SPRITE* pikmin2){
	int px1, py1, px2, py2;
	int dx, dy;
	px1 = pikmin1->x + 16;
	py1 = pikmin1->y + 16;
	px2 = pikmin2->x + 16;
	py2 = pikmin2->y + 16;
	dx = px1 - px2;
	dy = py1 - py2;
	if(dx*dx < 32*32 && dy*dy < 32*32){
		pikmin1->x += dx/10;
		pikmin1->y += dy/10;}
}
	


//game initialization

