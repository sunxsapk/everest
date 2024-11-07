/*
 * ========== renderer ===========
 * Author: Sunil Sapkota
 * Description: Renderer2d is a small copyof renderer class(managed by it),
 * specializing in rendering 2D stuffs on the screen. The reason for making a
 * separate 2d renderer is so that we can optimize it for quad rendering only.
 * All 2d shapes, sprites, etc are just extensions of quad, so this will tailored
 * for those stuffs only.*/

#pragma once
#include "vao.h"
#include "shader.h"
#include "scene/camera.h"
#include "texture.h"

namespace Everest {
    struct RendererStorage{
        ref<VAO> vertArray;
        ref<Shader> textureShader;
        ref<Texture> whiteTexture;
    };

    class Renderer2D {
        public:
            static void init();
            static void quit();

            static void beginScene(Camera& camera);
            static void endScene();

            static void drawQuad(vec3 position, vec2 scale, f32 rotation,
                    vec4 color = vec4(1.f), ref<Texture> texture = NULL);
            static void drawQuad(vec2 position, vec2 scale, f32 rotation,
                    vec4 color=vec4(1.f), ref<Texture> texture = NULL);
        private:
            static RendererStorage *_data;
    };
}
