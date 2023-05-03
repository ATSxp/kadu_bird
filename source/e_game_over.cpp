#include <string.h>
#include "../include/e_game_over.hpp"
#include "../include/global.hpp"

#include "gfx_hands.h"

#define HAND_CENTER_POS (SCREEN_WIDTH - 32) / 2

GameOver::GameOver() {
  TONC_CPY(pal_obj_bank[4], gfx_handsPal);
  TONC_CPY(&tile_mem[4][tid_hand], gfx_handsTiles);

  for (ii = 0; ii < static_cast<int>(hand_spr.size()); ii++) {
    hand_spr[ii] = T_addObj(
        hand_pos[ii].x, hand_pos[ii].y,
        OBJ_32X32, tid_hand, 4, 0, NULL
      );
  }
  T_flipObj(hand_spr[1], TRUE, FALSE);
}

GameOver::~GameOver() {
  for (ii = 0; ii < static_cast<int>(hand_spr.size()); ii++) {
    REM_SPR(hand_spr[ii]);
  }
}

void GameOver::update(Player &p) {
  char dst[strlen(txt)];
  FIXED sx = ((SCREEN_WIDTH - p.w) >> 1) << 8, sy = ((SCREEN_HEIGHT - p.h) >> 1) << 8;
  FIXED dx, dy;

  dx = ArcTan(sx - p.pos.x) >> 3;
  dy = ArcTan(sy - p.pos.y) >> 3;

  p.dx = dx;
  p.dy = dy;

  for (ii = 0; ii < static_cast<int>(hand_spr.size()); ii++) {
    if (dx == 0x00 || dy == 0x00)
      hand_pos[ii].x += hand_speed[ii];

    hand_spr[ii]->x = hand_pos[ii].x >> 8;
    hand_spr[ii]->y = hand_pos[ii].y >> 8;

    if (ii == 0) {
      if ((hand_pos[ii].x >> 8) > HAND_CENTER_POS + (hand_minus[ii] >> 8)) {
        show_txt = true;
        hand_pos[ii].x = (HAND_CENTER_POS << 8) + hand_minus[ii];
      }
    } else {
      if ((hand_pos[ii].x >> 8) < HAND_CENTER_POS + (hand_minus[ii] >> 8))
        hand_pos[ii].x = (HAND_CENTER_POS << 8) + hand_minus[ii];
    }
  }

  if (show_txt) {
    posprintf(dst, txt, p.points);
    tte_write(dst);

    if (key_hit(KEY_START))
      SoftReset();
  }
}
