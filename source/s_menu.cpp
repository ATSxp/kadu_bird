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

namespace Menu {
  int ii;
  FIXED t;
  bool started, in_scene;
  FIXED bg_dy;
  FIXED kadu_dy;

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
    {0x00, -(159 << 8)},
    {0x00, -(95 << 8)},
    {0x00, -(48 << 8)},
  };

  std::shared_ptr<Map> bg[4]{nullptr};
  std::shared_ptr<Button> btns{nullptr};
  TSprite *start_spr[3]{nullptr};

  void init() {
    const TFont *fonts[2] = {
      &verdana10Font,
      &verdana9Font
    };

    T_setMode(0);

    T_enableObjs();
    T_initObjs();

    // tte_init_chr4c(
    //     0, BG_CBB(2) | BG_SBB(25),
    //     SE_PALBANK(15),
    //     14, CLR_WHITE,
    //     &verdana10Font, (fnDrawg)chr4c_drawg_b1cts_fast
    //   );

    tte_set_font_table(fonts);

    LZ77UnCompVram(map_menu1Pal, pal_bg_mem);
    for (ii = 0; ii < 4; ii++) {
      if (maps[ii]) {
        LZ77UnCompVram(tiles[ii], &tile8_mem[0][tile_ids[ii]]);

        bg[ii] = std::make_shared<Map>(ii, maps[ii], 32, 32, 0, 31 - ii, true);
        bg[ii]->setBpp(true);

        bg[ii]->pos.x = -bg_init_pos[ii].x;
        bg[ii]->pos.y = -bg_init_pos[ii].y;
      }

      REG_BGCNT[ii] &= ~BG_REG_32x32;
      REG_BGCNT[ii] |= BG_REG_32x64;
    }

    // btns = std::make_shared<Button>();
    //
    // btns->add("Play", [](void) {
    //     VBlankIntrDelay(10);
    //     Scener::set(Global::s_game);
    //   }
    // );
    //
    // btns->add("Options", NULL);
    // btns->add("Extras", NULL);
    //
    // btns->space = 15;
    // t = 0x00;

    TONC_CPY(pal_obj_mem, gfx_menu_textPal);
    TONC_CPY(tile_mem[4], gfx_menu_textTiles);

    for (ii = 0; ii < 3; ii++) {
      start_spr[ii] = T_addObj(
          ii * 32, 0, OBJ_32X16, 8 * ii, 0, 0, NULL
        );

      T_hideObj(start_spr[ii]);
    }

    started = false;
    in_scene = true;
    bg_dy = BG_SPEED;
    kadu_dy = 0x00;
  }

  void update() {
    t += 0x0400;

    // btns->setTextPos(
    //     25 + ( ( 40 * lu_cos(t) >> 8 ) >> 8 ),
    //     6 << 3
    //   );
    //
    // btns->update();

    if (key_hit(KEY_START)) {
      VBlankIntrDelay(10);
      Scener::set(Global::s_game);
    }

    if (!in_scene) {
      for (ii = 0; ii < 3; ii++) {
        start_spr[ii]->x = ((ii << 4) << 1) + (( SCREEN_WIDTH - 80) >> 1);
        start_spr[ii]->y = (SCREEN_HEIGHT - 32) + ( ( 40 * lu_cos(t) >> 8 ) >> 8 );
        T_showObj(start_spr[ii]);
      }
    }

    if (bg[2]->pos.y <= 0x00) {
      bg_dy = 0x00;
      kadu_dy = BG_SPEED;
    } else
      bg_dy = BG_SPEED;

    if (-bg[1]->pos.y >= 0x00) {
      kadu_dy = 0x00;
      in_scene = false;
    }
    
    bg[1]->move(0x00, -(kadu_dy << 3));
    bg[2]->move(0x00, -bg_dy);
    bg[3]->move(0x00, -(bg_dy / 2));

    for (ii = 0; ii < 4; ii++) {
      if (bg[ii]) {
        bg[ii]->update();
      }
    }

    T_updateObjs(FALSE);
  }

  void end() {
    for (ii = 0; ii < 3; ii++) {
      REM_SPR(start_spr[ii]);
    }

    btns = nullptr;

    for (ii = 0; ii < 4; ii++)
      bg[ii] = nullptr;

    RegisterRamReset(RESET_PALETTE);
    RegisterRamReset(RESET_VRAM);
  }

}

namespace Global {
  Scener::Scene s_menu = {
    Menu::init,
    Menu::update,
    Menu::end
  };
}
