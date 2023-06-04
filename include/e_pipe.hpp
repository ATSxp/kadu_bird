#ifndef __PIPE_HPP__
#define __PIPE_HPP__

#include "e_player.hpp"
#include "engine/gba.h"
#include "global.hpp"
#include <array>

#include "gfx_pipe.h"

#define PIPE_SPR_COUNT 12

class Pipe {
private:
  int ii;

  u16 tid{27};
  int space_btw;
  bool pointed{false};

  Global::SprBase pipe_base[PIPE_SPR_COUNT]{
      // Up
      {0, 64, OBJ_32X32, 0},
      {32, 64, OBJ_32X32, 0},
      {0, 32, OBJ_32X32, 8},
      {32, 32, OBJ_32X32, 8},
      {0, 0, OBJ_32X32, 8},
      {32, 0, OBJ_32X32, 8},

      // Down
      {0, 0, OBJ_32X32, 0},
      {32, 0, OBJ_32X32, 0},
      {0, 32, OBJ_32X32, 8},
      {32, 32, OBJ_32X32, 8},
      {0, 64, OBJ_32X32, 8},
      {32, 64, OBJ_32X32, 8},
  };

  void updateSprs();

public:
  POINT32 pos;
  FIXED dx{0x00};
  u32 w{64}, h{96};
  bool dead{false};
  std::array<TSprite *, PIPE_SPR_COUNT> spr;

  Pipe(int x = 0, int y = 0, FIXED speed = -0x080);
  ~Pipe();

  void update();
  void PipeVsPlayer(Player &p);
  void die();
};

#endif // !__PIPE_HPP__
