#include <memory>
#include <tonc.h>
#include "../include/s_menu.hpp"
#include "../include/global.hpp"
#include "../include/e_player.hpp"
#include "../include/map.hpp"
#include "../include/buttons.hpp"

#include "map_menu1.h"
#include "map_menu2.h"
#include "map_menu3.h"
#include "gfx_menu_text.h"

#define BG_SPEED 0x080
#define BG_GRAV float2fx(0.08f)

namespace Menu {
  int ii;
  int jumps;
  bool started, in_scene, in_menu;
  FIXED t, bg_dy, kadu_dy;

  const SCR_ENTRY *maps[4]{
    NULL,
    (SCR_ENTRY*)map_menu1Map,
    (SCR_ENTRY*)map_menu2Map,
    (SCR_ENTRY*)map_menu3Map,
  };

  const TILE *tiles[4]{
    NULL,
    (TILE*)map_menu1Tiles,
    (TILE*)map_menu2Tiles,
    (TILE*)map_menu3Tiles,
  };

  const u16 tile_ids[4]{0, 0, 92, 357};
  const POINT32 bg_init_pos[4]{
    {0x00, 0x00},
    {0x00, -(150 << 8)},
    {0x00, -(95 << 8)},
    {0x00, -(48 << 8)},
  };

  std::shared_ptr<Map> bg[4]{nullptr};
  std::shared_ptr<Button> btns{nullptr};
  TSprite *start_spr[3]{nullptr};

  void initIntro();
  void updateIntro();

  void initMenu();
  void updateMenu();

  void init() {
    T_setMode(0);
    T_initObjs();

    initIntro();

    t = 0x00;
    started = false;
    in_scene = true;
    in_menu = false;
    bg_dy = BG_SPEED;
    kadu_dy = 0x00;
    jumps = 2;
  }

  void update() {
    t += 0x0400;

    updateIntro();
    updateMenu();

    for (ii = 0; ii < 4; ii++) {
      if (bg[ii]) {
        bg[ii]->update();
      }
    }

    T_updateObjs(FALSE);
  }

  void end() {
    for (ii = 0; ii < 3; ii++) {
      if (start_spr[ii]) {
        REM_SPR(start_spr[ii]);
      }
    }

    btns = nullptr;

    for (ii = 0; ii < 4; ii++)
      bg[ii] = nullptr;

    RegisterRamReset(RESET_PALETTE);
    RegisterRamReset(RESET_VRAM);
  }

  void initIntro() {
    LZ77UnCompVram(map_menu1Pal, pal_bg_mem);

    for (ii = 0; ii < 4; ii++) {
      if (maps[ii]) {
        LZ77UnCompVram(tiles[ii], &tile8_mem[0][tile_ids[ii]]);

        bg[ii] = std::make_shared<Map>(ii, maps[ii], 32, 32, 0, 31 - ii, true);
        bg[ii]->setBpp(true);

        bg[ii]->pos.x = -bg_init_pos[ii].x;
        bg[ii]->pos.y = -bg_init_pos[ii].y;
      }

      if (ii == 2) {
        REG_BGCNT[ii] &= ~BG_REG_32x32;
        REG_BGCNT[ii] |= BG_REG_32x64;
      }
    }

    T_disableBg(1);

    TONC_CPY(pal_obj_mem, gfx_menu_textPal);
    TONC_CPY(tile_mem[4], gfx_menu_textTiles);

    for (ii = 0; ii < 3; ii++) {
      start_spr[ii] = T_addObj(
          (ii << 4) << 1, 0, OBJ_32X16, ii << 3, 0, 0, NULL
        );
    }
  }

  void updateIntro() {
    if (started) return;

    if (!in_scene) {
      T_enableObjs();

      if (!BIT_EQ(REG_DISPCNT, 1 << 8)) 
        T_enableBg(1);

      for (ii = 0; ii < 4; ii++)
        bg[ii]->pos.y = 0x0100; // Initial position

      if (key_hit(KEY_START)) {
        VBlankIntrDelay(10);
        started = true;

        if (!in_menu) {
          in_menu = true;
          initMenu();
        }

      }

    } else {
      if (key_hit(KEY_START) && in_scene) in_scene = false;

      if (bg[2]->pos.y <= 0x00) {
        T_enableBg(1);
        bg_dy = 0x00;

        if (-bg[1]->pos.y >= 0x00) {
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
      
      bg[1]->move(0x00, -(kadu_dy << 3));
      bg[2]->move(0x00, -bg_dy);
      bg[3]->move(0x00, -(bg_dy >> 1));
    }

    for (ii = 0; ii < 3; ii++) {
      start_spr[ii]->x = ((ii << 4) << 1) + ((SCREEN_WIDTH - 80) >> 1);
      start_spr[ii]->y = (SCREEN_HEIGHT - 32) + ((40 * lu_cos(t) >> 8) >> 8);
    }

  }

  void initMenu() {
    RegisterRamReset(RESET_PALETTE);
    RegisterRamReset(RESET_VRAM);

    const TFont *fonts[2] = {
      &verdana10Font,
      &verdana9Font
    };

    for (ii = 0; ii < 4; ii++)
      bg[ii] = nullptr;

    for (ii = 0; ii < 3; ii++) {
      REM_SPR(start_spr[ii]);
    }

    T_enableBg(0);
    T_enableObjs();

    tte_init_chr4c(
        0, BG_CBB(0) | BG_SBB(31),
        SE_PALBANK(15),
        14, CLR_WHITE,
        &verdana10Font, (fnDrawg)chr4c_drawg_b1cts_fast
      );

    tte_set_font_table(fonts);

    btns = std::make_shared<Button>();

    btns->add("Play", [](void) {
        VBlankIntrDelay(10);
        Scener::set(Global::s_game);
      }
    );

    btns->add("Options", NULL);
    btns->add("Extras", NULL);

    btns->space = 15;
  }

  void updateMenu() {
    if (!in_menu) return;

    btns->setTextPos(
        25 + ( ( 40 * lu_cos(t) >> 8 ) >> 8 ),
        6 << 3
      );

    btns->update();
  }

}

namespace Global {
  Scener::Scene s_menu = {
    Menu::init,
    Menu::update,
    Menu::end
  };
}
