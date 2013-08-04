#include "input.h"

extern unsigned short keysNow;
extern unsigned short keysThen;

void scanKeys(void){
	keysThen = keysNow;
	keysNow = (~(REG_KEYINPUT));
}

unsigned short keysHeld(void){
	return keysNow;}
	
unsigned short keysPressed(void){
	return (keysNow ^ keysThen) & keysNow;}

unsigned short keysReleased(void){
	return (keysNow ^ keysThen) & (~keysNow);}