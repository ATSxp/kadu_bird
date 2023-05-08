#ifndef __E_GAME_OVER_HPP__
#define __E_GAME_OVER_HPP__

#include <array>
#include "engine/gba.h"
#include "e_player.hpp"

class GameOver {
  private:
    int ii;
    bool show_txt{false};
    bool record_breaked{false};

    CSTR txt{
      "#{P:84,120;ci:4}Game over"
      "#{P:92,136;ci:3}[START]"
    };

    cu16 tid_hand{71};
    std::array<const FIXED, 2> hand_minus{-(8 << 8), (8 << 8)};

    POINT32 hand_pos[2]{
      {-( 32 << 8), int2fx((SCREEN_HEIGHT - 24) >> 1)},
      {SCREEN_WIDTH << 8, int2fx((SCREEN_HEIGHT - 24) >> 1)}
    };

    std::array<const FIXED, 2> hand_speed{0x0680, -0x0680};
    std::array<TSprite *, 2> hand_spr{nullptr, nullptr};

  public:

    GameOver();
    ~GameOver();

    void update(Player &p);
};

#endif // !__E_GAME_OVER_HPP__
