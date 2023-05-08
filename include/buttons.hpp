#ifndef __BUTTONS_HPP__
#define __BUTTONS_HPP__

#include <vector>
#include <string>
#include <tonc.h>
#include "engine/gba.h"

typedef struct {
  std::string name;
  fnptr callback;
} ButtonTemplate;

class Button {
  private:
    int ii;
    int cursor{0};
    POINT32 txt_pos{0, 0};
    TSprite *glass_spr{nullptr};
  
  public:
    int space{8};
    std::vector<ButtonTemplate> btns;

    Button(u16 tid = 0, u16 pb = 0);
    ~Button();

    void update();
    void show();

    inline void add(std::string name, fnptr callback) 
    { btns.push_back({name, callback}); }

    inline void setTextPos(int x, int y) 
    { txt_pos = {x, y}; }
};

#endif // !__BUTTONS_HPP__
