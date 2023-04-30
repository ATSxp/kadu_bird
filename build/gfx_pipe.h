
//{{BLOCK(gfx_pipe)

//======================================================================
//
//	gfx_pipe, 32x64@4, 
//	Transparent color : FF,00,FF
//	+ palette 16 entries, not compressed
//	+ 32 tiles Metatiled by 4x4 not compressed
//	Total size: 32 + 1024 = 1056
//
//	Time-stamp: 2023-04-29, 21:05:58
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GFX_PIPE_H
#define GRIT_GFX_PIPE_H

#define gfx_pipeTilesLen 1024
extern const unsigned int gfx_pipeTiles[256];

#define gfx_pipePalLen 32
extern const unsigned short gfx_pipePal[16];

#endif // GRIT_GFX_PIPE_H

//}}BLOCK(gfx_pipe)
