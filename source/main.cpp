#include "../include/engine/gba.h"
#include "../include/global.hpp"
#include <maxmod.h>
#include "soundbank_bin.h"

int main () {
  mgba_open();
  T_init(mmVBlank);
  mmInitDefault((mm_addr)soundbank_bin, 16);

  Scener::set(Global::s_intro);

  while(true) {
    if (!Global::sound)
      mmEffectCancelAll();

    mmFrame();
    T_update();
    Scener::update();
    Global::seed_rand++;
  }

  return 0;
}
