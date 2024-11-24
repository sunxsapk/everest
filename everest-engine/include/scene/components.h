/*
 * ============= Components ===============
 * Author: Sunil Sapkota
 * Description : Contains all of the components made available by the engine.
 */
#pragma once
#include "camera.h"
#include "renderer/sprite.h"
#include "scriptable.h"

namespace Everest {
    struct transform_c {
        mat4 transform{1.f};

        operator mat4&(){return transform;}
        operator const mat4&()const{return transform;}
    };

    struct tag_c {
        std::string tag;
    };
    
    struct spriteRenderer_c{
        vec4 color{1.f}; 
        Sprite sprite;
    };

    struct camera_c {
        Camera camera;
        bool fixedAspect = false;

        operator Camera&(){return camera;}
    };

    struct nativeScript_c {
        Scriptable* _instance;

        Scriptable* (*create)();
        void (*destroy)(nativeScript_c*);

        template<typename script_t>
        void bind(){
            create = [](){return static_cast<Scriptable*>(new script_t());};
            destroy = [](nativeScript_c* nsc){
                delete nsc->_instance;
                nsc->_instance = nullptr;
            };
        }
    };
}
