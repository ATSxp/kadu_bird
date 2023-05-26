#include <memory>
#include <vector>
#include <array>
#include <string.h>
#include <tonc.h>
#include "../include/s_game.hpp"
#include "../include/global.hpp"
#include "../include/e_player.hpp"
#include "../include/e_pipe.hpp"
#include "../include/e_game_over.hpp"
#include "../include/map.hpp"

#include "gfx_board.h"
#include "gfx_hp.h"
#include "gfx_pipe.h"

#include "map_bg1.h"
#include "map_bg2.h"
#include "map_bg3.h"

#include "../include/verdana11.h"

#define BOARD_TID 59
#define HP_TID 67

namespace Game {
  int ii;
  size_t kk;

  std::shared_ptr<Player> p{nullptr};
  std::array<std::shared_ptr<Map>, 3> bg{nullptr, nullptr, nullptr};
  std::shared_ptr<GameOver> gmovr{nullptr};
  TSprite *board_spr[2]{nullptr, nullptr};
  std::vector<Pipe> pipe_l;
  FIXED pipe_t;
  u8 evy;
  bool paused, game_over;
  int bg_size = static_cast<int>(bg.size());

  void spawnPipes();
  void diePipes();
  void hideHud();
  void showHud();

  void init(void) {
    const TFont *fonts[2]{
      &verdana9Font, &verdana9bFont
    };

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

    sqran(Global::seed_rand);

    T_setMode(0);
    T_enableBg(0);

    T_enableObjs();
    T_initObjs();

    REG_BLDCNT = (BLD_ALL & ~BLD_BG0) | BLD_BLACK;

    tte_init_chr4c(
        0, 
        BG_CBB(1) | BG_SBB(20) | BG_PRIO(0), 
        SE_PALBANK(15), 
        2, 
        CLR_BLACK, 
        &verdana11Font, (fnDrawg)chr4c_drawg_b1cts_fast
      );
    tte_set_font_table(fonts);

    GRIT_CPY(&tile_mem[4][BOARD_TID], gfx_boardTiles);
    GRIT_CPY(&tile_mem[4][HP_TID], gfx_hpTiles);

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

    for (ii = 0; ii < bg_size; ii++) {
      LZ77UnCompVram(tiles[ii], &tile_mem[0][ tids[ii] ]);

      bg[ii] = std::make_shared<Map>(ii + 1, maps[ii], 64, 32, 0, 30 - (ii << 1), true);
      bg[ii]->move(bg_speeds[ii], 0x00);
    }

    LZ77UnCompVram(map_bg1Pal, pal_bg_mem);
    GRIT_CPY(pal_obj_bank[2], gfx_boardPal);
    GRIT_CPY(pal_obj_bank[3], gfx_hpPal);
    pal_bg_bank[15][3] = CLR_WHITE;
    pal_bg_bank[15][4] = CLR_RED;

    paused = false;
    game_over = false;
    pipe_t = 0x00;
    evy = 0x00;
  }

  void update(void) {
    CSTR board_txt = "#{es;ci:5;P:6,1}%06d";
    char dst_board[strlen(board_txt) - 16];

    posprintf(dst_board, board_txt, p->points);
    tte_write(dst_board);

    if (key_hit(KEY_START)) paused = !paused;

    if (p->hp < 3)
      T_setTileObj(Global::hp_spr[p->hp], HP_TID + 4);

    // Fade background and pipes
    evy = clamp(evy, 0x00, 0x081);
    REG_BLDY = BLDY_BUILD(evy >> 3);

    if (!paused)
      clr_fade(gfx_pipePal, CLR_BLACK, pal_obj_bank[1], 16, evy >> 2);

    if (p->dead) {
      REG_BLDCNT &= ~BLD_OBJ;
      evy += 4;

      if (evy >= 0x080) {
        bg[0]->move(0x00, 0x00);
        bg[0]->pos.x = 0x00;
        diePipes();
      }

      tte_erase_line();
      hideHud();

      if (!game_over) {
        game_over = true;
        gmovr = std::make_shared<GameOver>();
      }

      gmovr->update(*p);
    }

    if (paused && !p->dead) {
      evy = 0x040;
      tte_write("#{el;P:96,72;ci:3}Paused");

      hideHud();
    } else {
      if (!p->dead) {
        evy = 0x00;
        showHud();
      }

      p->update();
      spawnPipes();

      for (ii = 0; ii < bg_size; ii++)
        bg[ii]->update();

      // Update Pipes
      if (pipe_l.size() != 0) {
        for (kk = 0; kk < pipe_l.size(); kk++) {
          if (pipe_l[kk].dead)
            pipe_l.erase(pipe_l.begin() + kk);

          pipe_l[kk].update();
          pipe_l[kk].PipeVsPlayer(*p);
        }
      }

    }

    T_updateObjs(FALSE);
  }

  void end(void) {
    RegisterRamReset(RESET_PALETTE);
    RegisterRamReset(RESET_VRAM);

    REG_BLDY = 0;

    tte_erase_screen();
    tte_set_pos(0, 0);

    p = nullptr;
    gmovr = nullptr;

    diePipes();

    for (ii = 0; ii < bg_size; ii++) {
      REM_SPR(Global::hp_spr[ii]);
      bg[ii] = nullptr;
      if (ii > 2) break;
      REM_SPR(board_spr[ii]);
    }

  }

  void spawnPipes() {
    pipe_t -= 0x020;

    if (pipe_t <= 0 && pipe_l.size() < 5 && !p->dead) {
      std::shared_ptr<Pipe> pipe = std::make_shared<Pipe>(SCREEN_WIDTH, qran_range(-24, -90));
      pipe_l.push_back(*pipe);
      pipe_t = 0x02000;
    }

  }

  void diePipes() {
    if (pipe_l.size() == 0) return;

    for (kk = 0; kk < pipe_l.size(); kk++)
      pipe_l[kk].die();

    pipe_l.clear();
  }

  void hideHud() {
    for (ii = 0; ii < 3; ii++) {
      T_hideObj(Global::hp_spr[ii]);
      if (ii > 2) break;
      T_hideObj(board_spr[ii]);
    }

  }

  void showHud() {
    for (ii = 0; ii < 3; ii++) {
      T_showObj(Global::hp_spr[ii]);
      if (ii > 2) break;
      T_showObj(board_spr[ii]);
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
