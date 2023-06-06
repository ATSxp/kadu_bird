#include "../include/s_intro.hpp"
#include "../include/global.hpp"
#include <vector>

#include "map_intro01.h"
#include "map_intro02.h"
#include "map_intro03.h"

namespace Intro {
#define MAX_T_SLIDE 0x0800

static FIXED evy{0x080}, evdy{0x00}, t_slide{MAX_T_SLIDE};
static u8 count{0}, old_count{0};

static const std::vector<const void *> intro{
    map_intro01Bitmap,
    map_intro02Bitmap,
    map_intro03Bitmap,
};

void reloadSlide();

void init() {
  T_setMode(3);
  T_enableBg(2);

  REG_BLDCNT = BLD_ALL | BLD_WHITE;
  REG_BLDY = BLDY_BUILD(evy >> 3);

  reloadSlide();
}

void update() {
  if (t_slide <= 0x00) {
    t_slide = MAX_T_SLIDE;
    count++;
  } else if (evy <= 0x00)
    t_slide -= 0x010;

  if (old_count != count) {
    evdy = 0x03;
    reloadSlide();
  }

  if (count == 2) {
    REG_BLDCNT &= ~BLD_WHITE;
    REG_BLDCNT |= BLD_BLACK;
  } else if (count == 3) {
    REG_BLDCNT &= ~BLD_BLACK;
    REG_BLDCNT |= BLD_WHITE;
  }

  if ((count >= (int)intro.size() && evy >= 0x080) || key_hit(KEY_START))
    Scener::set(Global::s_menu);

  evy = clamp(evy += evdy, 0x00, 0x081);
  REG_BLDY = BLDY_BUILD(evy >> 3);
}

void end() {
  RegisterRamReset(RESET_PALETTE);
  RegisterRamReset(RESET_VRAM);
  REG_BLDY = 0;
}

void reloadSlide() {
  if (count >= (u8)intro.size())
    return;

  if (evy >= 0x080) {
    old_count = count;
    LZ77UnCompVram(intro[count], m3_mem);
    evdy = -0x02;
  }
}
} // namespace Intro

namespace Global {
Scener::Scene s_intro = {Intro::init, Intro::update, Intro::end};
}
