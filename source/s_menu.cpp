#include "../include/s_menu.hpp"
#include "../include/global.hpp"
#include "../include/e_player.hpp"

namespace Menu {
  void init() {
  }

  void update() {
    T_updateObjs(FALSE);
  }

  void end() {

  }

}

namespace Global {
  Scener::Scene s_menu = {
    Menu::init,
    Menu::update,
    Menu::end
  };
}
