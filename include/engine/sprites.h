#ifndef __ENGINE_SPRITES__
#define __ENGINE_SPRITES__

#include <tonc.h>

#define OBJ_8X8   0x00
#define OBJ_16X16 0x01
#define OBJ_32X32 0x02
#define OBJ_64X64 0x03

#define OBJ_16X8  0x04
#define OBJ_32X8  0x05
#define OBJ_32X16 0x06
#define OBJ_64X32 0x07

#define OBJ_8X16  0x08
#define OBJ_8X32  0x09
#define OBJ_16X32 0x0A
#define OBJ_32X64 0x0B

#define OBJ_SIZES 0x0C

#define INIT_OBJ(name) TSprite *name = NULL;
#define SET_GFX_OBJ(bpp, gfx) &((TGfx){bpp, (TILE*)gfx##Tiles, (COLOR*)gfx##Pal, gfx##TilesLen, gfx##PalLen})

#define REM_SPR(spr) T_removeObj((spr)); (spr) = NULL;

typedef struct {
  BOOL bpp;
  TILE *tiles;
  COLOR *pal;
  u32 tiles_len, pal_len;
} TGfx;

typedef struct {
  OBJ_ATTR obj;
  u8 id;
  int x, y;

  TGfx gfx;

  BOOL active;
  u16 size;
} TSprite;

extern TSprite spr_buffer[128];
extern u8 g_spr_count, g_unussed_ids_count;

INLINE void T_setTileObj(TSprite *spr, u16 id);
INLINE void T_setPalBankObj(TSprite *spr, u16 pb);
INLINE void T_setPrioObj(TSprite *spr, u16 prio);
INLINE void T_setPosObj(TSprite *spr, int x, int y);
INLINE void T_flipObj(TSprite *spr, BOOL h, BOOL v);
INLINE void T_hideObj(TSprite *spr);
INLINE void T_showObj(TSprite *spr);

TSprite *T_addObj(int x, int y, u16 size, u16 tid, u16 pb, u16 prio, TGfx *gfx);
void T_removeObj(TSprite *spr);
void T_updateObjs(BOOL sort);
void T_initObjs();
BOOL T_objVsObj(TSprite *s, TSprite *s2);

INLINE void T_setTileObj(TSprite *spr, u16 tid) {
  BFN_SET(spr_buffer[spr->id].obj.attr2, tid, ATTR2_ID);
}

INLINE void T_setPalBankObj(TSprite *spr, u16 pb) {
  BFN_SET(spr_buffer[spr->id].obj.attr2, pb, ATTR2_PALBANK);
}

INLINE void T_setPrioObj(TSprite *spr, u16 prio) {
  BFN_SET(spr_buffer[spr->id].obj.attr2, prio, ATTR2_PRIO);
}

INLINE void T_setPosObj(TSprite *spr, int x, int y) {
  BFN_SET2(spr_buffer[spr->id].obj.attr1, x, ATTR1_X);
  BFN_SET2(spr_buffer[spr->id].obj.attr0, y, ATTR0_Y);
}

INLINE void T_flipObj(TSprite *spr, BOOL h, BOOL v) {
  OBJ_ATTR *obj = &spr_buffer[spr->id].obj;

  if (h) BIT_SET(obj->attr1, ATTR1_HFLIP);
  else BIT_CLEAR(obj->attr1, ATTR1_HFLIP);

  if (v) BIT_SET(obj->attr1, ATTR1_VFLIP);
  else BIT_CLEAR(obj->attr1, ATTR1_VFLIP);
}

INLINE void T_hideObj(TSprite *spr) 
{ obj_hide(&spr_buffer[spr->id].obj); }

INLINE void T_showObj(TSprite *spr) 
{ obj_unhide(&spr_buffer[spr->id].obj, 0); }

#endif // !__ENGINE_SPRITES__
