
//{{BLOCK(level1a)

//======================================================================
//
//	level1a, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 1024 tiles not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 65536 + 2048 = 68096
//
//	Time-stamp: 2008-04-25, 02:58:37
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com )
//
//======================================================================

#ifndef __LEVEL1A__
#define __LEVEL1A__

#define level1aPalLen 512
extern const unsigned short level1aPal[256];

#define level1aTilesLen 65536
extern const unsigned short level1aTiles[32768];

#define level1aMapLen 2048
extern const unsigned short level1aMap[1024];

#endif // __LEVEL1A__

//}}BLOCK(level1a)
