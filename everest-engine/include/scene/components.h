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
#include "scripting/evscript.h"
#include "scene/component_type.h"

namespace Everest {

    struct id_c : public component_t {
        UUID id;

        id_c(Entity ent, UUID id_):component_t(ent), id(id_){}
        id_c(Entity ent):component_t(ent){}

        operator u64() const {return id;}
    };

    struct tag_c : public component_t {
        std::string tag;

        tag_c(Entity entity, const char* tag_ = "Entity"):tag(tag_), component_t(entity){}
    };
    
    struct transform_c : public component_t {
        vec3 position{0.f}, rotation{0.f}, scale{1.f};

        transform_c() = default;
        transform_c(Entity ent) : component_t(ent) {}
        void makeCopyUsing(const transform_c& other, Entity ent){
            *this = other;
            entity = ent;
            active = other.active;
        }

        operator mat4(){
            mat4 trl = glm::translate(mat4(1.f), position);
            mat4 rot = glm::toMat4(glm::quat(glm::radians(rotation)));
            mat4 scl = glm::scale(mat4( 1.f), scale);
            return trl * rot * scl;
        }

    };

    struct nativeScript_c : public component_t {
        Scriptable* _instance;

        Scriptable* (*create)();
        void (*destroy)(nativeScript_c*);

        nativeScript_c() = default;
        nativeScript_c(Entity ent):component_t(ent){}

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
    using Rigidbody2d = rigidbody2d_c;
    using SpringJoint2d = springJoint2d_c;
    using BoxCollider2d = boxCollider2d_c;
    using CircleCollider2d = circleCollider2d_c;
    using EvScript = Scripting::evscript_c;

#ifndef __NO_3D__
    using Rigidbody = rigidbody_c;
    using SpringJoint = springJoint_c;
#endif

    using Collider2D = collider2d_t;
}
