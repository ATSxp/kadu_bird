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

namespace Menu {
  int ii;
  FIXED t;

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

  const u16 tile_ids[4] = {0, 0, 92, 357};

  std::shared_ptr<Map> bg[4]{nullptr};
  std::shared_ptr<Button> btns{nullptr};

  void init() {
    const TFont *fonts[2] = {
      &verdana10Font,
      &verdana9Font
    };

    T_setMode(0);
    // T_enableBg(0);

    T_enableObjs();
    T_initObjs();

    tte_init_chr4c(
        0, BG_CBB(0) | BG_SBB(23),
        SE_PALBANK(15),
        14, CLR_WHITE,
        &verdana10Font, (fnDrawg)chr4c_drawg_b1cts_fast
      );

    tte_set_font_table(fonts);

    LZ77UnCompVram(map_menu1Pal, pal_bg_mem);
    for (ii = 0; ii < 4; ii++) {
      if (maps[ii]) {
        LZ77UnCompVram(tiles[ii], &tile8_mem[0][tile_ids[ii]]);

        bg[ii] = std::make_shared<Map>(ii, maps[ii], 32, 32, 0, 31 - ii, true);
        bg[ii]->setBpp(true);
      }
    }

    btns = std::make_shared<Button>();

    btns->add("Play", [](void) {
        VBlankIntrDelay(10);
        Scener::set(Global::s_game);
      }
    );

    btns->add("Options", NULL);
    btns->add("Extras", NULL);

    btns->space = 15;
    t = 0x00;
  }

  void update() {
    t += 0x0400;
    btns->setTextPos(
        25 + ( ( 40 * lu_cos(t) >> 8 ) >> 8 ),
        6 << 3
      );

    // btns->update();

    T_updateObjs(FALSE);
  }

  void end() {
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
