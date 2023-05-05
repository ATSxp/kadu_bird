#include "../include/engine/gba.h"
#include "../include/global.hpp"

int main () {
  mgba_open();
  T_init();

  Scener::set(Global::s_menu);

  while(true) {
    T_update();
    Scener::update();
    Global::seed_rand++;
  }

  return 0;
}
