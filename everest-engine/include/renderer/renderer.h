/*
 * ========== renderer ===========
 * Author: Sunil Sapkota
 * Description: renderer is responsible rendering the mesh from some mesh data
 * that is provided to it.
 */

#pragma once
#include "vao.h"

namespace Everest {
    class Renderer {
        public:
            Renderer();
            ~Renderer();
        protected:
            VAO _vao;
    };
}
