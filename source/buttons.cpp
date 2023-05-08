#include "../include/buttons.hpp"
#include "../include/global.hpp"

#include "gfx_glass.h"

Button::Button(u16 tid, u16 pb) {
  TONC_CPY(pal_obj_bank[pb], gfx_glassPal);
  TONC_CPY(&tile_mem[4][tid], gfx_glassTiles);

  glass_spr = T_addObj(
      txt_pos.x, txt_pos.y,
      OBJ_16X8,
      tid, pb, 0, NULL
    );
}

Button::~Button() {
  REM_SPR(glass_spr);
}

void Button::update() {
  show();

  cursor += bit_tribool(key_hit(-1), KI_DOWN, KI_UP);
  cursor = clamp(cursor, 0, btns.size());

  if (key_hit(KEY_A)) {
    if (btns[cursor].callback)
      btns[cursor].callback();
  }

  glass_spr->x = txt_pos.x - 18;
  glass_spr->y = txt_pos.y + 4 + (cursor * space);
}

void Button::show() {
  tte_erase_screen();
  for (ii = 0; ii < static_cast<int>(btns.size()); ii++) {
    tte_set_pos(txt_pos.x, txt_pos.y + ii * space);
    tte_write(btns[ii].name.c_str());
  }
}
