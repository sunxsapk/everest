/*
 * ============= Gizmos ===============
 * Author: Sunil Sapkota
 * Description: Contains the gizmos for the scene viewport
 */

#pragma once
#include <everest.h>
#include "editorcamera.h"


namespace Everest {

    class Gizmos {
        public:
            static void init();
            static void quit();
            static void renderGrid(EditorCamera& camera, vec2 size = {1000.f, 1000.f}, bool is2D = false);

        private:
             static Shader *_gridShader;
             static vec3 _gridVertices[4];
             static ref<VAO> _vao;
    };

}
