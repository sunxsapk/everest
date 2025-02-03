/*
 * ============= Components ===============
 * Author: Sunil Sapkota
 * Description : Contains all of the components made available by the engine.
 */
#pragma once

#include "camera.h"
#include "physics/colliders2d.h"
#include "scriptable.h"
#include "utils/uuid.h"
#include "renderer/spriteRenderer.h"
#include "renderer/circleRenderer.h"
#include "physics/rigidbody.h"
#include "physics/springJoint.h"

namespace Everest {

    struct id_c {
        UUID id;

        operator u64() const {return id;}
    };

    struct tag_c {
        std::string tag;
    };
    
    struct transform_c {
        vec3 position{0.f}, rotation{0.f}, scale{1.f};

        operator mat4(){
            mat4 trl = glm::translate(mat4(1.f), position);
            mat4 rot = glm::toMat4(glm::quat(glm::radians(rotation)));
            mat4 scl = glm::scale(mat4( 1.f), scale);
            return trl * rot * scl;
        }

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

    using Transform = transform_c;
    using Tag = tag_c;
    using Camera = camera_c;
    using SpriteRenderer = spriteRenderer_c;
    using CircleRenderer = circleRenderer_c;
    using Rigidbody = rigidbody_c;
    using Rigidbody2d = rigidbody2d_c;
    using SpringJoint = springJoint_c;
    using BoxCollider2d = boxCollider2d_c;
    using CircleCollider2d = circleCollider2d_c;
    using Collider2D = collider2d_c;
}
