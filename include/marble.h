//======================================================================
//
//	marble, 256x1024@8, 
//	+ palette 256 entries, not compressed
//	+ 739 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x128 
//	Total size: 512 + 47296 + 8192 = 56000
//
//	Time-stamp: 2008-04-02, 19:02:52
//	Exported by Cearn's Usenti v1.7.4
//	(comments, kudos, flames to "daytshen@hotmail.com")
//
//======================================================================

#ifndef __MARBLE__
#define __MARBLE__

#define marblePalLen 512
extern const unsigned short marblePal[256];

#define marbleTilesLen 47296
extern const unsigned short marbleTiles[23648];

#define marbleMapLen 8192
extern const unsigned short marbleMap[4096];

#endif // __MARBLE__

