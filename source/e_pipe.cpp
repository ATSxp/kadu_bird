#include "../include/e_pipe.hpp"
#include "../include/global.hpp"

#include "gfx_pipe.h"

const Global::SprBase pipe_base[PIPE_SPR_COUNT] = {
  // Up
  {0,  64, OBJ_32X32, 0},
  {32, 64, OBJ_32X32, 0},
  {0,  32, OBJ_32X32, 8},
  {32, 32, OBJ_32X32, 8},
  {0,   0, OBJ_32X32, 8},
  {32,  0, OBJ_32X32, 8},

  // Down
  {0,   0, OBJ_32X32, 0},
  {32,  0, OBJ_32X32, 0},
  {0,  32, OBJ_32X32, 8},
  {32, 32, OBJ_32X32, 8},
  {0,  64, OBJ_32X32, 8},
  {32, 64, OBJ_32X32, 8},
};

Pipe::Pipe(int x, int y, FIXED speed) : dx(speed) {
  int ii;
  const Global::SprBase *b = pipe_base;

  space_btw = 96 + qran_range(64, 80);
  pos.x = x << 8;
  pos.y = y << 8;

  TONC_CPY(pal_obj_bank[1], gfx_pipePal);
  TONC_CPY(&tile_mem[4][tid], gfx_pipeTiles);

  for (ii = 0; ii < PIPE_SPR_COUNT; ii++) {
    spr[ii] = T_addObj(
        x + b->offsetx, (y + b->offsety),
        b->size, tid + b->offset_tid, 1, 1, NULL
      );
    b++;
  }

  mgba_printf(MGBA_LOG_DEBUG, "Pipe created");
}

Pipe::~Pipe() {}

void Pipe::update() {
  if ((pos.x >> 8) + w <= 0) {
    die();
  }

  pos.x += dx;
  updateSprs();
}

void Pipe::updateSprs() {
  int ii, add;
  POINT32 pt = {pos.x >> 8, pos.y >> 8};
  const Global::SprBase *b = pipe_base;

  for (ii = 0; ii < PIPE_SPR_COUNT; ii++) {
    add = ii > (PIPE_SPR_COUNT >> 1) - 1 ? space_btw: 0;

    spr[ii]->x = pt.x + b->offsetx;
    spr[ii]->y = (pt.y + add) + b->offsety;

    spr[ii | 1]->obj.attr1 |= ATTR1_HFLIP;
    spr[ii & 1]->obj.attr1 |= ATTR1_VFLIP;

    b++;
  }
}

void Pipe::PipeVsPlayer(Player &p) {
  int ii, kk;

  if (p.dead) return;

  for (ii = 0; ii < PIPE_SPR_COUNT; ii++) {
    for (kk = 0; kk < PLAYER_SPR_COUNT; kk++) {
      if (T_objVsObj(spr[ii], p.spr[kk])) {
        p.dead = true;
      }
    }
  }

  if (p.spr[0]->x > spr[1]->x + 16 && !pointed) {
    pointed = true;
    p.points++;
  }

}

void Pipe::die() {
  int ii;

  if (!dead) {
    dead = true;

    for (ii = 0; ii < PIPE_SPR_COUNT; ii++) {
      REM_SPR(spr[ii]);
    }

    mgba_printf(MGBA_LOG_DEBUG, "Pipe deleted");
  }
}
