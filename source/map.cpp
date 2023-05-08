#include "../include/map.hpp"
#include <tonc.h>

Map::Map(u16 bg, const SCR_ENTRY *map, u32 w, u32 h, u16 cbb, u16 sbb, bool compressed) : 
        bg(bg), w(w), h(h), cbb(cbb), sbb(sbb), compressed(compressed)
{
  u16 bgnr;

  if (bg < 2)
    bgnr = (bg + 1) << 8;
  else
    bgnr = ((bg + (bg == 3 ? 1 : 0)) << 8) << 1;

  if (!BIT_EQ(REG_DISPCNT, bgnr)) 
    T_enableBg(bg);

  if (map) {
    if (compressed)
      LZ77UnCompVram(map, &se_mem[sbb]);
    else
      tonccpy(&se_mem[sbb], map, (w << 3) << 3);
  }

  REG_BG_OFS[bg].x = 0x00;
  REG_BG_OFS[bg].y = 0x00;
  REG_BGCNT[bg] = BG_CBB(cbb) | BG_SBB(sbb) | BG_4BPP | BG_PRIO(3);

  if (w == 64 && h == 32)
    REG_BGCNT[bg] |= BG_REG_64x32;
  else if (w == 64 && h == 64)
    REG_BGCNT[bg] |= BG_REG_64x64;
  else
    REG_BGCNT[bg] |= BG_REG_32x32;
}

Map::~Map() {
  CBB_CLEAR(cbb);
  SBB_CLEAR(sbb);
  REG_BGCNT[bg] = 0x00;
  toncset(&se_mem[sbb][cbb], 0x00, (w << 3) << 3);
  T_disableBg(bg);
}

void Map::update() {
  pos.x += dx;
  pos.y += dy;

  REG_BG_OFS[bg].x = pos.x >> 8;
  REG_BG_OFS[bg].y = pos.y >> 8;
}
