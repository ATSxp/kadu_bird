#ifndef __SCENER_HPP__
#define __SCENER_HPP__

#include <tonc.h>

namespace Scener
{
  typedef struct {
    fnptr init, update, end;
  } Scene;

  void set(Scene &s);
  void update();
}

#endif // !__SCENER_HPP__
