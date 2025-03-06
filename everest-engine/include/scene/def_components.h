#pragma once

#include "math/types.h"
#include "component_type.h"
#include "pch.h"
#include "utils/uuid.h"

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

}
