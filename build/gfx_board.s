
@{{BLOCK(gfx_board)

@=======================================================================
@
@	gfx_board, 32x16@4, 
@	Transparent color : FF,00,FF
@	+ palette 16 entries, not compressed
@	+ 8 tiles Metatiled by 2x1 not compressed
@	Total size: 32 + 256 = 288
@
@	Time-stamp: 2023-05-01, 01:05:33
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global gfx_boardTiles		@ 256 unsigned chars
	.hidden gfx_boardTiles
gfx_boardTiles:
	.word 0x11111110,0x22222211,0x22222221,0x22222221,0x22222221,0x22222221,0x22222221,0x22222221
	.word 0x11111111,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222
	.word 0x11111111,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222
	.word 0x11111111,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222
	.word 0x22222221,0x22222221,0x22222221,0x22222221,0x22222221,0x22222231,0x33333311,0x11111110
	.word 0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x33333333,0x11111111
	.word 0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x33333333,0x11111111
	.word 0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x33333333,0x11111111

	.section .rodata
	.align	2
	.global gfx_boardPal		@ 32 unsigned chars
	.hidden gfx_boardPal
gfx_boardPal:
	.hword 0x7C1F,0x1084,0x7BDE,0x5E90,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(gfx_board)
