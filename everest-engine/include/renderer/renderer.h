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

            static inline void issue_clear(){ _api->clear();}
            static inline void issue_setClearColor(vec4 color){
                _api->setClearColor(color);
            }
        private:
            static RenderAPI *_api;
            static SceneData _scene;
    };
}
