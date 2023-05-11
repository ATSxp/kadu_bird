#include "../include/e_player.hpp"

#include "gfx_kadu.h"

#define PLAYER_SPRS 2
#define PLAYER_SPR_TILES 9
#define PLAYER_GRAVITY float2fx(0.15f)
#define PLAYER_JUMP 0x0300

Player::Player() {
  Global::SprBase *b = p_base_spr;

  pos.x = (w << 1) << 8;
  pos.y = ((SCREEN_HEIGHT - h) >> 1) << 8;

  GRIT_CPY(tile_mem[4], gfx_kaduTiles);

  for (ii = 0; ii < PLAYER_SPR_COUNT; ii++) {
    spr[ii] = T_addObj(
        (pos.x >> 8) + b->offsetx, 
        (pos.y >> 8) + b->offsety,
        b->size,
        tid + b->offset_tid,
        0, 2, NULL
      );

    b++;
  }
  
  damage_t = PLAYER_DAMAGE_MAX_T;

  GRIT_CPY(pal_obj_mem, gfx_kaduPal);
  mgba_printf(MGBA_LOG_DEBUG, "Player created");
}

Player::~Player() {
  for (ii = 0; ii < PLAYER_SPR_COUNT; ii++) {
    REM_SPR(spr[ii]);
  }

  mgba_printf(MGBA_LOG_DEBUG, "Player deleted");
}

void Player::update() {
  POINT32 pt = {pos.x >> 8, pos.y >> 8};
  // dx = (key_tri_horz() << 8) << 1;
  hp = clamp(hp, 0, 4);
  
  if (!dead) {
    if (damaged)
      damage_t -= 0x030;

    if (damage_t <= 0x00) {
      damage_t = PLAYER_DAMAGE_MAX_T;
      damaged = false;
    }

    // Player die
    if ((pt.y > SCREEN_HEIGHT - 2 || hp <= 0)) {
      dead = true;
      damaged = true;
    }
  }

  if (key_hit(KEY_A) && !dead) dy = -PLAYER_JUMP;

  if (dy < 0x00 && !damaged) setTile(PLAYER_SPR_TILES);
  else if (damaged) setTile(PLAYER_SPR_TILES << 1);
  else           setTile(0);

  if (pt.y <= 0)
    pos.y = 0x00;
  else if (pt.y > SCREEN_HEIGHT)
    pos.y = SCREEN_HEIGHT << 8;

  if (!dead || pt.y < SCREEN_HEIGHT)
    dy += PLAYER_GRAVITY;

  pos.x += dx;
  pos.y += dy;
  updateSprites();
}

void Player::updateSprites() {
  Global::SprBase *b = p_base_spr;
  POINT32 pt = {pos.x >> 8, pos.y >> 8};

  for (ii = 0; ii < PLAYER_SPR_COUNT; ii++) {
    spr[ii]->x = pt.x + b->offsetx;
    spr[ii]->y = pt.y + b->offsety;
    b++;
  }
}

void Player::setTile(u16 tid) {
  Global::SprBase *b = p_base_spr;

  for (ii = 0; ii < PLAYER_SPR_COUNT; ii++) {
    T_setTileObj(spr[ii], (tid + b->offset_tid));
    b++;
  }
}
