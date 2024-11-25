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

    struct tag_c {
        std::string tag;
    };
    
    struct transform_c {
        vec3 position{0.f}, rotation{0.f}, scale{1.f};

        mat4 getTransform(){
            mat4 trl = glm::translate(mat4(1.f), position);

            vec3 rrot = glm::radians(rotation);
            mat4 rotx = glm::rotate(mat4(1.f), rrot.x, {1.f, 0.f, 0.f});
            mat4 roty = glm::rotate(mat4(1.f), rrot.y, {0.f, 1.f, 0.f});
            mat4 rotz = glm::rotate(mat4(1.f), rrot.z, {0.f, 0.f, 1.f});

            mat4 scl = glm::scale(mat4( 1.f), scale);

            return trl * (rotz * roty * rotx) * scl;
        }
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
