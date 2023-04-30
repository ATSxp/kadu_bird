
@{{BLOCK(gfx_kadu)

@=======================================================================
@
@	gfx_kadu, 24x48@4, 
@	Transparent color : FF,00,FF
@	+ palette 16 entries, not compressed
@	+ 18 tiles not compressed
@	Total size: 32 + 576 = 608
@
@	Time-stamp: 2023-04-29, 15:33:48
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global gfx_kaduTiles		@ 576 unsigned chars
	.hidden gfx_kaduTiles
gfx_kaduTiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x01111000,0x01223110,0x12231000,0x26676660,0x75887007
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000067,0x00000688
	.word 0x11000110,0x22111210,0x22232210,0x22232210,0x22322100,0x33223100,0x22231000,0x33110000
	.word 0xA5587111,0x96672222,0xA2223222,0x22232222,0x22243222,0x22224333,0x32222222,0x13333333
	.word 0x00000699,0x00099999,0x00000999,0x00000001,0x00000001,0x00000001,0x00000001,0x00000000
	.word 0x11000000,0x00000000,0x10000000,0x10000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x01111111,0x00100010,0x00011001,0x00001000,0x00000000,0x00000000,0x00000000,0x00000000

	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x01111000,0x01223110,0x12231000,0x26676660,0x75887007
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000067,0x00000688
	.word 0x11000000,0x22111111,0x22222221,0x22232210,0x22232100,0x22223100,0x43221000,0x31333100
	.word 0xA5587111,0x96672222,0xA2222222,0x22232222,0x22243222,0x22224433,0x32222244,0x13333333
	.word 0x00000699,0x00099999,0x00000999,0x00000001,0x00000001,0x00000001,0x00000001,0x00000000
	.word 0x11111100,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.word 0x01111111,0x00100010,0x01010100,0x10001000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000001,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global gfx_kaduPal		@ 32 unsigned chars
	.hidden gfx_kaduPal
gfx_kaduPal:
	.hword 0x7C1F,0x0000,0x475D,0x21B2,0x210D,0x6AF3,0x3126,0x2443
	.hword 0x7FD9,0x26FD,0x2192,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(gfx_kadu)