/*
 * ========== renderer ===========
 * Author: Sunil Sapkota
 * Description: renderer is responsible rendering the mesh from some mesh data
 * that is provided to it.
 */

#pragma once
#include "renderAPI.h"
#include "scene/camera.h"
#include "shader.h"

namespace Everest {
    struct SceneData {
        mat4 viewProjectionMatrix;
    };

    class Renderer {
        public:
            static void init();
            static void quit();

            static void beginScene(Camera& camera);
            static void endScene();
            static void submit(ref<VAO>& vertexArray,
                    ref<Shader>& shader, mat4 transform);

            static inline void issue_clear(){RenderAPI::clear();}
            static inline void issue_setClearColor(vec4 color){
                RenderAPI::setClearColor(color);
            }
            static inline void setViewPort(u32 x, u32 y, u32 width, u32 height){
                glViewport(x, y, width, height);
            }

        private:
            static SceneData _scene;
    };
}
