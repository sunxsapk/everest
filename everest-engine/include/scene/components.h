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

        operator mat4(){
            mat4 trl = glm::translate(mat4(1.f), position);

            vec3 rrot = glm::radians(rotation);
            mat4 rot = glm::toMat4(glm::quat(rrot));

            mat4 scl = glm::scale(mat4( 1.f), scale);

            return trl * rot * scl;
        }

    };

    struct spriteRenderer_c{
        vec4 color{1.f}; 
        Sprite sprite; // TODO: single and multiple
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
