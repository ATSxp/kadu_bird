#ifndef __PIPE_HPP__
#define __PIPE_HPP__

#include "engine/gba.h"
#include "e_player.hpp"
#include <array>

#define PIPE_SPR_COUNT 12

class Pipe {
  private:
    u16 tid{27};
    int space_btw;
    bool pointed{false};

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
