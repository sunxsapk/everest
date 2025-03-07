/*
 * ========== renderer ===========
 * Author: Sunil Sapkota
 * Description: Renderer2d is a small copyof renderer class(managed by it),
 * specializing in rendering 2D stuffs on the screen. The reason for making a
 * separate 2d renderer is so that we can optimize it for quad rendering only.
 * All 2d shapes, sprites, etc are just extensions of quad, so this will tailored
 * for those stuffs only.*/

#pragma once

#include "renderer/circleRenderer.h"
#include "renderer/spriteRenderer.h"
#include "vao.h"
#include "shader.h"
#include "scene/camera.h"
#include "texture.h"
#include "renderer/renderAPI.h"

#ifdef APPLE
#define MAX_TEXTURES 16
#else
#define MAX_TEXTURES 32
#endif

namespace Everest {
    struct QuadVertex {
        vec4 color;
        vec3 position;
        vec2 uv;
        f32 textureIndex;

#ifdef EDITOR_BUILD
        i32 id;
#endif
    };

    struct CircleVertex {
        vec4 color;
        vec3 position;
        vec2 normCoord;
        vec2 uv;
        f32 textureIndex;
        f32 thickness;
        f32 fade;

#ifdef EDITOR_BUILD
        i32 id;
#endif
    };

    struct LineVertex {
        vec3 position;
        vec4 color;
    };

    struct RendererStats {
        u32 quadCount = 0;
        u32 lineCount = 0;
        u32 vertexCount = 0;
        u32 textureCount = 0;
        u32 drawCalls = 0;
    };

    struct Renderer2Ddata {
        const u32 maxQuads = 10000;
        const u32 maxVertices = maxQuads * 4;
        const u32 maxIndices = maxQuads * 6;
        static const i32 maxTexSlots = MAX_TEXTURES; // TODO: glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_data->maxTexSlots);
        const i32 maxLines = maxVertices / 2;

        std::array<vec4, 4> quadVertPos;

        QuadVertex *quadVertBase, *quadVertPtr;
        ref<VAO> quadVertArray;
        ref<Shader> quadShader;

        CircleVertex *circleVertBase, *circleVertPtr;
        ref<VAO> circleVertArray;
        ref<Shader> circleShader;

        LineVertex *lineVertBase, *lineVertPtr;
        ref<VAO> lineVertArray;
        ref<Shader> lineShader;

        ref<Texture> whiteTexture;
        std::array<ref<Texture>, maxTexSlots> textures;

        u32 quadIndexCount, circleIndexCount, lineCount, texCount;
    };

    class Renderer2D {
        public:
            static void init();
            static void quit();

            static void beginScene(camera_c& camera, mat4 cameraTransform);
            static void endScene();

            static void drawQuad(vec3 position, vec2 scale = vec2(1.f), f32 rotation = 0.f,
                    vec4 color = vec4(1.f), ref<Texture> texture = nullptr);
            static void drawCircle(vec3 position, f32 diameter = 1.f, vec4 color = vec4(1.f),
                    f32 thickness = 1.f, f32 fade = 0.f, ref<Texture> = nullptr);
            static void drawLine(vec3 point1, vec3 point2, vec4 color = vec4(1.f));
            static void drawRect(mat4 transform, vec4 color = vec4(1.f));
            static void drawRect(vec3 pos, f32 rotation = 0.f, vec2 scale = vec2(1.f), vec4 color = vec4(1.f));


            static void drawSprite(mat4 transform, const spriteRenderer_c& sprite
#ifdef EDITOR_BUILD
                    , i32 id = 0
#endif
                    );
            static void drawSprite(const spriteRenderer_c& sprite, vec3 position, vec2 scale = vec2(1.f),
                    f32 rotation = 0.f
#ifdef EDITOR_BUILD
                    , i32 id = 0
#endif
                    );
            static void drawCircle(mat4 transform, const circleRenderer_c& circle
#ifdef EDITOR_BUILD
                    , i32 id = 0
#endif
                    );

            static void drawCircle(mat4 transform, vec4 color, f32 thickness, f32 fade
#ifdef EDITOR_BUILD
                    , i32 id = 0
#endif
                    );


            static inline const RendererStats& getStats(){return _stats;}
            static inline void setLineWidth(f32 width){
                RenderAPI::setLineWidth(width);}

        private:
            static void flushTextured();
            static void flushLines();
            static void _checkTexture(i32& texIndex, ref<Texture> texture);
        private:
            static Renderer2Ddata *_data;
            static RendererStats _stats;
    };
}
