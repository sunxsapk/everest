#pragma once

#include "math/types.h"

namespace Everest {

    struct circleRenderer_c {
        vec4 color{1.f};
        f32 thickness = 1.f;
        f32 fade = 0.f;
    };

}
