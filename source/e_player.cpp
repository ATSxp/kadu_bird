#include "../include/e_player.hpp"
#include "../include/global.hpp"

#include "gfx_kadu.h"

#define PLAYER_SPRS 2
#define PLAYER_SPR_TILES 9
#define PLAYER_GRAVITY float2fx(0.15f)
#define PLAYER_JUMP 0x0280

Global::SprBase p_base_spr[PLAYER_SPR_COUNT] = {
  {0, 0,  OBJ_16X8, 0},
  {0, 8,  OBJ_16X8, 3},
  {0, 16, OBJ_16X8, 6},
  {16, 0,  OBJ_8X8, 2},
  {16, 8,  OBJ_8X8, 5},
  {16, 16, OBJ_8X8, 8},
};

Player::Player() {
  int ii;
  Global::SprBase *b = p_base_spr;

  pos.x = (w << 1) << 8;
  pos.y = ((SCREEN_HEIGHT - h) >> 1) << 8;

  TONC_CPY(pal_obj_mem, gfx_kaduPal);
  TONC_CPY(tile_mem[4], gfx_kaduTiles);

  for (ii = 0; ii < PLAYER_SPR_COUNT; ii++) {
    spr[ii] = T_addObj(
        (pos.x >> 8) + b->offsetx, 
        (pos.y >> 8) + b->offsety,
        b->size,
        tid + b->offset_tid,
        0, 0, NULL
      );

    b++;
  }

  mgba_printf(MGBA_LOG_DEBUG, "Player created");
}

Player::~Player() {
  int ii;
  for (ii = 0; ii < PLAYER_SPR_COUNT; ii++) {
    REM_SPR(spr[ii]);
  }

  mgba_printf(MGBA_LOG_DEBUG, "Player deleted");
}

void Player::update() {
  POINT32 pt = {pos.x >> 8, pos.y >> 8};
  // dx = (key_tri_horz() << 8) << 1;

  if (pt.y + (h - 2) < 0 || pt.y > SCREEN_HEIGHT - 2) dead = true;

  if (key_hit(KEY_A) && !dead) dy = -PLAYER_JUMP;

  if (dy < 0x00) setTile(PLAYER_SPR_TILES);
  else           setTile(0);

  dy += PLAYER_GRAVITY;

  pos.x += dx;
  pos.y += dy;
  updateSprites();
}

void Player::updateSprites() {
  int ii;
  Global::SprBase *b = p_base_spr;
  POINT32 pt = {pos.x >> 8, pos.y >> 8};

  for (ii = 0; ii < PLAYER_SPR_COUNT; ii++) {
    spr[ii]->x = pt.x + b->offsetx;
    spr[ii]->y = pt.y + b->offsety;
    b++;
  }
}

void Player::setTile(u16 tid) {
  int ii;
  Global::SprBase *b = p_base_spr;

  for (ii = 0; ii < PLAYER_SPR_COUNT; ii++) {
    T_setTileObj(spr[ii], (tid + b->offset_tid));
    b++;
  }
}