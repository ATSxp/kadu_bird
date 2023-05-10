#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

#include <tonc.h>
#include "engine/gba.h"
#include "scener.hpp"

// SLOW!! GRIT_CPY is bestter
#define TONC_CPY(dst, src) tonccpy(dst, src, src##Len)

namespace Global {
  typedef struct {
    int offsetx, offsety;
    u16 size, offset_tid;
  } SprBase;

  INLINE bool AABB(POINT32 p1, u32 w1, u32 h1, POINT32 p2, u32 w2, u32 h2);

  void se_ballon(SCR_ENTRY *sbb, int x, int y, int w, int h, SCR_ENTRY se);

  extern Scener::Scene s_menu;
  extern Scener::Scene s_game;
  extern u32 seed_rand, record_point;
  extern int times[5];

  extern TSprite *hp_spr[3];

  INLINE bool AABB(POINT32 p1, u32 w1, u32 h1, POINT32 p2, u32 w2, u32 h2) {
    return (p1.x < p2.x + static_cast<int>(w2) && 
            p1.y < p2.y + static_cast<int>(h2) && 
            p2.x < p1.x + static_cast<int>(w1) && 
            p2.y < p1.y + static_cast<int>(h1));
  }
}

#endif // !__GLOBAL_HPP__
