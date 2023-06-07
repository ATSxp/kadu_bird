#include "../include/e_game_over.hpp"
#include "../include/global.hpp"
#include "../include/libsavgba/err_def.h"
#include "../include/libsavgba/gba_sram.h"
#include <maxmod.h>
#include <string.h>

#include "../include/verdana11.h"
#include "gfx_ballon2.h"
#include "gfx_hands.h"

#define HAND_CENTER_POS (SCREEN_WIDTH - 32) / 2

GameOver::GameOver() {
  GRIT_CPY(&tile_mem[4][tid_hand], gfx_handsTiles);
  GRIT_CPY(&tile_mem[0][279], gfx_ballon2Tiles);

  for (ii = 0; ii < static_cast<int>(hand_spr.size()); ii++) {
    hand_spr[ii] = T_addObj(hand_pos[ii].x, hand_pos[ii].y, OBJ_32X32, tid_hand,
                            4, ii, NULL);
  }

  T_flipObj(hand_spr[1], TRUE, FALSE);

  btn = std::make_shared<Button>(96, 5);

  btn->add("Try again", [](void) { Scener::set(Global::s_game); });

  btn->add("Main Menu", [](void) { Scener::set(Global::s_menu); });

  btn->setTextPos(20, (2 << 3) + 2);
  btn->space = 14;

  GRIT_CPY(pal_obj_bank[4], gfx_handsPal);
  GRIT_CPY(pal_bg_bank[1], gfx_ballon2Pal);
  btn->loadPal();

  mmStart(MOD_ADAGIOMC, MM_PLAY_ONCE);
}

GameOver::~GameOver() {
  for (ii = 0; ii < static_cast<int>(hand_spr.size()); ii++) {
    REM_SPR(hand_spr[ii]);
  }
}

void GameOver::update(Player &p) {
  FIXED sx = ((SCREEN_WIDTH - p.w) >> 1) << 8, sy = ((SCREEN_HEIGHT - p.h) >> 1)
                                                    << 8;

  CSTR point_txt = "#{P:4,2}Points: %06d";
  char d[43];

  FIXED dx{ArcTan(sx - p.pos.x) >> 3};
  FIXED dy{ArcTan(sy - p.pos.y) >> 3};

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

        if (!play_effect) {
          play_effect = true;
          mmEffectEx(&Global::snd_hand_smash);
        }
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

    Global::se_ballon(&se_mem[30][0], 0, 0, 31, 3, SE_ID(279) | SE_PALBANK(1));
    posprintf(d, point_txt, p.points);

    tte_set_ink(5);
    tte_set_font(&verdana9bFont);
    tte_write(d);

    if (record_breaked) {
      Global::record_point = p.points;
      Global::se_ballon(&se_mem[30][0], 18, 2, 13, 4,
                        SE_ID(279) | SE_PALBANK(1));

      tte_set_font(&verdana11Font);
      tte_write("#{ci:4;P:147,20}New Record!!");

      u8 r8 = (u8)Global::record_point;
      Global::checkSaveError(sram_write(Global::SAVE_RECORD, &r8, 1));
    }

    if (key_hit(KEY_ACCEPT | KEY_A | KEY_B)) {
      show_menu = true;

      if (!play_effect2) {
        play_effect2 = true;
        mmEffectEx(&Global::snd_select);
      }
    }
  }

  if (show_menu) {
    int h{(bh >> 8) >> 3};
    int max_h{int2fx(5 << 3)};

    Global::se_ballon(&se_mem[30][0], 0, 2, 11, h, SE_ID(279) | SE_PALBANK(1));

    tte_set_ink(5);
    tte_set_font(&verdana10Font);
    if (bh >= max_h)
      btn->update();

    bh += 0x280;
    bh = clamp(bh, 0x00, max_h + 0x0100);
  }
}
