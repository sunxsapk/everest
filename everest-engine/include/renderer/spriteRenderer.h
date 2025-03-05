/*
 * ========= Sprite Renderer =================
 */


#pragma once


#include "math/types.h"
#include "scene/component_type.h"
#include "texture.h"

namespace Everest {

    struct spriteRenderer_c : public component_t{
        ref<Texture> texture = nullptr;
        vec4 color{1.f}; 
        vec2 startUV = vec2(0.f);
        vec2 sizeUV  = vec2(1.f);

        spriteRenderer_c() = default;
        spriteRenderer_c(Entity ent) : component_t(ent) {}
        void makeCopyUsing(const spriteRenderer_c& other, Entity ent){
            *this = other;
            entity = ent;
            active = other.active;
        }
    };

}
