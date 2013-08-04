#define STILE_BASE 0x6010000
#define SBANK(i) (STILE_BASE+((i)*0x1000))

#define PALRAM (0x5000000)

#define OAM_MEM (0x7000000)
#define OAMBLOCK(i) (OAM_MEM + ((i)*0x8))

#define ATTR0_REG (0<<8)
#define ATTR0_AFF (1<<8)
#define ATTR0_HIDE (2<<8)
#define ATTR0_AFF_DBL (3<<8)
#define ATTR0_4BPP (0<<13)
#define ATTR0_8BPP (1<<13)
#define ATTR0_SQUARE (0<<14)
#define ATTR0_WIDE (1<<14)
#define ATTR0_TALL (2<<14)

#define ATTR1_SIZE_8x8 (0<<14)
#define ATTR1_SIZE_16x16 (1<<14)
#define ATTR1_SIZE_32x32 (2<<14)
#define ATTR1_SIZE_64x64 (3<<14)

#define ATTR2_PRIO_0 (0<<10)
#define ATTR2_PRIO_1 (1<<10)
#define ATTR2_PRIO_2 (2<<10)
#define ATTR2_PRIO_3 (3<<10)

#define DEFATTR0 (ATTR0_8BPP | ATTR0_SQUARE)
#define	DEFATTR1 (ATTR1_SIZE_64x64)
#define TABLE_SIZE 7

#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)

typedef struct{
int x;
int y;
int magnitude;
int screenX;
int screenY;
int width;
int height;
int direction;
int quadrant;
int totalStates;
int curState;
int cycleTime;
int curCycle;
int bufferSlot;
int distolimar;
int inactive;						//when sprites are processed if this is flagged the sprite is disabled and processed as inactive (Process = 7)
int process;
int id;								//unique identifier for objects n/a for sprites
int aODist;
int bODist;
int cODist;
int dODist;							//if there are less active objects than listed then set the dist to 1000
void* object;						//this is a pointer to object of focus to the pikmin
unsigned short att2;				//determines the slot on the four place slot in memory for 64x64 sprites
unsigned short* curStatePtr;		//current state, is set everytime shiftState is run on a sprite struct
unsigned short* startPtr;
unsigned short att0;
}SPRITE;

typedef struct{
unsigned short* morningPal;
unsigned short* dayPal;
unsigned short* eveningPal;
unsigned short* duskPal;
int state;
}WPAL;

typedef struct{
int x1;
int y1;
int x2;
int y2;
int x3;
int y3;
int x4;
int y4;}PATH4;

void shiftState(SPRITE* sprite);
void placeIntoBuffer(SPRITE sprite, SPRITE* table);
void streamBuffer(SPRITE* table);
void checkCycles(SPRITE* sprite);
void buildOAM(int block, unsigned short att0, unsigned short att1, unsigned short att2);