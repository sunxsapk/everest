#include "scene/camera.h"

namespace Everest {

    camera_c::camera_c(bool is2d){
        _isOrtho = is2d;
        recalc();
    }
}
