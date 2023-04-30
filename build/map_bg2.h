
//{{BLOCK(map_bg2)

//======================================================================
//
//	map_bg2, 512x256@4, 
//	Transparent color : FF,00,FF
//	+ palette 20 entries, lz77 compressed
//	+ 53 tiles (t|f reduced) lz77 compressed
//	+ regular map (in SBBs), lz77 compressed, 64x32 
//	Total size: 40 + 312 + 640 = 992
//
//	Time-stamp: 2023-04-29, 15:06:34
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP_BG2_H
#define GRIT_MAP_BG2_H

#define map_bg2TilesLen 312
extern const unsigned int map_bg2Tiles[78];

#define map_bg2MapLen 640
extern const unsigned short map_bg2Map[320];

#define map_bg2PalLen 40
extern const unsigned short map_bg2Pal[20];

#endif // GRIT_MAP_BG2_H

//}}BLOCK(map_bg2)
