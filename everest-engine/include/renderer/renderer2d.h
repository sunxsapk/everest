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
#include "sprite.h"

namespace Everest {
    struct QuadProps {
        ref<Texture> texture = NULL;
        vec4 color = vec4(1.f);
        vec3 position = vec3(0.f);
        f32 rotation = 0.f;
        vec2 scale = vec2(1.f);
        f32 tilingFactor = 1.f;
    };

    struct QuadVertex {
        vec3 position;
        vec4 color;
        vec2 uv;
        f32 textureIndex;
        f32 tilingFactor;
    };

    struct RendererStats {
        u32 quadCount = 0;
        u32 vertexCount = 0;
        u32 textureCount = 0;
        u32 drawCalls = 0;
    };

    struct Renderer2Ddata {
        const u32 maxQuads = 10000;
        const u32 maxVertices = maxQuads * 4;
        const u32 maxIndices = maxQuads * 6;
        static const i32 maxTexSlots = 32; // TODO: glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_data->maxTexSlots);

        std::array<vec3, 4> quadVertPos;

        QuadVertex *vertBase, *vertPtr;

        ref<VAO> vertArray;
        ref<Shader> textureShader;
        ref<Texture> whiteTexture;
        std::array<ref<Texture>, maxTexSlots> textures;

        u32 indexCount, texCount;
    };

    class Renderer2D {
        public:
            static void init();
            static void quit();

            static void beginScene(Camera& camera);
            static void endScene();

            static void drawQuad(vec3 position, vec2 scale, f32 rotation,
                    vec4 color = vec4(1.f), ref<Texture> texture = NULL, f32 tilingFactor = 1.f);
            static void drawQuad(vec2 position, vec2 scale, f32 rotation,
                    vec4 color=vec4(1.f), ref<Texture> texture = NULL, f32 tilingFactor = 1.f);
            static void drawQuad(const QuadProps& props);

            static void drawSprite(Sprite sprite, vec3 position, vec2 scale = vec2(1.f),
                    f32 rotation = 0.f, vec4 color=vec4(1.f), f32 tilingFactor=1.f);

            static inline const RendererStats& getStats(){return _stats;}

        private:
            static void flush();
        private:
            static Renderer2Ddata *_data;
            static RendererStats _stats;
    };
}
