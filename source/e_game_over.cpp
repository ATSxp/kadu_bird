#include <string.h>
#include "../include/e_game_over.hpp"
#include "../include/global.hpp"

#include "gfx_hands.h"
#include "gfx_ballon2.h"

#define HAND_CENTER_POS (SCREEN_WIDTH - 32) / 2

GameOver::GameOver() {
  GRIT_CPY(&tile_mem[4][tid_hand], gfx_handsTiles);
  GRIT_CPY(&tile_mem[0][279], gfx_ballon2Tiles);

  for (ii = 0; ii < static_cast<int>(hand_spr.size()); ii++) {
    hand_spr[ii] = T_addObj(
        hand_pos[ii].x, hand_pos[ii].y,
        OBJ_32X32, tid_hand, 4, ii, NULL
      );
  }

  T_flipObj(hand_spr[1], TRUE, FALSE);

  GRIT_CPY(pal_obj_bank[4], gfx_handsPal);
  GRIT_CPY(pal_bg_bank[1], gfx_ballon2Pal);
}

GameOver::~GameOver() {
  for (ii = 0; ii < static_cast<int>(hand_spr.size()); ii++) {
    REM_SPR(hand_spr[ii]);
  }
}

void GameOver::update(Player &p) {
  char dst[strlen(txt)];
  FIXED sx = ((SCREEN_WIDTH - p.w) >> 1) << 8, sy = ((SCREEN_HEIGHT - p.h) >> 1) << 8;

  CSTR point_txt ="#{P:4,4;ci:%d}Points: %06d"; 
  char d[43];

  FIXED dx {ArcTan(sx - p.pos.x) >> 3};
  FIXED dy {ArcTan(sy - p.pos.y) >> 3};

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
    SBB_CLEAR(30);
    REG_BLDCNT &= ~BLD_BG1;
    REG_BG1CNT = BG_CBB(0) | BG_SBB(30) | BG_PRIO(2);

    if (p.points > Global::record_point)
      record_breaked = true;

    tte_erase_screen();

    posprintf(d, point_txt, 5, p.points);
    posprintf(dst, txt, p.points);

    Global::se_ballon(&se_mem[30][0], 0, 0, 15, 4, SE_ID(279) | SE_PALBANK(1));
    Global::se_ballon(&se_mem[30][0], 6, 15, 17, 4, SE_ID(279) | SE_PALBANK(1));

    tte_write(d);
    tte_write(dst);

    if (record_breaked) {
      Global::record_point = p.points;
      tte_write("#{P:4,44;ci:4}New Record!!");
      Global::se_ballon(&se_mem[30][0], 0, 5, 13, 4, SE_ID(279) | SE_PALBANK(1));
    }

    if (key_hit(KEY_START))
      Scener::set(Global::s_game);
    else if (key_hit(KEY_SELECT))
      Scener::set(Global::s_menu);
  }
}
