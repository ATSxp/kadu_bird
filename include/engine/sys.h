#ifndef __ENGINE_SYS__
#define __ENGINE_SYS__

#include <tonc.h>

// ==============================
// ========== Macros ============
// ==============================

// ==============================
// ======= Prototypes ===========
// ==============================

INLINE void T_init();
INLINE void T_update();
INLINE void T_setMode(u16 mode);
INLINE void T_enableBg(u16 bg);
INLINE void T_disableBg(u16 bg);
INLINE void T_enableObjs();
INLINE void T_disableObjs();

// ==============================
// ======= Functions ============
// ==============================

INLINE void T_init() {
  irq_init(NULL);
  irq_add(II_VBLANK, NULL);
}

INLINE void T_update() {
  VBlankIntrWait();
  key_poll();
}

INLINE void T_setMode(u16 mode)
{ REG_DISPCNT = mode; }

INLINE void T_enableBg(u16 bg) {
  if (bg < 2) {
    REG_DISPCNT |= (bg + 1) << 8;
  } else {
    REG_DISPCNT |= ((bg + (bg == 3 ? 1 : 0)) << 8) << 1;
  }

}

INLINE void T_disableBg(u16 bg) {
  if (bg < 2) {
    REG_DISPCNT &= ~(bg + 1) << 8;
  } else {
    REG_DISPCNT &= ~((bg + (bg == 3 ? 1 : 0)) << 8) << 1;
  }

}

INLINE void T_enableObjs() {
  REG_DISPCNT |= DCNT_OBJ | DCNT_OBJ_1D;
}

INLINE void T_disableObjs() {
  REG_DISPCNT &= ~(DCNT_OBJ);
  REG_DISPCNT &= ~(DCNT_OBJ_1D);
}

#endif // !__ENGINE_SYS__
