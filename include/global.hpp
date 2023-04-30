#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

#include <tonc.h>
#include "scener.hpp"

#define TONC_CPY(dst, src) tonccpy(dst, src, src##Len)

namespace Global {
  typedef struct {
    int offsetx, offsety;
    u16 size, offset_tid;
  } SprBase;

  extern Scener::Scene s_menu;
  extern Scener::Scene s_game;
}

#endif // !__GLOBAL_HPP__
