#include "../include/scener.hpp"

#define FUNC_IS_NULL(fn) if (fn) (fn)()

static bool l_change_scene{false};
static Scener::Scene l_cur_scene = {NULL, NULL, NULL};
static Scener::Scene l_new_scene = {NULL, NULL, NULL};

namespace Scener {
  void set(Scene &s) {
    l_change_scene = true;
    l_new_scene = s;
  }

  void update() {
    if (l_change_scene) {
      l_change_scene = false;
      FUNC_IS_NULL(l_cur_scene.end);

      l_cur_scene = l_new_scene;

      FUNC_IS_NULL(l_cur_scene.init);
    }

    FUNC_IS_NULL(l_cur_scene.update);
  }


}
