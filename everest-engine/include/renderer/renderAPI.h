/*
 * ============= render API =============
 * Author: Sunil Sapkota
 * Description: render api contains the abstraction and implementation for
 * render commands, which are responsible for telling the graphics card what to
 * do.
 */

#pragma once
#include "vao.h"

namespace Everest {
    class RenderAPI {
        public:
            static void clear();
            static void setClearColor(vec4 color);
            static void drawIndexed(ref<VAO>& vertexArray, u32 indexCount = 0);
    };
}
