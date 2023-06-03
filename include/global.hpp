#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

#include <tonc.h>
#include <maxmod.h>
#include "engine/gba.h"
#include "scener.hpp"

// tonccpy is SLOW!! GRIT_CPY(memcpy16/32) is better
#define TONC_CPY(dst, src) tonccpy(dst, src, src##Len)

#define MY_HOLY_DAY 14

namespace Global {
  typedef struct {
    int offsetx, offsety;
    u16 size, offset_tid;
  } SprBase;

  INLINE bool AABB(POINT32 p1, u32 w1, u32 h1, POINT32 p2, u32 w2, u32 h2);

  RECT se_ballon(SCR_ENTRY *sbb, int x, int y, int w, int h, SCR_ENTRY se);

  extern Scener::Scene s_menu;
  extern Scener::Scene s_game;
  extern Scener::Scene s_options;
  extern Scener::Scene s_egg;

  extern u32 seed_rand, record_point;
  extern int times[5];
  extern u32 money;

  // Audio
  extern bool sound;
  extern mm_sound_effect snd_select;
  extern mm_sound_effect snd_select2;
  extern mm_sound_effect snd_hand_smash;
  extern mm_sound_effect snd_jump;
  extern mm_sound_effect snd_hit;

  extern TSprite *hp_spr[3];

  INLINE bool AABB(POINT32 p1, u32 w1, u32 h1, POINT32 p2, u32 w2, u32 h2) {
    return (p1.x < p2.x + static_cast<int>(w2) && 
            p1.y < p2.y + static_cast<int>(h2) && 
            p2.x < p1.x + static_cast<int>(w1) && 
            p2.y < p1.y + static_cast<int>(h1));
  }
}

#endif // !__GLOBAL_HPP__
