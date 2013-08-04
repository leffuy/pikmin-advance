
//{{BLOCK(map)

//======================================================================
//
//	map, 256x1024@4, 
//	+ palette 256 entries, not compressed
//	+ 4096 tiles not compressed
//	+ regular map (flat), not compressed, 32x128 
//	Total size: 512 + 131072 + 8192 = 139776
//
//	Time-stamp: 2008-05-02, 04:17:41
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com )
//
//======================================================================

#ifndef __MAP__
#define __MAP__

#define mapPalLen 512
extern const unsigned short mapPal[256];

#define mapTilesLen 131072
extern const unsigned short mapTiles[65536];

#define mapMapLen 8192
extern const unsigned short mapMap[4096];

#endif // __MAP__

//}}BLOCK(map)
