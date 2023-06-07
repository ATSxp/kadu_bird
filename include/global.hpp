#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

#include "engine/gba.h"
#include "scener.hpp"
#include "libsavgba/err_def.h"
#include "libsavgba/gba_sram.h"
#include <maxmod.h>
#include <tonc.h>
#include "soundbank.h"

// tonccpy is SLOW!! memcpy16/32 is better
//#define TONC_CPY(dst, src) tonccpy(dst, src, src##Len)
#define MY_HOLY_DAY 14 // <3
#define D_EASY      0x00
#define D_MEDIUM    0x01
#define D_HARD      0x02

#define SRAM_BUFFER_SIZE 50

namespace Global {
typedef struct {
  int offsetx, offsety;
  u16 size, offset_tid;
} SprBase;

enum SaveIds {
  SAVE_RECORD = 9,
  SAVE_RSEED,
};

INLINE bool AABB(POINT32 p1, u32 w1, u32 h1, POINT32 p2, u32 w2, u32 h2);
INLINE void checkSaveError(u32 err);


RECT se_ballon(SCR_ENTRY *sbb, int x, int y, int w, int h, SCR_ENTRY se);
void loadSave();
void saveRecord();

extern Scener::Scene s_menu;
extern Scener::Scene s_game;
extern Scener::Scene s_options;
extern Scener::Scene s_egg;
extern Scener::Scene s_intro;

extern u32 seed_rand, record_point;
extern u32 money;
// extern int times[5];
extern int df;
extern u8 sram_buffer[SRAM_BUFFER_SIZE];

// Audio
extern bool sound;
extern mm_sound_effect snd_select;
extern mm_sound_effect snd_select2;
extern mm_sound_effect snd_hand_smash;
extern mm_sound_effect snd_jump;
extern mm_sound_effect snd_hit;
extern mm_sound_effect snd_burenyu;

extern TSprite *hp_spr[3];

INLINE bool AABB(POINT32 p1, u32 w1, u32 h1, POINT32 p2, u32 w2, u32 h2) {
  return (p1.x < p2.x + static_cast<int>(w2) &&
          p1.y < p2.y + static_cast<int>(h2) &&
          p2.x < p1.x + static_cast<int>(w1) &&
          p2.y < p1.y + static_cast<int>(h1));
}

INLINE void checkSaveError(u32 err) {
  if (err)
    mgba_printf(MGBA_LOG_ERROR, "SRAM Write Error: %s",
                SavErrMsgs[err]);
}

} // namespace Global

#endif // !__GLOBAL_HPP__
