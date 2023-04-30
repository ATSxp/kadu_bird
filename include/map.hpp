#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "engine/gba.h"

class Map {
  u16 bg;

  public:
    POINT32 pos{0x00, 0x00};
    FIXED dx{0x00}, dy{0x00};
    u32 w, h;
    u16 cbb, sbb;
    bool compressed{false};

    Map(u16 bg, const SCR_ENTRY *map, u32 w = 32, u32 h = 32, u16 cbb = 0, u16 sbb = 31, bool compressed = false);
    ~Map();

    void update();
    void move(FIXED dx, FIXED dy) {
      this->dx = dx;
      this->dy = dy;
    }
};

#endif // !__MAP_HPP__
