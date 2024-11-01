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
            void clear();
            void setClearColor(vec4 color);
            void drawIndexed(p_shared(VAO) vertexArray);
    };
}
