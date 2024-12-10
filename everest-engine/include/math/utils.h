/*
 * ========== math utility library ===========
 * Author: Sunil Sapkota
 * Description: Contains some utility math functions for in-engine & out-engine
 * use
 */

#pragma once
#include "types.h"
#include "scene/components.h"

namespace Everest {
    namespace Math {
        vec3 transformOrtho(vec3 point, vec3 position, vec2 scale, f32 rotation);
        bool decomposeTransform(transform_c& transform, mat4& transformMatrix);
    }
}
