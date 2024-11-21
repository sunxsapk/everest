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
        const char* tag;
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

        std::function<void()> create;
        std::function<void()> destroy;

        std::function<void()> onCreate;
        std::function<void()> onUpdate;
        std::function<void()> onDestroy;

        template<typename script_t>
        void bind(){
            create = [&](){_instance = new script_t();};
            destroy = [&](){delete (script_t*)_instance;};

            onCreate = [&](){ ((script_t*)_instance)->onCreate();};
            onUpdate = [&](){ ((script_t*)_instance)->onUpdate();};
            onDestroy = [&](){ ((script_t*)_instance)->onDestroy();};
        }
    };
}
