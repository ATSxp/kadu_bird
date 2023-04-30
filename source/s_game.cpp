#include <memory>
#include <vector>
#include <tonc.h>
#include "../include/s_game.hpp"
#include "../include/global.hpp"
#include "../include/e_player.hpp"
#include "../include/e_pipe.hpp"
#include "../include/map.hpp"

#include "map_bg1.h"
#include "map_bg2.h"
#include "map_bg3.h"

#define PIPE_TIMER_MAX 0x02000

namespace Game {
  int ii;

  constexpr SCR_ENTRY *maps[3] = {
    (SCR_ENTRY*)map_bg1Map,
    (SCR_ENTRY*)map_bg2Map,
    (SCR_ENTRY*)map_bg3Map,
  };

  constexpr COLOR *pals[3] = {
    (COLOR*)map_bg1Pal,
    (COLOR*)map_bg2Pal,
    (COLOR*)map_bg3Pal,
  };

  const TILE4 *tiles[3] = {
    (TILE4*)map_bg1Tiles,
    (TILE4*)map_bg2Tiles,
    (TILE4*)map_bg3Tiles,
  };

  constexpr s16 tids[3] = {0, 155, 208};
  constexpr FIXED bg_speeds[3] = {0x090, 0x050, -0x020};

  std::shared_ptr<Player> p{nullptr};
  std::shared_ptr<Map> bg[3]{nullptr, nullptr, nullptr};
  std::vector<Pipe> pipe_l;
  FIXED pipe_t;

  void spawnPipes();

  void init(void) {
    T_setMode(0);
    T_enableBg(0);
    T_enableObjs();
    T_initObjs();

    tte_init_se_default(0, BG_CBB(1) | BG_SBB(20));

    p = std::make_shared<Player>();

    for (ii = 0; ii < 3; ii++) {
      LZ77UnCompVram(pals[ii], pal_bg_bank[ii]);
      LZ77UnCompVram(tiles[ii], &tile_mem[0][ tids[ii] ]);

      bg[ii] = std::make_shared<Map>(ii + 1, maps[ii], 64, 32, 0, 30 - (2 * ii), true);
      bg[ii]->move(bg_speeds[ii], 0x00);
    }

    pipe_t = 0x00;
  }

  void update(void) {
    size_t kk;

    p->update();
    spawnPipes();

    for (ii = 0; ii < 3; ii++)
      bg[ii]->update();

    if (pipe_l.size() != 0) {
      for (kk = 0; kk < pipe_l.size(); kk++) {
        if (pipe_l[kk].dead)
          pipe_l.erase(pipe_l.begin() + kk);

        pipe_l[kk].PipeVsPlayer(*p);
        pipe_l[kk].update();
      }
    }

    T_updateObjs(FALSE);
  }

  void end(void) {
    p = nullptr;

    size_t kk;
    for (kk = 0; kk < pipe_l.size(); ii++)
      pipe_l[kk].dead = true;

    pipe_l.clear();

    for (ii = 0; ii < 3; ii++)
      bg[ii] = nullptr;
  }

  void spawnPipes() {
    pipe_t -= 0x020;

    if (pipe_t <= 0 && pipe_l.size() < 5) {
      pipe_l.push_back(*std::make_shared<Pipe>(SCREEN_WIDTH, qran_range(-80, -16)));
      pipe_t = PIPE_TIMER_MAX;
    }

  }

}

namespace Global {
  Scener::Scene s_game = {
    Game::init,
    Game::update,
    Game::end,
  };

}
