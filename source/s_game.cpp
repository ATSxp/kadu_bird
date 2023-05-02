#include <memory>
#include <vector>
#include <string.h>
#include <tonc.h>
#include "../include/s_game.hpp"
#include "../include/global.hpp"
#include "../include/e_player.hpp"
#include "../include/e_pipe.hpp"
#include "../include/map.hpp"

#include "gfx_board.h"
#include "gfx_hp.h"

#include "map_bg1.h"
#include "map_bg2.h"
#include "map_bg3.h"

#include "../include/verdana11.h"

#define BOARD_TID 72
#define HP_TID 80

namespace Game {
  int ii;

  constexpr SCR_ENTRY *maps[3] = {
    (SCR_ENTRY*)map_bg1Map,
    (SCR_ENTRY*)map_bg2Map,
    (SCR_ENTRY*)map_bg3Map,
  };

  const TILE4 *tiles[3] = {
    (TILE4*)map_bg1Tiles,
    (TILE4*)map_bg2Tiles,
    (TILE4*)map_bg3Tiles,
  };

  constexpr u16 tids[3] = {0, 155, 208};
  constexpr FIXED bg_speeds[3] = {0x090, 0x050, -0x020};

  std::shared_ptr<Player> p{nullptr};
  std::shared_ptr<Map> bg[3]{nullptr, nullptr, nullptr};
  std::vector<Pipe> pipe_l;
  FIXED pipe_t;
  TSprite *board_spr[2] = {nullptr};

  void spawnPipes();

  void init(void) {
    T_setMode(0);
    T_enableBg(0);
    T_enableObjs();
    T_initObjs();

    tte_init_chr4c(
        0, 
        BG_CBB(1) | BG_SBB(20) | BG_PRIO(0), 
        SE_PALBANK(15), 
        14, 
        CLR_BLACK, 
        &verdana11Font, (fnDrawg)chr4c_drawg_b1cts_fast
      );

    TONC_CPY(pal_obj_bank[2], gfx_boardPal);
    TONC_CPY(&tile_mem[4][BOARD_TID], gfx_boardTiles);

    TONC_CPY(pal_obj_bank[3], gfx_hpPal);
    TONC_CPY(&tile_mem[4][HP_TID], gfx_hpTiles);

    p = std::make_shared<Player>();

    board_spr[0] = T_addObj(1, 1, OBJ_16X32, BOARD_TID, 2, 1, NULL);
    board_spr[1] = T_addObj(33, 1, OBJ_16X32, BOARD_TID, 2, 1, NULL);
    T_flipObj(board_spr[1], TRUE, FALSE); // Flip sprite right on the board

    for (ii = 0; ii < 3; ii++) {
      Global::hp_spr[ii] = T_addObj(
          (ii << 4) + 1, SCREEN_HEIGHT - 17, 
          OBJ_16X16, 
          HP_TID, 3, 1, NULL
        );
    }

    LZ77UnCompVram(map_bg1Pal, pal_bg_mem);
    for (ii = 0; ii < 3; ii++) {
      LZ77UnCompVram(tiles[ii], &tile_mem[0][ tids[ii] ]);

      bg[ii] = std::make_shared<Map>(ii + 1, maps[ii], 64, 32, 0, 30 - (2 * ii), true);
      bg[ii]->move(bg_speeds[ii], 0x00);
    }

    pipe_t = 0x00;
  }

  void update(void) {
    CSTR board_txt = "#{es;P:6,1}%06d";
    char dst_board[strlen(board_txt)];

    p->update();
    spawnPipes();

    posprintf(dst_board, board_txt, p->points);
    tte_write(dst_board);

    if (p->hp < 3)
      T_setTileObj(Global::hp_spr[p->hp], HP_TID + 4);

    for (ii = 0; ii < 3; ii++)
      bg[ii]->update();

    // Update Pipes
    if (pipe_l.size() != 0) {
      size_t kk;
      for (kk = 0; kk < pipe_l.size(); kk++) {
        if (pipe_l[kk].dead)
          pipe_l.erase(pipe_l.begin() + kk);

        pipe_l[kk].update();
        pipe_l[kk].PipeVsPlayer(*p);
      }
    }

    T_updateObjs(FALSE);
  }

  void end(void) {
    p = nullptr;

    size_t kk;
    for (kk = 0; kk < pipe_l.size(); ii++)
      pipe_l[kk].die();

    pipe_l.clear();

    for (ii = 0; ii < 2; ii++) {
      REM_SPR(board_spr[ii]);
    }

    for (ii = 0; ii < 3; ii++) {
      REM_SPR(Global::hp_spr[ii]);
      bg[ii] = nullptr;
    }
  }

  void spawnPipes() {
    pipe_t -= 0x020;

    if (pipe_t <= 0 && pipe_l.size() < 5) {
      std::shared_ptr<Pipe> pipe = std::make_shared<Pipe>(SCREEN_WIDTH, qran_range(-90, -16));
      pipe_l.push_back(*pipe);
      pipe_t = 0x02000;
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
