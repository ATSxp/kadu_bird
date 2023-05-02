#ifndef __PLAYER__HPP__
#define __PLAYER__HPP__

#include "engine/gba.h"

#define PLAYER_SPR_COUNT 6
#define PLAYER_DAMAGE_MAX_T 0x0800

class Player {
  private:
    u16 tid{0};
    FIXED damage_t{PLAYER_DAMAGE_MAX_T};

    void updateSprites();
    void setTile(u16 tid);

  public:
    POINT32 pos;
    FIXED dx{0x00}, dy{0x00};
    u32 w{24}, h{24};
    bool dead{false};
    TSprite *spr[PLAYER_SPR_COUNT];
    u32 points{0}, hp{3};
    bool damaged{false};

    Player();
    ~Player();

    void update();
};

#endif // !__PLAYER__HPP__
