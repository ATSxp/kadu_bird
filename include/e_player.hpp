#ifndef __PLAYER__HPP__
#define __PLAYER__HPP__

#include "../include/global.hpp"
#include "engine/gba.h"

#include "gfx_kadu.h"

#define PLAYER_SPR_COUNT 6
#define PLAYER_DAMAGE_MAX_T 0x0800

class Player {
private:
  int ii;

  u16 tid{0};
  FIXED damage_t{PLAYER_DAMAGE_MAX_T};

  Global::SprBase p_base_spr[PLAYER_SPR_COUNT] = {
      {0, 0, OBJ_16X8, 0}, {0, 8, OBJ_16X8, 3}, {0, 16, OBJ_16X8, 6},
      {16, 0, OBJ_8X8, 2}, {16, 8, OBJ_8X8, 5}, {16, 16, OBJ_8X8, 8},
  };

  void updateSprites();
  void setTile(u16 tid);

public:
  POINT32 pos;
  FIXED dx{0x00}, dy{0x00};
  u32 w{24}, h{24};
  bool dead{false};
  TSprite *spr[PLAYER_SPR_COUNT];
  u32 points{0}, hp{3}, money{0};
  bool damaged{false};

  Player(bool show = true);
  ~Player();

  void update();
  inline void loadPal() { GRIT_CPY(pal_obj_mem, gfx_kaduPal); }
};

#endif // !__PLAYER__HPP__
