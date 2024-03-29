#include "../include/buttons.hpp"
#include "../include/global.hpp"
#include <string.h>

#include "soundbank.h"
#include <maxmod.h>

Button::Button(u16 tid, u16 pb) : pb(pb) {
  GRIT_CPY(&tile_mem[4][tid], gfx_glassTiles);
  glass_spr = T_addObj(txt_pos.x, txt_pos.y, OBJ_16X8, tid, pb, 0, NULL);
  T_hideObj(glass_spr);
}

Button::~Button() { REM_SPR(glass_spr); }

void Button::update() {
  show();

  glass_spr->x = txt_pos.x - 18;
  glass_spr->y = txt_pos.y + 4 + (cursor * space);

  event = false;

  if (!on)
    return;

  old_cursor = cursor;
  cursor += bit_tribool(key_hit(-1), KI_DOWN, KI_UP);

  if (lock_cursor) {
    cursor = clamp(cursor, 0, btns.size());
  } else {
    if (cursor < 0)
      cursor = static_cast<int>(btns.size()) - 1;
    else if (cursor >= static_cast<int>(btns.size()))
      cursor = 0;
  }

  if (key_hit(KEY_UP | KEY_DOWN) && cursor != old_cursor) {
    mmEffectEx(&Global::snd_select2);
  }

  if (key_hit(KEY_A) && !event) {
    event = true;
    mmEffectEx(&Global::snd_select);

    if (btns[cursor].callback) {
      btns[cursor].callback();
    }
  }
}

void Button::show() {
  T_showObj(glass_spr);

  for (ii = 0; ii < static_cast<int>(btns.size()); ii++) {
    tte_set_pos(txt_pos.x, txt_pos.y + ii * space);

    if (btns[ii].show_status) {
      char dst[strlen(btns[ii].name) + 6];
      posprintf(dst, btns[ii].name);

      if (btns[ii].on)
        tte_write(strcat(dst, ": on"));
      else
        tte_write(strcat(dst, ": off"));
    } else
      tte_write(btns[ii].name);
  }
}
