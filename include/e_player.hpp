#ifndef __PLAYER__HPP__
#define __PLAYER__HPP__

#include "engine/gba.h"

#define PLAYER_SPR_COUNT 6

class Player {
  u16 tid{0};

  void updateSprites();
  void setTile(u16 tid);

  public:
    POINT32 pos;
    FIXED dx{0x00}, dy{0x00};
    u32 w{24}, h{24};
    bool dead{false};
    TSprite *spr[PLAYER_SPR_COUNT];

    Player();
    ~Player();

    void update();
};

#endif // !__PLAYER__HPP__
