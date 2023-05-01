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

  if (compressed)
    LZ77UnCompVram(map, &se_mem[sbb][cbb]);
  else
    tonccpy(&se_mem[sbb][cbb], map, (w << 3) << 3);

  REG_BG_OFS[bg].x = 0x00;
  REG_BG_OFS[bg].y = 0x00;
  REG_BGCNT[bg] = BG_CBB(cbb) | BG_SBB(sbb) | BG_4BPP | BG_REG_64x64 | BG_PRIO(3);
}

Map::~Map() {
  CBB_CLEAR(cbb);
  SBB_CLEAR(sbb);
  REG_BGCNT[bg] = 0x00;
  toncset(&se_mem[sbb][cbb], 0x00, (w << 3) << 3);
}

void Map::update() {
  pos.x += dx;
  pos.y += dy;

  REG_BG_OFS[bg].x = pos.x >> 8;
  REG_BG_OFS[bg].y = pos.y >> 8;
}
