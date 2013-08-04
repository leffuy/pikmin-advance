
//{{BLOCK(level1bg)

//======================================================================
//
//	level1bg, 256x1024@8, 
//	+ palette 256 entries, not compressed
//	+ 1332 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x128 
//	Total size: 512 + 85248 + 8192 = 93952
//
//	Time-stamp: 2008-04-26, 17:42:26
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com )
//
//======================================================================

#ifndef __LEVEL1BG__
#define __LEVEL1BG__

#define level1PalLen 512
extern const unsigned short level1Pal[256];

#define level1TilesLen 85248
extern const unsigned short level1Tiles[42624];

#define level1MapLen 8192
extern const unsigned short level1Map[4096];

#endif // __LEVEL1BG__

//}}BLOCK(level1bg)
