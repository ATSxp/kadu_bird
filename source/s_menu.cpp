#include "../include/s_menu.hpp"
#include "../include/buttons.hpp"
#include "../include/global.hpp"
#include "../include/map.hpp"
#include <maxmod.h>
#include <memory>
#include <tonc.h>

#include "map_menu1.h"
#include "map_menu2.h"
#include "map_menu3.h"

#include "map_bg_pic.h"
#include "map_select1.h"

#include "../include/verdana11.h"
#include "gfx_ballon1.h"
#include "gfx_menu_text.h"

#define BG_SPEED 0x080
#define BG_GRAV float2fx(0.08f)

namespace Menu {
int ii;
int jumps{2};
bool started{false}, in_scene{true}, in_menu;
FIXED t, bg_dy, kadu_dy, evy;
bool go_to_scene, egg_on{false};
u32 next_scene{0};
u8 egg_count;

std::shared_ptr<Map> bg[3]{nullptr};
std::shared_ptr<Button> btns{nullptr};
TSprite *start_spr[3]{nullptr};

Scener::Scene scenes[3]{Global::s_game, Global::s_options, NULL};

void initIntro();
void updateIntro();

void initMenu();
void updateMenu();

void init() {
  T_setMode(0);
  T_initObjs();

  if (!started) {
    in_menu = false;
    initIntro();
  } else {
    in_menu = true;
    initMenu();
  }

  t = 0x00;
  go_to_scene = false;
  bg_dy = BG_SPEED;
  kadu_dy = 0x00;
  evy = 0x080;

  egg_count = 0;
}

void update() {
  t += 0x0400;

  if (in_scene)
    evy -= 0x02;
  else
    REG_BLDCNT = BLD_ALL | BLD_BLACK;

  evy = clamp(evy, 0, 0x081);
  REG_BLDY = BLDY_BUILD(evy >> 3);

  tte_erase_screen();

  updateIntro();
  updateMenu();

  for (ii = 0; ii < 3; ii++)
    bg[ii]->update();

  T_updateObjs(FALSE);
}

void end() {
  RegisterRamReset(RESET_PALETTE);
  RegisterRamReset(RESET_VRAM);

  REG_BLDY = 0;

  for (ii = 0; ii < 3; ii++) {
    if (start_spr[ii]) {
      REM_SPR(start_spr[ii]);
    }

    bg[ii] = nullptr;
  }

  btns = nullptr;
}

void initIntro() {
  const SCR_ENTRY *maps[3]{
      (SCR_ENTRY *)map_menu1Map,
      (SCR_ENTRY *)map_menu2Map,
      (SCR_ENTRY *)map_menu3Map,
  };

  const TILE *tiles[3]{
      (TILE *)map_menu1Tiles,
      (TILE *)map_menu2Tiles,
      (TILE *)map_menu3Tiles,
  };

  const u16 tile_ids[3]{0, 202, 469};
  const POINT32 bg_init_pos[3]{
      {0x00, -(150 << 8)},
      {0x00, -(95 << 8)},
      {0x00, -(48 << 8)},
  };

  if (!started)
    REG_BLDCNT = BLD_ALL | BLD_WHITE;

  for (ii = 0; ii < 3; ii++) {
    bg[ii] = std::make_shared<Map>(ii + 1, maps[ii], 32, 32, 0, 31 - (ii << 1),
                                   true);
    bg[ii]->setBpp(true);

    LZ77UnCompVram(tiles[ii], &tile8_mem[bg[ii]->cbb][tile_ids[ii]]);

    bg[ii]->pos.x = -bg_init_pos[ii].x;
    bg[ii]->pos.y = -bg_init_pos[ii].y;

    if (ii > 1) {
      REG_BGCNT[ii] &= ~BG_REG_32x32;
      REG_BGCNT[ii] |= BG_REG_32x64;
    }
  }

  // Clear part of Map from BG 1
  memset16(&se_mem[bg[0]->sbb][bg[0]->cbb], 0x00, 202);
  memset16(&se_mem[bg[0]->sbb][bg[0]->cbb + 210], 0x00, 2);

  T_disableBg(1); // Hide BG 1 for intro

  GRIT_CPY(tile_mem[4], gfx_menu_textTiles);

  GRIT_CPY(pal_bg_mem, map_menu1Pal);
  GRIT_CPY(pal_obj_mem, gfx_menu_textPal);

  for (ii = 0; ii < 3; ii++) {
    start_spr[ii] = T_addObj((ii << 4) << 1, 0, OBJ_32X16, ii << 3, 0, 0, NULL);
  }
}

void updateIntro() {
  if (started) {
    if (!in_menu)
      evy += 0x05;

    if (evy >= 0x080) {
      evy = 0x080;

      if (egg_count >= MY_HOLY_DAY) {
        Scener::set(Global::s_egg);
      } else if (!in_menu) {
        in_menu = true;

        RegisterRamReset(RESET_PALETTE);
        RegisterRamReset(RESET_VRAM);
        initMenu();
      }
    }

    for (ii = 0; ii < 3; ii++)
      bg[ii]->pos.y = 0x0100; // Initial position

    return;
  }

  // Secrets hehe
  if (key_hit(KEY_A))
    egg_count++;
  else if (egg_count >= MY_HOLY_DAY && !egg_on) {
    egg_on = true;
    mmEffectEx(&Global::snd_burenyu);
  }

  egg_count = clamp(egg_count, 0, MY_HOLY_DAY + 1);

  if (!in_scene) {
    T_enableObjs();

    LZ77UnCompVram(map_menu1Map, &se_mem[bg[0]->sbb][bg[0]->cbb]);

    if (!BIT_EQ(REG_DISPCNT, 1 << 8))
      T_enableBg(1);

    for (ii = 0; ii < 3; ii++)
      bg[ii]->pos.y = 0x0100; // Initial position

    if (key_hit(KEY_START)) {
      started = true;
      mmEffectEx(&Global::snd_select); // tmp
    }

  } else {
    if (key_hit(KEY_START) && in_scene) {
      evy = 0x00;
      in_scene = false;
    }

    if (bg[1]->pos.y <= 0x00) {
      T_enableBg(1);
      bg_dy = 0x00;

      if (-bg[0]->pos.y >= 0x00) {
        if (jumps > 0) {
          kadu_dy = -0x0130;
          jumps--;
        } else {
          kadu_dy = 0x00;
          in_scene = false;
        }

      } else
        kadu_dy += BG_GRAV;

    } else
      bg_dy = BG_SPEED;

    bg[0]->move(0x00, -(kadu_dy << 3));
    bg[1]->move(0x00, -bg_dy);
    bg[2]->move(0x00, -(bg_dy >> 1));
  }

  for (ii = 0; ii < 3; ii++) {
    start_spr[ii]->x = ((ii << 4) << 1) + ((SCREEN_WIDTH - 80) >> 1);
    start_spr[ii]->y = (SCREEN_HEIGHT - 32) + ((40 * lu_cos(t) >> 8) >> 8);
  }
}

void initMenu() {
  const TFont *fonts[2] = {&verdana10Font, &verdana9Font};

  for (ii = 0; ii < 3; ii++) {
    if (start_spr[ii]) {
      REM_SPR(start_spr[ii]);
    }

    bg[ii] = nullptr;
  }

  bg[0] = std::make_shared<Map>(1, map_select1Map, 32, 32, 0, 29, true);
  bg[1] = std::make_shared<Map>(2, nullptr, 32, 32, 0, 27, false);
  bg[2] = std::make_shared<Map>(3, map_bg_picMap, 32, 32, 0, 25, false);

  btns = std::make_shared<Button>();

  T_enableBg(0);
  T_enableObjs();

  tte_init_chr4c(0, BG_CBB(1) | BG_SBB(31), SE_PALBANK(15), 14, CLR_WHITE,
                 &verdana10Font, (fnDrawg)chr4c_drawg_b1cts_fast);
  tte_set_font_table(fonts);

  LZ77UnCompVram(map_select1Tiles, tile_mem);
  GRIT_CPY(&tile_mem[0][248], map_bg_picTiles);
  GRIT_CPY(&tile_mem[0][403], gfx_ballon1Tiles);

  btns->add("Play", [](void) { next_scene = 0; });
  btns->add("Options", [](void) { next_scene = 1; });
  btns->add("Extras", NULL);

  btns->space = 15;

  GRIT_CPY(pal_bg_bank[0], map_select1Pal);
  GRIT_CPY(pal_bg_bank[1], map_bg_picPal);
  GRIT_CPY(pal_bg_bank[2], gfx_ballon1Pal);
  btns->loadPal();

  Global::se_ballon(&se_mem[bg[0]->sbb][0], 0, 0, 10, 4,
                    SE_ID(403) | SE_PALBANK(2));
}

void updateMenu() {
  if (!in_menu)
    return;
  char d[10];

  if (evy > 0x040)
    btns->onInput(false);
  else
    btns->onInput(true);

  if (btns->getEvent()) {
    if (evy <= 0x00)
      go_to_scene = true;
  }

  if (!go_to_scene)
    evy -= 0x05;
  else
    evy += 0x08;

  if (evy >= 0x080 && go_to_scene) {
    evy = 0x080;
    Scener::set(scenes[next_scene]);
  }

  tte_set_ink(15);
  tte_set_font(&verdana11Font);
  posprintf(d, "#{P:5,4}$ %05d", Global::money);
  tte_write(d);

  tte_set_ink(14);
  tte_set_font(&verdana10Font);
  btns->setTextPos(25 + ((40 * lu_cos(t) >> 8) >> 8), 13 << 3);

  btns->update();

  bg[2]->pos.y = 0x04000;
  bg[2]->move(0x080, 0x00);
  bg[2]->update();
}

} // namespace Menu

namespace Global {
Scener::Scene s_menu = {Menu::init, Menu::update, Menu::end};
}
