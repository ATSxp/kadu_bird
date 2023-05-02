#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

#include <tonc.h>
#include "engine/gba.h"
#include "scener.hpp"

#define TONC_CPY(dst, src) tonccpy(dst, src, src##Len)

namespace Global {
  typedef struct {
    int offsetx, offsety;
    u16 size, offset_tid;
  } SprBase;

  INLINE bool AABB(POINT32 p1, u32 w1, u32 h1, POINT32 p2, u32 w2, u32 h2);

  extern Scener::Scene s_menu;
  extern Scener::Scene s_game;

  extern TSprite *hp_spr[3];

  INLINE bool AABB(POINT32 p1, u32 w1, u32 h1, POINT32 p2, u32 w2, u32 h2) {
    return (p1.x < p2.x + static_cast<int>(w2) && 
            p1.y < p2.y + static_cast<int>(h2) && 
            p2.x < p1.x + static_cast<int>(w1) && 
            p2.y < p1.y + static_cast<int>(h1));
  }
}

#endif // !__GLOBAL_HPP__
