#include "../include/s_menu.hpp"
#include "../include/global.hpp"
#include "../include/e_player.hpp"

namespace Menu {
  void init() {
    const TFont *fonts[2] = {
      &verdana10Font,
      &verdana9Font
    };

    T_setMode(4);
    T_enableBg(2);

    tte_init_bmp(4, &verdana10Font, (fnDrawg)bmp8_drawg_b1cts_fast);
    pal_bg_bank[15][1] = CLR_WHITE;
    
    tte_set_font_table(fonts);
    tte_write("#{P:2,60}Menu#{P:2,76;f:1}[Start]");
  }

  void update() {
    if (key_hit(KEY_START)) Scener::set(Global::s_game);
  }

  void end() {
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
