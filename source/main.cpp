#include "../include/engine/gba.h"
#include "../include/global.hpp"
#include "../include/libsavgba/err_def.h"
#include "../include/libsavgba/gba_sram.h"
#include "soundbank_bin.h"
#include <maxmod.h>

int main() {
  u32 err;
  mgba_open();

  if (Global::sram_buffer[0] != 'S') {
    err = sram_write(0, (u8 *)"SRAM_Vnnn", 9);
    err = sram_write(10, 0, SRAM_BUFFER_SIZE - 9);

    if (err) {
      mgba_printf(MGBA_LOG_ERROR, "SRAM Write Error: %s\n", SavErrMsgs[err]);
      goto end;
    } else {
      mgba_printf(MGBA_LOG_DEBUG, "SRAM Write Sucess");
    }
  }

  err = sram_read(0, Global::sram_buffer, SRAM_BUFFER_SIZE);
  if (err) {
    mgba_printf(MGBA_LOG_ERROR, "SRAM Read Error: %s\n", SavErrMsgs[err]);
    goto end;
  } else {
    mgba_printf(MGBA_LOG_DEBUG, "SRAM Read Sucess");
  }

  mgba_printf(MGBA_LOG_DEBUG, "SRAM Test Passed!");

  if (Global::sram_buffer[0] == 'S')
    Global::loadSave();

end:
  T_init(mmVBlank);
  mmInitDefault((mm_addr)soundbank_bin, 16);

  Scener::set(Global::s_intro);

  while (true) {
    if (!Global::sound)
      mmEffectCancelAll();

    mmFrame();
    T_update();
    Scener::update();
    Global::seed_rand++;
  }

  return 0;
}
