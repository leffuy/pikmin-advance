
//{{BLOCK(level1)

//======================================================================
//
//	level1, 256x1024@4, 
//	+ palette 256 entries, not compressed
//	+ 1713 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x128 
//	Total size: 512 + 54816 + 8192 = 63520
//
//	Time-stamp: 2008-05-02, 04:21:39
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com )
//
//======================================================================

#ifndef __LEVEL1__
#define __LEVEL1__

#define level1PalLen 512
extern const unsigned short level1Pal[256];

#define level1TilesLen 54816
extern const unsigned short level1Tiles[27408];

#define level1MapLen 8192
extern const unsigned short level1Map[4096];

#endif // __LEVEL1__

//}}BLOCK(level1)
