/*
  date: 08/06/2023
  file: s_options.cpp
  author: ATSxp
  desc: Options scene
*/

#include "../include/s_options.hpp"
#include "../include/buttons.hpp"
#include "../include/global.hpp"
#include "../include/map.hpp"
#include <memory>

#include "gfx_ballon2.h"

namespace Options {
int ii;

std::shared_ptr<Map> bg[2]{nullptr, nullptr};
std::shared_ptr<Button> btns{nullptr};

int bw{18}, bh;
POINT32 board_pos{(SCREEN_WIDTH_T - bw) >> 1, 0};
FIXED evy;

void init() {
  mmStop();

  T_setMode(0);
  T_enableBg(0);

  T_initObjs();
  T_enableObjs();

  REG_BLDCNT = BLD_ALL | BLD_BLACK;

  tte_init_chr4c(0, BG_CBB(1) | BG_SBB(31), SE_PALBANK(15), 14, CLR_BLACK,
                 &verdana9bFont, (fnDrawg)chr4c_drawg_b1cts_fast);

  GRIT_CPY(&tile_mem[0][1], gfx_ballon2Tiles);
  // Fill tile id 6 with black color
  memset32(&tile_mem[0][6], 0xffffffff, 8);

  bg[0] = std::make_shared<Map>(1, nullptr, 32, 32, 0, 29);
  bg[1] = std::make_shared<Map>(2, nullptr, 32, 32, 0, 27);

  btns = std::make_shared<Button>();

  btns->add(
      "Sound",
      [](void) {
        Global::sound = !Global::sound;
        mmEffectCancelAll();
      },
      true);

  btns->add("Erase Save", [](void) { Global::clearSram(); });
  btns->add("Back To Menu", [](void) { Scener::set(Global::s_menu); });

  bh = btns->btns.size() * 3;

  board_pos.y = (SCREEN_HEIGHT_T - bh) >> 1;

  Global::se_ballon(&se_mem[29][0], board_pos.x, board_pos.y, bw, bh - 2, SE_ID(1));
  se_fill(&se_mem[27][0], SE_ID(6));

  GRIT_CPY(pal_bg_mem, gfx_ballon2Pal);
  btns->loadPal();

  btns->setTextPos((board_pos.x << 3) + 8, (board_pos.y << 3) + 4);
  btns->space = 12;

  evy = 0x080;
}

void update() {
  tte_erase_screen();

  if (evy > 0x00)
    evy -= 0x03;
  evy = clamp(evy, 0, 0x081);
  REG_BLDY = BLDY_BUILD(evy >> 3);

  btns->setButtonStatus(0, Global::sound);

  if (key_hit(KEY_B))
    Scener::set(Global::s_menu);

  btns->update();

  T_updateObjs(FALSE);
}

void end() {
  RegisterRamReset(RESET_PALETTE);
  RegisterRamReset(RESET_VRAM);

  REG_BLDY = 0;

  for (ii = 0; ii < 2; ii++) {
    bg[ii].reset();
  }

  btns.reset();
}

} // namespace Options

namespace Global {
Scener::Scene s_options = {Options::init, Options::update, Options::end};
}
// EOF
