#include "../include/global.hpp"

namespace Global {
TSprite *hp_spr[3];
u32 seed_rand{0};
u32 record_point;
u32 money{0};
int df{D_MEDIUM};
int times[5];

u8 sram_buffer[SRAM_BUFFER_SIZE];

// Audio
bool sound{true};

mm_sound_effect snd_select{SFX_SELECT, 0x0400, 0, 255, 128};
mm_sound_effect snd_select2{SFX_SELECT2, 0x0400, 1, 128, 128};
mm_sound_effect snd_hand_smash{SFX_HAND_SMASH, 0x0400, 2, 255, 128};
mm_sound_effect snd_jump{SFX_JUMP, 0x0400, 3, 128, 128};
mm_sound_effect snd_hit{SFX_HIT, 0x0400, 4, 255, 128};
mm_sound_effect snd_burenyu{SFX_BURENYU, 0x0400, 0, 255, 128};

RECT se_ballon(SCR_ENTRY *sbb, int x, int y, int w, int h, SCR_ENTRY se) {
  u16 id = BFN_GET(se, SE_ID);
  u16 pb = SE_PALBANK(BFN_GET(se, SE_PALBANK));

  // Fill
  se_rect(sbb, x, y, x + (w - 1), y + (h - 1), (id + 3) | pb);

  // Vertical lines
  se_rect(sbb, x, y, x + 1, y + (h - 1), (id + 1) | pb);
  se_rect(sbb, x + (w - 2), y, x + (w - 1), y + (h - 1),
          (id + 1) | pb | SE_HFLIP);

  // Horizontal lines
  se_rect(sbb, x, y, x + (w - 1), y + 1, (id + 2) | pb | SE_VFLIP);
  se_rect(sbb, x, y + (h - 2), x + (w - 1), y + (h - 1), (id + 2) | pb);

  // Borders
  se_plot(sbb, x, y, id | pb);
  se_plot(sbb, x + (w - 2), y, id | pb | SE_HFLIP);
  se_plot(sbb, x, y + (h - 2), id | pb | SE_VFLIP);
  se_plot(sbb, x + (w - 2), y + (h - 2), id | pb | SE_HFLIP | SE_VFLIP);

  return (RECT){x, y, x + (w - 1), y + (h - 1)};
}

void loadSave() {
  record_point = (u32)sram_buffer[SAVE_RECORD];
  seed_rand = (u32)sram_buffer[SAVE_RSEED];

  mgba_printf(MGBA_LOG_DEBUG, "SRAM Game Data Loaded");
}
} // namespace Global
