#ifndef __BUTTONS_HPP__
#define __BUTTONS_HPP__

#include "engine/gba.h"
#include <tonc.h>
#include <vector>

#include "gfx_glass.h"

typedef struct {
  CSTR name;
  fnptr callback;
  bool show_status, on;
} ButtonTemplate;

class Button {
private:
  int ii;
  int cursor{0};
  int old_cursor{0};
  bool on{true};
  bool event{false};
  POINT32 txt_pos{0, 0};
  TSprite *glass_spr{nullptr};

public:
  int space{8};
  std::vector<ButtonTemplate> btns;
  u16 tid, pb;
  bool lock_cursor{false};

  Button(u16 tid = 0, u16 pb = 0);
  ~Button();

  void update();
  void show();

  inline void add(CSTR name, fnptr callback) {
    btns.push_back({name, callback});
  }

  inline void add(CSTR name, fnptr callback, bool status) {
    btns.push_back({name, callback, status});
  }

  inline void setTextPos(int x, int y) { txt_pos = {x, y}; }

  inline void onInput(bool on = true) { this->on = on; }

  inline bool getEvent() { return event; }

  inline void setButtonStatus(u8 id, bool on) { btns[id].on = on; }

  inline void loadPal() {
    GRIT_CPY(pal_obj_bank[this->pb], gfx_glassPal);
  }
};

#endif // !__BUTTONS_HPP__
