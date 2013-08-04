
//{{BLOCK(text)

//======================================================================
//
//	text, 256x16@8, 
//	+ palette 256 entries, not compressed
//	+ 59 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x2 
//	Total size: 512 + 3776 + 128 = 4416
//
//	Time-stamp: 2008-05-01, 21:28:56
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com )
//
//======================================================================

#ifndef __TEXT__
#define __TEXT__

#define textPalLen 512
extern const unsigned short textPal[256];

#define textTilesLen 3776
extern const unsigned short textTiles[1888];

#define textMapLen 128
extern const unsigned short textMap[64];

#endif // __TEXT__

//}}BLOCK(text)
