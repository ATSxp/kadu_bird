/*
  date: 02/06/2023
  file: egg.cpp
  author: ATSxp
  desc: easter egg, hehe
*/

#include "../include/egg.hpp"
#include "../include/global.hpp"
#include <tonc.h>
#include <vector>

#include "map_egg01.h"
#include "map_egg02.h"
#include "map_egg03.h"
#include "map_egg04.h"
#include "map_egg05.h"
#include "map_egg06.h"
#include "map_egg07.h"

#include "gfx_egg_text01.h"
#include "gfx_egg_text02.h"
#include "gfx_egg_text03.h"

namespace Egg {
#define FADE_SPEED 0x05
#define CBB 0
#define SBB 31

#define EGG(e)                                                                 \
  { (COLOR *)e##Pal, (TILE *)e##Tiles, (SCR_ENTRY *)e##Map }

typedef struct {
  COLOR *pal;
  TILE *tile;
  SCR_ENTRY *map;
} EggTemplate;

int ii;

static u8 count, old_count;
static FIXED evy, evdy;
static bool go;
static POINT32 pos;

static TSprite *txt_move[3];
static TSprite *txt_next[4];
static TSprite *txt_prev[4];

const std::vector<EggTemplate> eggs{
    EGG(map_egg01), EGG(map_egg02), EGG(map_egg03), EGG(map_egg04),
    EGG(map_egg05), EGG(map_egg06), EGG(map_egg07),
};

void reloadSlide();

void init() {
  T_setMode(0);
  T_enableBg(0);
  T_enableObjs();

  T_initObjs();

  TONC_CPY(tile_mem[4], gfx_egg_text01Tiles);
  TONC_CPY(&tile_mem[4][13], gfx_egg_text02Tiles);
  TONC_CPY(&tile_mem[4][29], gfx_egg_text03Tiles);

  REG_BG0CNT = BG_CBB(CBB) | BG_SBB(SBB) | BG_8BPP | BG_REG_32x32;
  REG_BLDCNT = (BLD_ALL & ~BLD_OBJ) | BLD_BLACK;

  evy = 0x080;
  evdy = 0x00;
  old_count = count = 0;
  go = false;
  pos.x = (16 << 8) >> 1; // Map width - Screen width / 2
  pos.y = (96 << 8) >> 1; // Map heigth - Screen heigth / 2

  for (ii = 0; ii < 3; ii++)
    txt_move[ii] =
        T_addObj((ii << 4) + 1, 160 - 17, OBJ_16X16, ii << 2, 0, 0, NULL);

  for (ii = 0; ii < 4; ii++)
    txt_next[ii] = T_addObj((ii << 4) + (240 - (16 << 2)) + 12, 0, OBJ_16X16,
                            (ii << 2) + 13, 1, 0, NULL);

  for (ii = 0; ii < 4; ii++)
    txt_prev[ii] =
        T_addObj((ii << 4) + 1, 0, OBJ_16X16, (ii << 2) + 29, 2, 0, NULL);

  reloadSlide();
  TONC_CPY(pal_obj_mem, gfx_egg_text01Pal);
  TONC_CPY(pal_obj_bank[1], gfx_egg_text02Pal);
  TONC_CPY(pal_obj_bank[2], gfx_egg_text02Pal);
}

void update() {
  if (evy < 0x020) {
    if (key_hit(KEY_L) && count > 0)
      count--;
    else if (key_hit(KEY_R) && count < eggs.size() - 1)
      count++;

    if (key_hit(KEY_B)) {
      REG_BLDCNT |= BLD_OBJ;
      evdy = FADE_SPEED;
      go = true;
    }

    pos.x = clamp(pos.x += (key_tri_horz() << 8), 0, 16 << 8);
    pos.y = clamp(pos.y += (key_tri_vert() << 8), 0, 96 << 8);
  }

  if (count != old_count) {
    evdy = FADE_SPEED;
    reloadSlide();
  }

  if (evy >= 0x080 && go)
    Scener::set(Global::s_menu);

  evy = clamp(evy += evdy, 0x00, 0x081);
  REG_BLDY = BLDY_BUILD(evy >> 3);

  REG_BG_OFS[0].x = pos.x >> 8;
  REG_BG_OFS[0].y = pos.y >> 8;

  T_updateObjs(FALSE);
}

void end() {
  RegisterRamReset(RESET_PALETTE);
  RegisterRamReset(RESET_VRAM);

  REG_BLDY = 0;
  REG_BG_OFS[0].x = REG_BG_OFS[0].y = 0;

  for (ii = 0; ii < 4; ii++) {
    REM_SPR(txt_next[ii]);
    REM_SPR(txt_prev[ii]);

    if (ii > 3)
      break;
    REM_SPR(txt_move[ii]);
  }
}

void reloadSlide() {
  if (evy >= 0x080) {
    old_count = count;
    LZ77UnCompVram(eggs[count].tile, tile_mem);
    LZ77UnCompVram(eggs[count].map, &se_mem[SBB][CBB]);
    LZ77UnCompVram(eggs[count].pal, pal_bg_mem);
    evdy = -FADE_SPEED;
    pos.x = (16 << 8) >> 1; // Map width - Screen width / 2
    pos.y = (96 << 8) >> 1; // Map heigth - Screen heigth / 2
  }

  if (count >= eggs.size() - 1) {
    for (ii = 0; ii < 4; ii++)
      T_hideObj(txt_next[ii]);
  } else if (count <= 0) {
    for (ii = 0; ii < 4; ii++)
      T_hideObj(txt_prev[ii]);
  } else {
    for (ii = 0; ii < 4; ii++) {
      T_showObj(txt_next[ii]);
      T_showObj(txt_prev[ii]);
    }
  }
}
} // namespace Egg

namespace Global {
Scener::Scene s_egg = {Egg::init, Egg::update, Egg::end};
}
