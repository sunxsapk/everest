/*
 * ========= Sprite Renderer =================
 */


#pragma once


#include "math/types.h"
#include "texture.h"

namespace Everest {

    struct spriteRenderer_c{
        ref<Texture> texture = nullptr;
        vec4 color{1.f}; 
        vec2 startUV = vec2(0.f);
        vec2 sizeUV  = vec2(1.f);
    };

}
