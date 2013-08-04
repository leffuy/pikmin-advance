
//{{BLOCK(blend)

//======================================================================
//
//	blend, 256x256@8, 
//	+ 2 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 128 + 2048 = 2176
//
//	Time-stamp: 2008-04-28, 05:08:23
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com )
//
//======================================================================

#ifndef __BLEND__
#define __BLEND__

#define blendTilesLen 128
extern const unsigned short blendTiles[64];

#define blendMapLen 2048
extern const unsigned short blendMap[1024];

#endif // __BLEND__

//}}BLOCK(blend)
