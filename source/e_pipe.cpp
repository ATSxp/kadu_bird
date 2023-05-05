#include "../include/e_pipe.hpp"

#include "gfx_pipe.h"

Pipe::Pipe(int x, int y, FIXED speed) : dx(speed) {
  space_btw = static_cast<int>(h) + qran_range(64, 80);

  pos.x = x << 8;
  pos.y = y << 8;

  TONC_CPY(pal_obj_bank[1], gfx_pipePal);
  TONC_CPY(&tile_mem[4][tid], gfx_pipeTiles);

  for (ii = 0; ii < PIPE_SPR_COUNT; ii++) {
    spr[ii] = T_addObj(
        x + pipe_base[ii].offsetx, y + pipe_base[ii].offsety,
        pipe_base[ii].size, tid + pipe_base[ii].offset_tid, 1, 2, NULL
      );
  }

  mgba_printf(MGBA_LOG_DEBUG, "Pipe created");
}

Pipe::~Pipe() {
  mgba_printf(MGBA_LOG_DEBUG, "Pipe deleted");
}

void Pipe::update() {
  if ((pos.x >> 8) + w <= 0) die();

  pos.x += dx;
  updateSprs();
}

void Pipe::updateSprs() {
  int add;
  POINT32 pt = {pos.x >> 8, pos.y >> 8};

  for (ii = 0; ii < PIPE_SPR_COUNT; ii++) {
    add = ii > (PIPE_SPR_COUNT >> 1) - 1 ? space_btw: 0;

    spr[ii]->x = pt.x + pipe_base[ii].offsetx;
    spr[ii]->y = (pt.y + add) + pipe_base[ii].offsety;

    spr[ii | 1]->obj.attr1 |= ATTR1_HFLIP;
    spr[ii & 1]->obj.attr1 |= ATTR1_VFLIP;
  }
}

void Pipe::PipeVsPlayer(Player &p) {
  POINT32 pt1 = {pos.x >> 8, pos.y >> 8};
  POINT32 pt2 = {(p.pos.x >> 8) + 16, (p.pos.y >> 8) + 8};
  u32 pw = p.w - 16, ph = p.h - 16;

  bool up_pipe = Global::AABB(
      pt1, w, h,
      pt2, pw, ph
    ), 
      down_pipe = Global::AABB(
      (POINT32){pt1.x, pt1.y + space_btw}, w, h,
      pt2, pw, ph
    );

  if (p.dead) return;

  if ((up_pipe || down_pipe) && !p.damaged) {
    p.damaged = true;
    p.hp--;
  }

  if (p.spr[0]->x > spr[1]->x + 16 && !pointed) {
    pointed = true;
    p.points++;
  }
}

void Pipe::die() {
  if (!dead) {
    dead = true;

    for (ii = 0; ii < PIPE_SPR_COUNT; ii++) {
      REM_SPR(spr[ii]);
    }

    mgba_printf(MGBA_LOG_DEBUG, "Pipe sprite removed");
  }
}
